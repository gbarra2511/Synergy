#ifndef CASESYSTEM_HPP
#define CASESYSTEM_HPP

#include "Case.hpp"
#include <vector>
#include <memory>

/**
 * @brief Class that manages the collection of cases in the system.
 * 
 */
class CaseSystem {

    private:
        std::vector<std::shared_ptr<Case>> _cases;
    
    public:
        /**
         * @brief Construct a new Case System object
         * 
         */
        CaseSystem();
        /**
         * @brief Destroy the Case System object
         * 
         */
        ~CaseSystem();



    // Getters

    /**
     * @brief Get a case by its ID
     * 
     * @param caseId 
     * @return std::shared_ptr<Case> 
     */
    std::shared_ptr<Case> getCase(int caseId) const;
        
    /**
     * @brief Get all cases in the system
     * @return const std::vector<std::shared_ptr<Case>>& 
     */
    const std::vector<std::shared_ptr<Case>>& getAllCases() const;

    /**
     * @brief Get all active cases in the system
     * @return std::vector<std::shared_ptr<Case>> List of active cases
     */
    std::vector<std::shared_ptr<Case>> getActiveCases() const;

    /**
     * @brief Get all inactive cases in the system
     * @return std::vector<std::shared_ptr<Case>> List of inactive cases
     */
    std::vector<std::shared_ptr<Case>> getInactiveCases() const;

    std::shared_ptr<Case> getCaseByName(const std::string& caseName);
   

    // Outros Métodos

    /**
     * @brief Add a new case to the system
     * 
     * @param newCase 
     */
    void addCase(std::shared_ptr<Case> newCase);
        
    // /**
    //  * @brief Remove a case from the system by its ID
    //  * 
    //  * @param caseId 
    //  */
    // void removeCase(int caseId);
    /**
     * @brief Remove a case from the system by its name
     * 
     * @param caseName 
     */
    void removeCaseByName(const std::string& caseName);

    /**
     * @brief Deactivate a case in the system by its ID
     * 
     * @param caseId 
     */
    void deactivateCase(int caseId);

    /**
     * @brief Activate a case in the system by its ID
     * 
     * @param caseId 
     */
    void activateCase(int caseId);
    



};

#endif