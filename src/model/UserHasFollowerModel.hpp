#ifndef USERHASFOLLOWERMODEL_HPP
#define USERHASFOLLOWERMODEL_HPP

#include <string>
#include <unordered_set>
#include <vector>

class UserHasFollowerModel {
  public:
    UserHasFollowerModel() {};

    /**
    * This method is to search a following record.
    * @param userId The profile user id.
    * @param followerId The follower user id.
    * @return bool This return the result, true if the record exist.
    */
    bool userHasThisFollower(std::string &userId, std::string &followerId);

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
    std::unordered_set<std::string> validUserIsFollowingFromList(std::string &followerId, std::vector<std::string> &userIds);
};

#endif // USERHASFOLLOWERMODEL_HPP