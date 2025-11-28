#include "../../include/backend/Utils.hpp"
#ifdef _WIN32
  #include <windows.h>
  #define CLEAR_COMMAND "cls"
#else
  #include <cstdlib>
  #define CLEAR_COMMAND "clear"
#endif

// namespace que contem funções uteis para manipular os dados
namespace Utils
{
  std::vector<std::shared_ptr<Entity>> findEntities(const std::vector<std::shared_ptr<Entity>>* all_entities, const std::vector<std::string>& names)
  {
    std::vector<std::shared_ptr<Entity>> resultado;
    for (const auto& name : names)
    {
      for (const auto& entidade : *all_entities)
      {
        if (entidade->getName() == name)
        {
          resultado.push_back(entidade);
          break;
        }
      }
    }
    return resultado;
  }

  std::shared_ptr<Entity> findEntity(const std::vector<std::shared_ptr<Entity>>* all_entities,  const std::string& name)
  {
    for (const auto& entidade : *all_entities)
    {
      if (entidade->getName() == name)
      {
          return entidade; // retorna a primeira encontrada
      }
    }

    return nullptr; // se não encontrar ninguém
  }

  std::shared_ptr<Entity> findEntity(const std::vector<std::shared_ptr<Entity>>* all_entities,  const int ID)
  {
    for (const auto& entidade : *all_entities)
    {
      if (entidade->getId() == ID)
      {
          return entidade; // retorna a primeira encontrada
      }
    }

    return nullptr; // se não encontrar ninguém
  }

  std::vector<std::shared_ptr<Post>> findPosts(const std::vector<std::shared_ptr<Post>>* all_posts, const std::vector<std::string>& names)
  {
    std::vector<std::shared_ptr<Post>> resultado;

    for (const auto& post : *all_posts)
    {
      auto author = post->getAuthor();

      for (const auto& name : names)
      {
        if (author == name)
        {
          resultado.push_back(post);
          break;
        }
      }
    }

    return resultado;
  }

  std::vector<std::shared_ptr<Post>> findPosts(const std::vector<std::shared_ptr<Post>>* all_posts, const std::string& name)
  {
    std::vector<std::shared_ptr<Post>> resultado;
    for (const auto& post : *all_posts)
    {
      auto author = post->getAuthor();
      if (author == name)
      {
        resultado.push_back(post);
      }
    }
    return resultado;
  }

  std::shared_ptr<Post> findPost( const std::vector<std::shared_ptr<Post>>* all_posts, const int ID)
  {
    for (const auto& p : *all_posts) {
      if (p->getID() == ID)
      {
        return p;
      }
    }
    return nullptr;
  }

  std::string findName(const std::vector<std::string>& names, const std::string& name)
  {
    for (const auto& n : names)
    {
      if (n == name)
      {
        return n;
      }
    }
    return "";
  }

  std::string getInput(const std::string& prompt)
  {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
  }

  int getIntInput(const std::string& prompt)
  {
    std::string input;
    while (true)
    {
      std::cout << prompt;
      std::getline(std::cin, input);
      try {
      return std::stoi(input);
      }
      catch(...){
        std::cout << "Entrada inválida! Digite um número.\n";
      }
    }
  }

  void clearScreen()
  {
    system(CLEAR_COMMAND);
  }
}
