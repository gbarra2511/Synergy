#include "doctest.h"
#include "System.hpp"
#include "User.hpp"
#include "Company.hpp"

#include <iostream>
#include <sstream>
#include <cstdio>

const std::string TEST_DB_NAME = "test_rede_social.db";

void simulateInput(const std::string& input) {
    static std::stringstream input_stream;

    input_stream.str(""); 
    input_stream.clear();
    input_stream << input;

    std::cin.clear();
    std::cin.rdbuf(input_stream.rdbuf());
}

void cleanTestDb() {
    std::remove(TEST_DB_NAME.c_str());
}

TEST_CASE("1. Fluxo de Registro e Login") {
    cleanTestDb(); 
    
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    std::stringstream bufferCerr;
    std::streambuf* oldCerr = std::cerr.rdbuf(bufferCerr.rdbuf());
    
    std::streambuf* originalCin = std::cin.rdbuf();
    
    {
        System sys(TEST_DB_NAME); 

        SUBCASE("Registrando Usuario") {
            std::string inputs = 
                "UserTest\n"                // Nome
                "usertest@email.com\n"      // Email
                "pass1234\n"                // Senha 
                "11999998888\n"             // TeleNumero 
                "11122233344\n"             // CPF 
                "Bio do usuario\n";         // Desc
        
            simulateInput(inputs);

            CHECK_NOTHROW(sys.registerUser());

            std::string saida = buffer.str();
            CHECK(saida.find("cadastrado com sucesso") != std::string::npos);
        }

        SUBCASE("Login // Logout") {
            std::string regInputs = 
                "Arthur\n"              // Nome
                "arthur@log.com\n"      // Email
                "pass1234\n"            // Senha
                "11999997777\n"         // TeleNumero
                "99988877766\n"         // CPF
                "Bio\n";                // Descrição

            simulateInput(regInputs);
            sys.registerUser();
        
            buffer.str(""); 

            std::string logInputs = 
                "Arthur\n"              // User
                "senhaerrada\n"         // Senha errada
                "Arthur\n"              // User 
                "pass1234\n";           // Senha certa 

            simulateInput("Arthur\nsenhaerrada\n");
            sys.login(); 
            CHECK(sys.hasCurrentUser() == false); 

            simulateInput("Arthur\npass1234\n");
            sys.login();
            CHECK(sys.hasCurrentUser() == true); 

            sys.logout();
            CHECK(sys.hasCurrentUser() == false);
        }
    }     
    
    std::cout.rdbuf(oldCout);
    std::cerr.rdbuf(oldCerr);
    std::cin.rdbuf(originalCin);
}

// TEST_CASE("2. Fluxo de Empresa") {
//     cleanTestDb();

//     std::stringstream buffer;
//     std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

//     std::streambuf* originalCin = std::cin.rdbuf();

//     {
//         System sys(TEST_DB_NAME);

//         // Registro da Empresa
//         std::string regComp = 
//             "TechCorp\n"
//             "tech@corp.com\n"
//             "12345678\n"
//             "Solucoes TI\n"
//             "1133334444\n"
//             "12345678000199\n";

//         simulateInput(regComp);
//         sys.registerCompany();

//         simulateInput("TechCorp\n12345678\n");
//         sys.login();
//         CHECK(sys.isCurrentUserCompany() == true);

//         SUBCASE("Adicionando Cases") {
//             std::string caseInput = 
//                 "Bug Java\n"
//                 "Arrume o NullPointer\n"
//                 "Use Optional\n"
//                 "100\n";

//             simulateInput(caseInput);
            
//             CHECK_NOTHROW(sys.addCaseInteractive());

//             buffer.str(""); 
//             sys.listMyCases();
//             std::string saida = buffer.str();
            
//             CHECK(saida.find("Bug Java") != std::string::npos);
//             CHECK(saida.find("100") != std::string::npos);
//         }

//         SUBCASE("Removendo Case") {
//             simulateInput("CaseRemove\nDesc\nResp\n50\n");
//             sys.addCaseInteractive();

//             sys.listMyCases();
            
//             simulateInput("1\n"); 
//             sys.removeCase();

//             buffer.str("");
//             sys.listMyCases();
        
//             CHECK(buffer.str().find("CaseRemove") == std::string::npos); 
//         }
//     }

//     std::cout.rdbuf(oldCout);
//     std::cin.rdbuf(originalCin);
// }

TEST_CASE("3. Fluxo de Edicao de Perfil e Posts") {
    cleanTestDb(); 

    std::stringstream bufferCout;
    std::streambuf* oldCout = std::cout.rdbuf(bufferCout.rdbuf());
    
    std::stringstream bufferCerr;
    std::streambuf* oldCerr = std::cerr.rdbuf(bufferCerr.rdbuf());

    std::streambuf* originalCin = std::cin.rdbuf();

    {
        System sys(TEST_DB_NAME);

        
        std::string inputs = 
            "UserSocial\n"
            "social@test.com\n"
            "pass1234\n"
            "11999991111\n"
            "11122233344\n"
            "Bio Original\n";
        
        simulateInput(inputs);
        sys.registerUser();

        simulateInput("UserSocial\npass1234\n");
        sys.login();
        
        CHECK(sys.hasCurrentUser() == true);

        SUBCASE("Edicao Perfil") {
            simulateInput("Nome Alterado\n");
            sys.updateUserName();

            simulateInput("Nova Bio Top\n");
            sys.updateUserDescription();

            bufferCout.str(""); 
            sys.viewMyProfile();
            std::string saida = bufferCout.str();

            CHECK(saida.find("Nome Alterado") != std::string::npos);
            CHECK(saida.find("Nova Bio Top") != std::string::npos);
        }

        SUBCASE("Criacao de Post") {
            // Como o ID ta Static no Post.hpp Vai Ter q Procurar o Id Dentro do Teste
            simulateInput("Meu post\n");
            CHECK_NOTHROW(sys.createPost());

            bufferCout.str(""); 
            sys.viewMyPosts();
            std::string output = bufferCout.str();
            
            CHECK(bufferCout.str().find("Meu post") != std::string::npos);

            std::regex idRegex(R"(ID:\s*(\d+))"); 
            std::smatch match;

            std::string idEncontrado = "0";

            if (std::regex_search(output, match, idRegex)) {
                idEncontrado = match[1].str();
            } else {
                CHECK_MESSAGE(false, "Nao foi possivel encontrar o ID do post na saida visual.");
            }

            std::string inputInteracao = idEncontrado + "\n1\n";

            simulateInput(inputInteracao); 
            CHECK_NOTHROW(sys.interactWithPost());
        }
    } 

    std::cout.rdbuf(oldCout);
    std::cerr.rdbuf(oldCerr);
    std::cin.rdbuf(originalCin);
}

TEST_CASE("4. Fluxo de Interacoes") {
    cleanTestDb(); 

    std::stringstream bufferCout;
    std::streambuf* oldCout = std::cout.rdbuf(bufferCout.rdbuf());
    
    std::stringstream bufferCerr;
    std::streambuf* oldCerr = std::cerr.rdbuf(bufferCerr.rdbuf());

    std::streambuf* originalCin = std::cin.rdbuf();

    {
        System sys(TEST_DB_NAME);

        // User Braulio
        simulateInput(
            "Braulio\n"
            "braulio@mail.com\n"
            "pass1234\n"
            "11999992222\n"
            "22222222222\n"
            "Bio Braulio\n");

        sys.registerUser();

        // User Arthur
        simulateInput(
            "Arthur\n"
            "arthur@mail.com\n"
            "pass1234\n"
            "11999991111\n"
            "11111111111\n"
            "Bio Arthur\n");

        sys.registerUser();

        simulateInput("Arthur\npass1234\n");
        sys.login();
        CHECK(sys.hasCurrentUser() == true);

        SUBCASE("Seguindo um Usuario") {
            bufferCout.str("");
            sys.listFollowing();
            CHECK(bufferCout.str().find("Braulio") == std::string::npos);

            simulateInput("1\nBraulio\n");
            CHECK_NOTHROW(sys.manageFollowUnfollow());

            bufferCout.str("");
            sys.listFollowing();
            CHECK(bufferCout.str().find("Braulio") != std::string::npos);
        }

        SUBCASE("Deseguindo um Usuario") {
            simulateInput("1\nBraulio\n");
            sys.manageFollowUnfollow();

            simulateInput("2\nBraulio\n");
            CHECK_NOTHROW(sys.manageFollowUnfollow());

            bufferCout.str("");
            sys.listFollowing();
            
            bool BraulioEstaNaLista = bufferCout.str().find("- Braulio") != std::string::npos; 
            CHECK(BraulioEstaNaLista == false);
        }

        SUBCASE("Listando Seguidores") {
            simulateInput("1\nBraulio\n");
            sys.manageFollowUnfollow();

            sys.logout();

            simulateInput("Braulio\npass1234\n");
            sys.login();

            bufferCout.str("");
            sys.listFollowers();
            
            CHECK(bufferCout.str().find("Arthur") != std::string::npos);
        }

        SUBCASE("Visualizando Rankings") {   
            CHECK_NOTHROW(sys.viewGlobalRanking());
            
            CHECK_NOTHROW(sys.viewMyRanking());
            
            simulateInput("TechCorp\n");
            CHECK_NOTHROW(sys.viewCompanyRanking()); 
        }
    } 

    std::cout.rdbuf(oldCout);
    std::cerr.rdbuf(oldCerr);
    std::cin.rdbuf(originalCin);
}

TEST_CASE("5. Fluxo de Dados (Save/Load)") {
    cleanTestDb(); 

    std::stringstream bufferCout;
    std::streambuf* oldCout = std::cout.rdbuf(bufferCout.rdbuf());
    
    std::stringstream bufferCerr;
    std::streambuf* oldCerr = std::cerr.rdbuf(bufferCerr.rdbuf());

    std::streambuf* originalCin = std::cin.rdbuf();

    // CRIAÇÃO DE DADOS E FECHAMENTO DO SISTEMA
   
    {
        System sysUm(TEST_DB_NAME);

        simulateInput(
            "ThiagoUser\n"
            "thiago@test.com\n"
            "pass1234\n"
            "11999996666\n"
            "66666666666\n"
            "Vivo Morto\n");

        sysUm.registerUser();

        simulateInput("ThiagoUser\npass1234\n");
        sysUm.login();
        CHECK(sysUm.hasCurrentUser() == true);

        simulateInput("Dado Persistente\n");
        sysUm.createPost();

        sysUm.logout();

    } 
    
    // REABRINDO O SISTEMA E VERIFICANDO OS DADOS
   
    {
        System sysDois(TEST_DB_NAME);

        simulateInput("ThiagoUser\npass1234\n");
        sysDois.login();

        CHECK_MESSAGE(sysDois.hasCurrentUser() == true, "Falha: Usuario nao foi salvo/carregado corretamente.");

        if (sysDois.hasCurrentUser()) {
            bufferCout.str(""); 
            sysDois.viewMyPosts();
            
            std::string output = bufferCout.str();
            CHECK_MESSAGE(output.find("Dado Persistente") != std::string::npos, "Falha: Post nao foi salvo/carregado.");
        }
        
        if (sysDois.hasCurrentUser()) {
            bufferCout.str("");
            sysDois.viewMyProfile();
            std::string output = bufferCout.str();
            
            CHECK(output.find("thiago@test.com") != std::string::npos);
            CHECK(output.find("66666666666") != std::string::npos);
        }
    } 

    std::cout.rdbuf(oldCout);
    std::cerr.rdbuf(oldCerr);
    std::cin.rdbuf(originalCin);
}

TEST_CASE("6. Menus e Validacoes") {
    cleanTestDb(); 
    
    std::stringstream bufferCout;
    std::streambuf* oldCout = std::cout.rdbuf(bufferCout.rdbuf());

    std::stringstream bufferCerr;
    std::streambuf* oldCerr = std::cerr.rdbuf(bufferCerr.rdbuf());
    
    std::streambuf* originalCin = std::cin.rdbuf();

    {
        System sys(TEST_DB_NAME);

        SUBCASE("Menus de Exibicao") {
            // Main Menu -> Opção 4 (Sair)
            simulateInput("4\n");
            sys.showMainMenu();

            simulateInput(
                "UserCov\n"
                "user@cov.com\n"
                "pass1234\n"
                "11999991111\n"
                "11111111111\n"
                "Bio\n");
            sys.registerUser();
            
            simulateInput("UserCov\npass1234\n");
            sys.login();

            // User Menu -> Opção 5 (Logout)
            simulateInput("5\n");
            sys.showUserMenu();

            // Social Menu -> Opção 9 (Voltar)
            simulateInput("9\n");
            sys.showSocialMenu();

            // Ranking Menu -> Opção 4 (Voltar)
            simulateInput("4\n");
            sys.showRankingMenu();

            // Edit Profile -> Opção 6 (Voltar)
            simulateInput("6\n");
            sys.editProfile();

            sys.logout();
        }
        
        SUBCASE("Validacoes") {
            std::string inputsErros = 
                // 1. Nome Vazio -> Nome Curto -> Nome Certo
                "\n" "Oi\n" "Valid Name\n" 
                
                // 2. Email Vazio -> Email Ruim -> Email Existente -> Email Bom
                "\n" "emailruim\n" "user@cov.com\n" "novo@email.com\n"
                
                // 3. Senha Vazia -> Senha Curta -> Senha Boa
                "\n" "123\n" "pass1234\n"
                
                // 4. Numero Vazio -> Numero Ruim -> Numero Bom
                "\n" "abc\n" "11999992222\n"
                
                // 5. CPF Vazio -> CPF Tamanho Errado -> CPF Letras -> CPF Existente -> CPF Bom
                "\n" "123\n" "abcdefghijk\n" "11111111111\n" "22222222222\n"
                
                // 6. Bio
                "Bio Nova\n";

            simulateInput(inputsErros);
            
            CHECK_NOTHROW(sys.registerUser()); 
        }
    }

    std::cout.rdbuf(oldCout);
    std::cerr.rdbuf(oldCerr);
    std::cin.rdbuf(originalCin);
}