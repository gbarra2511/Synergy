#include "../../include/backend/User.hpp"
#include "../../include/backend/Company.hpp"

User::User(const std::string& name, const std::string& email,
        const std::string& phone_number, const std::string& password, bool state,
        const std::string& description, double totalScore, const std::string& cpf)
    : Entity(name, email, phone_number, password, state, description), _totalScore(totalScore), _cpf(cpf){
        if(cpf.empty()) {
            throw std::invalid_argument("CPF não pode ser vazio.");
        }
        if(cpf.size() != 11) {
            throw std::invalid_argument("CPF deve conter 11 dígitos.");
        }
        if(!std::all_of(cpf.begin(), cpf.end(), ::isdigit)) {
            throw std::invalid_argument("CPF deve conter apenas números.");
        }
    }

std::string User::getCpf() const {
    return this->_cpf; 
}

 double User::getTotalScore() const { 
    return _totalScore; 
}

double User::getScoreByCompany(std::shared_ptr<Company> company) const {
    if(!company) {
        throw std::invalid_argument("Empresa inválida passada(nula).");
    }
    
    auto cs = _companyScores.find(company);
    if (cs != _companyScores.end()) {
        return cs->second;
    }
    return 0.0;
}

std::vector<int>& User::getSolvedCases() {
    return this->_solvedCases;
}

const std::vector<int>& User::getSolvedCases() const {
    return this->_solvedCases;
}

void User::setTotalScore(double score) {
    if(score < 0) {
        throw std::invalid_argument("Pontuação total não pode ser negativa.");
    }
    _totalScore = score;
}

void User::addScore(std::shared_ptr<Company> company, double score) {
    if (!company) {
        throw std::invalid_argument("Empresa inválida (nullptr).");
    }
    if (score <= 0.0) {
        throw std::invalid_argument("Score deve ser maior que zero.");
    }

    _companyScores[company] += score;
    _totalScore += score;
}

void User::setCpf(const std::string& cpf) {
    if(cpf.empty()) {
        throw std::invalid_argument("CPF não pode ser vazio.");
    }
    if(cpf.size() != 11) {
        throw std::invalid_argument("CPF deve conter 11 dígitos.");
    }
    if(!std::all_of(cpf.begin(), cpf.end(), ::isdigit)) {
        throw std::invalid_argument("CPF deve conter apenas números.");
    }
    this->_cpf = cpf;
    return;
}

void User::registerSolvedCase(int caseId) {
    _solvedCases.push_back(caseId);
}

void User::displayProfile() {
  std::cout << "=== Perfil do Usuário ===\n";
  std::cout << "Nome: " << _name << "\n";
  std::cout << "Email: " << _email << "\n";
  std::cout << "Estado: " << (_state ? "Ativo" : "Inativo") << "\n\n";

  std::cout << "--- Pontuação por Empresa ---\n";
  if (_companyScores.empty()) {
      std::cout << "Nenhuma pontuação registrada.\n";
  } else {
      for (const auto& it : _companyScores) {
          if (it.first) {
              std::cout << it.first->getName() << ": " << it.second << " pontos\n";
          }
      }
  }
  std::cout << "Pontuação Total: " << _totalScore << " pontos\n\n";

  std::cout << "--- Casos Resolvidos ---\n";
  if (_solvedCases.empty()) {
      std::cout << "Nenhum caso resolvido ainda.\n";
  } else {
      for (int id : _solvedCases) {
          std::cout << "Caso ID: " << id << "\n";
      }
  }
  std::cout << "============================\n";
}

std::ostream& operator<<(std::ostream& os, const User& user) {
    os << "ID: " << user.getId() << std::endl;
    os << "Name: " << user.getName() << std::endl;
    os << "Email: " << user.getEmail() << std::endl;
    os << "Phone: " << user.getPhoneNumber() << std::endl;
    os << "Description: " << user.getDescription() << std::endl;
    os << "State: " << (user.getState() ? "Active" : "Inactive") << std::endl;
    os << "CPF: " << user.getCpf() << std::endl;
    os << "Total Score: " << user.getTotalScore() << std::endl;
    return os;
}
