#include "../include/backend/System.hpp"
#include "../include/backend/Utils.hpp"
#include <iostream>

#ifdef _WIN32
    #include <windows.h>
#endif

int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(65001); // 65001 é o código para UTF-8
    #endif
    
    Utils::clearScreen();

    System system("./rede_social.db");
    bool running = true;

    while (running) {
        try {
            // Se não há usuário logado, mostra menu principal
            if (!system.hasCurrentUser()) {
                std::string choice = system.showMainMenu();
                Utils::clearScreen();
                
                if (choice == "1") {
                    system.login();
                }
                else if (choice == "2") {
                    system.registerUser();
                }
                else if (choice == "3") {
                    system.registerCompany();
                }
                else if (choice == "4") {
                    std::cout << "\n✓ Obrigado por usar o Synergy!\n";
                    running = false;
                }
                else {
                    std::cout << " Opção inválida!\n";
                }
            }
            // Se há usuário logado
            else {
                // Verifica se é empresa ou usuário
                if (system.isCurrentUserCompany()) {
                    // ==================== MENU EMPRESA ====================
                    std::string choice = system.showCompanyMenu();
                    Utils::clearScreen();
                    
                    if (choice == "1") {
                        // Submenu de cases para empresa
                        bool inCaseMenu = true;
                        while (inCaseMenu && system.hasCurrentUser()) {
                            try {
                                std::string caseChoice = system.showCaseMenu();
                                Utils::clearScreen();
                                
                                if (caseChoice == "1") {
                                    system.addCaseInteractive();
                                }
                                else if (caseChoice == "2") {
                                    system.removeCase();
                                }
                                else if (caseChoice == "3") {
                                    system.toggleCaseStatus();
                                }
                                else if (caseChoice == "4") {
                                    system.listMyCases();
                                }
                                else if (caseChoice == "5") {
                                    system.listGlobalCases();
                                }
                                else if (caseChoice == "6") {
                                    system.searchCases();
                                }
                                else if (caseChoice == "7") {
                                    inCaseMenu = false;
                                }
                                else {
                                    std::cout << " Opção inválida!\n";
                                }
                            }
                            catch (const std::exception& e) {
                                std::cout << " Erro: " << e.what() << "\n";
                            }
                        }
                    }
                    else if (choice == "2") {
                        system.viewMyCompanyRanking();
                    }
                    else if (choice == "3") {
                        // Submenu rede social
                        bool inSocialMenu = true;
                        while (inSocialMenu && system.hasCurrentUser()) {
                            try {
                                std::string socialChoice = system.showSocialMenu();
                                Utils::clearScreen();
                                
                                if (socialChoice == "1") {
                                    system.viewMyFeed();
                                }
                                else if (socialChoice == "2") {
                                    system.viewMyPosts();
                                }
                                else if (socialChoice == "3") {
                                    system.createPost();
                                }
                                else if (socialChoice == "4") {
                                    system.interactWithPost();
                                }
                                else if (socialChoice == "5") {
                                    system.manageFollowUnfollow();
                                }
                                else if (socialChoice == "6") {
                                    system.viewProfile();
                                }
                                else if (socialChoice == "7") {
                                    system.listFollowers();
                                }
                                else if (socialChoice == "8") {
                                    system.listFollowing();
                                }
                                else if (socialChoice == "9") {
                                    inSocialMenu = false;
                                }
                                else {
                                    std::cout << " Opção inválida!\n";
                                }
                            }
                            catch (const std::exception& e) {
                                std::cout << " Erro: " << e.what() << "\n";
                            }
                        }
                    }
                    else if (choice == "4") {
                        // Submenu de perfil
                        bool inProfileMenu = true;
                        while (inProfileMenu && system.hasCurrentUser()) {
                            std::cout << "\n1. Ver Perfil\n";
                            std::cout << "2. Editar Perfil\n";
                            std::cout << "3. Voltar\n";
                            
                            std::string profileChoice = Utils::getInput("Escolha uma opção: ");
                            Utils::clearScreen();
                            
                            if (profileChoice == "1") {
                                system.viewMyProfile();
                            }
                            else if (profileChoice == "2") {
                                system.editProfile();
                            }
                            else if (profileChoice == "3") {
                                inProfileMenu = false;
                            }
                            else {
                                std::cout << " Opção inválida!\n";
                            }
                        }
                    }
                    else if (choice == "5") {
                        system.logout();
                    }
                    else {
                        std::cout << " Opção inválida!\n";
                    }
                }
                else {
                    // ==================== MENU USUÁRIO ====================
                    std::string choice = system.showUserMenu();
                    Utils::clearScreen();
                    
                    if (choice == "1") {
                        // Submenu rede social
                        bool inSocialMenu = true;
                        while (inSocialMenu && system.hasCurrentUser()) {
                            try {
                                std::string socialChoice = system.showSocialMenu();
                                Utils::clearScreen();
                                
                                if (socialChoice == "1") {
                                    system.viewMyFeed();
                                }
                                else if (socialChoice == "2") {
                                    system.viewMyPosts();
                                }
                                else if (socialChoice == "3") {
                                    system.createPost();
                                }
                                else if (socialChoice == "4") {
                                    system.interactWithPost();
                                }
                                else if (socialChoice == "5") {
                                    system.manageFollowUnfollow();
                                }
                                else if (socialChoice == "6") {
                                    system.viewProfile();
                                }
                                else if (socialChoice == "7") {
                                    system.listFollowers();
                                }
                                else if (socialChoice == "8") {
                                    system.listFollowing();
                                }
                                else if (socialChoice == "9") {
                                    inSocialMenu = false;
                                }
                                else {
                                    std::cout << " Opção inválida!\n";
                                }
                            }
                            catch (const std::exception& e) {
                                std::cout << " Erro: " << e.what() << "\n";
                            }
                        }
                    }
                    else if (choice == "2") {
                        // Submenu cases para usuário
                        bool inCaseMenu = true;
                        while (inCaseMenu && system.hasCurrentUser()) {
                            try {
                                std::string caseChoice = system.showCaseMenu();
                                Utils::clearScreen();
                                
                                if (caseChoice == "1") {
                                    system.listGlobalCases();
                                }
                                else if (caseChoice == "2") {
                                    system.listCompanyCases();
                                }
                                else if (caseChoice == "3") {
                                    system.submitCaseSolution();
                                }
                                else if (caseChoice == "4") {
                                    system.searchCases();
                                }
                                else if (caseChoice == "5") {
                                    inCaseMenu = false;
                                }
                                else {
                                    std::cout << " Opção inválida!\n";
                                }
                            }
                            catch (const std::exception& e) {
                                std::cout << " Erro: " << e.what() << "\n";
                            }
                        }
                    }
                    else if (choice == "3") {
                        // Submenu rankings
                        bool inRankingMenu = true;
                        while (inRankingMenu && system.hasCurrentUser()) {
                            try {
                                std::string rankChoice = system.showRankingMenu();
                                Utils::clearScreen();
                                
                                if (rankChoice == "1") {
                                    system.viewGlobalRanking();
                                }
                                else if (rankChoice == "2") {
                                    system.viewMyRanking();
                                }
                                else if (rankChoice == "3") {
                                    system.viewCompanyRanking();
                                }
                                else if (rankChoice == "4") {
                                    inRankingMenu = false;
                                }
                                else {
                                    std::cout << " Opção inválida!\n";
                                }
                            }
                            catch (const std::exception& e) {
                                std::cout << " Erro: " << e.what() << "\n";
                            }
                        }
                    }
                    else if (choice == "4") {
                        // Submenu de perfil
                        bool inProfileMenu = true;
                        while (inProfileMenu && system.hasCurrentUser()) {
                            std::cout << "\n1. Ver Perfil\n";
                            std::cout << "2. Editar Perfil\n";
                            std::cout << "3. Voltar\n";
                            
                            std::string profileChoice = Utils::getInput("Escolha uma opção: ");
                            Utils::clearScreen();
                            
                            if (profileChoice == "1") {
                                system.viewMyProfile();
                            }
                            else if (profileChoice == "2") {
                                system.editProfile();
                            }
                            else if (profileChoice == "3") {
                                inProfileMenu = false;
                            }
                            else {
                                std::cout << " Opção inválida!\n";
                            }
                        }
                    }
                    else if (choice == "5") {
                        system.logout();
                    }
                    else {
                        std::cout << " Opção inválida!\n";
                    }
                }
            }
        }
        catch (const std::exception& e) {
            std::cout << " Erro: " << e.what() << "\n";
        }
    }

    std::cout << "\n════════════════════════════════════\n";
    std::cout << "    Sistema encerrado com sucesso!    \n";
    std::cout << "════════════════════════════════════\n";

    return 0;
}
