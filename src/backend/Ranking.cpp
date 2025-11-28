#include "../../include/backend/Ranking.hpp"
#include <stdexcept>

Ranking::Ranking() : _users() {}


std::vector<std::shared_ptr<User>>& Ranking::getUsers(){
    return this->_users;
}
int Ranking::getSize() const{
    return this->_users.size();
}


void Ranking::addUser(std::shared_ptr<User> user) {
    if (user == nullptr) {
        throw std::invalid_argument("Usuário não pode ser inválido!");
    }
    for(const auto& u : _users) {
        if(u == user) {
            throw std::runtime_error("Usuário já existe no ranking.");
        }
    }
    this->_users.push_back(user);
}
void Ranking::sort() {
    for(const auto& user : _users){
        if(!user) {
            throw std::runtime_error("Ranking contém um usuário inválido(nulo). Impossível ordenar.");
        }
    }
    std::sort(_users.begin(), _users.end(),
            [](const std::shared_ptr<User>& a, const std::shared_ptr<User>& b) {
                return a->getTotalScore() > b->getTotalScore();
            }
        );
}


void Ranking::display() const {}
