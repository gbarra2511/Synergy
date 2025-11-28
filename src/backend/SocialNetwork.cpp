#include "../../include/backend/SocialNetwork.hpp"
#include "../../include/backend/Utils.hpp"
#include <iostream>
#include <algorithm>

SocialNetwork::SocialNetwork(
  const std::shared_ptr<Entity>* current_user,
  const std::vector<std::shared_ptr<Entity>>* all_users,
  const std::vector<std::shared_ptr<Entity>>* all_companies,
  std::vector<std::shared_ptr<Post>>* all_posts, Ranking* _rankingGlobal, CaseSystem* all_cases)
  : current_user(current_user),
    all_users(all_users),
    all_companies(all_companies),
    all_posts(all_posts),
    _rankingGlobal(_rankingGlobal),
    all_cases(all_cases) {}

SocialNetwork::~SocialNetwork() { }

void SocialNetwork::viewMyFeed(){ 
  feed.clear();

  auto following = (*current_user)->getFollowing();

  following.push_back((*current_user)->getName());

  auto posts = Utils::findPosts(all_posts, following);

  feed.addPosts(posts);

  std::cout << feed;
}

void SocialNetwork::viewMyPosts(){
  auto posts = Utils::findPosts(all_posts, (*current_user)->getName());

  Feed auxFeed;

  auxFeed.addPosts(posts);

  std::cout << auxFeed;
}

void SocialNetwork::viewProfile(std::string profile_name) {
  if (profile_name.empty()) {
    throw std::invalid_argument("Nome de perfil vazio.");
  }
  auto user = Utils::findEntity(all_users, profile_name);
  auto company = Utils::findEntity(all_companies, profile_name);

  if (user)
  {
    user->displayProfile();
  }
  else if (company)
  {
    company->displayProfile();
  }
  else
  {
    std::cout << "Perfil nao encontrado!" << std::endl;
  }
}

void SocialNetwork::createPost(const std::string& description) {
  if (description.empty()) {
    throw std::invalid_argument("Descrição não pode ser vazia.");
  }
  auto post = std::make_shared<Post>((*current_user)->getName(), description);

  all_posts->push_back(post); 
}

void SocialNetwork::deletePost(const int ID) {
  for (size_t i = 0; i < all_posts->size();)
  {
    auto postAuthor = (*all_posts)[i]->getAuthor();
    auto postID = (*all_posts)[i]->getID();

    if (postAuthor == (*current_user)->getName() && postID == ID)
    {
      all_posts->erase(all_posts->begin() + i);
    }
    else
    {
      i ++;
    }
  }
}

void SocialNetwork::followEntity(const std::string& followed_name) {
  if(followed_name.empty()) {
    throw std::invalid_argument("Nome de usuário inválido.");
  }

  if (followed_name == (*current_user)->getName()) {
    throw std::invalid_argument("Você não pode seguir a si mesmo.");
  }
  
  auto user = Utils::findEntity(all_users, followed_name);
  auto company = Utils::findEntity(all_companies, followed_name);

  if (user){
    auto current_following = (*current_user)->getFollowing();
    if (Utils::findName(current_following, followed_name) == followed_name){
      throw std::logic_error("Você já segue esse usuario!");
      return;
    }
    user->addFollower((*current_user)->getName());
    (*current_user)->addFollowing(followed_name);
  }else if (company){
    auto current_following = (*current_user)->getFollowing();
    if (Utils::findName(current_following, followed_name) == followed_name){
      throw std::logic_error("Você já segue essa empresa!");
      return;
    }
    company->addFollower((*current_user)->getName());
    (*current_user)->addFollowing(followed_name);
  }else{
    throw std::invalid_argument("Perfil nao encontrado!");
    return;
  }
  std::cout << "Você agora segue " << followed_name << "!\n";
}

void SocialNetwork::unfollowEntity(const std::string& unfollowed_name){
  if(unfollowed_name.empty()) {
    throw std::invalid_argument("Nome de usuário inválido.");
  }
  
  if(unfollowed_name == (*current_user)->getName()) {
    throw std::invalid_argument("Você não pode parar de seguir a si mesmo.");
  }

  auto user = Utils::findEntity(all_users, unfollowed_name);
  auto company = Utils::findEntity(all_companies, unfollowed_name);

  if (user) {
    auto current_following = (*current_user)->getFollowing();
    if (Utils::findName(current_following, unfollowed_name) == "") {
      throw std::logic_error("Você não segue esse usuário!");  
    }
    user->removeFollower((*current_user)->getName());
    (*current_user)->removeFollowing(unfollowed_name);
    
  } else if (company) {
    auto current_following = (*current_user)->getFollowing();
    if (Utils::findName(current_following, unfollowed_name) == "") {
      throw std::logic_error("Você não segue essa empresa!");  
    }
    company->removeFollower((*current_user)->getName());
    (*current_user)->removeFollowing(unfollowed_name);
    
  } else {
    throw std::invalid_argument("Perfil não encontrado!");  
  }
  
  std::cout << " Você parou de seguir " << unfollowed_name << "!\n";
}


void SocialNetwork::commentPost(const int postID, const std::string& comment){
  if (!*current_user) {
    throw std::runtime_error("Você precisa estar logado para comentar!");
  }
  auto post = Utils::findPost(all_posts, postID);
  if (!post) {
    throw std::runtime_error("Post não encontrado!");
  }
  post->addComment((*current_user)->getName(), comment);
}

void SocialNetwork::removeCommentPost(const int postID, const int which_comment){
  if (!*current_user) {
    throw std::runtime_error("Você precisa estar logado para remover um comentário!");
  } 
  auto post = Utils::findPost(all_posts, postID);
  if (!post) {
    throw std::runtime_error("Post não encontrado!");
  }
  post->removeComment(which_comment);
}

void SocialNetwork::likePost(const int postID){
  if (!*current_user) {
    throw std::runtime_error("Você precisa estar logado para curtir um post!");
  }
  auto post = Utils::findPost(all_posts, postID);
  if (!post) {
    throw std::runtime_error("Post não encontrado!");
  }
  post->addLiker((*current_user)->getName());
}

void SocialNetwork::unlikePost(const int postID){ 
  if (!*current_user) {
    throw std::runtime_error("Você precisa estar logado para descurtir um post!");
  }
  auto post = Utils::findPost(all_posts, postID);
  if (!post) {
    throw std::runtime_error("Post não encontrado!");
  }
  post->removeLiker((*current_user)->getName());
}

void SocialNetwork::displayGlobalRanking(){ 
  if (!_rankingGlobal) {
    throw std::logic_error("Ranking global não inicializado");
  }
  if (_rankingGlobal->getSize() == 0) {
    throw std::logic_error("Sistema não possui nenhum usuario em rankings!");
  }

  std::cout << "\n=== Ranking Global ===\n";
  if (_rankingGlobal->getSize() < 10) {
    int tam =_rankingGlobal->getSize();
    for (int i = 0; i < tam; i++) {
      std::cout << i+1 << ". " << _rankingGlobal->getUsers()[i]->getName() << ": " <<
      _rankingGlobal->getUsers()[i]->getTotalScore() << "\n";
    }
  }
  else if (_rankingGlobal->getSize() >= 10) {
    for (int i = 0; i < 10; i++) {
      std::cout << i+1 << ". " << _rankingGlobal->getUsers()[i]->getName() << ": " <<
      _rankingGlobal->getUsers()[i]->getTotalScore() << "\n";
    }
    std::cout << "...\n";
  }
  std::cout << "======================\n";
  return;
}

void SocialNetwork::displayUserRanking(){
  //sem raking global inicializado nao tem jeito 
  if (!_rankingGlobal || _rankingGlobal->getSize() == 0) {
    throw std::logic_error("Ranking global vazio!");
  }
  std::cout << "\n=== Ranking Global ===\n";
  //a ideia aqui é analisar se todas as operações abaixo sao feitas corretamente
  //caso nao forem a exceção vai ser jogada com pos = -1
  int pos = -1;
  for (int i = 0; i < _rankingGlobal->getSize(); i++) {
    if (_rankingGlobal->getUsers()[i]->getName() == (*current_user)->getName()) {
      pos = i + 1;
      break;
    }
  }
    
  if (pos == -1) {
    throw std::runtime_error("Você não está no ranking!");
  }
  
  auto userPtr = std::dynamic_pointer_cast<User>(*current_user);

  for (auto it : _rankingGlobal->getUsers()) {
    if (it->getName() == userPtr->getName()) {
      std::cout << "...\n";
      std::cout << pos << ". " << userPtr->getName() << ": " << userPtr->getTotalScore() << "\n";
      std::cout << "...\n";
    }
  }
  std::cout << "======================\n";
}

void SocialNetwork::attemptSolveCase(int caseId, const std::string& answer, std::shared_ptr<Company> company) {
    // ==================== VALIDAÇÕES ====================
    
    if (!current_user || !*current_user) {
        throw std::runtime_error("Você precisa estar logado para resolver cases!");
    }
    
    if (answer.empty()) {
        throw std::invalid_argument("Resposta não pode estar vazia!");
    }
    
    if (!company) {
        throw std::runtime_error("Case deve estar vinculado a uma empresa!");
    }
    
    // Buscar case da company
    auto caseItem = company->getCase(caseId);
    if (!caseItem) {
        throw std::runtime_error("Case não encontrado na empresa!");
    }
    
    // Cast para User
    auto userPtr = std::dynamic_pointer_cast<User>(*current_user);
    if (!userPtr) {
        throw std::runtime_error("Apenas usuários podem resolver cases!");
    }
    
    // ==================== VERIFICAÇÕES ====================
    try {
        // Verificar se já resolveu
        const auto& solvedCases = userPtr->getSolvedCases();
        for (int id : userPtr->getSolvedCases()) {
            std::cout << id << " ";
        }

        bool alreadySolved = false;
        for (int id : solvedCases) {
            if (id == caseItem->getId()) {
                alreadySolved = true;
                break;
            }
        }
    
    if (alreadySolved) {
        throw std::logic_error("Você já resolveu este case anteriormente!");
    }
        
        // ==================== TENTAR RESOLVER ====================
        
        bool solved = (answer == caseItem->getSolution());
        
        if (solved) {
            std::cout << "\n Resposta CORRETA!\n";
            
            userPtr->addScore(company, caseItem->getPoints());
            std::cout << "Você ganhou " << caseItem->getPoints() << " pontos!\n";
            
            userPtr->registerSolvedCase(caseItem->getId());
            
            // Atualizar ranking da company
            auto companyRanking = company->getRanking();
            if (companyRanking) {
              bool userInRanking = false;
              for (const auto& user : companyRanking->getUsers()) {
                  if (user == userPtr) {
                      userInRanking = true;
                      break;
                  }
              }
              
              if (!userInRanking) {
                  companyRanking->addUser(userPtr);
              }
              companyRanking->sort();
              std::cout << "Ranking da empresa atualizado!\n";
            }
            
            // Atualizar ranking global
            if (_rankingGlobal) {
              bool userInGlobalRanking = false;
              for (const auto& user : _rankingGlobal->getUsers()) {
                  if (user == userPtr) {
                      userInGlobalRanking = true;
                      break;
                  }
              }
              
              if (!userInGlobalRanking) {
                  _rankingGlobal->addUser(userPtr);
              }
              _rankingGlobal->sort();
              std::cout << "Ranking global atualizado!\n";
            }
            
        } else {
            std::cout << "\nResposta INCORRETA! Tente novamente.\n";
        }
        
    } catch (const std::logic_error& e) {
        std::cerr << " " << e.what() << "\n";
        throw;
    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << "\n";
        throw;
    }
}