#include "doctest.h"
#include "Entity.hpp"
#include "Company.hpp"
#include "Case.hpp"
#include <memory>
#include <vector>

TEST_CASE("1. Criacao de company e validacoes") {
    SUBCASE("Criacao de company") {
        std::shared_ptr<Company> empresa;
        
        empresa = std::make_shared<Company>(
            "Google",                  //Name
            "google@email.com",        //Email
            "31987654321",             // Phone_Number
            "senhasegura",             // Password
            true,                      // State
            "Empresa de tecnologia",   // Description
            "12345678000190");          //CPNJ

        // GETTERS ESPECIFICOS DA COMPANY
        CHECK(empresa->getCnpj() == "12345678000190");
        CHECK(empresa->getRanking() != nullptr);

        // GETTERS DO NETWORK
        CHECK(empresa->getFollowers().empty() == true);
        CHECK(empresa->getFollowing().empty() == true);

        // GETTERS DE CASES
        CHECK(empresa->getAllCases().empty() == true);
        CHECK(empresa->getActiveCases().empty() == true);
        CHECK(empresa->getInactiveCases().empty() == true);
    }
    SUBCASE("Validacoes Especificas do CNPJ") {
        // CNPJ VAZIO
        CHECK_THROWS_WITH_AS(
            Company("Empresa", "empresa@email.com", "31999999999", "senha123", true, "Bio", ""),
            "CNPJ não pode ser vazio.",
            std::invalid_argument
        );

        // CNPJ COM TAMANHO INCORRETO
        CHECK_THROWS_WITH_AS(
            Company("Empresa", "empresa@email.com", "31999999999", "senha123", true, "Bio", "123456"),
            "CNPJ deve conter 14 dígitos na formatação XX.XXX.XXX/XXXX-XX.",
            std::invalid_argument
        );

        CHECK_THROWS_WITH_AS(
            Company("Empresa", "empresa@email.com", "31999999999", "senha123", true, "Bio", "123456789012345"),
            "CNPJ deve conter 14 dígitos na formatação XX.XXX.XXX/XXXX-XX.",
            std::invalid_argument
        );

        // CNPJ COM CARACTERES NAO NUMERICOS
        CHECK_THROWS_WITH_AS(
            Company("Empresa", "empresa@email.com", "31999999999", "senha123", true, "Bio", "1234567800019A"),
            "CNPJ deve conter apenas números.",
            std::invalid_argument
        );

        CHECK_THROWS_WITH_AS(
            Company("Empresa", "empresa@email.com", "31999999999", "senha123", true, "Bio", "12.345.678/0001-90"),
            "CNPJ deve conter 14 dígitos na formatação XX.XXX.XXX/XXXX-XX.",
            std::invalid_argument
        );
    }
}

TEST_CASE("2. Setters da Company") {
    Company empresa(
        "Sydle", 
        "sydle@email.com", 
        "31999999999", 
        "senhacripto", 
        true, 
        "Bio Original", 
        "12345678000190");
    
    SUBCASE("Atualizando CNPJ") {
        empresa.setCnpj("98765432000111");
        CHECK(empresa.getCnpj() == "98765432000111");
    }
    
    SUBCASE("Validando Erros no Setter de CNPJ") {
        // CNPJ VAZIO
        CHECK_THROWS_WITH_AS(
            empresa.setCnpj(""),
            "CNPJ não pode ser vazio.",
            std::invalid_argument
        );

        // CNPJ COM TAMANHO INCORRETO
        CHECK_THROWS_WITH_AS(
            empresa.setCnpj("123"),
            "CNPJ deve conter 14 dígitos na formatação XX.XXX.XXX/XXXX-XX.",
            std::invalid_argument
        );

        CHECK_THROWS_WITH_AS(
            empresa.setCnpj("123456789012345"),
            "CNPJ deve conter 14 dígitos na formatação XX.XXX.XXX/XXXX-XX.",
            std::invalid_argument
        );
    }
}

TEST_CASE("3. Sistema de Seguidores (Followers/Following)") {
    Company empresaA(
        "Company A", 
        "companya@email.com", 
        "31991111111", 
        "senha123", 
        true, 
        "Empresa A", 
        "11111111000111");

    Company empresaB(
        "Company B", 
        "companyb@email.com", 
        "31992222222", 
        "senha123", 
        true, 
        "Empresa B", 
        "22222222000122");

    SUBCASE("Adicionar seguidores") {
        // A Começa a Seguir B
        empresaA.addFollowing(empresaB.getName()); 
        empresaB.addFollower(empresaA.getName());

        // Verifica lista de 'Seguindo' da Empresa A
        std::vector<std::string> followingA = empresaA.getFollowing();
        CHECK(followingA.size() == 1);
        CHECK(followingA[0] == "Company B");

        // Verifica lista de 'Seguidores' da Empresa B
        std::vector<std::string> followersB = empresaB.getFollowers();
        CHECK(followersB.size() == 1);
        CHECK(followersB[0] == "Company A");
    }

    SUBCASE("Remover seguidores") {
        // Momento Inicial
        empresaA.addFollowing("Company B");
        empresaB.addFollower("Company A");

        // Removendo
        empresaA.removeFollowing("Company B");
        empresaB.removeFollower("Company A");

        // Checando a Remoção
        CHECK(empresaA.getFollowing().size() == 0);
        CHECK(empresaB.getFollowers().size() == 0);
    }
}