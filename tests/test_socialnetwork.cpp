#include "doctest.h"
#include "SocialNetwork.hpp"
#include "User.hpp"
#include "Company.hpp"
#include "Post.hpp"
#include "Ranking.hpp"
#include "CaseSystem.hpp"

#include <memory>
#include <vector>
#include <sstream>

// --- SETUP PROS TESTES --- 

struct TestSetup {
    std::vector<std::shared_ptr<Entity>> users;
    std::vector<std::shared_ptr<Entity>> companies;
    std::vector<std::shared_ptr<Post>> posts;
    Ranking globalRanking;
    CaseSystem caseSystem;
    
    std::shared_ptr<User> userArthur;
    std::shared_ptr<User> userBraulio;
    std::shared_ptr<Company> companyTech;
    std::shared_ptr<Entity> currentUserEntity;

    TestSetup() {
        // USER = Arthur
        userArthur = std::make_shared<User>(
            "Arthur", 
            "arthur@test.com", 
            "11999991111", 
            "pass1234", 
            true, 
            "Bio", 
            0.0, 
            "11111111111");
        users.push_back(userArthur);

        // USER = Braulio
        userBraulio = std::make_shared<User>(
            "Braulio", 
            "braulio@test.com", 
            "11999992222", 
            "pass1234", 
            true, 
            "Bio", 
            0.0, 
            "22222222222");
        users.push_back(userBraulio);

        // COMPANY 
        companyTech = std::make_shared<Company>(
            "TechCorp", 
            "tech@corp.com", 
            "1133334444", 
            "passCorp", 
            true, 
            "Desc", 
            "12345678000199");
        companies.push_back(companyTech);

        currentUserEntity = userArthur;
    }
};

TEST_CASE("1. Fluxo de Posts") {
    TestSetup setupVar;
    SocialNetwork sn(
        &setupVar.currentUserEntity, 
        &setupVar.users, 
        &setupVar.companies, 
        &setupVar.posts, 
        &setupVar.globalRanking,
        &setupVar.caseSystem);

    SUBCASE("Criar e Deletar Post") {
        CHECK(setupVar.posts.size() == 0);

        // Criar
        CHECK_NOTHROW(sn.createPost("Post"));
        CHECK(setupVar.posts.size() == 1);
        CHECK(setupVar.posts[0]->getAuthor() == "Arthur");

        // Deletar
        int id = setupVar.posts[0]->getID();
        CHECK_NOTHROW(sn.deletePost(id));
        CHECK(setupVar.posts.size() == 0);
    }

    SUBCASE("Validacoes") {
        // Desc Vazia
        CHECK_THROWS_WITH_AS(sn.createPost(""), "Descrição não pode ser vazia.", std::invalid_argument);
    }

    SUBCASE("Deletar post de Outro User") {
        auto postBraulio = std::make_shared<Post>("Braulio", "Post do Braulio");
        setupVar.posts.push_back(postBraulio);

        sn.deletePost(postBraulio->getID());

        CHECK(setupVar.posts.size() == 1);
        CHECK(setupVar.posts[0]->getAuthor() == "Braulio");
    }
}

TEST_CASE("2. Fluxo de Relacionamentos") {
    // "Silenciar" os std::cin no Terminal
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    TestSetup setupVar;
    SocialNetwork sn(
        &setupVar.currentUserEntity, 
        &setupVar.users, 
        &setupVar.companies, 
        &setupVar.posts, 
        &setupVar.globalRanking,
        &setupVar.caseSystem);

    // Arthur está LOGADO

    SUBCASE("Follow User/Company") {
        // Arthur -> Braulio
        sn.followEntity("Braulio");
        CHECK(setupVar.userArthur->getFollowing().size() == 1);
        CHECK(setupVar.userArthur->getFollowing()[0] == "Braulio");
        CHECK(setupVar.userBraulio->getFollowers()[0] == "Arthur");

        // Arthur -> TechCorp
        sn.followEntity("TechCorp");
        CHECK(setupVar.userArthur->getFollowing().size() == 2);
        CHECK(setupVar.companyTech->getFollowers()[0] == "Arthur");
    }

    SUBCASE("Unfollow") {
        sn.followEntity("Braulio");
        CHECK(setupVar.userArthur->getFollowing().size() == 1);

        sn.unfollowEntity("Braulio");

        CHECK(setupVar.userArthur->getFollowing().size() == 0);
        CHECK(setupVar.userBraulio->getFollowers().size() == 0);
    }

    SUBCASE("Validacoes") {
        // FOLLOW SI MESMO
        CHECK_THROWS_WITH_AS(
            sn.followEntity("Arthur"),
            "Você não pode seguir a si mesmo.",
            std::invalid_argument
        );

        // FOLLOW DUPLICADO
        sn.followEntity("Braulio"); 
        CHECK_THROWS_WITH_AS(
            sn.followEntity("Braulio"),
            "Você já segue esse usuario!",
            std::logic_error
        );

        // FOLLOW N EXISTENTE
        CHECK_THROWS_WITH_AS(
            sn.followEntity("Niguem"),
            "Perfil nao encontrado!",
            std::invalid_argument
        );
    }

    std::cout.rdbuf(oldCout);
}

TEST_CASE("3. Fluxo de Interacoes(Curtidas/Comentarios)") {
    TestSetup setupVar;
    SocialNetwork sn(
        &setupVar.currentUserEntity, 
        &setupVar.users, 
        &setupVar.companies, 
        &setupVar.posts, 
        &setupVar.globalRanking,
        &setupVar.caseSystem);

    // Braulio cria um post para Arthur interagir
    auto postBraulio = std::make_shared<Post>("Braulio", "Post Braulio");
    setupVar.posts.push_back(postBraulio);
    int idPost = postBraulio->getID();

    SUBCASE("Comentando no Post") {
        CHECK_NOTHROW(sn.commentPost(idPost, "Comentario Arthur"));

        CHECK(postBraulio->getNumComments() == 1);
        CHECK(postBraulio->getComments()[0].description == "Comentario Arthur");
        CHECK(postBraulio->getComments()[0].author == "Arthur");

        // Remover Comentario 
        CHECK_NOTHROW(sn.removeCommentPost(idPost, 0));
        CHECK(postBraulio->getNumComments() == 0);
    }

    SUBCASE("Curtidas/Descurtidas") {
        // Like
        CHECK_NOTHROW(sn.likePost(idPost));
        CHECK(postBraulio->getNumLikes() == 1);
        CHECK(postBraulio->getLikedBy()[0] == "Arthur");

        // Unlike
        CHECK_NOTHROW(sn.unlikePost(idPost));
        CHECK(postBraulio->getNumLikes() == 0);
    }

    SUBCASE("Validacoes") {
        // COMENTARIO EM ID INEXISTENTE
        CHECK_THROWS_WITH_AS(
            sn.commentPost(9999, "Texto"),
            "Post não encontrado!",
            std::runtime_error
        );

        // COMENTARIO VAZIO
        CHECK_THROWS_WITH_AS(
            sn.commentPost(idPost, ""),
            "Comentário não pode ser vazio.",
            std::invalid_argument
        );

        // CURTIDA EM POST INEXISTENTE
        CHECK_THROWS_WITH_AS(
            sn.likePost(9999),
            "Post não encontrado!",
            std::runtime_error
        );
    }
}

TEST_CASE("4. Logica do Feed") {
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    TestSetup setupVar;
    SocialNetwork sn(
        &setupVar.currentUserEntity, 
        &setupVar.users, 
        &setupVar.companies, 
        &setupVar.posts, 
        &setupVar.globalRanking,
        &setupVar.caseSystem);

    // --- CENÁRIO ---
    // Arthur (Logada) segue Braulio.
    // Arthur NÃO segue Thiago.
    // Braulio, Thiago e Arthur fazem posts.
    
    // Criando Thiago
    auto userThiago = std::make_shared<User>(
        "Thiago", 
        "Thiago@test.com", 
        "11999993333", 
        "pass1234", 
        true, 
        "Bio", 
        0.0, 
        "33333333333");
    setupVar.users.push_back(userThiago);

    // Arthur segue Braulio
    sn.followEntity("Braulio");

    // Criação dos Posts 
    auto postArthur = std::make_shared<Post>("Arthur", "Post do Arthur (Eu mesmo)");
    auto postBraulio = std::make_shared<Post>("Braulio", "Post do Braulio (Amigo)");
    auto postThiago = std::make_shared<Post>("Thiago", "Post do Thiago (Desconhecido)");
    
    setupVar.posts.push_back(postArthur);
    setupVar.posts.push_back(postBraulio);
    setupVar.posts.push_back(postThiago);

    // --- TESTE DE IMPRESSÃO ---
    // Captura da impressão do viewMyFeed

    sn.viewMyFeed();

    std::cout.rdbuf(oldCout); // Restaura cout original
    
    std::string saida = buffer.str();

    // --- VERIFICAÇÕES ---
    
    // Arthur deve ver seu próprio post
    CHECK(saida.find("Post do Arthur") != std::string::npos);

    // Arthur deve ver o post do Braulio
    CHECK(saida.find("Post do Braulio") != std::string::npos);

    // Arthur NÃO deve ver o post do Thiago
    CHECK(saida.find("Post do Thiago") == std::string::npos);
}

TEST_CASE("5. Logica do Ranking") {
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    TestSetup setupVar;
    SocialNetwork sn(
        &setupVar.currentUserEntity, 
        &setupVar.users, 
        &setupVar.companies, 
        &setupVar.posts, 
        &setupVar.globalRanking, 
        &setupVar.caseSystem);

    SUBCASE("Display do Ranking Global") {
        // Criação dos Users
        for (int i = 0; i < 12; ++i) {
            std::string nome = "User" + std::to_string(i);
            auto u = std::make_shared<User>(
                nome, 
                "email@.com", 
                "11999993333", 
                "pass1234", 
                true, 
                "bio", 
                0.0, 
                "33333333333");
            
            u->addScore(setupVar.companyTech, (i + 1) * 10); 
            setupVar.globalRanking.addUser(u);
        }

        CHECK_NOTHROW(sn.displayGlobalRanking());
    }

    SUBCASE("Validacao") {
        // Ranking Vazio
        CHECK_THROWS_WITH_AS(
            sn.displayGlobalRanking(),
            "Sistema não possui nenhum usuario em rankings!",
            std::logic_error
        );

        // Ranking Global não inicializado
        SocialNetwork snErro(
            &setupVar.currentUserEntity, 
            &setupVar.users, 
            &setupVar.companies, 
            &setupVar.posts, 
            nullptr,  
            &setupVar.caseSystem
        );

        CHECK_THROWS_WITH_AS(
            snErro.displayGlobalRanking(),
            "Ranking global não inicializado",
            std::logic_error
        );
    }
    
    std::cout.rdbuf(oldCout); 
}

TEST_CASE("6. Logica dos Case") {
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    TestSetup setupVar;
    SocialNetwork sn(
        &setupVar.currentUserEntity, 
        &setupVar.users, 
        &setupVar.companies, 
        &setupVar.posts, 
        &setupVar.globalRanking, 
        &setupVar.caseSystem);

    // Criação do CaseTest
    auto case1 = std::make_shared<Case>(
        "Bug Crítico", 
        "Arrume o bug", 
        "RespostaCerta", 
        50);
    
    setupVar.companyTech->addCase(case1);
    int idDoCase = case1->getId();

    SUBCASE("Resolvendo Corretamente") {
        CHECK_NOTHROW(sn.attemptSolveCase(idDoCase, "RespostaCerta", setupVar.companyTech));
        
        CHECK(setupVar.userArthur->getTotalScore() == 50);
    }

    SUBCASE("Resolvendo Incorretamente") {
        CHECK_NOTHROW(sn.attemptSolveCase(idDoCase, "RespostaErrada", setupVar.companyTech));
        
        CHECK(setupVar.userArthur->getTotalScore() == 0);
    }

    SUBCASE("Case Ja Resolvido") {
        sn.attemptSolveCase(idDoCase, "RespostaCerta", setupVar.companyTech);

        CHECK_THROWS_WITH_AS(
            sn.attemptSolveCase(idDoCase, "RespostaCerta", setupVar.companyTech),
            "Você já resolveu este case anteriormente!",
            std::logic_error
        );
    }

    SUBCASE("Outras Validacoes") {
        // ID INVALIDO
        CHECK_THROWS_AS(
        sn.attemptSolveCase(999, "Resp", setupVar.companyTech),
        std::runtime_error
    );

        // RESPOSTA VAZIA
        CHECK_THROWS_WITH_AS(
            sn.attemptSolveCase(idDoCase, "", setupVar.companyTech),
            "Resposta não pode estar vazia!",
            std::invalid_argument
        );

        // EMPRESA NULA
        CHECK_THROWS_WITH_AS(
            sn.attemptSolveCase(idDoCase, "Resp", nullptr),
            "Case deve estar vinculado a uma empresa!",
            std::runtime_error
        );
    }

    std::cout.rdbuf(oldCout);
}