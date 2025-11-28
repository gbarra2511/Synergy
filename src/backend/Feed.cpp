#include "../../include/backend/Feed.hpp"

#include <algorithm>
#include <iostream>

Feed::Feed() {}

std::shared_ptr<Post> Feed::getPost(const int index)
{
  return _posts[index];
}

int Feed::getNumPosts() const 
{
  return _posts.size();
}

void Feed::addPost(const std::shared_ptr<Post>& post) 
{
  // Segurança para nn aceitar posts nulls
  /*
  if (post) {
    _posts.push_back(post);
  }
  */
  if(!post) {
    throw std::invalid_argument("Tentativa de adicionar post nulo ao feed.");
  }

  _posts.push_back(post);
}

void Feed::removePost(const std::shared_ptr<Post>& post) 
{
  if (!post) {
    throw std::invalid_argument("Tentativa de remover post nulo.");
  }

  for (size_t i = 0; i < _posts.size();)
  {
    if (_posts[i] == post)
    {
      _posts.erase(_posts.begin() + i);
    }
    else
    {
      i ++;
    }
  }
}

void Feed::addPosts(const std::vector<std::shared_ptr<Post>>& posts) 
{
  for(const auto& p : posts) {
    if(!p) {
      throw std::invalid_argument("Lista contém post nulo.");
    }
  }
  _posts.insert(_posts.end(), posts.begin(), posts.end());
}

void Feed::removePosts(const std::vector<std::shared_ptr<Post>>& posts)  
{
  for (const auto& p : posts) {
    if(!p) {
      throw std::invalid_argument("Lista contém post nulo para remoção.");
    }
  }
  for(const auto& post : posts) {
    removePost(post);
  }
}

void Feed::clear()
{
  _posts.clear();
}

bool comparePostsByDate(const std::shared_ptr<Post>& a, const std::shared_ptr<Post>& b) 
{
  return a->getTimestamp() > b->getTimestamp();
}

// Funcão de Uso Unico Auxiliar
void Feed::sortByDate() 
{
  std::sort(_posts.begin(), _posts.end(), comparePostsByDate);
}

// operator overloading
std::ostream& operator<<(std::ostream& os, const Feed& feed)
{
  if (feed._posts.empty()) {
    os << "O feed está vazio.\n";
    return os;
  }

  for (const auto& post_ptr : feed._posts) {
    if (post_ptr) {
      os << "\n----------------------------------------\n\n";
      os << *post_ptr;
      os << "\n----------------------------------------\n\n";
    }
  }

  return os;
}


