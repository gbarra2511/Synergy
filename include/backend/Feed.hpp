#ifndef FEED_H
#define FEED_H

#include <vector>
#include <memory>
#include <ostream>
#include <stdexcept>
#include "Post.hpp"

class Feed {
private:
  /**
   * @brief A vector storing shared pointers to the posts contained in the feed.
   */
  std::vector<std::shared_ptr<Post>> _posts;

public:
  /**
   * @brief Constructs a new empty Feed object.
   */
  Feed();

  /**
   * @brief Destroys the Feed object.
   */
  virtual ~Feed() = default;

  // --- Getter ---

  /**
   * @brief Retrieves a post from the feed by its index.
   * @param index The index of the post to retrieve.
   * @return std::shared_ptr<Post> A shared pointer to the requested post.
   */
  std::shared_ptr<Post> getPost(const int index);

  /**
   * @brief Returns the total number of posts in the feed.
   * @return int Count of posts.
   */
  int getNumPosts() const;

  // --- Outros Métodos ---

  /**
   * @brief Adds a single post to the feed.
   * @param post Shared pointer to the post to be added.
   */
  void addPost(const std::shared_ptr<Post>& post);

  /**
   * @brief Removes a specific post from the feed.
   * @param post Shared pointer to the post to be removed.
   */
  void removePost(const std::shared_ptr<Post>& post);

  /**
   * @brief Adds a list of posts to the feed.
   * @param posts Vector of shared pointers to the posts to be added.
   */
  void addPosts(const std::vector<std::shared_ptr<Post>>& post);

  /**
   * @brief Removes a list of posts from the feed.
   * @param posts Vector of shared pointers to the posts to be removed.
   */
  void removePosts(const std::vector<std::shared_ptr<Post>>& post);

  /**
   * @brief Removes all posts from the feed.
   */
  void clear();

  /**
   * @brief Sorts the posts in the feed based on their timestamp.
   */
  void sortByDate();

  /**
   * @brief Overload of the insertion operator to print the feed content.
   * @param os The output stream.
   * @param feed The feed object to be printed.
   * @return std::ostream& The updated output stream.
   */
  friend std::ostream& operator<<(std::ostream& os, const Feed& feed);
};

#endif
