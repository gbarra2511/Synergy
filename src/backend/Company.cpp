#include "../../include/backend/Company.hpp"
#include "../../include/backend/CaseSystem.hpp" 
#include "../../include/backend/Ranking.hpp"
#include <stdexcept>

Company::Company(const std::string& name, const std::string& email,
          const std::string& phone_number, const std::string& password, 
          bool state, const std::string& description, const std::string& cnpj)
    : Entity(name, email, phone_number, password, state, description), _cnpj(cnpj), _companyRanking(new Ranking()), _companyCaseSystem(new CaseSystem) {
        if(cnpj.empty()) {
            throw std::invalid_argument("CNPJ não pode ser vazio.");
        }
        if(cnpj.size() != 14) {
            throw std::invalid_argument("CNPJ deve conter 14 dígitos na formatação XX.XXX.XXX/XXXX-XX.");
        }
        if(!std::all_of(cnpj.begin(), cnpj.end(), ::isdigit)) {
            throw std::invalid_argument("CNPJ deve conter apenas números.");
        }
    }

Company::~Company() {}

std::shared_ptr<Case> Company::getCase(int caseId) const {
    if(caseId < 0) {
        throw std::out_of_range("ID do case inválido.");
    }
    if(!_companyCaseSystem->getCase(caseId)) {
        throw std::runtime_error("Case não encontrado.");
    }
    return _companyCaseSystem->getCase(caseId);
}
std::vector<std::shared_ptr<Case>> Company::getActiveCases() const {
    return _companyCaseSystem->getActiveCases();
}
std::vector<std::shared_ptr<Case>> Company::getInactiveCases() const {
    return _companyCaseSystem->getInactiveCases();
}
const std::vector<std::shared_ptr<Case>>& Company::getAllCases() {
    return _companyCaseSystem->getAllCases();
}
std::string Company::getCnpj() const {
    return this->_cnpj; 
}
Ranking* Company::getRanking() {
    return _companyRanking;
}


void Company::setCnpj(const std::string& cnpj) {
    if(cnpj.empty()) {
        throw std::invalid_argument("CNPJ não pode ser vazio.");
    }
    if(cnpj.size() != 14) {
        throw std::invalid_argument("CNPJ deve conter 14 dígitos na formatação XX.XXX.XXX/XXXX-XX.");
    }
    if(!std::all_of(cnpj.begin(), cnpj.end(), ::isdigit)) {
        throw std::invalid_argument("CNPJ deve conter apenas números.");
    }
    this->_cnpj = cnpj;
    return;
}

void Company::addCase(const std::shared_ptr<Case>& c) {
    _companyCaseSystem->addCase(c);
}
void Company::removeCaseByName(const std::string& caseName) {
    if (caseName.empty()) {
        throw std::invalid_argument("Nome do case não pode ser vazio!");
    }
    _companyCaseSystem->removeCaseByName(caseName);
}
void Company::activateCase(int caseId) {
    if(caseId < 0) {
        throw std::out_of_range("ID do case não pode ser negativo.");
    }
    _companyCaseSystem->activateCase(caseId);
}
void Company::deactivateCase(int caseId) {
    if(caseId < 0) {
        throw std::out_of_range("ID do case não pode ser negativo.");
    }
    _companyCaseSystem->deactivateCase(caseId);
}
//serve para manipulação com o banco de dados, caso contrario use getAllCase
CaseSystem& Company::getCaseSystem() {
        return *_companyCaseSystem;
}

void Company::displayProfile() {}

std::ostream& operator<<(std::ostream& os, const Company& company) {
    os << "=== COMPANY PROFILE ===" << std::endl;
    os << "ID: " << company._id << std::endl;
    os << "Name: " << company._name << std::endl;
    os << "Email: " << company._email << std::endl;
    os << "Phone: " << company._phone_number << std::endl;
    os << "Description: " << company._description << std::endl;
    os << "State: " << (company._state ? "Active" : "Inactive") << std::endl;
    os << "Followers: " << company._followers.size() << std::endl;
    os << "Following: " << company._following.size() << std::endl;
    return os;
}