#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <regex>

class Entity {
private:
  /**
   * @brief Total count of instantiated entities.
   */
  static int _num_entities;

  /**
   * @brief Next ID available for assignment.
   */
  static int _next_id;

protected:
  /**
   * @brief Unique entity identifier.
   */
  int _id;

  /**
   * @brief Entity's name.
   */
  std::string _name;

  /**
   * @brief Entity's email address.
   */
  std::string _email;

  /**
   * @brief Entity's phone number.
   */
  std::string _phone_number;

  /**
   * @brief Account password.
   */
  std::string _password;

  /**
   * @brief Entity's active status.
   */
  bool _state;

  /**
   * @brief Entity's description or bio.
   */
  std::string _description; 

  /**
   * @brief List of usernames that follow this entity.
   */
  std::vector<std::string> _followers;

  /**
   * @brief List of usernames that this entity follows.
   */
  std::vector<std::string> _following;

public:
  /**
   * @brief Constructs a new Entity object.
   * @param name Entity's name.
   * @param email Entity's email.
   * @param phone_number Entity's phone number.
   * @param password Account password.
   * @param state Initial account state (true for active).
   * @param description Profile description.
   */
  Entity(const std::string& name, const std::string& email,
          const std::string& phone_number, const std::string& password, bool state, const std::string& description);

  /**
   * @brief Destroys the Entity object (virtual).
   */
  virtual ~Entity();

  // --- Getters ---

  /**
   * @brief Gets the entity ID.
   * @return int The unique identifier.
   */
  int getId() const;

  /**
   * @brief Gets the entity name.
   * @return std::string The name.
   */
  std::string getName() const;

  /**
   * @brief Gets the entity email.
   * @return std::string The email.
   */
  std::string getEmail() const;

  /**
   * @brief Gets the entity phone number.
   * @return std::string The phone number.
   */
  std::string getPhoneNumber() const;

  /**
   * @brief Gets the profile description.
   * @return std::string The description.
   */
  std::string getDescription() const;

  /**
   * @brief Gets the list of users this entity is following.
   * @return std::vector<std::string>& Reference to the following vector.
   */
  std::vector<std::string>& getFollowing();      

  /**
   * @brief Gets the list of followers.
   * @return std::vector<std::string>& Reference to the followers vector.
   */
  std::vector<std::string>& getFollowers();       

  /**
   * @brief Gets the account password.
   * @return std::string The current password.
   */
  std::string getPassword() const;

  /**
   * @brief Gets the account state.
   * @return true If the account is active.
   * @return false If the account is inactive.
   */
  bool getState() const;

  /**
   * @brief Checks if the provided password matches the account password.
   * @param password_to_verify The password to test.
   * @return true If passwords match.
   * @return false If passwords do not match.
   */
  bool checkPassword(const std::string& password_to_verify) const;

  // --- Setters --- 

  /**
   * @brief Sets a new name for the entity.
   * @param name The new name.
   */
  void setName(const std::string& name);

  /**
   * @brief Sets a new email address.
   * @param email The new email.
   */
  void setEmail(const std::string& email);

  /**
   * @brief Sets a new phone number.
   * @param phone_number The new phone number.
   */
  void setPhoneNumber(const std::string& phone_number);

  /**
   * @brief Sets the account state.
   * @param state The new state (true for active).
   */
  void setState(bool state);

  /**
   * @brief Sets a new description for the profile.
   * @param description The new description.
   */
  void setDescription(const std::string& description);

  /**
   * @brief Sets a new password.
   * @param password The new password.
   */
  void setPassword(const std::string& password);
  
  /**
   * @brief Adds a follower to the list.
   * @param username The username of the new follower.
   */
  void addFollower(const std::string& username);

  /**
   * @brief Adds a user to the following list.
   * @param username The username to follow.
   */
  void addFollowing(const std::string& username);

  /**
   * @brief Removes a follower from the list.
   * @param username The username to remove.
   */
  void removeFollower(const std::string& username);

  /**
   * @brief Removes a user from the following list.
   * @param username The username to stop following.
   */
  void removeFollowing(const std::string& username);
  
  // --- Outros Métodos ---

  /**
   * @brief Displays the entity's profile (Pure virtual).
   */
  virtual void displayProfile() = 0;


  /**
   * @brief Overload of the insertion operator to print the entity.
   * @param os The output stream.
   * @param entity The entity to print.
   * @return std::ostream& The updated output stream.
   */
  friend std::ostream& operator<<(std::ostream& os, const Entity& entity);
};

#endif
