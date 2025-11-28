#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "../database/DbManager.hpp"
#include "SocialNetwork.hpp"
#include "CaseSystem.hpp"
#include "Ranking.hpp"
#include "Utils.hpp"
#include <vector>
#include <memory>
#include <string>

/**
 * @brief Main system controller class.
 * @details acts as a Facade, integrating the Database, Social Network logic, 
 * Case management, and User interaction (Menus).
 */
class System {
private:
  /**
   * @brief Manager responsible for SQLite database operations.
   */
  DbManager dbManager;

  /**
   * @brief Subsystem responsible for feed, posts, and social interactions.
   */
  SocialNetwork socialNetwork;

  /**
   * @brief Global system for managing cases unrelated to specific companies.
   */
  CaseSystem caseSystemGlobal;

  /**
   * @brief Global ranking system for all users.
   */
  Ranking rankingGlobal;

  /**
   * @brief List of all registered users in the system.
   */
  std::vector<std::shared_ptr<Entity>> all_users;

  /**
   * @brief Pointer to the currently logged-in user (or nullptr if not logged in).
   */
  std::shared_ptr<Entity> current_user;

  /**
   * @brief List of all registered companies.
   */
  std::vector<std::shared_ptr<Entity>> all_companies;  

  /**
   * @brief List of all posts created in the network.
   */
  std::vector<std::shared_ptr<Post>> all_posts;

  /**
   * @brief Flag indicating if there are unsaved changes in memory.
   */
  bool modified;

  /**
   * @brief Loads user data from the database into memory.
   */
  void loadUsers();

  /**
   * @brief Loads company data from the database into memory.
   */
  void loadCompanies();

  /**
   * @brief Loads global cases from the database into memory.
   */
  void loadGlobalCases();

  /**
   * @brief Loads company-specific cases from the database into memory.
   */
  void loadCompanyCases();
  /**
   * @brief Loads the global ranking data.
   */
  void loadGlobalRanking();

  /**
   * @brief Loads posts from the database into memory.
   */
  void loadPosts();

public:
  /**
   * @brief Constructs the System object and initializes data loading.
   */
  System(std::string dbName = "./rede_social.db");

  /**
   * @brief Destroys the System object and saves data to the database.
   */
  ~System();

  // --- Database Methods --- 

  /**
   * @brief Saves all current runtime data (Users, Companies, Posts, Cases) to the database.
   */
  void saveAllData();

  /**
   * @brief Orchestrates the loading of all data from the database.
   */
  void loadData();

  // --- Menu Methods (return user choice) ---

  /**
   * @brief Displays the main entry menu (Login/Register).
   * @return String representing the user's menu choice.
   */
  std::string showMainMenu();

  /**
   * @brief Displays the menu for logged-in regular users.
   * @return String representing the user's menu choice.
   */
  std::string showUserMenu();

  /**
   * @brief Displays the menu for logged-in company accounts.
   * @return String representing the user's menu choice.
   */
  std::string showCompanyMenu();

  /**
   * @brief Displays the social network interaction menu.
   * @return String representing the user's menu choice.
   */
  std::string showSocialMenu();

  /**
   * @brief Displays the case management menu.
   * @return String representing the user's menu choice.
   */
  std::string showCaseMenu();

  /**
   * @brief Displays the ranking visualization menu.
   * @return String representing the user's menu choice.
   */
  std::string showRankingMenu();

  // --- Auxiliary Methods ---

  /**
   * @brief Checks if there is a user currently logged in.
   * @return true if a user is logged in, false otherwise.
   */
  bool hasCurrentUser() const;

  /**
   * @brief Checks if the current logged-in user is a company.
   * @return true if current user is a company, false otherwise.
   */
  bool isCurrentUserCompany() const;

  /**
   * @brief Displays the profile of the currently logged-in user.
   */
  void viewMyProfile();

  /**
   * @brief Displays the ranking of the current company (company users only).
   */
  void viewMyCompanyRanking();

  /**
   * @brief Interactive menu for interacting with posts (like, comment, delete).
   */
  void interactWithPost();

  /**
   * @brief Interactive menu for managing follow/unfollow actions.
   */
  void manageFollowUnfollow();

  /**
   * @brief Interactive interface for adding a new case (company users only).
   */
  void addCaseInteractive();

  /**
   * @brief Lists all cases belonging to the current company.
   */
  void listMyCases();

  /**
   * @brief Toggles the active/inactive status of a case.
   */
  void toggleCaseStatus();

  /**
   * @brief Removes a case from the system.
   */
  void removeCase();

  // --- Login/Logout/Register Methods ---

  /**
   * @brief Logs in a user or company by prompting for credentials.
   */
  void login();

  /**
   * @brief Logs out the currently logged-in user.
   */
  void logout();

  /**
   * @brief Registers a new regular user in the system by prompting for data.
   */
  void registerUser();

  /**
   * @brief Registers a new company in the system by prompting for data.
   */
  void registerCompany();

  // --- View Methods ---

  /**
   * @brief Displays the feed for the current user.
   */
  void viewMyFeed();

  /**
   * @brief Displays posts created by the current user.
   */
  void viewMyPosts();

  /**
   * @brief Displays the profile of a specific user or company by prompting for name.
   */
  void viewProfile();

  // --- Follow Methods --- 

  /**
   * @brief Lists all followers of the current user.
   */
  void listFollowers();

  /**
   * @brief Lists all entities the current user follows.
   */
  void listFollowing();

  // --- Post Methods ---

  /**
   * @brief Creates a new post by prompting for content.
   */
  void createPost();

  // --- Case Methods ---

  /**
   * @brief Lists all registered companies.
   */
  void listAllCompanies();

  /**
   * @brief Lists all global cases available.
   */
  void listGlobalCases();

  /**
   * @brief Lists cases belonging to a specific company by prompting for company ID/name.
   */
  void listCompanyCases();

  /**
   * @brief Submits a solution for a specific case by prompting for case ID and answer.
   */
  void submitCaseSolution();

  /**
   * @brief Searches for cases by name.
   */
  void searchCases();

  // --- Ranking Methods --- 

  /**
   * @brief Displays the global ranking of users.
   */
  void viewGlobalRanking();

  /**
   * @brief Displays the ranking specific to a company by prompting for company ID/name.
   */
  void viewCompanyRanking();

  /**
   * @brief Displays the current user's position in the ranking.
   */
  void viewMyRanking();

  // --- Profile Editing Methods (Setters) ---

  /**
   * @brief Updates the current user's name.
   */
  void updateUserName();

  /**
   * @brief Updates the current user's email address.
   */
  void updateUserEmail();

  /**
   * @brief Updates the current user's password.
   */
  void updateUserPassword();

  /**
   * @brief Updates the current user's description/bio.
   */
  void updateUserDescription();

  /**
   * @brief Updates the current user's phone number.
   */
  void updateUserPhone();

  /**
   * @brief Interactive menu for editing profile information.
   */
  void editProfile();
};

#endif