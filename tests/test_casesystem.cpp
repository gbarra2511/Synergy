#include "doctest.h"
#include "CaseSystem.hpp"
#include "Case.hpp"
#include <memory>

TEST_CASE("1. Criacao do CaseSystem") {
    CaseSystem sistema;

    SUBCASE("Sistema inicia vazio") {
        CHECK(sistema.getAllCases().empty() == true);
        CHECK(sistema.getActiveCases().empty() == true);
        CHECK(sistema.getInactiveCases().empty() == true);
    }
}

TEST_CASE("2. Adicao de cases") {
    CaseSystem sistema;

    auto case1 = std::make_shared<Case>("Case 1", "Descricao 1", "solucao1", 50.0);
    auto case2 = std::make_shared<Case>("Case 2", "Descricao 2", "solucao2", 100.0);

    SUBCASE("Adicionar case valido") {
        sistema.addCase(case1);
        
        CHECK(sistema.getAllCases().size() == 1);
        CHECK(sistema.getAllCases()[0]->getId() == case1->getId());
    }

    SUBCASE("Adicionar multiplos cases") {
        sistema.addCase(case1);
        sistema.addCase(case2);
        
        CHECK(sistema.getAllCases().size() == 2);
        CHECK(sistema.getAllCases()[0]->getId() == case1->getId());
        CHECK(sistema.getAllCases()[1]->getId() == case2->getId());
    }

    SUBCASE("Adicionar case nulo") {
        CHECK_THROWS_WITH_AS(
            sistema.addCase(nullptr),
            "O ponteiro de case não pode ser inválido!",
            std::invalid_argument
        );
    }

    SUBCASE("Adicionar case com ID duplicado") {
        sistema.addCase(case1);
        
        // Tenta adicionar de novo o mesmo case
        CHECK_THROWS_WITH_AS(
            sistema.addCase(case1),
            "Não é permitido adicionar um case com Id existente!",
            std::logic_error
        );
    }
}

TEST_CASE("3. Buscar case por ID (getCase)") {
    CaseSystem sistema;

    auto case1 = std::make_shared<Case>("Case 1", "Descricao 1", "solucao1", 50.0);
    auto case2 = std::make_shared<Case>("Case 2", "Descricao 2", "solucao2", 100.0);
    
    sistema.addCase(case1);
    sistema.addCase(case2);

    SUBCASE("Buscar case existente") {
        auto resultado = sistema.getCase(case1->getId());
        
        CHECK(resultado != nullptr);
        CHECK(resultado->getId() == case1->getId());
        CHECK(resultado->getName() == "Case 1");
    }

    SUBCASE("Buscar case inexistente") {
        auto resultado = sistema.getCase(99999);
        
        CHECK(resultado == nullptr);
    }

    SUBCASE("Buscar multiplos cases") {
        auto resultado1 = sistema.getCase(case1->getId());
        auto resultado2 = sistema.getCase(case2->getId());
        
        CHECK(resultado1->getId() == case1->getId());
        CHECK(resultado2->getId() == case2->getId());
    }
}

TEST_CASE("4. Remocao de Cases") {
    CaseSystem sistema;

    auto case1 = std::make_shared<Case>("Case 1", "Descricao 1", "solucao1", 50.0);
    auto case2 = std::make_shared<Case>("Case 2", "Descricao 2", "solucao2", 100.0);
    auto case3 = std::make_shared<Case>("Case 3", "Descricao 3", "solucao3", 150.0);

    SUBCASE("Remover case existente") {
        sistema.addCase(case1);
        sistema.addCase(case2);
        
        CHECK(sistema.getAllCases().size() == 2);
        
        sistema.removeCase(case1->getId());
        
        CHECK(sistema.getAllCases().size() == 1);
        CHECK(sistema.getCase(case1->getId()) == nullptr);
        CHECK(sistema.getCase(case2->getId()) != nullptr);
    }

    SUBCASE("Remover case do meio") {
        sistema.addCase(case1);
        sistema.addCase(case2);
        sistema.addCase(case3);
        
        sistema.removeCase(case2->getId());
        
        CHECK(sistema.getAllCases().size() == 2);
        CHECK(sistema.getCase(case1->getId()) != nullptr);
        CHECK(sistema.getCase(case2->getId()) == nullptr);
        CHECK(sistema.getCase(case3->getId()) != nullptr);
    }

    SUBCASE("Remover case inexistente") {
        sistema.addCase(case1);
        
        CHECK_THROWS_WITH_AS(
            sistema.removeCase(99999),
            "Id do case não encontrado para ser removido!",
            std::invalid_argument
        );
    }

    SUBCASE("Remover de sistema vazio") {
        CHECK_THROWS_WITH_AS(
            sistema.removeCase(1),
            "Id do case não encontrado para ser removido!",
            std::invalid_argument
        );
    }
}

TEST_CASE("5. Ativacao e Desativacao de Cases") {
    CaseSystem sistema;

    auto case1 = std::make_shared<Case>("Case 1", "Descricao 1", "solucao1", 50.0);
    auto case2 = std::make_shared<Case>("Case 2", "Descricao 2", "solucao2", 100.0);

    sistema.addCase(case1);
    sistema.addCase(case2);

    SUBCASE("Cases iniciam inativos") {
        CHECK(case1->getState() == false);
        CHECK(case2->getState() == false);
    }

    SUBCASE("Ativar case") {
        sistema.activateCase(case1->getId());
        
        CHECK(case1->getState() == true);
        CHECK(case2->getState() == false);
    }

    SUBCASE("Desativar Case") {
        sistema.activateCase(case1->getId());
        CHECK(case1->getState() == true);
        
        sistema.deactivateCase(case1->getId());
        CHECK(case1->getState() == false);
    }

    SUBCASE("Ativar case inexistente") {
        CHECK_THROWS_WITH_AS(
            sistema.activateCase(99999),
            "Id do case não encontrado para ser ativado!",
            std::invalid_argument
        );
    }

    SUBCASE("Desativar case inexistente") {
        CHECK_THROWS_WITH_AS(
            sistema.deactivateCase(99999),
            "Id do case não encontrado para ser desativado!",
            std::invalid_argument
        );
    }
}

TEST_CASE("6. Filtrar Cases Ativos e Inativos") {
    CaseSystem sistema;

    auto case1 = std::make_shared<Case>("Case 1", "Descricao 1", "solucao1", 50.0);
    auto case2 = std::make_shared<Case>("Case 2", "Descricao 2", "solucao2", 100.0);
    auto case3 = std::make_shared<Case>("Case 3", "Descricao 3", "solucao3", 150.0);

    sistema.addCase(case1);
    sistema.addCase(case2);
    sistema.addCase(case3);

    SUBCASE("Todos inativos inicialmente") {
        CHECK(sistema.getActiveCases().size() == 0);
        CHECK(sistema.getInactiveCases().size() == 3);
    }

    SUBCASE("Um case ativo") {
        sistema.activateCase(case1->getId());
        
        CHECK(sistema.getActiveCases().size() == 1);
        CHECK(sistema.getInactiveCases().size() == 2);
        
        CHECK(sistema.getActiveCases()[0]->getId() == case1->getId());
    }

    SUBCASE("Multiplos cases ativos") {
        sistema.activateCase(case1->getId());
        sistema.activateCase(case2->getId());
        
        CHECK(sistema.getActiveCases().size() == 2);
        CHECK(sistema.getInactiveCases().size() == 1);
    }

    SUBCASE("Todos cases ativos") {
        sistema.activateCase(case1->getId());
        sistema.activateCase(case2->getId());
        sistema.activateCase(case3->getId());
        
        CHECK(sistema.getActiveCases().size() == 3);
        CHECK(sistema.getInactiveCases().size() == 0);
    }

    SUBCASE("Alternando estados") {
        sistema.activateCase(case1->getId());
        sistema.activateCase(case2->getId());
        
        CHECK(sistema.getActiveCases().size() == 2);
        
        sistema.deactivateCase(case1->getId());
        
        CHECK(sistema.getActiveCases().size() == 1);
        CHECK(sistema.getInactiveCases().size() == 2);
        
        CHECK(sistema.getActiveCases()[0]->getId() == case2->getId());
    }
}

TEST_CASE("7. getAllCases retorna todos os cases") {
    CaseSystem sistema;

    auto case1 = std::make_shared<Case>("Case 1", "Descricao 1", "solucao1", 50.0);
    auto case2 = std::make_shared<Case>("Case 2", "Descricao 2", "solucao2", 100.0);
    auto case3 = std::make_shared<Case>("Case 3", "Descricao 3", "solucao3", 150.0);

    SUBCASE("Sistema vazio") {
        CHECK(sistema.getAllCases().size() == 0);
    }

    SUBCASE("Com cases inativos") {
        sistema.addCase(case1);
        sistema.addCase(case2);
        
        CHECK(sistema.getAllCases().size() == 2);
    }

    SUBCASE("Com cases ativos e inativos misturados") {
        sistema.addCase(case1);
        sistema.addCase(case2);
        sistema.addCase(case3);
        
        sistema.activateCase(case1->getId());
        sistema.activateCase(case3->getId());
        
        // getAllCases deve retornar TODOS, independente do estado
        CHECK(sistema.getAllCases().size() == 3);
    }
}

TEST_CASE("8. Integracao - Operacoes Combinadas") {
    CaseSystem sistema;

    auto case1 = std::make_shared<Case>("Case A", "Desc A", "sol_a", 100.0);
    auto case2 = std::make_shared<Case>("Case B", "Desc B", "sol_b", 200.0);
    auto case3 = std::make_shared<Case>("Case C", "Desc C", "sol_c", 300.0);

    SUBCASE("Adicionar, ativar e remover") {
        sistema.addCase(case1);
        sistema.addCase(case2);
        sistema.addCase(case3);
        
        sistema.activateCase(case1->getId());
        sistema.activateCase(case2->getId());
        
        CHECK(sistema.getActiveCases().size() == 2);
        
        sistema.removeCase(case1->getId());
        
        CHECK(sistema.getAllCases().size() == 2);
        CHECK(sistema.getActiveCases().size() == 1);
        CHECK(sistema.getActiveCases()[0]->getId() == case2->getId());
    }

    SUBCASE("Ativar, desativar e buscar") {
        sistema.addCase(case1);
        
        sistema.activateCase(case1->getId());
        CHECK(sistema.getCase(case1->getId())->getState() == true);
        
        sistema.deactivateCase(case1->getId());
        CHECK(sistema.getCase(case1->getId())->getState() == false);
    }
}