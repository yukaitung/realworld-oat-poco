#ifndef USERHASFOLLOWERMODEL_HPP
#define USERHASFOLLOWERMODEL_HPP

#include <string>
#include <unordered_set>

class UserHasFollowerModel {
  public:
    UserHasFollowerModel() {};
    bool userHasThisFollower(std::string &userId, std::string &followerId);
    bool userNewFollower(std::string &userId, std::string &followerId);
    bool userRemoveFollower(std::string &userId, std::string &followerId);
    std::unordered_set<std::string> validUserIsFollowingFromList(std::string &followerId, std::vector<std::string> &userIds);
};

#endif // USERHASFOLLOWERMODEL_HPP