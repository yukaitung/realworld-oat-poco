#include "service/ProfileService.hpp"

oatpp::Object<UserProfileJsonDto> ProfileService::getProfile(std::string &id, std::string &profileUsername) {
  OATPP_ASSERT_HTTP(!profileUsername.empty(), Status::CODE_400, "Missing username");

  auto profile = userModel.getProfileFromUsername(profileUsername);
  if(!id.empty()) {
    // See if the user is following the profile
    profile.first->following = userHasFollowerModel.userHasThisFollower(profile.second, id);
  }

  auto response = UserProfileJsonDto::createShared();
  response->profile = profile.first;
  return response;
 }