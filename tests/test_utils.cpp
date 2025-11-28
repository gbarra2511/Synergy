#include "doctest.h"
#include "Utils.hpp"
#include "User.hpp" 
#include "Post.hpp"
#include <iostream>
#include <sstream> 

TEST_CASE("1. Funcoes de Busca (Finders)") {
    std::vector<std::shared_ptr<Entity>> users;
    std::vector<std::shared_ptr<Post>> posts;

    // Criando Users
    auto user1 = std::make_shared<User>(
        "Arthur", 
        "arthur@email.com", 
        "11999999999", 
        "senha1234", 
        true, 
        "Bio", 
        0.0, 
        "11111111111");

    auto user2 = std::make_shared<User>(
        "Braulio", 
        "braulio@email.com", 
        "11888888888", 
        "senha1234", 
        true, 
        "Bio", 
        0.0, 
        "22222222222");
    users.push_back(user1);
    users.push_back(user2);

    // Criando Posts
    auto post1 = std::make_shared<Post>("Arthur", "Post do Arthur");
    auto post2 = std::make_shared<Post>("Braulio", "Post do Braulio");
    posts.push_back(post1);
    posts.push_back(post2);

    SUBCASE("Metodo findEntity (Nome // ID)") {
        // Busca por Nome 
        auto encontrado = Utils::findEntity(&users, "Arthur");
        CHECK(encontrado != nullptr);
        CHECK(encontrado->getName() == "Arthur");

        // Busca por Nome Inexistente
        auto naoEncontrado = Utils::findEntity(&users, "Thiago");
        CHECK(naoEncontrado == nullptr);

        // Busca por ID
        int idArthur = user1->getId();
        auto encontradoID = Utils::findEntity(&users, idArthur);
        CHECK(encontradoID != nullptr);
        CHECK(encontradoID->getName() == "Arthur");

        // Busca por ID Inexistente
        CHECK(Utils::findEntity(&users, 99999) == nullptr);
    }

    SUBCASE("Metodo findEntities (Lista de Nomes)") {
        std::vector<std::string> nomesProcurados = {"Arthur", "Braulio", "Thiago"};
        
        auto resultado = Utils::findEntities(&users, nomesProcurados);

        // FIND = Arthur e Braulio // IGNORA = Thiago
        CHECK(resultado.size() == 2);
        CHECK(resultado[0]->getName() == "Arthur");
        CHECK(resultado[1]->getName() == "Braulio");
    }

    SUBCASE("Metodo findPost (ID)") {
        int idPost1 = post1->getID();
        
        auto postEncontrado = Utils::findPost(&posts, idPost1);
        CHECK(postEncontrado != nullptr);
        CHECK(postEncontrado->getAuthor() == "Arthur");

        CHECK(Utils::findPost(&posts, 99999) == nullptr);
    }

    SUBCASE("Metodo findPosts (Autor // Lista)") {
        auto postsArthur = Utils::findPosts(&posts, "Arthur");
        CHECK(postsArthur.size() == 1);
        CHECK(postsArthur[0]->getAuthor() == "Arthur");

        std::vector<std::string> autores = {"Arthur", "Braulio"};
        auto postsTodos = Utils::findPosts(&posts, autores);
        CHECK(postsTodos.size() == 2);
    }

    SUBCASE("Metodo findName (Strings Simples)") {
        std::vector<std::string> lista = {"UM", "DOIS", "TRES"};
        
        CHECK(Utils::findName(lista, "DOIS") == "DOIS");
        CHECK(Utils::findName(lista, "DEZ") == ""); 
    }
}

TEST_CASE("2. Funcoes de Entrada de Dados (Input)") {
    // Stringstream para imitar a entrada do usuario (std::cin)
    
    SUBCASE("Metodo getInput (String)") {
        // Redireciona cin
        std::istringstream inputSimulado("TextoTeste\n");
        std::streambuf* cinBackup = std::cin.rdbuf(inputSimulado.rdbuf()); 

        // Captura o output para não sujar o terminal
        std::stringstream outputMudo;
        std::streambuf* coutBackup = std::cout.rdbuf(outputMudo.rdbuf());

        std::string resultado = Utils::getInput("Digite algo: ");
        
        CHECK(resultado == "TextoTeste");

        // Restaura cin/cout originais 
        std::cin.rdbuf(cinBackup);
        std::cout.rdbuf(coutBackup);
    }

    SUBCASE("Metodo getIntInput (Int)") {
        std::istringstream inputSimulado("42\n");
        std::streambuf* cinBackup = std::cin.rdbuf(inputSimulado.rdbuf());
    
        std::stringstream outputMudo;
        std::streambuf* coutBackup = std::cout.rdbuf(outputMudo.rdbuf());

        int numero = Utils::getIntInput("Digite numero: ");
        CHECK(numero == 42);

        std::cin.rdbuf(cinBackup);
        std::cout.rdbuf(coutBackup);
    }

    SUBCASE("Metodo getIntInput (Tratamento de Erro)") {
        // abc (invalido) -> 10 (valido)
        std::istringstream inputSimulado("abc\n10\n");
        std::streambuf* cinBackup = std::cin.rdbuf(inputSimulado.rdbuf());

        std::stringstream outputMudo;
        std::streambuf* coutBackup = std::cout.rdbuf(outputMudo.rdbuf());

        // Loop ate ler o 10
        int numero = Utils::getIntInput("Digite numero: ");
        
        CHECK(numero == 10);
        
        // Verifica a mensagem de erro 
        std::string saidaTela = outputMudo.str();
        CHECK(saidaTela.find("Entrada inválida!") != std::string::npos);

        std::cin.rdbuf(cinBackup);
        std::cout.rdbuf(coutBackup);
    }
}