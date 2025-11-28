#include "doctest.h"
#include "Entity.hpp"
#include "User.hpp"

TEST_CASE("1. Criacao de Entidade e Validacoes") {
    SUBCASE("Criacao do User") {
        User usuario(
            "Joao",                 // Name
            "joao@email.com",       // Email
            "31999999999",       // Phone_Number
            "senhaSegura123",       // Password
            true,                   // State
            "Bio do Joao",          // Description
            0.0,                  // TotalScore 
            "12345678900"        // CPF 
        );
        
        // GETTERS PRINCIPAIS
        CHECK(usuario.getName() == "Joao");
        CHECK(usuario.getEmail() == "joao@email.com");
        CHECK(usuario.getPhoneNumber() == "31999999999");
        CHECK(usuario.getDescription() == "Bio do Joao"); 

        // GETTERS SISTEMA
        CHECK(usuario.getState() == true);
        CHECK(usuario.getPassword() == "senhaSegura123");
        CHECK(usuario.getId() > 0);

        // GETTERS ESPECIFICOS DO USER
        CHECK(usuario.getTotalScore() == 0.0);
        CHECK(usuario.getCpf() == "12345678900");

        // GETTERS DO NETWORK
        CHECK(usuario.getFollowers().empty() == true);
        CHECK(usuario.getFollowing().empty() == true);
    }

    SUBCASE("Validacoes Herdadas da Entity") {
        // NOME VAZIO
        CHECK_THROWS_WITH_AS(
            User("", "t@t.com", "31999999999", "senha123", true, "Bio", 0.0, "12345678900"), 
            "Nome não pode ser vazio.", 
            std::invalid_argument
        );

        // EMAIL INVALIDO
        // CHECK_THROWS_AS(
        //     User("Joao", "email_invalid0.com", "31999999999", "senha123", true, "Bio", 0.0, "12345678900"), 
        //     std::invalid_argument
        // );

        // CHECK_THROWS_AS(
        //     User("Joao", "email_invalid0@blabla", "31999999999", "senha123", true, "Bio", 0.0, "12345678900"), 
        //     std::invalid_argument
        // );

        // --- NUMERO INVALIDO ---

        // NUMERO CURTO
        // CHECK_THROWS_WITH_AS(
        //     User("Joao", "joao@email.com", "123", "senha123", true, "Bio", 0.0, "12345678900"),
        //     "Número de telefone inválido.",
        //     std::invalid_argument
        // );

        // NUMERO C/ LETRA
        // CHECK_THROWS_WITH_AS(
        //     User("Joao", "joao@email.com", "telefone_errado", "senha123", true, "Bio", 0.0, "12345678900"),
        //     "Número de telefone inválido.",
        //     std::invalid_argument
        // );

        // DESCRIÇÃO INVALIDA (>500)
        std::string descricao_gigante(501, 'a');

        CHECK_THROWS_WITH_AS(
            User("Joao", "joao@email.com", "3199999-9999", "senha123",true, descricao_gigante, 0.0, "12345678900"),
            "Descrição deve ter no máximo 500 caracteres.", 
            std::invalid_argument
        );

        // SENHA FRACA
        CHECK_THROWS_WITH_AS(
            User("Joao", "t@t.com", "31999999999", "123", true, "Bio", 0.0, "12345678900"),
            "Senha deve ter pelo menos 8 caracteres.",
            std::invalid_argument
        );
    }
}

TEST_CASE("2. Setters da Entity") {
    User usuario(
        "Original", 
        "orig@email.com", 
        "31999999999", 
        "pass1234", 
        true, 
        "Desc", 
        0.0, 
        "12345678900");

    SUBCASE("Atualizando os Dados") {
        usuario.setName("Novo Nome");
        usuario.setEmail("novo@email.com");
        usuario.setPhoneNumber("31999999998");
        usuario.setPassword("1234pass");
        usuario.setState(false);
        usuario.setDescription("Desc Nova");
        
        CHECK(usuario.getName() == "Novo Nome");
        CHECK(usuario.getEmail() == "novo@email.com");
        CHECK(usuario.getPhoneNumber() == "31999999998");
        CHECK(usuario.getPassword() == "1234pass");
        CHECK(usuario.getState() == false);
        CHECK(usuario.getDescription() == "Desc Nova");
    }
    
    SUBCASE("Validando Erros no Setters") {
        // NOME VAZIO
        CHECK_THROWS_WITH_AS(
            usuario.setName(""), 
            "Nome não pode ser vazio", 
            std::invalid_argument
        ); 

        // EMAIL INVALIDO
        CHECK_THROWS_WITH_AS(
            usuario.setEmail("email_sem_arroba.com"), 
            "Email inválido.", 
            std::invalid_argument
        );

        CHECK_THROWS_WITH_AS(
            usuario.setEmail("email@incompleto"), 
            "Email inválido.", 
            std::invalid_argument
        );

        // TELEFONE INVALIDO
        CHECK_THROWS_WITH_AS(
            usuario.setPhoneNumber("abcdefghijk"), 
            "Número de telefone inválido.", 
            std::invalid_argument
        );

        CHECK_THROWS_WITH_AS(
            usuario.setPhoneNumber("123"), 
            "Número de telefone inválido.", 
            std::invalid_argument
        );

        // DESC GRANDE
        std::string descricao_gigante(501, 'a');
        
        CHECK_THROWS_WITH_AS(
            usuario.setDescription(descricao_gigante), 
            "Descrição deve ter no máximo 500 caracteres.", 
            std::invalid_argument
        );

        // SENHA PEQUENA
        CHECK_THROWS_WITH_AS(
            usuario.setPassword("curta"), 
            "Senha deve ter pelo menos 8 caracteres.", 
            std::invalid_argument
        );
    }
}

TEST_CASE("3. Verificacao de Senha (checkPassword)") {
    User usuario(
        "Invasor", 
        "invasor@email.com", 
        "1199999-9999", 
        "senhaSecreta123", 
        true, 
        "Bio", 
        0.0, 
        "11122233344");

    SUBCASE("Senha correta") {
        CHECK(usuario.checkPassword("senhaSecreta123") == true);
    }

    SUBCASE("Senha incorreta") {
        CHECK(usuario.checkPassword("senhaErrada") == false);
        CHECK(usuario.checkPassword("SENHASECRETA123") == false); 
        CHECK(usuario.checkPassword("") == false);
    }
}

TEST_CASE("4. Sistema de Seguidores (Followers/Following)") {
    User userA(
        "Alice", 
        "alice@email.com", 
        "1191111-1111", 
        "senha123", 
        true, 
        "A", 
        0.0, 
        "11111111111");

    User userB(
        "Bruno", 
        "bruno@email.com", 
        "1192222-2222", 
        "senha123", 
        true, 
        "B", 
        0.0, 
        "22222222222");

    SUBCASE("Adicionar seguidores") {
        // A Começa a Seguir B
        
        userA.addFollowing(userB.getName()); 
        userB.addFollower(userA.getName());

        // Verifica lista de 'Seguindo' da Alice
        std::vector<std::string> followingA = userA.getFollowing();
        CHECK(followingA.size() == 1);
        CHECK(followingA[0] == "Bruno");

        // Verifica lista de 'Seguidores' do Bruno
        std::vector<std::string> followersB = userB.getFollowers();
        CHECK(followersB.size() == 1);
        CHECK(followersB[0] == "Alice");
    }

    SUBCASE("Remover seguidores") {
        // Momento Iniciaç
        userA.addFollowing("Bruno");
        userB.addFollower("Alice");

        // Removendo
        userA.removeFollowing("Bruno");
        userB.removeFollower("Alice");

        // Checkando a Remoção
        CHECK(userA.getFollowing().size() == 0);
        CHECK(userB.getFollowers().size() == 0);
    }
}
