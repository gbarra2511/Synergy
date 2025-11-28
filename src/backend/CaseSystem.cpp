#include "../../include/backend/CaseSystem.hpp"

CaseSystem::CaseSystem() : _cases() {};

CaseSystem::~CaseSystem() {}

std::shared_ptr<Case> CaseSystem::getCase(int caseId) const {
    for (auto c : _cases) {
        if (c->getId() == caseId) {
            return c;
        }
    }
    return nullptr;
}
const std::vector<std::shared_ptr<Case>>& CaseSystem::getAllCases() const {
    return this->_cases;
}
std::vector<std::shared_ptr<Case>> CaseSystem::getActiveCases() const {
    std::vector<std::shared_ptr<Case>> ativos;
    for (const auto& c : _cases) {
        if (c && c->getState()) ativos.push_back(c);
    }
    return ativos;
}
std::vector<std::shared_ptr<Case>> CaseSystem::getInactiveCases() const {
    std::vector<std::shared_ptr<Case>> inativos;
    for (const auto& c : _cases) {
        if (c && !c->getState()) inativos.push_back(c);
    }
    return inativos;
}

std::shared_ptr<Case> CaseSystem::getCaseByName(const std::string& caseName) {
    if (caseName.empty()) {
        return nullptr;
    }
    
    for (const auto& c : _cases) {
        if (c && c->getName() == caseName) {
            return c;
        }
    }
    
    return nullptr; 
}

void CaseSystem::addCase(std::shared_ptr<Case> newCase) {
    if (newCase == nullptr) {
        throw std::invalid_argument("O ponteiro de case não pode ser inválido!");
    }

    if (getCase(newCase->getId())) {
        throw std::logic_error("Não é permitido adicionar um case com Id existente!");
    }

    _cases.push_back(newCase);
}
// void CaseSystem::removeCase(int caseId) {
//     for (auto it = _cases.begin(); it != _cases.end(); ++it) {
//         if (*it && (*it)->getId() == caseId) {
//             _cases.erase(it);
//             return;
//         }
//     }
//     throw std::invalid_argument("Id do case não encontrado para ser removido!");
// }
void CaseSystem::removeCaseByName(const std::string& caseName) {
    if (caseName.empty()) {
        throw std::invalid_argument("Nome do case não pode ser vazio!");
    }
    
    for (auto it = _cases.begin(); it != _cases.end(); ++it) {
        if (*it && (*it)->getName() == caseName) {
            _cases.erase(it);
            return;
        }
    }
}
void CaseSystem::deactivateCase(int caseId) {
    auto c = getCase(caseId);
    if (c == nullptr) {
        throw std::invalid_argument("Id do case não encontrado para ser desativado!");
    }
    c->setState(false);
}
void CaseSystem::activateCase(int caseId) {
    auto c = getCase(caseId);
    if (c == nullptr) {
        throw std::invalid_argument("Id do case não encontrado para ser ativado!");
        return;
    }
    c->setState(true);
}
