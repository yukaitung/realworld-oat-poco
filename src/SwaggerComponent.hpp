#ifndef SWAGGERCOMPONENT_HPP
#define SWAGGERCOMPONENT_HPP

#include "oatpp-swagger/Model.hpp"
#include "oatpp-swagger/Resources.hpp"
#include "oatpp/core/macro/component.hpp"

/**
 *  Swagger ui is served at
 *  http://host:port/swagger/ui
 */
class SwaggerComponent {
public:
  /**
   *  General API docs info
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, swaggerDocumentInfo)([] {
    oatpp::swagger::DocumentInfo::SecuritySchemeBuilder securitySchemeBuilder;
    securitySchemeBuilder
    .setType("apiKey")
    .setName("Authorization")
    .setIn("header")
    .setDescription("Enter: \"Token the.jwt.token\"");

    oatpp::swagger::DocumentInfo::Builder builder;
    builder
    .setTitle("RealWorld Example App")
    .setDescription("C++, POCO, Oat++ codebase containing real-world examples (CRUD, auth, MVC patterns, etc.)")
    .setVersion("1.0")
    .setContactName("Yu Kai Tung")
    .setContactUrl("https://gitlab.com/yukaitung")
    .addSecurityScheme("Token", securitySchemeBuilder.build())
    //.setLicenseName("Apache License, Version 2.0")
    //.setLicenseUrl("http://www.apache.org/licenses/LICENSE-2.0")
    .addServer("http://localhost:8000", "server on localhost");
    
    return builder.build(); 
  }());
  
  
  /**
   *  Swagger-Ui Resources
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, swaggerResources)([] {
    // The Conan should have it
    return oatpp::swagger::Resources::loadResources(OATPP_SWAGGER_RES_PATH);
  }());
  
};

#endif // SWAGGERCOMPONENT_HPP