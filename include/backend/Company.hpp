#ifndef COMPANY_HPP
#define COMPANY_HPP

#include <string>
#include <map>
#include <memory>
#include <vector>

#include "Entity.hpp"


class Case;
class CaseSystem;
class Ranking;

class Company : public Entity {
private:
    /// @brief Company's CNPJ (Brazilian business ID).
    std::string _cnpj;

    /// @brief Company-specific user ranking system.
    Ranking* _companyRanking;

    /// @brief Company-specific case management system.
    CaseSystem* _companyCaseSystem;

public:
/** 
     * @brief Constructs a new Company object.
     * @param name Company name.
     * @param email Company email.
     * @param phoneNumber Company phone number.
     * @param cnpj Company CNPJ.
     */
    Company(const std::string& name, const std::string& email,
          const std::string& phone_number, const std::string& password, 
          bool state, const std::string& description, const std::string& cnpj);

    /**
     * @brief Destroy the Company object
     * 
     */
    ~Company();



    // Getters


    /**
     * @brief Get the Case object
     * 
     * @param caseId 
     * @return std::shared_ptr<Case> 
     */
    std::shared_ptr<Case> getCase(int caseId) const;

    /**
     * @brief Get the Cnpj 
     * 
     * @return std::string 
     */
    std::string getCnpj() const;

    /**
     * @brief Gets the company's ranking object.
     * @return Pointer to the CompanyRanking object.
     */
    Ranking* getRanking();

    /**
     * @brief Gets all cases managed by the company.
     * @return const std::vector<std::shared_ptr<Case>>& Reference to vector of all cases.
     */
    const std::vector<std::shared_ptr<Case>>& getAllCases() ;

    /**
     * @brief Gets all active cases managed by the company.
     * @return std::vector<std::shared_ptr<Case>> All active cases.
     */
    std::vector<std::shared_ptr<Case>> getActiveCases() const;

    /**
     * @brief Gets all inactive cases managed by the company.
     * @return std::vector<std::shared_ptr<Case>> All inactive cases.
     */
    std::vector<std::shared_ptr<Case>> getInactiveCases() const;



    // Setters

    /**
     * @brief Set the Cnpj
     * 
     * @param cnpj 
     */
    void setCnpj(const std::string& cnpj);



    // Outros Métodos

    
    /**
     * @brief Add a case to the company
     * @param _activeCases
     * @param c 
     */
    void addCase(const std::shared_ptr<Case>& c);

    /**
     * @brief Close a case
     * 
     * @param caseName 
     */
    void removeCaseByName(const std::string& caseName);

    /**
     * @brief Activates a case by ID.
     * @param caseId ID of the case to activate.
     */
    void activateCase(int caseId);

    /**
     * @brief Deactivates a case by ID.
     * @param caseId ID of the case to deactivate.
     */
    void deactivateCase(int caseId);
    
    // Não Implementados

    /**
     * @brief Display the company profile
     * 
     */
    void displayProfile() override;
    /**
     * @brief Display the company ranking
     * 
     */
    //sobrecarga para manipulação com o banco de dados 
    CaseSystem& getCaseSystem();
    
    const CaseSystem& getCaseSystem() const;

    friend std::ostream& operator<<(std::ostream& os, const Company& company);
};
#endif
