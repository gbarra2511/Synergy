#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>
#include <memory>
#include "Entity.hpp"
#include "Post.hpp"

/**
 * @brief Utility functions namespace
 */
namespace Utils
{
  /**
   * @brief Finds multiple entities by their names
   * @param all_entities Pointer to vector containing all entities
   * @param names Vector of entity names to search for
   * @return Vector of shared pointers to found entities
   */
  std::vector<std::shared_ptr<Entity>> findEntities(const std::vector<std::shared_ptr<Entity>>* all_entities, const std::vector<std::string>& names);

  /**
   * @brief Finds a single entity by name
   * @param all_entities Pointer to vector containing all entities
   * @param name Name of the entity to search for
   * @return Shared pointer to the found entity
   */
  std::shared_ptr<Entity> findEntity(const std::vector<std::shared_ptr<Entity>>* all_entities,  const std::string& name);

  /**
   * @brief Finds a single entity by ID
   * @param all_entities Pointer to vector containing all entities
   * @param ID ID of the entity to search for
   * @return Shared pointer to the found entity
   */
  std::shared_ptr<Entity> findEntity(const std::vector<std::shared_ptr<Entity>>* all_entities,  const int ID);

  /**
   * @brief Finds multiple posts by their names
   * @param all_posts Pointer to vector containing all posts
   * @param names Vector of post names to search for
   * @return Vector of shared pointers to found posts
   */
  std::vector<std::shared_ptr<Post>> findPosts(const std::vector<std::shared_ptr<Post>>* all_posts, const std::vector<std::string>& names);

  /**
   * @brief Finds posts by a single name
   * @param all_posts Pointer to vector containing all posts
   * @param name Name to search for
   * @return Vector of shared pointers to found posts
   */
  std::vector<std::shared_ptr<Post>> findPosts(const std::vector<std::shared_ptr<Post>>* all_posts, const std::string& name);

  /**
   * @brief Finds a single post by ID
   * @param all_posts Pointer to vector containing all posts
   * @param ID ID of the post to search for
   * @return Shared pointer to the found post
   */
  std::shared_ptr<Post> findPost( const std::vector<std::shared_ptr<Post>>* all_posts, const int ID);

  /**
   * @brief Finds a name in a vector of names
   * @param names Vector of names to search in
   * @param name Name to search for
   * @return The found name string
   */
  std::string findName(const std::vector<std::string>& names, const std::string& name);

  /**
   * @brief Gets string input from user
   * @param prompt Prompt message to display
   * @return User input as string
   */
  std::string getInput(const std::string& prompt);

  /**
   * @brief Gets integer input from user
   * @param prompt Prompt message to display
   * @return User input as integer
   */
  int getIntInput(const std::string& prompt);

  /**
   * @brief Clears the terminal screen
   */
  void clearScreen();
}

#endif