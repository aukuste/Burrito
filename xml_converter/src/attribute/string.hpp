#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"

class XMLError;

std::string parse_string(rapidxml::xml_attribute<>* input, std::vector<XMLError*>* errors);

std::string string_to_xml_attribute(const std::string& attribute_name, const std::string* value);

// Zero Cost Abstraction identity functions to make parsing and writing protobufs more uniform
inline std::string const& from_proto_string(const std::string& x) {
    return x;
}
inline std::string const& to_proto_string(const std::string& x) {
    return x;
}
