#ifndef USERHASFOLLOWERMODEL_HPP
#define USERHASFOLLOWERMODEL_HPP

#include <string>
#include <unordered_set>
#include <vector>

class UserHasFollowerModel {
  public:
    UserHasFollowerModel() {};

    /**
    * This method is to add a following record.
    * @param userId The profile user id.
    * @param followerId The follower user id.
    * @return bool This return the result.
    */
    bool userNewFollower(std::string &userId, std::string &followerId);

    /**
    * This method is to remove a following record.
    * @param userId The profile user id.
    * @param followerId The follower user id.
    * @return bool This return the result.
    */
    bool userRemoveFollower(std::string &userId, std::string &followerId);

    /**
    * This method is to search all following record for a user.
    * @param userId The profile user id.
    * @param followingId The follower user id.
    * @return std::unordered_set<std::string> This return a set that the user is following from the input
    */
    std::unordered_set<std::string> validUserFollowing(std::string &userId, std::vector<std::string> &followingId);
};

#endif // USERHASFOLLOWERMODEL_HPP