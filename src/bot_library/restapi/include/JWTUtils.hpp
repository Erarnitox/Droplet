#pragma once

#include <Poco/JSON/Object.h>

#include <string>

namespace JWTUtils {
std::string generateToken(const Poco::JSON::Object& claims, const std::string& secret, const std::string& issuer);
}