#ifndef USERMODEL_HPP
#define USERMODEL_HPP

#include "dto/UserDto.hpp"

#include <string>
#include <utility>

class UserModel {
  private:
    /**
    * This method is to create hashing password.
    * @param passwordPlusSalt The password concatenates with a salt.
    * @return std::string This return a password hash string.
    */
    std::string hashPassword(const std::string &passwordPlusSalt);

  public:
    UserModel() {};
    /**
    * This method is to vaildate the email or username is taken.
    * @param email The email.
    * @param username The username.
    * @return std::pair<bool, bool> This return a pair, .first for email, .second for username, ture represent it is taken.
    */
    std::pair<bool, bool> validateUser(std::string &email, std::string &username);
    
    /**
    * This method is to create a user, then obtain a user credential.
    * @param email The email.
    * @param username The username.
    * @param password The password.
    * @return oatpp::Object<UserDto> Completed UserDto object.
    */
    oatpp::Object<UserDto> createUser(std::string &email, std::string &username, std::string &password);
   
    /**
    * This method is to validate the email and password, then obtain a user credential.
    * @param email The email.
    * @param username The username.
    * @return oatpp::Object<UserDto> Completed UserDto object.
    */
    oatpp::Object<UserDto> login(std::string &email, std::string &password);
   
    /**
    * This method is to obtain a user credential when the user is authorizated.
    * @param id The user id.
    * @return oatpp::Object<UserDto> Completed UserDto object.
    */
    oatpp::Object<UserDto> getUser(std::string &id);
   
    /**
    * This method is to update information for the user, then get a new credential.
    * @param id The user id.
    * @param email The email.
    * @param username The username.
    * @param password The password.
    * @param bio The user bio.
    * @param image The user image.
    * @return oatpp::Object<UserDto> Completed UserDto object.
    */
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
    * @return std::pair<oatpp::Object<UserProfileDto>, std::string> This returns a profile object and user ID. The profile requires adding the following status. If the user ID is empty, it is not found.
    */
    std::pair<oatpp::Object<UserProfileDto>, std::string> getProfileFromUsername(std::string &username);
};

#endif // USERMODEL_HPP