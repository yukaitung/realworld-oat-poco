#include "helper/CommonHelper.hpp"

std::string CommonHelper::corsOrigin = "*";

std::string CommonHelper::timeTz(const std::string &time) {
  std::string tz = time + "Z";
  tz[10] = 'T';
  return tz; 
}

void CommonHelper::setCorsOrigin(const std::string &origin) {
  CommonHelper::corsOrigin = origin;
}