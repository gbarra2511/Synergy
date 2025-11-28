#include "../../include/backend/Case.hpp"

int Case::_next_id = 1;

Case::Case(const std::string& name, const std::string& description, const std::string& solution, const double& points) 
    : _id(_next_id), _name(name), _description(description), _solution(solution), _points(points),_state(false) {
 
        if(name.empty()) {
            throw std::invalid_argument("Nome não pode ser vazio.");
        }

        if(description.size() > 2000) {
            throw std::invalid_argument("Descrição deve ter no máximo 2000 caracteres.");
        }
    
      _next_id ++;
    }

Case::~Case() {}

int Case::getId() const {
    return this->_id;
}
const std::string& Case::getName() const {
    return this->_name;
}
const std::string& Case::getDescription() const {
    return this->_description;
}
const std::string& Case::getSolution() const {
    return this->_solution;
}
bool Case::getState() const {
    return this->_state;
}
double Case::getPoints() const {
    return this->_points;
}

void Case::setName(const std::string& name) {
    if(name.empty()) {
        throw std::invalid_argument("Nome não pode ser vazio.");
    }
    this->_name = name;
    return;
}
void Case::setDescription(const std::string& description) {
    if(description.size() > 2000) {
        throw std::invalid_argument("Descrição deve ter no máximo 2000 caracteres.");
    }
    this->_description = description;
    return;
}
void Case::setSolution(const std::string& solution) {
    this->_solution = solution;
    return;
}
void Case::setState(bool state) {
    this->_state = state;
    return;
}
void Case::setPoints(double points) {
    if(points < 0) {
        throw std::invalid_argument("Pontuação não pode ser negativa.");
    }
    this->_points = points;
}


