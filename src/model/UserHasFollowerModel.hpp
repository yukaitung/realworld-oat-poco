#ifndef USERHASFOLLOWERMODEL_HPP
#define USERHASFOLLOWERMODEL_HPP

#include <string>

class UserHasFollowerModel {
  public:
    UserHasFollowerModel() {};
    bool userHasThisFollower(std::string &userId, std::string &followerId);
    bool userNewFollower(std::string &userId, std::string &followerId);
    bool userRemoveFollower(std::string &userId, std::string &followerId);
};

#endif // USERHASFOLLOWERMODEL_HPP