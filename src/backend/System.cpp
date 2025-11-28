#include "../../include/backend/System.hpp"
#include <iostream>
#include <algorithm>
#include <memory>
#include <regex>

System::System(std::string dbName) 
    : dbManager(dbName), 
      socialNetwork(&current_user, &all_users, &all_companies, &all_posts, &rankingGlobal, &caseSystemGlobal), 
      modified(false) {

    std::cout << "\n-> Iniciando o sistema \n";
    loadData();
    std::cout << "Sistema inicializado com sucesso!\n\n";
}

System::~System() {
    saveAllData();
}
void System::saveAllData() {
    if (modified || current_user != nullptr) {
        std::cout << "\n--- Salvando todos os dados ---\n";

        // Converter shared_ptr<User> para User para salvar
        std::vector<User> usersToSave;
        for (const auto& user : all_users) {
            auto u = std::dynamic_pointer_cast<User>(user);
            if (u) usersToSave.push_back(*u);
        }

        // Converter shared_ptr<Company> para Company para salvar
        std::vector<Company> companiesToSave;
        for (const auto& company : all_companies) {
            auto c = std::dynamic_pointer_cast<Company>(company);
            if (c) companiesToSave.push_back(*c);
        }

        // ==================== SALVAR DADOS BÁSICOS ====================
        
        if (!dbManager.saveUsers(usersToSave)) {
            std::cerr << "❌ Erro ao salvar usuários!\n";
        }
        
        if (!dbManager.saveCompany(companiesToSave)) {
            std::cerr << "❌ Erro ao salvar empresas!\n";
        }
        
        if (!dbManager.saveCase(caseSystemGlobal)) {
            std::cerr << "❌ Erro ao salvar cases globais!\n";
        }
        
        dbManager.savePosts(all_posts);

        // ==================== SALVAR CASES POR COMPANY ====================
        
        std::cout << "\n  Salvando cases das empresas...\n";
        int companiesWithCases = 0;
        
        for (const auto& company : all_companies) {
            auto c = std::dynamic_pointer_cast<Company>(company);
            if (c) {
                auto& companyCases = c->getCaseSystem();
                
                // ← VERIFICAR SE TEM CASES
                if (!companyCases.getAllCases().empty()) {
                    if (dbManager.saveCompanyCases(c->getId(), companyCases)) {
                        companiesWithCases++;
                    } else {
                        std::cerr << "  Erro ao salvar cases de " << c->getName() << "\n";
                    }
                }
            }
        }
        std::cout << " " << companiesWithCases << " empresa(s) com cases salva(s)!\n";

        // ==================== SALVAR SOLVED CASES ====================
        
        std::cout << "\n  Salvando cases resolvidos...\n";
        for (const auto& user : all_users) {
            auto u = std::dynamic_pointer_cast<User>(user);
            if (u) {
                if (!u->getSolvedCases().empty()) {
                    dbManager.saveSolvedCases(u->getId(), u->getSolvedCases());
                }
            }
        }

        // ==================== SALVAR RANKING GLOBAL ====================
        
        std::vector<std::shared_ptr<User>> usersForRanking;
        for (const auto& user : all_users) {
            auto u = std::dynamic_pointer_cast<User>(user);
            if (u) usersForRanking.push_back(u);
        }
        
        if (!usersForRanking.empty()) {
            dbManager.saveGlobalRanking(usersForRanking);
        }

        // ==================== SALVAR RANKING INTERNO COMPANY ====================
        
        std::cout << "\n  Salvando rankings das empresas...\n";
        for (const auto& company : all_companies) {
            auto c = std::dynamic_pointer_cast<Company>(company);
            if (c) {
                auto ranking = c->getRanking();
                if (ranking && ranking->getSize() > 0) {
                    std::vector<std::shared_ptr<User>> companyRankingUsers;
                    const auto& rankingUsers = ranking->getUsers();
                    for (const auto& user : rankingUsers) {
                        auto u = std::dynamic_pointer_cast<User>(user);
                        if (u) companyRankingUsers.push_back(u);
                    }
                    
                    if (!companyRankingUsers.empty()) {
                        dbManager.saveCompanyRanking(c->getId(), companyRankingUsers);
                    }
                }
            }
        }

        std::cout << "\n Todos os dados salvos com sucesso!\n";
        modified = false;
    }
}
//============================Métodos loadData============================

void System::loadData() {
    std::cout << "\n--- Carregando dados do banco ---\n";
    
    loadUsers();
    loadCompanies();
    loadGlobalCases();
    
    auto rankingGlobalIds = dbManager.loadGlobalRanking();
    for (int userId : rankingGlobalIds) {
        for (const auto& user : all_users) {
            auto u = std::dynamic_pointer_cast<User>(user);
            if (u && u->getId() == userId) {
                rankingGlobal.addUser(u);
                break;
            }
        }
    }
    rankingGlobal.sort();
    
    for (const auto& company : all_companies) {
        auto c = std::dynamic_pointer_cast<Company>(company);
        if (c) {
            auto companyRankingIds = dbManager.loadCompanyRanking(c->getId());
            auto companyRanking = c->getRanking();
            if (companyRanking) {
                for (int userId : companyRankingIds) {
                    for (const auto& user : all_users) {
                        auto u = std::dynamic_pointer_cast<User>(user);
                        if (u && u->getId() == userId) {
                            companyRanking->addUser(u);
                            break;
                        }
                    }
                }
                companyRanking->sort();
            }
        }
    }
    
    loadPosts();
    
    std::cout << " Dados carregados!\n";
}

void System::loadUsers()
{
  auto users = dbManager.loadUsers();

  // Carregar followers/following
  for (const auto& user : users)
  {
    auto newUser = std::make_shared<User>(user);
    all_users.push_back(newUser);
  }

  std::cout << "System -> " << all_users.size() << " usuários carregados!\n";
}

void System::loadCompanies()
{
  auto companies = dbManager.loadCompanies();

  for (const auto& company : companies)
  {
    auto newCompany = std::make_shared<Company>(company);
    all_companies.push_back(newCompany);
  }

  std::cout << "System " << all_companies.size() << " companies carregadas!\n";
}

void System::loadCompanyCases() {
    std::cout << "\n--- Carregando cases das empresas ---\n";
    
    for (auto& company : all_companies) {
        auto c = std::dynamic_pointer_cast<Company>(company);
        if (c) {
            auto cases = dbManager.loadCompanyCases(c->getId());
            int loadedCount = 0;
            
            for (const auto& caseItem : cases.getAllCases()) {
                if (caseItem) {
                    c->addCase(caseItem);
                    
                    auto existingCase = caseSystemGlobal.getCase(caseItem->getId());
                    if (!existingCase) {
                        caseSystemGlobal.addCase(caseItem);
                    }
                    
                    loadedCount++;
                }
            }
            
            if (loadedCount > 0) {
                std::cout  << loadedCount << " cases para " 
                          << c->getName() << "\n";
            }
        }
    }
}

void System::loadGlobalCases() {
    caseSystemGlobal = dbManager.loadGlobalCases();
    loadCompanyCases();  
}

void System::loadGlobalRanking()
{
  auto userIds = dbManager.loadGlobalRanking();
}

void System::loadPosts()
{
  all_posts = dbManager.loadPosts();
  std::cout << "System -> " << all_posts.size() << " posts carregados!\n";
}

//=================================Métodos de menu (retornam escolha)========================================

std::string System::showMainMenu()
{
  std::cout << "\n╔════════════════════════════════════╗\n";
  std::cout <<   "║        SYNERGY - BEM-VINDO         ║\n";
  std::cout <<   "╚════════════════════════════════════╝\n";
  std::cout << "1. Login\n";
  std::cout << "2. Registrar Usuário\n";
  std::cout << "3. Registrar Empresa\n";
  std::cout << "4. Sair\n";
  std::cout << "────────────────────────────────────\n";

  return Utils::getInput("Escolha uma opção: ");
}

std::string System::showUserMenu()
{
  std::cout << "\n╔════════════════════════════════════╗\n";
  std::cout <<   "║     MENU PRINCIPAL - USUÁRIO       ║\n";
  std::cout <<   "╚════════════════════════════════════╝\n";
  std::cout << "Bem-vindo, " << current_user->getName() << "!\n";
  std::cout << "────────────────────────────────────\n";
  std::cout << "1. Rede Social\n";
  std::cout << "2. Cases\n";
  std::cout << "3. Rankings\n";
  std::cout << "4. Perfil\n";
  std::cout << "5. Logout\n";
  std::cout << "────────────────────────────────────\n";

  return Utils::getInput("Escolha uma opção: ");
}

std::string System::showCompanyMenu()
{
  std::cout << "\n╔════════════════════════════════════╗\n";
  std::cout <<   "║     MENU PRINCIPAL - EMPRESA       ║\n";
  std::cout <<   "╚════════════════════════════════════╝\n";
  std::cout << "Bem-vindo, " << current_user->getName() << "!\n";
  std::cout << "────────────────────────────────────\n";
  std::cout << "1. Gerenciar Cases\n";
  std::cout << "2. Ver ranking da Empresa\n";
  std::cout << "3. Rede Social\n";
  std::cout << "4. Perfil\n";
  std::cout << "5. Logout\n";
  std::cout << "────────────────────────────────────\n";

  return Utils::getInput("Escolha uma opção: ");
}

std::string System::showSocialMenu()
{
  std::cout << "\n╔════════════════════════════════════╗\n";
  std::cout <<   "║            REDE SOCIAL             ║\n";
  std::cout <<   "╚════════════════════════════════════╝\n";
  std::cout << "1. Ver Feed\n";
  std::cout << "2. Meus Posts\n";
  std::cout << "3. Criar Post\n";
  std::cout << "4. Interagir com Post\n";
  std::cout << "5. Seguir/Deixar de Seguir\n";
  std::cout << "6. Ver Perfil\n";
  std::cout << "7. Listar Seguidores\n";
  std::cout << "8. Listar Seguindo\n";
  std::cout << "9. Voltar\n";
  std::cout << "────────────────────────────────────\n";

  return Utils::getInput("Escolha uma opção: ");
}

std::string System::showCaseMenu()
{
  auto company = std::dynamic_pointer_cast<Company>(current_user);
  std::cout << "\n╔════════════════════════════════════╗\n";
  std::cout <<   "║               CASES                ║\n";
  std::cout <<   "╚════════════════════════════════════╝\n";
  
  if (company)
  {
    std::cout << "1. Adicionar Case\n";
    std::cout << "2. Remover Case\n";
    std::cout << "3. Ativar/Desativar Case\n";
    std::cout << "4. Listar Meus Cases\n";
    std::cout << "5. Listar Cases Globais\n";
    std::cout << "6. Buscar Cases\n";
    std::cout << "7. Voltar\n";
    std::cout << "────────────────────────────────────\n";
  }
  else 
  {
    std::cout << "1. Listar Cases Globais\n";
    std::cout << "2. Listar Cases por Empresa\n";
    std::cout << "3. Submeter Solução\n";
    std::cout << "4. Buscar Cases\n";
    std::cout << "5. Voltar\n";
    std::cout << "────────────────────────────────────\n";
  }

  return Utils::getInput("Escolha uma opção: ");
}

std::string System::showRankingMenu()
{
  std::cout << "\n╔════════════════════════════════════╗\n";
  std::cout <<   "║             RANKINGS               ║\n";
  std::cout <<   "╚════════════════════════════════════╝\n";
  std::cout << "1. Ranking Global\n";
  std::cout << "2. Meu Ranking\n";
  std::cout << "3. Ranking por Empresa\n";
  std::cout << "4. Voltar\n";
  std::cout << "────────────────────────────────────\n";

  return Utils::getInput("Escolha uma opção: ");
}

//==================================Métodos auxiliares==================================

bool System::hasCurrentUser() const {
    return current_user != nullptr;
}

bool System::isCurrentUserCompany() const {
    if (!current_user) return false;
    return std::dynamic_pointer_cast<Company>(current_user) != nullptr;
}

void System::viewMyProfile() {
    if (!current_user) {
        throw std::runtime_error("Você precisa estar logado.");
    }
    
    std::cout << "\n╔════════════════════════════════════╗\n";
    std::cout <<   "║            MEU PERFIL              ║\n";
    std::cout <<   "╚════════════════════════════════════╝\n";
    std::cout << "Nome: " << current_user->getName() << "\n";
    std::cout << "Email: " << current_user->getEmail() << "\n";
    std::cout << "Descrição: " << current_user->getDescription() << "\n";
    std::cout << "Telefone: " << current_user->getPhoneNumber() << "\n";
    
    if (isCurrentUserCompany()) {
        auto company = std::dynamic_pointer_cast<Company>(current_user);
        std::cout << "CNPJ: " << company->getCnpj() << "\n";
        std::cout << "Número de Cases: " << company->getAllCases().size() << "\n";
    } else {
        auto user = std::dynamic_pointer_cast<User>(current_user);
        std::cout << "CPF: " << user->getCpf() << "\n";
        std::cout << "Pontos: " << user->getTotalScore() << "\n";
        std::cout << "Cases Resolvidos: " << user->getSolvedCases().size() << "\n";
    }
    
    std::cout << "Seguidores: " << current_user->getFollowers().size() << "\n";
    std::cout << "Seguindo: " << current_user->getFollowing().size() << "\n";
    std::cout << "════════════════════════════════════\n";
}

void System::viewMyCompanyRanking() {
    if (!isCurrentUserCompany()) {
        std::cout << "Apenas empresas podem ver seu ranking.\n";
        return;
    }
    
    auto company = std::dynamic_pointer_cast<Company>(current_user);
    auto ranking = company->getRanking();
    ranking->display();
}

void System::interactWithPost() {
    if (!current_user) {
        throw std::runtime_error("Você precisa estar logado.");
    }
    
    int postID = Utils::getIntInput("Digite o ID do post: ");
    
    std::cout << "\n1. Curtir\n";
    std::cout << "2. Descurtir\n";
    std::cout << "3. Comentar\n";
    std::cout << "4. Remover comentário\n";
    std::cout << "5. Deletar post\n";
    std::cout << "6. Voltar\n";
    
    std::string choice = Utils::getInput("Escolha uma ação: ");
    
    if (choice == "1") {
        socialNetwork.likePost(postID);
        modified = true;
    }
    else if (choice == "2") {
        socialNetwork.unlikePost(postID);
        modified = true;
    }
    else if (choice == "3") {
        std::string comment = Utils::getInput("Digite seu comentário: ");
        socialNetwork.commentPost(postID, comment);
        modified = true;
    }
    else if (choice == "4") {
        int commentIndex = Utils::getIntInput("Digite o índice do comentário: ");
        socialNetwork.removeCommentPost(postID, commentIndex);
        modified = true;
    }
    else if (choice == "5") {
        socialNetwork.deletePost(postID);
        modified = true;
    }
}

void System::manageFollowUnfollow() {
    if (!current_user) {
        throw std::runtime_error("Você precisa estar logado.");
    }
    
    std::cout << "\n1. Seguir alguém\n";
    std::cout << "2. Deixar de seguir\n";
    std::cout << "3. Voltar\n";
    
    std::string choice = Utils::getInput("Escolha uma opção: ");
    
    if (choice == "1") {
        std::string name = Utils::getInput("Digite o nome para seguir: ");
        socialNetwork.followEntity(name);
        modified = true;
    }
    else if (choice == "2") {
        std::string name = Utils::getInput("Digite o nome para deixar de seguir: ");
        socialNetwork.unfollowEntity(name);
        modified = true;
    }
}

void System::addCaseInteractive() {
    if (!isCurrentUserCompany()) {
        std::cout << "Apenas empresas podem adicionar cases.\n";
        return;
    }

    std::string name = Utils::getInput("Nome do case: ");
    std::string description = Utils::getInput("Descrição: ");
    std::string answer = Utils::getInput("Resposta correta: ");
    int points = Utils::getIntInput("Pontos: ");
 
    auto newCase = std::make_shared<Case>(name, description, answer, points);
 
    newCase->setState(true);
 
    auto company = std::dynamic_pointer_cast<Company>(current_user);
    company->addCase(newCase);

    caseSystemGlobal.addCase(newCase);
 
    std::cout << "\nCase adicionado com sucesso! ID: " << newCase->getId() << "\n";
    modified = true;
}

void System::listMyCases() {
    if (!isCurrentUserCompany()) {
        std::cout << "Apenas empresas podem ver seus cases.\n";
        return;
    }
    
    auto company = std::dynamic_pointer_cast<Company>(current_user);
    auto cases = company->getAllCases();
    
    if (cases.empty()) {
        std::cout << "\nVocê não tem cases cadastrados.\n";
        return;
    }
    
    std::cout << "\n=== Meus Cases ===\n";
    for (const auto& caseItem : cases) {
        if (caseItem) {
            std::cout << "\nID: " << caseItem->getId() << "\n";
            std::cout << "Nome: " << caseItem->getName() << "\n";
            std::cout << "Status: " << (caseItem->getState() ? "Ativo" : "Inativo") << "\n";
            std::cout << "Pontos: " << caseItem->getPoints() << "\n";
            std::cout << "-----------------------------------\n";
        }
    }
}

void System::toggleCaseStatus() {
    if (!isCurrentUserCompany()) {
        std::cout << "Apenas empresas podem gerenciar cases.\n";
        return;
    }
    
    int caseId = Utils::getIntInput("Digite o ID do case: ");
    
    auto company = std::dynamic_pointer_cast<Company>(current_user);
    auto caseItem = company->getCase(caseId);
    
    if (!caseItem) {
        std::cout << "Case não encontrado.\n";
        return;
    }
    
    if (caseItem->getState()) {
        company->deactivateCase(caseId);
        auto globalCase = caseSystemGlobal.getCase(caseId);
        if (globalCase) {
            globalCase->setState(true);
        }
        std::cout << "Case desativado com sucesso!\n";
    } else {
        company->activateCase(caseId);
        std::cout << "Case ativado com sucesso!\n";
    }
    
    modified = true;
}

void System::removeCase() {
    if (!isCurrentUserCompany()) {
        std::cout << "Apenas empresas podem remover cases.\n";
        return;
    }
    
    std::string caseName = Utils::getInput("Digite o nome do case que deseja remover: ");
    
    auto company = std::dynamic_pointer_cast<Company>(current_user);
    
    try {
        // Validar se nome é vazio
        if (caseName.empty()) {
            throw std::invalid_argument("Nome do case não pode ser vazio!");
        }
        
        
        auto caseToRemove = company->getCaseSystem().getCaseByName(caseName);
        if (!caseToRemove) {
            throw std::invalid_argument("Case '" + caseName + "' não encontrado na sua empresa!");
        }
        
        // Remover da company
        company->getCaseSystem().removeCaseByName(caseName);
        std::cout << " Case removido da sua empresa.\n";
        
        // Remover do global 
        try {
            caseSystemGlobal.removeCaseByName(caseName);
            std::cout << " Case removido do sistema global.\n";
        } catch (const std::exception& e) {
            std::cout << " Case não estava no sistema global.\n";
        }
        
        modified = true;
        std::cout << " Case '" << caseName << "' removido com sucesso!\n";
        
    } catch (const std::invalid_argument& e) {
        std::cerr  << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }
}
//==================================Métodos login/logout==================================

void System::login() {
    std::string username = Utils::getInput("Digite o nome de usuário: ");
    std::string password = Utils::getInput("Digite a senha: ");

    if(username.empty() || password.empty()) {
        throw std::invalid_argument("Usuário e senha não podem ser vazios.");
    }
    
    for (auto& user : all_users) {
        if (user->getName() == username) {
            if (user->checkPassword(password)) {
                current_user = user;
                std::cout << "\n✓ Login bem-sucedido! Bem-vindo " << username << "!\n";
                modified = true;
                return;
            } else {
                std::cerr << " Senha incorreta!\n";
                return;
            }
        }
    }
    
    // Verifica em empresas
    for (auto& company : all_companies) {
        if (company->getName() == username) {
            if (company->checkPassword(password)) {
                current_user = company;
                std::cout << "\n✓ Login bem-sucedido! Bem-vindo " << username << "!\n";
                modified = true;
                return;
            } else {
                std::cerr << " Senha incorreta!\n";
                return;
            }
        }
    }
    
    std::cerr << " Usuário não encontrado!\n";
}

void System::logout() {
    if (current_user) {
        std::cout << "\n✓ Logout realizado! Até logo, " << current_user->getName() << "!\n";
        current_user = nullptr;
        modified = true;
    }
}

//==================================Métodos register==================================

void System::registerUser()
{
  std::string name;
  while (true)
  {
    name = Utils::getInput("Digite o nome: ");
    if (name.empty())
    {
      std::cerr << "Erro: Nome não pode ser vazio!\n";
      continue;
    }
    if (name.length() < 3)
    {
      std::cerr << "Erro: Nome deve ter pelo menos 3 caracteres!\n";
      continue;
    }
    break;
  }

  std::string email;
  while (true)
  {
    email = Utils::getInput("Digite o email: ");
    if (email.empty())
    {
      std::cerr << "Erro: Email não pode ser vazio!\n";
      continue;
    }
    const std::regex email_regex(R"((\w+)(\.?\w*)@(\w+)(\.\w+)+)");
    if (!std::regex_match(email, email_regex))
    {
      std::cerr << "Erro: Email inválido! Use o formato: exemplo@dominio.com\n";
      continue;
    }
    bool emailExists = false;
    for (const auto& user : all_users)
    {
      if (user->getEmail() == email)
      {
        emailExists = true;
        break;
      }
    }
    if (emailExists)
    {
      std::cerr << "Erro: Este email já está cadastrado!\n";
      continue;
    }
    break;
  }
  
  std::string password;
  while (true)
  {
    password = Utils::getInput("Digite a senha: ");
    if (password.empty())
    {
      std::cerr << "Erro: Senha não pode ser vazia!\n";
      continue;
    }
    if (password.length() < 8)
    {
      std::cerr << "Erro: Senha deve ter pelo menos 8 caracteres!\n";
      continue;
    }
    break;
  }

  std::string phone;
  while (true)
  {
    phone = Utils::getInput("Digite o telefone: ");
    if (phone.empty())
    {
      std::cerr << "Erro: Telefone não pode ser vazio!\n";
      continue;
    }
    const std::regex phone_regex("^(?:\\([1-9]{2}\\)|[1-9]{2}) ?9?[0-9]{4}-?[0-9]{4}$");
    if (!std::regex_match(phone, phone_regex))
    {
      std::cerr << "Erro: Telefone inválido! Use o formato: (XX) 9XXXX-XXXX\n";
      continue;
    }
    break;
  }

  std::string cpf;
  while (true)
  {
    cpf = Utils::getInput("Digite o CPF: ");
    if (cpf.empty())
    {
      std::cerr << "Erro: CPF não pode ser vazio!\n";
      continue;
    }
    if (cpf.length() != 11)
    {
      std::cerr << "Erro: CPF deve ter exatamente 11 dígitos!\n";
      continue;
    }
    if (!std::all_of(cpf.begin(), cpf.end(), ::isdigit))
    {
      std::cerr << "Erro: CPF deve conter apenas números!\n";
      continue;
    }
    bool cpfExists = false;
    for (const auto& userEntity : all_users)
    {
      auto user = std::dynamic_pointer_cast<User>(userEntity);
      if (user && user->getCpf() == cpf)
      {
        cpfExists = true;
        break;
      }
    }
    if (cpfExists)
    {
      std::cerr << "Erro: Este CPF já está cadastrado!\n";
      continue;
    }
    break;
  }

  std::string description = Utils::getInput("Digite a descrição: ");

  try
  {
    auto newUser = std::make_shared<User>(name, email, phone, password, true, description, 0.0, cpf);
    all_users.push_back(newUser);
    rankingGlobal.addUser(newUser);
    modified = true;
    Utils::clearScreen();
    std::cout << "\n✓ Usuário " << name << " cadastrado com sucesso!\n";
    std::cout << "Usuário: " << name << "\n";
    std::cout << "Email: " << email << "\n";
    std::cout << "CPF: " << cpf << "\n\n";
    std::cout << "Você já pode fazer login!\n";
  }
  catch (const std::exception& e)
  {
    std::cerr << "\nErro ao criar usuário: " << e.what() << "\n";
    std::cout << "Por favor, tente novamente.\n";
  }
}

void System::registerCompany() {
    std::string name = Utils::getInput("Digite o nome da empresa: ");
    std::string email = Utils::getInput("Digite o email: ");
    std::string password = Utils::getInput("Digite a senha: ");
    std::string description = Utils::getInput("Digite a descrição: ");
    std::string phone = Utils::getInput("Digite o telefone: ");
    std::string cnpj = Utils::getInput("Digite o CNPJ: ");

    if(name.empty() || email.empty() || password.empty()) {
        throw std::invalid_argument("Nome, email e senha não podem ser vazios.");
    }

    auto newCompany = std::make_shared<Company>(name, email, phone, 
                                                password, true, description, cnpj);
    all_companies.push_back(newCompany);

    modified = true;

    std::cout << "\n✓ Empresa " << name << " cadastrada com sucesso!\n";
}

//==================================Métodos de visualização==================================

void System::viewMyFeed() {
    if (!current_user) {
        throw std::runtime_error("Você precisa estar logado para ver o feed.");
    }
    socialNetwork.viewMyFeed();
}

void System::viewMyPosts() {
    if (!current_user) {
        throw std::runtime_error("Você precisa estar logado para ver seus posts.");
    }
    socialNetwork.viewMyPosts();
}

void System::viewProfile() {
    std::string profile_name = Utils::getInput("Digite o nome do perfil que deseja visualizar: ");
    
    if (profile_name.empty()) {
        throw std::invalid_argument("Nome do perfil não pode ser vazio.");
    }
    socialNetwork.viewProfile(profile_name);
}

//==================================Métodos de seguir/deixar de seguir==================================

void System::listFollowers() {
    if (!current_user) {
        throw std::runtime_error("Você precisa estar logado.");
    }
    
    if(current_user->getFollowers().empty()) {
        std::cout << "Você não tem seguidores.\n";
        return;
    }
    
    std::cout << "\n=== Seus Seguidores ===\n";
    for(const auto& follower : current_user->getFollowers()) {
        std::cout << "  - " << follower << "\n";
    }
    std::cout << "========================\n";
}

void System::listFollowing() {
    if (!current_user) {
        throw std::runtime_error("Você precisa estar logado.");
    }
    
    if(current_user->getFollowing().empty()) {
        std::cout << "Você não está seguindo ninguém.\n";
        return;
    }
    
    std::cout << "\n=== Você está seguindo ===\n";
    for(const auto& following : current_user->getFollowing()) {
        std::cout << "  - " << following << "\n";
    }
    std::cout << "===========================\n";
}

// =================Métodos de Post=============================

void System::createPost() {
    if(!current_user) {
        throw std::runtime_error("Você precisa estar logado para criar posts.");
    }
    
    std::string content = Utils::getInput("Digite o conteúdo do post: ");
    socialNetwork.createPost(content);
    modified = true;
}

// ================= Métodos de Case=============================

void System::listAllCompanies() {
    if (all_companies.empty()) {
        std::cout << "\nNenhuma empresa cadastrada.\n";
        return;
    }
    
    std::cout << "\n=== Empresas Cadastradas ===\n";
    for (const auto& company : all_companies) {
        std::cout << "  - " << company->getName() << " (ID: " << company->getId() << ")\n";
    }
    std::cout << "=============================\n";
}

void System::listGlobalCases() {
    auto cases = caseSystemGlobal.getAllCases();

    if (cases.empty()) {
        std::cout << "\nNenhum case global disponível no momento.\n";
        return;
    }
 
    std::cout << "\n=== Cases Globais Disponíveis ===\n";
    for (const auto& caseItem : cases) {
        if (caseItem) {
            std::cout << "\nID: " << caseItem->getId() << "\n";
            std::cout << "Nome: " << caseItem->getName() << "\n";
            std::cout << "Descrição: " << caseItem->getDescription() << "\n";
            std::cout << "Pontos: " << caseItem->getPoints() << "\n";
            std::cout << "Status: " << (caseItem->getState() ? "Ativo" : "Inativo") << "\n";
            std::cout << "-----------------------------------\n";
        }
    }
}

void System::listCompanyCases() {
    std::string input = Utils::getInput("Digite o ID ou nome da empresa: ");
    
    std::shared_ptr<Entity> companyEntity = nullptr;
    
    // Tenta converter para int, se funcionar busca por ID, senão busca por nome
    try {
        int companyId = std::stoi(input);
        companyEntity = Utils::findEntity(&all_companies, companyId);
    }
    catch(...) {
        companyEntity = Utils::findEntity(&all_companies, input);
    }
    
    if (!companyEntity) {
        std::cout << "Empresa não encontrada.\n";
        return;
    }
    
    auto targetCompany = std::dynamic_pointer_cast<Company>(companyEntity);

    // Listar cases da empresa
    auto cases = targetCompany->getActiveCases();
    if (cases.empty()) {
        std::cout << "\nNenhum case disponível para " << targetCompany->getName() << ".\n";
        return;
    }

    std::cout << "\n=== Cases de " << targetCompany->getName() << " ===\n";
    for (const auto& caseItem : cases) {
        if (caseItem) {
            std::cout << "\nID: " << caseItem->getId() << "\n";
            std::cout << "Nome: " << caseItem->getName() << "\n";
            std::cout << "Descrição: " << caseItem->getDescription() << "\n";
            std::cout << "Pontos: " << caseItem->getPoints() << "\n";
            std::cout << "-----------------------------------\n";
        }
    }
}


void System::submitCaseSolution() {
    if (!current_user) {
        throw std::runtime_error("Você precisa estar logado!");
    }

    int caseId = Utils::getIntInput("Digite o ID do case: ");
    std::string answer = Utils::getInput("Digite sua resposta/solução: ");

    try {
        // Buscar empresa proprietária do case
        std::shared_ptr<Company> owner = nullptr;
        
        for (const auto& company : all_companies) {
            auto c = std::dynamic_pointer_cast<Company>(company);
            if (!c) continue;

            auto caseItem = c->getCase(caseId);
            if (caseItem) {
                owner = c;
                break;
            }
        }

        if (!owner) {
            std::cerr << "Case não encontrado em nenhuma empresa!\n";
            return;
        }

        socialNetwork.attemptSolveCase(caseId, answer, owner);
        modified = true;
        
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }
}

void System::searchCases() {
    std::string name = Utils::getInput("Digite o nome do case que deseja buscar: ");
    
    if (name.empty()) {
        throw std::invalid_argument("Nome de busca não pode ser vazio.");
    }
 
    bool found = false;
 
    std::cout << "\n=== Resultados da Busca por '" << name << "' ===\n";
 
    // buscar em cases globais
    for(const auto& caseItem : caseSystemGlobal.getAllCases()) {
        if (caseItem && caseItem->getName().find(name) != std::string::npos) {
            std::cout << "\n[GLOBAL] ID: " << caseItem->getId() << "\n";
            std::cout << "Nome: " << caseItem->getName() << "\n";
            std::cout << "Pontos: " << caseItem->getPoints() << "\n";
            std::cout << "Status: " << (caseItem->getState() ? "Ativo" : "Inativo") << "\n";
            found = true;
        }
    }

    if (!found) {
        std::cout << "\nNenhum case encontrado com esse nome.\n";
    }
 
    std::cout << "===========================================\n";
}

//==================================Métodos de Ranking==================================

void System::viewGlobalRanking() {
    socialNetwork.displayGlobalRanking();
}

void System::viewCompanyRanking() {
    std::string input = Utils::getInput("Digite o ID ou nome da empresa: ");
    
    std::shared_ptr<Entity> companyEntity = nullptr;
    
    // Tenta converter para int, se funcionar busca por ID, senão busca por nome
    try {
        int companyId = std::stoi(input);
        companyEntity = Utils::findEntity(&all_companies, companyId);
    }
    catch(...) {
        companyEntity = Utils::findEntity(&all_companies, input);
    }

    if (!companyEntity) {
        std::cout << "Empresa não encontrada.\n";
        return;
    }
    
    auto targetCompany = std::dynamic_pointer_cast<Company>(companyEntity);
 
    auto ranking = targetCompany->getRanking();
    if (!ranking) {
        std::cout << "Ranking não inicializado.\n";
        return;
    }

    std::cout << "\n=== Ranking de " << targetCompany->getName() << " ===\n";
    int posicao = 1;
    for (const auto& user : ranking->getUsers()) {
    if (user) {
        std::cout << posicao << ". " << user->getName() << " - " << user->getTotalScore() << " pontos\n";
        posicao++;
    }
}
std::cout << "====================================\n";
}

void System::viewMyRanking() {
    if (!current_user) {
        throw std::runtime_error("Você precisa estar logado para ver seu ranking.");
    }
    socialNetwork.displayUserRanking();
}

//==================================Métodos Setters==================================

void System::updateUserName() {
    if (!current_user) {
        throw std::runtime_error("Você precisa estar logado.");
    }
    
    std::string newName = Utils::getInput("Digite o novo nome: ");
    current_user->setName(newName);
    modified = true;
    std::cout << " Nome atualizado com sucesso!\n";
}

void System::updateUserEmail() {
    if (!current_user) {
        throw std::runtime_error("Você precisa estar logado.");
    }
    
    std::string newEmail = Utils::getInput("Digite o novo email: ");
    
    // Verifica se email já existe
    for (const auto& user : all_users) {
        if (user->getEmail() == newEmail && user != current_user) {
            std::cerr << " Email já está em uso!\n";
            return;
        }
    }
    for (const auto& company : all_companies) {
        if (company->getEmail() == newEmail && company != current_user) {
            std::cerr << " Email já está em uso!\n";
            return;
        }
    }
    
    current_user->setEmail(newEmail);
    modified = true;
    std::cout << " Email atualizado com sucesso!\n";
}

void System::updateUserPassword() {
    if (!current_user) {
        throw std::runtime_error("Você precisa estar logado.");
    }
    
    std::string oldPassword = Utils::getInput("Digite a senha atual: ");
    
    if (!current_user->checkPassword(oldPassword)) {
        std::cerr << " Senha atual incorreta!\n";
        return;
    }
    
    std::string newPassword = Utils::getInput("Digite a nova senha: ");
    std::string confirmPassword = Utils::getInput("Confirme a nova senha: ");
    
    if (newPassword != confirmPassword) {
        std::cerr << " As senhas não coincidem!\n";
        return;
    }
    
    current_user->setPassword(newPassword);
    modified = true;
    std::cout << " Senha atualizada com sucesso!\n";
}

void System::updateUserDescription() {
    if (!current_user) {
        throw std::runtime_error("Você precisa estar logado.");
    }
    
    std::string newDescription = Utils::getInput("Digite a nova descrição: ");
    current_user->setDescription(newDescription);
    modified = true;
    std::cout << " Descrição atualizada com sucesso!\n";
}

void System::updateUserPhone() {
    if (!current_user) {
        throw std::runtime_error("Você precisa estar logado.");
    }
    
    std::string newPhone = Utils::getInput("Digite o novo telefone: ");
    current_user->setPhoneNumber(newPhone);
    modified = true;
    std::cout << " Telefone atualizado com sucesso!\n";
}

void System::editProfile() {
    if (!current_user) {
        throw std::runtime_error("Você precisa estar logado.");
    }
    
    std::cout << "\n╔════════════════════════════════════╗\n";
    std::cout << "║        EDITAR PERFIL              ║\n";
    std::cout << "╚════════════════════════════════════╝\n";
    std::cout << "1. Alterar Nome\n";
    std::cout << "2. Alterar Email\n";
    std::cout << "3. Alterar Senha\n";
    std::cout << "4. Alterar Descrição\n";
    std::cout << "5. Alterar Telefone\n";
    std::cout << "6. Voltar\n";
    std::cout << "────────────────────────────────────\n";
    
    std::string choice = Utils::getInput("Escolha uma opção: ");
    
    if (choice == "1") {
        updateUserName();
    }
    else if (choice == "2") {
        updateUserEmail();
    }
    else if (choice == "3") {
        updateUserPassword();
    }
    else if (choice == "4") {
        updateUserDescription();
    }
    else if (choice == "5") {
        updateUserPhone();
    }
}
