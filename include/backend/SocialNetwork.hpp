#ifndef SOCIALNETWORK_HPP
#define SOCIALNETWORK_HPP

#include "Feed.hpp"
#include "Entity.hpp"
#include "Ranking.hpp"
#include "CaseSystem.hpp"

class SocialNetwork 
{
private:
  /**
   * @brief The feed object associated with the current user session.
   */
  Feed feed;
  
  /**
   * @brief Pointer to the currently logged-in user.
   */
  const std::shared_ptr<Entity>* current_user;
  
  // APENAS REFERÊNCIA, NÃO OWNERSHIP
  const std::vector<std::shared_ptr<Entity>>* all_users;
  const std::vector<std::shared_ptr<Entity>>* all_companies;
  std::vector<std::shared_ptr<Post>>* all_posts;
  Ranking* _rankingGlobal;
  CaseSystem* all_cases;
  

public:

  /**
   * @brief Constructs a new Social Network controller.
   * @param current_user Pointer to the currently logged-in user.
   * @param all_users Pointer to the list of all users.
   * @param all_companies Pointer to the list of all companies.
   * @param all_posts Pointer to the list of all posts.
   * @param _rankingGlobal Pointer to the ranking object.
   * @param all_cases
   */
  SocialNetwork(const std::shared_ptr<Entity>* current_user,const std::vector<std::shared_ptr<Entity>>* all_users,
    const std::vector<std::shared_ptr<Entity>>* all_companies, std::vector<std::shared_ptr<Post>>* all_posts,
    Ranking* _rankingGlobal, CaseSystem* all_cases);

  /**
   * @brief Destroys the Social Network object.
   */
  ~SocialNetwork();

  /**
   * @brief Generates and displays the feed for the current user.
   * @details Includes posts from entities the user follows.
   */
  void viewMyFeed();

  /**
   * @brief Displays all posts created by the current user.
   */
  void viewMyPosts();

  /**
   * @brief Searches for and displays the profile of a specific entity.
   * @param profile_name The name of the user or company to view.
   * @throws std::invalid_argument If profile_name is empty.
   */
  void viewProfile(std::string profile_name);

  /**
   * @brief Creates a new post for the current user.
   * @param content The text content of the post.
   * @throws std::invalid_argument If content is empty.
   */
  void createPost(const std::string& content);

  /**
   * @brief Deletes a specific post.
   * @details Only deletes if the current user is the author of the post.
   * @param ID The unique identifier of the post to delete.
   */
  void deletePost(const int ID);

  /**
   * @brief Follows another user or company.
   * @param followed_name The name of the entity to follow.
   * @throws std::invalid_argument If name is invalid or user tries to follow themselves.
   * @throws std::logic_error If already following the entity.
   */
  void followEntity(const std::string& followed_name);

  /**
   * @brief Unfollows a user or company.
   * @param unfollowed_name The name of the entity to unfollow.
   * @throws std::invalid_argument If name is invalid.
   * @throws std::logic_error If not currently following the entity.
   */
  void unfollowEntity(const std::string& unfollowed_name);

  /**
   * @brief Adds a comment to a specific post.
   * @param postID The ID of the target post.
   * @param comment The text content of the comment.
   * @throws std::runtime_error If user is not logged in or post is not found.
   * @throws std::invalid_argument If comment is empty.
   */
  void commentPost(const int postID, const std::string& comment);

  /**
   * @brief Removes a specific comment from a post.
   * @param postID The ID of the post containing the comment.
   * @param which_comment The index of the comment to remove.
   * @throws std::runtime_error If user is not logged in or post is not found.
   */
  void removeCommentPost(const int postID, const int which_comment);

  /**
   * @brief Adds a like to a post.
   * @param postID The ID of the post to like.
   * @throws std::runtime_error If user is not logged in or post is not found.
   */
  void likePost(const int postID);

  /**
   * @brief Removes a like from a post.
   * @param postID The ID of the post to unlike.
   * @throws std::runtime_error If user is not logged in or post is not found.
   */
  void unlikePost(const int postID);

  /**
   * @brief Displays the top users in the global ranking.
   * @throws std::logic_error If ranking is not initialized or empty.
   */
  void displayGlobalRanking();

  /**
   * @brief Displays the current user's position in the global ranking.
   * @throws std::logic_error If ranking is empty.
   * @throws std::runtime_error If the current user is not found in the ranking.
   */
  void displayUserRanking();

  void attemptSolveCase(int caseId, const std::string& answer, std::shared_ptr<Company> company);
};

#endif
