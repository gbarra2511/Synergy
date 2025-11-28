// #include "doctest.h"
// #include "User.hpp"
// #include "Company.hpp"
// #include <memory>
// #include <vector>

// TEST_CASE("1. Validacoes Especificas do CPF") {
//     SUBCASE("CPF Valido") {
//         User usuario(
//             "Joao",
//             "joao@email.com",
//             "31999999999",
//             "senha123",
//             true,
//             "Bio",
//             0.0,
//             "12345678900"
//         );
        
//         CHECK(usuario.getCpf() == "12345678900");
//     }

//     SUBCASE("CPF Vazio no Construtor") {
//         CHECK_THROWS_WITH_AS(
//             User("Joao", "joao@email.com", "31999999999", "senha123", true, "Bio", 0.0, ""),
//             "CPF não pode ser vazio.",
//             std::invalid_argument
//         );
//     }

//     SUBCASE("CPF com Tamanho Incorreto no Construtor") {
//         CHECK_THROWS_WITH_AS(
//             User("Joao", "joao@email.com", "31999999999", "senha123", true, "Bio", 0.0, "123456789"),
//             "CPF deve conter 11 dígitos.",
//             std::invalid_argument
//         );

//         CHECK_THROWS_WITH_AS(
//             User("Joao", "joao@email.com", "31999999999", "senha123", true, "Bio", 0.0, "123456789012"),
//             "CPF deve conter 11 dígitos.",
//             std::invalid_argument
//         );
//     }

//     SUBCASE("CPF com Caracteres Nao Numericos no Construtor") {
//         CHECK_THROWS_WITH_AS(
//             User("Joao", "joao@email.com", "31999999999", "senha123", true, "Bio", 0.0, "1234567890A"),
//             "CPF deve conter apenas números.",
//             std::invalid_argument
//         );

//         CHECK_THROWS_WITH_AS(
//             User("Joao", "joao@email.com", "31999999999", "senha123", true, "Bio", 0.0, "123.456.789-00"),
//             "CPF deve conter 11 dígitos.",
//             std::invalid_argument
//         );
//     }
// }

// TEST_CASE("2. Setter de CPF e Validacoes") {
//     User usuario(
//         "Carlos",
//         "carlos@email.com",
//         "31999999999",
//         "senha123",
//         true,
//         "Bio",
//         0.0,
//         "12345678900"
//     );

//     SUBCASE("Atualizar CPF Valido") {
//         usuario.setCpf("98765432100");
//         CHECK(usuario.getCpf() == "98765432100");
//     }

//     SUBCASE("CPF Vazio no Setter") {
//         CHECK_THROWS_WITH_AS(
//             usuario.setCpf(""),
//             "CPF não pode ser vazio.",
//             std::invalid_argument
//         );
//     }

//     SUBCASE("CPF com Tamanho Incorreto no Setter") {
//         CHECK_THROWS_WITH_AS(
//             usuario.setCpf("123"),
//             "CPF deve conter 11 dígitos.",
//             std::invalid_argument
//         );

//         CHECK_THROWS_WITH_AS(
//             usuario.setCpf("123456789012345"),
//             "CPF deve conter 11 dígitos.",
//             std::invalid_argument
//         );
//     }

//     SUBCASE("CPF com Caracteres Nao Numericos no Setter") {
//         CHECK_THROWS_WITH_AS(
//             usuario.setCpf("1234567890A"),
//             "CPF deve conter apenas números.",
//             std::invalid_argument
//         );

//         CHECK_THROWS_WITH_AS(
//             usuario.setCpf("123.456.789-00"),
//             "CPF deve conter 11 dígitos.",
//             std::invalid_argument
//         );
//     }
// }

// TEST_CASE("3. Setter e Validacoes de TotalScore") {
//     User usuario(
//         "Maria",
//         "maria@email.com",
//         "31988888888",
//         "senha123",
//         true,
//         "Bio Maria",
//         100.0,
//         "98765432100"
//     );

//     SUBCASE("Atualizar TotalScore") {
//         usuario.setTotalScore(250.0);
//         CHECK(usuario.getTotalScore() == 250.0);
//     }

//     SUBCASE("TotalScore Zero e Valido") {
//         usuario.setTotalScore(0.0);
//         CHECK(usuario.getTotalScore() == 0.0);
//     }

//     SUBCASE("TotalScore Negativo e Invalido") {
//         CHECK_THROWS_WITH_AS(
//             usuario.setTotalScore(-50.0),
//             "Pontuação total não pode ser negativa.",
//             std::invalid_argument
//         );
//     }
// }

// TEST_CASE("4. Sistema de Pontuacao por Empresa") {
//     User usuario(
//         "Giovana",
//         "Giovana@email.com",
//         "31999999999",
//         "senhaSegura123",
//         true,
//         "Bio do João",
//         0.0,
//         "12345678900"
//     );

//     auto empresa1 = std::make_shared<Company>(
//         "Google",
//         "google@email.com",
//         "31988888888",
//         "senha123",
//         true,
//         "Empresa Tech",
//         "12345678000190"
//     );

//     auto empresa2 = std::make_shared<Company>(
//         "Sydle",
//         "Sydle@email.com",
//         "31977777777",
//         "senha123",
//         true,
//         "Empresa de Dados",
//         "98765432000111"
//     );

//     SUBCASE("Pontuacao Inicial") {
//         CHECK(usuario.getTotalScore() == 0.0);
//         CHECK(usuario.getScoreByCompany(empresa1) == 0.0);
//     }

//     SUBCASE("Adicionar Score de Uma Empresa") {
//         usuario.addScore(empresa1, 100.0);
        
//         CHECK(usuario.getTotalScore() == 100.0);
//         CHECK(usuario.getScoreByCompany(empresa1) == 100.0);
//         CHECK(usuario.getScoreByCompany(empresa2) == 0.0);
//     }

//     SUBCASE("Adicionar Score de Multiplas Empresas") {
//         usuario.addScore(empresa1, 100.0);
//         usuario.addScore(empresa2, 50.0);
        
//         CHECK(usuario.getTotalScore() == 150.0);
//         CHECK(usuario.getScoreByCompany(empresa1) == 100.0);
//         CHECK(usuario.getScoreByCompany(empresa2) == 50.0);
//     }

//     SUBCASE("Adicionar Score Multiplas Vezes na Mesma Empresa") {
//         usuario.addScore(empresa1, 50.0);
//         usuario.addScore(empresa1, 30.0);
//         usuario.addScore(empresa1, 20.0);
        
//         CHECK(usuario.getTotalScore() == 100.0);
//         CHECK(usuario.getScoreByCompany(empresa1) == 100.0);
//     }

//     SUBCASE("Validacoes ao Adicionar Score") {
//         // EMPRESA NULA
//         CHECK_THROWS_WITH_AS(
//             usuario.addScore(nullptr, 50.0),
//             "Empresa inválida (nullptr).",
//             std::invalid_argument
//         );

//         // SCORE ZERO
//         CHECK_THROWS_WITH_AS(
//             usuario.addScore(empresa1, 0.0),
//             "Score deve ser maior que zero.",
//             std::invalid_argument
//         );

//         // SCORE NEGATIVO
//         CHECK_THROWS_WITH_AS(
//             usuario.addScore(empresa1, -10.0),
//             "Score deve ser maior que zero.",
//             std::invalid_argument
//         );
//     }

//     SUBCASE("Validacao getScoreByCompany com Empresa Nula") {
//         CHECK_THROWS_WITH_AS(
//             usuario.getScoreByCompany(nullptr),
//             "Empresa inválida passada(nula).",
//             std::invalid_argument
//         );
//     }
// }

// TEST_CASE("5. Sistema de Cases Resolvidos") {
//     User usuario(
//         "Pedro",
//         "pedro@email.com",
//         "31977777777",
//         "senha123",
//         true,
//         "Bio Pedro",
//         0.0,
//         "11122233344"
//     );

//     SUBCASE("Lista Inicial Vazia") {
//         CHECK(usuario.getSolvedCases().empty() == true);
//     }

//     SUBCASE("Registrar Um Case Resolvido") {
//         usuario.registerSolvedCase(1);
        
//         std::vector<int> solved = usuario.getSolvedCases();
//         CHECK(solved.size() == 1);
//         CHECK(solved[0] == 1);
//     }

//     SUBCASE("Registrar Multiplos Cases Resolvidos") {
//         usuario.registerSolvedCase(1);
//         usuario.registerSolvedCase(5);
//         usuario.registerSolvedCase(10);
        
//         std::vector<int> solved = usuario.getSolvedCases();
//         CHECK(solved.size() == 3);
//         CHECK(solved[0] == 1);
//         CHECK(solved[1] == 5);
//         CHECK(solved[2] == 10);
//     }
// }

// TEST_CASE("6. Integracao - Pontuacao e Cases Resolvidos") {
//     User usuario(
//         "Lucia",
//         "lucia@email.com",
//         "31966666666",
//         "senha123",
//         true,
//         "Bio",
//         0.0,
//         "55566677788"
//     );

//     auto empresa1 = std::make_shared<Company>(
//         "Microsoft",
//         "microsoft@email.com",
//         "31955555555",
//         "senha123",
//         true,
//         "Empresa de softwares",
//         "11122233000144"
//     );

//     auto empresa2 = std::make_shared<Company>(
//         "Candice",
//         "candice@email.com",
//         "31944444444",
//         "senha123",
//         true,
//         "Segunda Empresa",
//         "22233344000155"
//     );

//     SUBCASE("Resolver Cases e Acumular Pontos de Uma Empresa") {
//         // Simula resolver 3 cases da empresa1
//         usuario.registerSolvedCase(1);
//         usuario.addScore(empresa1, 100.0);
        
//         usuario.registerSolvedCase(2);
//         usuario.addScore(empresa1, 150.0);
        
//         usuario.registerSolvedCase(3);
//         usuario.addScore(empresa1, 50.0);
        
//         CHECK(usuario.getSolvedCases().size() == 3);
//         CHECK(usuario.getTotalScore() == 300.0);
//         CHECK(usuario.getScoreByCompany(empresa1) == 300.0);
//     }

//     SUBCASE("Resolver Cases de Multiplas Empresas") {
//         usuario.registerSolvedCase(1);
//         usuario.addScore(empresa1, 100.0);
        
//         usuario.registerSolvedCase(2);
//         usuario.addScore(empresa2, 200.0);
        
//         usuario.registerSolvedCase(3);
//         usuario.addScore(empresa1, 50.0);
        
//         CHECK(usuario.getSolvedCases().size() == 3);
//         CHECK(usuario.getTotalScore() == 350.0);
//         CHECK(usuario.getScoreByCompany(empresa1) == 150.0);
//         CHECK(usuario.getScoreByCompany(empresa2) == 200.0);
//     }
// }