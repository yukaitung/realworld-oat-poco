#include "helper/CommonHelper.hpp"

std::string CommonHelper::timeTz(const std::string &time) {
  std::string tz = time + ".000Z";
  tz[10] = 'T';
  return tz; 
}