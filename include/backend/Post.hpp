#ifndef POST_H
#define POST_H

#include <string>
#include <vector>
#include <ctime>
#include <memory>
#include <iostream>
#include "Entity.hpp"

/**
 * @brief Structure representing a user comment on a post.
 */
struct Comment {
    /**
     * @brief The index/order of the comment within the post.
     */
    int index;  
    
    /**
     * @brief The username of the comment author.
     */
    std::string author;

    /**
     * @brief The text content of the comment.
     */
    std::string description;

    /**
     * @brief Constructs a new Comment object.
     * @param i Index of the comment.
     * @param a Author's username.
     * @param d Content of the comment.
     */
    Comment(int i, std::string a, const std::string& d)
        : index(i), author(a), description(d) {}
};

class Post {
private:
    /**
     * @brief Unique identifier for the post.
     */
    int _id;

    /**
     * @brief Static counter to generate unique IDs for new posts.
     */
    static int _next_id;

    /**
     * @brief Pointer to the Entity (User) who created the post.
     */
    std::string _author;

    /**
     * @brief The textual content of the post.
     */
    std::string _description;

    /**
     * @brief The time when the post was created.
     */
    std::time_t _timestamp;

    /**
     * @brief List of comments attached to this post.
     */
    std::vector<Comment> _comments;

    /**
     * @brief List of usernames who have liked this post.
     */
    std::vector<std::string> _liked_by;

public:
    /**
     * @brief Constructs a new Post object.
     * @details Validates that description is not empty and under 1000 characters.
     * @param author Shared pointer to the creator of the post.
     * @param description The text content of the post.
     */
    Post(const std::string& author, const std::string& description);

    // --- Getters ---

    /**
     * @brief Gets the unique ID of the post.
     * @return int The post ID.
     */
    int getID() const;

    /**
     * @brief Gets the author of the post.
     * @return std::shared_ptr<Entity> The author entity.
     */
    std::string getAuthor() const;

    /**
     * @brief Gets the description (content) of the post.
     * @return std::string The post content.
     */
    std::string getDescription() const;

    /**
     * @brief Gets the timestamp of creation.
     * @return std::time_t The creation time.
     */
    std::time_t getTimestamp() const;

    /**
     * @brief Gets the list of comments.
     * @return std::vector<Comment> A vector containing all comments.
     */
    std::vector<Comment> getComments() const;

    /**
     * @brief Gets the list of users who liked the post.
     * @return std::vector<std::string> A vector of usernames.
     */
    std::vector<std::string> getLikedBy() const;

    /**
     * @brief Gets the total number of likes.
     * @return int Count of likes.
     */
    int getNumLikes() const;

    /**
     * @brief Gets the total number of comments.
     * @return int Count of comments.
     */
    int getNumComments() const;

    // --- Setters ---

    /**
     * @brief Updates the post description.
     * @details Throws an error if description is empty or exceeds 1000 characters.
     * @param description The new content for the post.
     */
    void setDescription(const std::string& description);

    // --- Outros Métodos ---

    /**
     * @brief Adds a comment to the post.
     * @details Validates that the comment is not empty and under 500 characters.
     * @param commentAuthor The username of the commenter.
     * @param commentDescription The text of the comment.
     */
    void addComment(const std::string& commentAuthor, const std::string& commentDescription);

    /**
     * @brief Removes a comment by its index.
     * @details Re-indexes remaining comments after removal.
     * @param index The index of the comment to remove.
     */
    void removeComment(int index);

    /**
     * @brief Adds a like from a specific user.
     * @details Throws logic_error if the user already liked the post.
     * @param liker The username adding the like.
     */
    void addLiker(const std::string& liker);

    /**
     * @brief Removes a like from a specific user.
     * @param liker The username removing the like.
     */
    void removeLiker(const std::string& liker);

    /**
     * @brief Checks equality between two posts based on author and description.
     * @param other The other post to compare.
     * @return true If author and description match.
     * @return false Otherwise.
     */
    bool operator==(const Post& other) const;

    /**
     * @brief Overload of the insertion operator to print post details.
     * @param os The output stream.
     * @param post The post object to print.
     * @return std::ostream& The updated output stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const Post& post);
};

#endif

