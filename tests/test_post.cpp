#include "doctest.h"
#include "Post.hpp"

#include <ctime>
#include <sstream>

TEST_CASE("1. Criacao do Post e Validacoes") {
    SUBCASE("Criacao do Post") {
        std::time_t before = std::time(nullptr);

        Post post(
            "Autor",
            "Desc Post"
        );

        std::time_t after = std::time(nullptr);
        
        // GETTERS PRINCIPAIS
        CHECK(post.getAuthor() == "Autor");
        CHECK(post.getDescription() == "Desc Post");

        // GETTERS SISTEMA
        CHECK(post.getID() > 0); 
        
        // GETTERS CONTADORES
        CHECK(post.getNumLikes() == 0);
        CHECK(post.getNumComments() == 0);

        // GETTER TIMESTAMP
        std::time_t postTimestamp = post.getTimestamp();
        CHECK(postTimestamp >= before);
        CHECK(postTimestamp <= after);
    }

    SUBCASE("Validacoes") {
        // AUTOR VAZIO
        CHECK_THROWS_WITH_AS(
            Post("", "Desc"),
            "Autor do post não pode ser vazio.",
            std::invalid_argument
        );

        // DESC VAZIA
        CHECK_THROWS_WITH_AS(
            Post("Autor", ""),
            "Conteúdo do post não pode ser vazio.",
            std::invalid_argument
        );

        // DESC < 1000
        std::string descricao_gigante(1001, 'a');

        CHECK_THROWS_WITH_AS(
            Post("Autor", descricao_gigante),
            "Descrição deve ter no máximo 1000 caracteres.",
            std::invalid_argument
        );
    }
}

TEST_CASE("2. Manipulacao de Comentarios") {
    Post post("AutorOriginal", "DescPost");

    SUBCASE("Criacao de Comentarios") {
        post.addComment("User1", "Comentario 1");
        post.addComment("User2", "Comentario 2");

        CHECK(post.getNumComments() == 2);
        
        std::vector<Comment> comentarios = post.getComments();
        CHECK(comentarios[0].author == "User1");
        CHECK(comentarios[0].description == "Comentario 1");
        CHECK(comentarios[0].index == 0); 
        CHECK(comentarios[1].index == 1); 
    }

    SUBCASE("Validando Comentarios") {
        // Autor Vazio
        CHECK_THROWS_WITH_AS(
            post.addComment("", "Texto"), 
            "Autor do comentário não pode ser vazio.", 
            std::invalid_argument
        );

        // Texto Vazio
        CHECK_THROWS_WITH_AS(
            post.addComment("User", ""), 
            "Comentário não pode ser vazio.", 
            std::invalid_argument
        );

        // Texto > 500 
        std::string texto_grande(501, 'b');
        CHECK_THROWS_WITH_AS(
            post.addComment("User", texto_grande), 
            "Comentário deve ter no máximo 500 caracteres.", 
            std::invalid_argument
        );
    }

    SUBCASE("Remocao de Comentarios + Reindexacao") {
        post.addComment("A", "C0");
        post.addComment("B", "C1");
        post.addComment("C", "C2");

        // Remoção do Comentário "B"
        post.removeComment(1);

        CHECK(post.getNumComments() == 2);

        std::vector<Comment> comentarios = post.getComments();
        
        // Conferindo a Reindexacao
        CHECK(comentarios[1].author == "C");
        CHECK(comentarios[1].index == 1); 

        // Remocao de Indice Inexistente
        CHECK_THROWS_WITH_AS(
            post.removeComment(50), 
            "Comentário não encontrado.", 
            std::out_of_range
        );
    }
}

TEST_CASE("3. Manipulacao de Curtidas") {
    Post post("AutorOriginal", "DescPost");

    SUBCASE("Adicicao e Remocao de Curtidas") {
        post.addLiker("Usuario_A");
        post.addLiker("Usuario_B");

        CHECK(post.getNumLikes() == 2);
        
        std::vector<std::string> likers = post.getLikedBy();
        CHECK(std::find(likers.begin(), likers.end(), "Usuario_A") != likers.end());
        CHECK(std::find(likers.begin(), likers.end(), "Usuario_B") != likers.end());

        post.removeLiker("Usuario_A");
        CHECK(post.getNumLikes() == 1);
        
        likers = post.getLikedBy();
        CHECK(likers.size() == 1);
        CHECK(likers[0] == "Usuario_B");
    }

    SUBCASE("Validacoes") {
        // USER VAZIO
        CHECK_THROWS_WITH_AS(
            post.addLiker(""),
            "Nome do usuário não pode ser vazio.",
            std::invalid_argument
        );

        // USER DUPLICADO
        post.addLiker("Usuario_Unico");

        CHECK_THROWS_WITH_AS(
            post.addLiker("Usuario_Unico"),
            "Usuário já curtiu este post.",
            std::logic_error
        );

        // !! PRECISA CRIAR UMA VALIDAÇÃO PARA CASO O REMOVELIKER NN EXISTA !!
    }
}

TEST_CASE("4. Edicao e Sobrecarga de Operadores") {
    Post post("AutorOriginal", "DescPost");

    SUBCASE("Metodo setDescription") {
        post.setDescription("Texto Editado");
        CHECK(post.getDescription() == "Texto Editado");

        // DESC VAZIA
        CHECK_THROWS_WITH_AS(
            post.setDescription(""),
            "Descrição não pode ser vazia.",
            std::invalid_argument
        );

        // VDESC > 1000
        std::string descricao_gigante(1001, 'a');
        CHECK_THROWS_WITH_AS(
            post.setDescription(descricao_gigante),
            "Descrição deve ter no máximo 1000 caracteres.",
            std::invalid_argument
        );
    }

    SUBCASE("Operador de Comparacao (==)") {
        Post p1("User", "Texto");
        Post p2("User", "Texto");
        Post p3("OutroUser", "Texto");
        Post p4("User", "OutroTexto");

        // Devem ser Iguais 
        CHECK(p1 == p2);

        // Devem ser diferentes
        CHECK_FALSE(p1 == p3); // Autor Diferente
        CHECK_FALSE(p1 == p4); // Descrição Diferente
    }

    SUBCASE("Operador de Insercao (<<)") {
        // Criação do Post
        Post pPrint("AutorPrint", "DescPrint");
        pPrint.addLiker("Liker1");
        pPrint.addComment("Commenter1", "ComentarioTexto");

        // Redireciona a saida para uma string
        std::stringstream ss;
        ss << pPrint;
        std::string saida = ss.str();

        // Verifica se as partes essenciais estao na string gerada
        // Nota: O teste procura por substrings dentro da saida formatada
        CHECK(saida.find("AutorPrint") != std::string::npos);
        CHECK(saida.find("DescPrint") != std::string::npos);
        CHECK(saida.find("Curtidas: 1") != std::string::npos);
        CHECK(saida.find("Commenter1") != std::string::npos);
        CHECK(saida.find("ComentarioTexto") != std::string::npos);
    }
}