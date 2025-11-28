#include "../../include/backend/Post.hpp"

#include <algorithm>
#include <iostream>

int Post::_next_id = 1;

Post::Post(const std::string& author, const std::string& description)
  : _id(_next_id), _author(author), _description(description), _timestamp(time(nullptr)) {
    
    if (author.empty()) {
      throw std::invalid_argument("Autor do post não pode ser vazio.");
    }
    
    if (description.empty()) {
      throw std::invalid_argument("Conteúdo do post não pode ser vazio.");
    }
    
    if(description.size() > 1000) {
      throw std::invalid_argument("Descrição deve ter no máximo 1000 caracteres.");
    }

    _next_id++;
  }


int Post::getID() const
{
  return _id;
}

std::string Post::getAuthor() const
{
  return _author; 
}

std::string Post::getDescription() const
{
  return _description; 
}

std::time_t Post::getTimestamp() const
{
  return _timestamp; 
}

std::vector<Comment> Post::getComments() const 
{
  return _comments; 
}

std::vector<std::string> Post::getLikedBy() const 
{
  return _liked_by; 
}

int Post::getNumLikes() const 
{
  return _liked_by.size();
}

int Post::getNumComments() const 
{
  return _comments.size(); 
}

void Post::setDescription(const std::string& description) {
  
  if(description.empty()) {
    throw std::invalid_argument("Descrição não pode ser vazia.");
  }

  if(description.size() > 1000) {
    throw std::invalid_argument("Descrição deve ter no máximo 1000 caracteres.");
  }
  if (!description.empty()) _description = description;
}

void Post::addComment(const std::string& commentAuthor, const std::string& commentDescription)
{
  if(commentAuthor.empty()) {
    throw std::invalid_argument("Autor do comentário não pode ser vazio.");
  }
  if(commentDescription.empty()) {
    throw std::invalid_argument("Comentário não pode ser vazio.");
  }
  if (commentDescription.size() > 500) {
    throw std::invalid_argument("Comentário deve ter no máximo 500 caracteres.");
  }
  int commentIndex = _comments.size(); // índice baseado na ordem de inserção
   _comments.emplace_back(commentIndex, commentAuthor, commentDescription);
}

void Post::removeComment(int index)
{
  auto it = std::find_if(_comments.begin(), _comments.end(), [index](const Comment& c) { return c.index == index; });
  if (it == _comments.end()) {
    throw std::out_of_range("Comentário não encontrado.");
  }
  _comments.erase(it);
    // Atualiza índices para manter a sequência correta
  for (size_t i = 0; i < _comments.size(); ++i) {
    _comments[i].index = i;
  }
  
}

void Post::addLiker(const std::string& liker)
{
  if (liker.empty()) {
    throw std::invalid_argument("Nome do usuário não pode ser vazio.");
  }
  if (std::find(_liked_by.begin(), _liked_by.end(), liker) != _liked_by.end()) {
    throw std::logic_error("Usuário já curtiu este post.");
  }
  _liked_by.push_back(liker);
}

void Post::removeLiker(const std::string& liker)
{
    auto it = std::remove(_liked_by.begin(), _liked_by.end(), liker);
    _liked_by.erase(it, _liked_by.end());
}

bool Post::operator==(const Post& other) const
{
    return (_author == other._author) && (_description == other._description);
}

std::ostream& operator<<(std::ostream& os, const Post& post)
{
  os << "Post_ID: " << post.getID() << "\n";
  os << "Autor: " << post.getAuthor() << "\n";
  os << "Post: " << post._description << "\n";
  os << "Curtidas: " << post.getNumLikes() << "\n";
  os << "--- Comentários (" << post.getNumComments() << ") ---\n";

  for (const auto& comment : post._comments) {
      os << "[" << comment.index << "] "
          << comment.author << ": " << comment.description << "\n";
  }
  return os;
}
