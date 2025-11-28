#ifndef USER_H
#define USER_H

#include "Entity.hpp"
#include <iostream>
#include <map>
#include <memory>


class Company;
/**
 * @class User
 * @brief Represents a user, inheriting features from the base Entity class.
 * @details Manages personal information like name, email, CPF, and the user's scores in challenges from different companies.
 */
class User : public Entity {
private:

    /// @brief A map of the user's score per company. The key is a pointer to the Company.
    std::map<std::shared_ptr<Company>, double> _companyScores; // Pontuação total por empresa

    /// @brief The user's overall total score, summing points from all companies.
    double _totalScore; // Pontuação total geral

    /// @brief The user's CPF (Brazilian individual taxpayer registry ID).
    std::string _cpf;

    /// @brief Vector with IDs of solved cases by the user
    std::vector<int> _solvedCases; 

public:
    /**
     * @brief Constructor for the User class.
     * @param name The user's name.
     * @param email The user's email address.
     * @param cpf The user's CPF.
     */
    User(const std::string& name, const std::string& email,
        const std::string& phone_number, const std::string& password, bool state,
         const std::string& description, double totalScore, const std::string& cpf);




    // Getters


    std::string getCpf() const;
    /**
     * @brief Gets the user's score for a specific company.
     * @param company Pointer to the Company object whose score is desired.
     * @return The user's score for the specified company.
     */
    double getScoreByCompany(std::shared_ptr<Company> company) const;

    /**
     * @brief Gets the user's total score.
     * @return The user's accumulated total score.
     */
    double getTotalScore() const;

    /**
     * @brief Gets the user's solved cases. 
     * @return The user's solved cases vector.
     */
    std::vector<int>& getSolvedCases();

    /**
     * @brief Gets the user's solved cases. (read only)
     * @return The user's solved cases vector.
     */
    const std::vector<int>& getSolvedCases() const;


    // Setters

    /**
     * @brief Sets the user's total score.
     */
    void setTotalScore(double score); //EXCLUSIVAMENTE PARA TESTES


    
    // Outros Métodos

    /**
     * @brief Adds points to the user's score for a specific company.
     * @details Updates both the score per company and the overall total score.
     * @param company Pointer to the Company object to which the score refers.
     * @param score The amount of points to be added.
     */
    void addScore(std::shared_ptr<Company> company, double score);

    void setCpf(const std::string& cpf);

    /**
     * @brief Registers a solved case for the user.
     * @param caseId The id of the solved case.
     */
    void registerSolvedCase(int caseId);
    


    // Não implementados

    /**
     * @brief Overrides the base class method to display the user's profile.
     */
    void displayProfile() override;

    friend std::ostream& operator<<(std::ostream& os, const User& user);
};

#endif
