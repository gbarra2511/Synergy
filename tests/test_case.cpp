// #include "doctest.h"
// #include "Case.hpp"
// #include "User.hpp"
// #include "Company.hpp"
// #include <memory>

// TEST_CASE("1. Criacao de case e validacoes"){
//     SUBCASE("Criacao de case") {
//         Case case1(
//             "Desafio Python",                       // Name
//             "Resolver algoritmo em Python",         // Description
//             "def solucao(): return 42",             // Solution
//             100.0                                   // Points
//         ); 

//         //GETTERS PRINCIPAIS
//         CHECK(case1.getName() == "Desafio Python");
//         CHECK(case1.getDescription() == "Resolver algoritmo em Python");
//         CHECK(case1.getSolution() == "def solucao(): return 42");
//         CHECK(case1.getPoints() == 100.0);

//         //GETTER SISTEMA
//         CHECK(case1.getId() > 0);
//         CHECK(case1.getState() == false);           // Começa inativo
//     }

//     SUBCASE("Validacoes do construtor") {
//         // NOME VAZIO
//         CHECK_THROWS_WITH_AS(
//             Case("", "Descricao valida", "solucao", 50.0),
//             "Nome não pode ser vazio.",
//             std::invalid_argument
//         );

//         // DESCRIÇÃO MUITO GRANDE (>2000)
//         std::string descricao_gigante(2001, 'a');
//         CHECK_THROWS_WITH_AS(
//             Case("Nome", descricao_gigante, "solucao", 50.0),
//             "Descrição deve ter no máximo 2000 caracteres.",
//             std::invalid_argument
//         );
//     }

//     SUBCASE("IDs Unicos e Incrementais") {
//         Case case2("Case A", "Desc A", "Sol A", 10.0);
//         Case case3("Case B", "Desc B", "Sol B", 20.0);
        
//         // Verifica que os IDs são diferentes e incrementais
//         CHECK(case3.getId() > case2.getId());
//     }
// }

// TEST_CASE("2. Setters do Case") {
//     Case case1("Original", "Descricao Original", "solucao_original", 50.0);

//     SUBCASE("Atualizando os Dados") {
//         case1.setName("Novo Nome");
//         case1.setDescription("Nova Descricao");
//         case1.setSolution("nova_solucao");
//         case1.setPoints(150.0);
//         case1.setState(true);
        
//         CHECK(case1.getName() == "Novo Nome");
//         CHECK(case1.getDescription() == "Nova Descricao");
//         CHECK(case1.getSolution() == "nova_solucao");
//         CHECK(case1.getPoints() == 150.0);
//         CHECK(case1.getState() == true);
//     }

//     SUBCASE("Validando Erros nos Setters") {
//         // NOME VAZIO
//         CHECK_THROWS_WITH_AS(
//             case1.setName(""),
//             "Nome não pode ser vazio.",
//             std::invalid_argument
//         );

//         // DESCRIÇÃO MUITO GRANDE
//         std::string descricao_gigante(2001, 'a');
//         CHECK_THROWS_WITH_AS(
//             case1.setDescription(descricao_gigante),
//             "Descrição deve ter no máximo 2000 caracteres.",
//             std::invalid_argument
//         );

//         // PONTUAÇÃO NEGATIVA
//         CHECK_THROWS_WITH_AS(
//             case1.setPoints(-10.0),
//             "Pontuação não pode ser negativa.",
//             std::invalid_argument
//         );
//     }
// }

// TEST_CASE("3. Sistema de Tentativa de Solucao (attemptSolve)") {
//     auto user = std::make_shared<User>(
//         "Giovana",
//         "giovana@email.com",
//         "31999999999",
//         "senha123",
//         true,
//         "Bio Giovana",
//         0.0,
//         "12345678900"
//     );

//     auto company = std::make_shared<Company>(
//         "Google",
//         "google@corp.com",
//         "31988888888",
//         "senha123",
//         true,
//         "Empresa Tech",
//         "12345678000190"
//     );
//     Case case1("Desafio 1", "Resolver problema", "resposta_correta", 100.0);

//     SUBCASE("Solucao Correta") {
//         double score_inicial = user->getTotalScore();
        
//         case1.attemptSolve(user, company, "resposta_correta");
        
//         // Verifica se a pontuação foi adicionada
//         CHECK(user->getTotalScore() == score_inicial + 100.0);
        
//         // Verifica se o case foi registrado como resolvido
//         std::vector<int> solved = user->getSolvedCases();
//         CHECK(solved.size() == 1);
//         CHECK(solved[0] == case1.getId());
//     }

//     SUBCASE("Solucao Incorreta") {
//         double score_inicial = user->getTotalScore();
        
//         // Captura a saída do cout (solução incorreta imprime mensagem)
//         case1.attemptSolve(user, company, "resposta_errada");
        
//         // Verifica que a pontuação NÃO foi alterada
//         CHECK(user->getTotalScore() == score_inicial);
        
//         // Verifica que o case NÃO foi registrado como resolvido
//         CHECK(user->getSolvedCases().size() == 0);
//     }

//     SUBCASE("Tentativa de Resolver Case ja Resolvido") {
//         // Resolve o case pela primeira vez
//         case1.attemptSolve(user, company, "resposta_correta");
        
//         // Tenta resolver novamente
//         CHECK_THROWS_WITH_AS(
//             case1.attemptSolve(user, company, "resposta_correta"),
//             "Usuário já resolveu este case anteriormente!",
//             std::logic_error
//         );
//     }

//     SUBCASE("Validacao de Ponteiros Nulos") {
//         // USER NULO
//         CHECK_THROWS_WITH_AS(
//             case1.attemptSolve(nullptr, company, "resposta"),
//             "O ponteiro de usuário não pode ser inválido!",
//             std::invalid_argument
//         );

//         // COMPANY NULO
//         CHECK_THROWS_WITH_AS(
//             case1.attemptSolve(user, nullptr, "resposta"),
//             "O ponteiro de empresa não pode ser inválido.",
//             std::invalid_argument
//         );
//     }
// }

// TEST_CASE("4. Mudanca de Estado do Case") {
//     Case case1("Case Ativo", "Descricao", "solucao", 75.0);

//     SUBCASE("Case Inicia Inativo") {
//         CHECK(case1.getState() == false);
//     }

//     SUBCASE("Ativando Case") {
//         case1.setState(true);
//         CHECK(case1.getState() == true);
//     }

//     SUBCASE("Desativando Case") {
//         case1.setState(true);
//         CHECK(case1.getState() == true);
        
//         case1.setState(false);
//         CHECK(case1.getState() == false);
//     }
// }

// TEST_CASE("5. Atualizacao de Pontuacao") {
//     Case case1("Case Pontos", "Descricao", "solucao", 50.0);

//     SUBCASE("Pontuacao Inicial") {
//         CHECK(case1.getPoints() == 50.0);
//     }

//     SUBCASE("Aumentando Pontuacao") {
//         case1.setPoints(200.0);
//         CHECK(case1.getPoints() == 200.0);
//     }

//     SUBCASE("Diminuindo Pontuacao") {
//         case1.setPoints(10.0);
//         CHECK(case1.getPoints() == 10.0);
//     }

//     SUBCASE("Pontuacao Zero e Valida") {
//         case1.setPoints(0.0);
//         CHECK(case1.getPoints() == 0.0);
//     }
// }