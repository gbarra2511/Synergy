#ifndef CASE_HPP
#define CASE_HPP

#include <iostream>
#include <string>
#include "User.hpp"
#include "Company.hpp"

/**
 * @file Case.hpp
 * @brief Declaration of the Case class.
 */ 
 

/**
 * @class Case
 * @brief Represents a challenge or problem that can be solved by a user.
 */
class Case {
private:
    int _id;                           ///< @brief Unique case identifier.
    static int _next_id;
    std::string _name;                 ///< @brief Case name/title.
    std::string _description;          ///< @brief Case description/details.
    std::string _solution;             ///< @brief Expected solution for the case.
    double _points;                    ///< @brief Number of points the case is worth.
    bool _state;                       ///< @brief Case active status.

public:
    /**
     * @brief Constructs a new Case object.
     * @param id Case ID.
     * @param name Case name.
     * @param description Case description.
     */
    Case(const std::string& name, const std::string& description, const std::string& solution, const double& points);

    /**
     * @brief Destroy the Case object virtual
     * 
     */
    virtual ~Case();



    // Getters

    /**
     * @brief Get the Id object
     * 
     * @return int 
     */
    int getId() const;

    /**
     * @brief Get the Name object
     * 
     * @return const std::string& 
     */
    const std::string& getName() const;

    /**
     * @brief Get the Description object
     * 
     * @return const std::string& 
     */
    const std::string& getDescription() const;

    /**
     * @brief Get the Solution object
     * 
     * @return const std::string& 
     */
    const std::string& getSolution() const;

    /**
     * @brief Gets case active status.
     * @return True if active, false otherwise.
     */
    bool getState() const;

    /**
     * @brief Gets amount of points the case is worth.
     * @return The amout of points the case is worth.
     */
    double getPoints() const;




    // Setters

    /**
     * @brief Set the Name object
     * @param name 
     */
    void setName(const std::string& name);

    /**
     * @brief Set the Description object
     * @param description 
     */
    void setDescription(const std::string& description);

    /**
     * @brief Set the Solution object
     * @param solution 
     */
    void setSolution(const std::string& solution);

    /**
     * @brief Sets case active status.
     * @param state New active status.
     */
    void setState(bool state);

    /**
     * @brief Set the amount of points the case is worth.
     * @param state The new amount of points.
     */
    void setPoints(double points);

};
#endif
