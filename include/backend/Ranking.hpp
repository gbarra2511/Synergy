#ifndef RANKING_HPP
#define RANKING_HPP

#include <vector>
#include <memory>
#include <algorithm>
#include "User.hpp"

/**
 * @file Ranking.hpp
 * @brief Declaration of the Ranking class(Abstract), which manages and displays user rankings.
 */

/**
 * @class Ranking
 * @brief Manages a collection of users and provides methods to sort and display rankings.
 */
class Ranking {
protected:
    /// @brief Vector of shared pointers to the users in the ranking.
    std::vector<std::shared_ptr<User>> _users;

public:
    /**
    * @brief Construct a new Ranking object
    * 
    */
    Ranking();

    

    // Getters

    /**
    * @brief Gets the _users shared pointer.
    * 
    */
    std::vector<std::shared_ptr<User>>& getUsers();

    /**
    * @brief Gets the number of users in the ranking.
    * 
    */
    int getSize() const;




    // Outros Métodos

    /**
     * @brief Adds a new user to the ranking list.
     * @param user A shared pointer (std::shared_ptr) to the User object to be added.
     */
    void addUser(std::shared_ptr<User> user);
    
    /**
     * @brief Abstract method for score.
     */
    void sort();



    // Não implementados

    /**
     * @brief Displays the current ranking to the console.
     * 
     */
    void display() const;
    
    /**
     * @brief Destroy the Ranking object
     * 
     */
    ~Ranking() = default;
};

#endif
