#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "../backend/User.hpp"
#include "../backend/Company.hpp"
#include "../backend/Case.hpp"
#include "../backend/CaseSystem.hpp"
#include "../backend/Ranking.hpp"
#include <iostream>
#include "../backend/SocialNetwork.hpp"
#include "../../include/backend/Utils.hpp"
#include <vector>
#include <memory>
#include <string>
#include "../database/sqlite3.h"

/**
 * @brief Manages database operations for the social network
 */
class DbManager{
private:
    sqlite3* db;
    std::string dbPath;
    
    /**
     * @brief Creates database tables
     */
    void createTables();
    
    /**
     * @brief Executes SQL command
     * @param sql SQL query to execute
     * @return True if successful, false otherwise
     */
    bool executerSQL(const std::string& sql);
    
public:
    /**
     * @brief Constructor
     * @param caminhoDb Path to database file
     */
    DbManager(const std::string& caminhoDb = "rede_social.db");
    
    /**
     * @brief Destructor
     */
    ~DbManager();
    
    // ==================== USERS ====================
    /**
     * @brief Saves users to database
     * @param usuarios Vector of users to save
     * @return True if successful, false otherwise
     */
    bool saveUsers(const std::vector<User>& usuarios);

    /**
     * @brief Loads users from database
     * @return Vector of loaded users
     */
    std::vector<User> loadUsers();
    
    // ==================== COMPANIES ====================
    /**
     * @brief Saves companies to database
     * @param companies Vector of companies to save
     * @return True if successful, false otherwise
     */
    bool saveCompany(const std::vector<Company>& companies);

    /**
     * @brief Loads companies from database
     * @return Vector of loaded companies
     */
    std::vector<Company> loadCompanies();
    
    // ==================== CASES ====================
    /**
     * @brief Saves global case system
     * @param caseSystem Case system to save
     * @return True if successful, false otherwise
     */
    bool saveCase(const CaseSystem& caseSystem);

    /**
     * @brief Loads global cases
     * @return Loaded case system
     */
    CaseSystem loadGlobalCases();
    
    // ==================== POSTS ====================
    /**
     * @brief Saves posts to database
     * @param posts Vector of posts to save
     * @return True if successful, false otherwise
     */
    bool savePosts(const std::vector<std::shared_ptr<Post>>& posts);
    
    /**
     * @brief Loads posts from database
     * @return Vector of loaded posts
     */
    std::vector<std::shared_ptr<Post>> loadPosts();

    // ==================== COMPANY CASES ====================
    /**
     * @brief Saves cases for specific company
     * @param companyId Company ID
     * @param caseSystem Case system to save
     * @return True if successful, false otherwise
     */
    bool saveCompanyCases(int companyId, const CaseSystem& caseSystem);

    /**
     * @brief Loads cases for specific company
     * @param companyId Company ID
     * @return Loaded case system
     */
    CaseSystem loadCompanyCases(int companyId);
    
    // ==================== RANKINGS ====================
    /**
     * @brief Saves global ranking
     * @param users Vector of users in ranking order
     * @return True if successful, false otherwise
     */
    bool saveGlobalRanking(const std::vector<std::shared_ptr<User>>& users);
    
    /**
     * @brief Loads global ranking
     * @return Vector of user IDs in ranking order
     */
    std::vector<int> loadGlobalRanking();
    
    /**
     * @brief Saves company ranking
     * @param companyId Company ID
     * @param users Vector of users in ranking order
     * @return True if successful, false otherwise
     */
    bool saveCompanyRanking(int companyId, const std::vector<std::shared_ptr<User>>& users);

    /**
     * @brief Loads company ranking
     * @param companyId Company ID
     * @return Vector of user IDs in ranking order
     */
    std::vector<int> loadCompanyRanking(int companyId);
    
    // ==================== SOLVED CASES ====================
    /**
     * @brief Saves solved cases for user
     * @param userId User ID
     * @param solvedCaseIds Vector of solved case IDs
     * @return True if successful, false otherwise
     */
    bool saveSolvedCases(int userId, const std::vector<int>& solvedCaseIds);
    
    /**
     * @brief Loads solved cases for user
     * @param userId User ID
     * @return Vector of solved case IDs
     */
    std::vector<int> loadSolvedCases(int userId);
    
    // ==================== UTILS ====================
    /**
     * @brief Loads database
     * @return True if successful, false otherwise
     */
    bool loadDB();
    
    /**
     * @brief Closes database connection
     */
    void closeDB();
    
    /**
     * @brief Verifies database connection
     * @return True if connected, false otherwise
     */
    bool verifyConnection();
};

#endif