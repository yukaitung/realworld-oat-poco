#ifndef COMMONHELPER_HPP
#define COMMONHELPER_HPP

#include <string>

class CommonHelper {
  public:
    /**
    * This method is to convert DateTime into ISO 8601.
    * @param time The time in SQL DateTime format
    * @return std::string This return time string in ISO 8601 format
    */
    static std::string timeTz(const std::string &time);
};

#endif // COMMONHELPER_HPP