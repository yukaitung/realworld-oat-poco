#ifndef USERMODEL_HPP
#define USERMODEL_HPP

#include "dto/UserDto.hpp"

#include <string>
#include <utility>

class UserModel {
  private:
    std::string hashPassword(const std::string &passwordPlusSalt);

  public:
    UserModel() {};
    oatpp::Object<UserDto> createUser(std::string &email, std::string &username, std::string &password);
    oatpp::Object<UserDto> login(std::string &email, std::string &password);
    oatpp::Object<UserDto> getUser(std::string &id);
    oatpp::Object<UserDto> updateUser(std::string &id, std::string &email, std::string &username, std::string &password, std::string &bio, std::string &image);
    /**
    * This method is to fetch an profile from database using user id.
    * @param id The user id.
    * @return oatpp::Object<UserProfileDto> This returns user profile object.
    */
    oatpp::Object<UserProfileDto> getProfileFromId(std::string &id);
    /**
    * This method is to fetch an profile from database using user id.
    * @param id The list of user id.
    * @return std::unordered_map<std::string, oatpp::Object<UserProfileDto>> This returns user id : user profile object.
    */
    std::unordered_map<std::string, oatpp::Object<UserProfileDto>> getProfilesFromId(std::vector<std::string> &ids);
    /**
    * This method is to fetch an profile from database using username.
    * @param username The username.
    * @return std::pair<oatpp::Object<UserProfileDto>, std::string> This returns profile object and profile user id.
    */
    std::pair<oatpp::Object<UserProfileDto>, std::string> getProfileFromUsername(std::string &username);
};

#endif // USERMODEL_HPP