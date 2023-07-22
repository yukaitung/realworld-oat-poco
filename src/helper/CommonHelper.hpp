#ifndef COMMONHELPER_HPP
#define COMMONHELPER_HPP

#include <string>

class CommonHelper {
  private:
    static std::string corsOrigin;

  public:
    /**
    * This method is to convert DateTime into ISO 8601.
    * @param time The time in SQL DateTime format
    * @return std::string This return time string in ISO 8601 format
    */
    static std::string timeTz(const std::string &time);

    /**
    * This method is to set default CORS origin
    * @param origin The URL of front end e.g http://127.0.0.1:12345
    */
    static void setCorsOrigin(const std::string &origin);

    /**
    * This method is to get default CORS origin
    * @return std::string This return CORS origin
    */
    static std::string getCorsOrigin() {return CommonHelper::corsOrigin;}
};

#endif // COMMONHELPER_HPP