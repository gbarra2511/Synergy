#include "../../include/backend/Entity.hpp"

int Entity::_num_entities = 0;
int Entity::_next_id = 1;

Entity::Entity(const std::string& name, const std::string& email,
                const std::string& phone_number, const std::string& password, bool state, const std::string& description)
    : 
      _id(_next_id),
      _name(name),
      _email(email),
      _phone_number(phone_number),
      _password(password),
      _state(state),
      _description(description)
{
    if(name.empty()) {
        throw std::invalid_argument("Nome não pode ser vazio.");
    }

    if(password.size() < 8) {
        throw std::invalid_argument("Senha deve ter pelo menos 8 caracteres.");
    }

    if(description.size() > 500) {
        throw std::invalid_argument("Descrição deve ter no máximo 500 caracteres.");
    }

    _next_id++;
    _num_entities++;
}

Entity::~Entity() {
    _num_entities--;
}

std::string Entity::getName() const {
    return this->_name;
}
std::string Entity::getEmail() const {
    return this->_email;
}
std::string Entity::getPhoneNumber() const {
    return this->_phone_number;
}
bool Entity::getState() const {
    return this->_state;
}
int Entity::getId() const { 
    return this->_id;
}
std::vector<std::string>& Entity::getFollowing()  {
    return this->_following;
}
std::string Entity::getDescription() const {
    return this->_description;
}
std::vector<std::string>& Entity::getFollowers()  {
    return this->_followers;
}
std::string Entity::getPassword() const {
    return this->_password;
}
void Entity::setName(const std::string& name) {
    if(name.empty()) {
        throw std::invalid_argument("Nome não pode ser vazio");
    }
    this->_name = name;
}
void Entity::setEmail(const std::string& email) {
    const std::regex email_regex(R"((\w+)(\.?\w*)@(\w+)(\.\w+)+)");
    if (!std::regex_match(email, email_regex)) {
        throw std::invalid_argument("Email inválido.");
    }
    this->_email = email;
}
void Entity::setPhoneNumber(const std::string& phone_number) {
    const std::regex phone_regex("^(?:\\([1-9]{2}\\)|[1-9]{2}) ?9?[0-9]{4}-?[0-9]{4}$");
    if (!std::regex_match(phone_number, phone_regex)) {
        throw std::invalid_argument("Número de telefone inválido.");
    }
    _phone_number = phone_number;
}
void Entity::setState(bool state) {
    this->_state = state;
}
void Entity::setDescription(const std::string& description) {
    if(description.size() > 500) {
        throw std::invalid_argument("Descrição deve ter no máximo 500 caracteres.");
    }
    this->_description = description;
}
void Entity::setPassword(const std::string& password){
    if(password.size() < 8) {
        throw std::invalid_argument("Senha deve ter pelo menos 8 caracteres.");
    }
    this->_password = password;
  }
bool Entity::checkPassword(const std::string& password_to_verify) const {
    return this->_password == password_to_verify; 
    // Talvez colocar como um Hash dps
}

void Entity::addFollower(const std::string& username) {
    _followers.push_back(username);
}

void Entity::addFollowing(const std::string& username) {
    _following.push_back(username);
}

void Entity::removeFollower(const std::string& username) {
  for (size_t i = 0; i < _followers.size();)
  {
    if (_followers[i] == username)
    {
      _followers.erase(_followers.begin() + i);
    }
    else
    {
      i ++;
    }
  }
}

void Entity::removeFollowing(const std::string& username) {
  for (size_t i = 0; i < _following.size();)
  {
    if (_following[i] == username)
    {
      _following.erase(_following.begin() + i);
    }
    else
    {
      i ++;
    }
  }
}

std::ostream& operator<<(std::ostream& os, const Entity& entity) {
    os << "ID: " << entity.getId() << std::endl;
    os << "Name: " << entity.getName() << std::endl;
    os << "Email: " << entity.getEmail() << std::endl;
    os << "Phone: " << entity.getPhoneNumber() << std::endl;
    os << "Description: " << entity.getDescription() << std::endl;
    os << "State: " << (entity.getState() ? "Active" : "Inactive") << std::endl;
    return os;
}
