#ifndef SWAGGERCOMPONENT_HPP
#define SWAGGERCOMPONENT_HPP

#include "oatpp-swagger/Model.hpp"
#include "oatpp-swagger/Resources.hpp"
#include "oatpp/core/macro/component.hpp"

#include "Config.h"

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
    .setTitle(REALWORLD_PROJECT_FULL_TITLE)
    .setDescription(REALWORLD_DESCRIPTION)
    .setVersion(REALWORLD_VERSION)
    .setContactName(REALWORLD_CONTACT_NAME)
    .setContactUrl(REALWORLD_HOMEPAGE_URL)
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
    return oatpp::swagger::Resources::loadResources("res");
  }());
  
};

#endif // SWAGGERCOMPONENT_HPP