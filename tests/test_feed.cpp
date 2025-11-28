#include "doctest.h"
#include "Feed.hpp"
#include "Post.hpp"

#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <sstream>

TEST_CASE("1. Manipulacao Basica  do Feed e Validacoes") {
    Feed feed;

    CHECK(feed.getNumPosts() == 0);

    // Posts Auxiliares
    auto post1 = std::make_shared<Post>("Autor1", "Conteudo1");
    auto post2 = std::make_shared<Post>("Autor2", "Conteudo2");

    SUBCASE("Adicao/Remocao de UM (Unico) Post)") {
        CHECK_NOTHROW(feed.addPost(post1));
        CHECK(feed.getNumPosts() == 1);
        
        CHECK(feed.getPost(0)->getAuthor() == "Autor1");
        
        CHECK_NOTHROW(feed.addPost(post2));
        CHECK(feed.getNumPosts() == 2);

        CHECK_NOTHROW(feed.removePost(post1));
        CHECK(feed.getNumPosts() == 1);

        CHECK(feed.getPost(0)->getAuthor() == "Autor2");
    }

    SUBCASE("Adicao/Remocao de UMA LISTA (Vector) de Posts") {
        std::vector<std::shared_ptr<Post>> listaPosts;
        listaPosts.push_back(post1);
        listaPosts.push_back(post2);

        // Adiciona lista
        CHECK_NOTHROW(feed.addPosts(listaPosts));
        CHECK(feed.getNumPosts() == 2);

        // Remove lista
        CHECK_NOTHROW(feed.removePosts(listaPosts));
        CHECK(feed.getNumPosts() == 0);
    }

    SUBCASE("Metodo Clear") {
        feed.addPost(post1);
        feed.addPost(post2);
        CHECK(feed.getNumPosts() == 2);

        feed.clear();
        CHECK(feed.getNumPosts() == 0);
    }

    SUBCASE("Validacoes") {
        std::shared_ptr<Post> postNulo = nullptr;

        // ADD POST NULO
        CHECK_THROWS_WITH_AS(
            feed.addPost(postNulo),
            "Tentativa de adicionar post nulo ao feed.",
            std::invalid_argument
        );
        CHECK(feed.getNumPosts() == 0); 

        // REMOVE POST NULO
        CHECK_THROWS_WITH_AS(
            feed.removePost(postNulo),
            "Tentativa de remover post nulo.",
            std::invalid_argument
        );

        // ADD POST LIST C/ NULO
        std::vector<std::shared_ptr<Post>> listaComNulo = { post1, nullptr };
        CHECK_THROWS_WITH_AS(
            feed.addPosts(listaComNulo),
            "Lista contém post nulo.",
            std::invalid_argument
        );
        CHECK(feed.getNumPosts() == 0);
    }
}

TEST_CASE("2. Ordenacao do Feed (SortByDate)") {
    Feed feed;

    SUBCASE("Ordenacao de Posts por Data") {
        // Post Antigo
        auto postAntigo = std::make_shared<Post>("AutorAntigo", "Mais Velho");
        
        // Timeout de 1seg
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Post Novo
        auto postNovo = std::make_shared<Post>("AutorNovo", "Mais Novo");

        feed.addPost(postAntigo);
        feed.addPost(postNovo);

        CHECK(feed.getPost(0)->getAuthor() == "AutorAntigo");
        CHECK(feed.getPost(1)->getAuthor() == "AutorNovo");

        feed.sortByDate();

        CHECK(feed.getPost(0)->getAuthor() == "AutorNovo");
        CHECK(feed.getPost(1)->getAuthor() == "AutorAntigo");
    }

    SUBCASE("Ordenacao com Feed Vazio/Unico") {
        CHECK_NOTHROW(feed.sortByDate());

        auto p = std::make_shared<Post>("Autor", "Conteudo");
        feed.addPost(p);
        CHECK_NOTHROW(feed.sortByDate());
        CHECK(feed.getPost(0) == p);
    }
}

TEST_CASE("3. Operador de Insercao (<<)") {
    Feed feed;
    std::stringstream ss;

    SUBCASE("Feed Vazio") {
        ss << feed;
        CHECK(ss.str() == "O feed está vazio.\n");
    }

    SUBCASE("Impressao do Feed") {
        auto p1 = std::make_shared<Post>("AutorA", "Post A");
        auto p2 = std::make_shared<Post>("AutorB", "Post B");
        
        feed.addPost(p1);
        feed.addPost(p2);

        ss << feed;
        std::string saida = ss.str();

        CHECK(saida.find("AutorA") != std::string::npos);
        CHECK(saida.find("Post A") != std::string::npos);
        
        CHECK(saida.find("AutorB") != std::string::npos);
        CHECK(saida.find("Post B") != std::string::npos);

        CHECK(saida.find("----------------------------------------") != std::string::npos);
    }
}