#include "category_gen.hpp"

#include <algorithm>
#include <iosfwd>
#include <string>
#include <type_traits>
#include <utility>

#include "attribute/bool.hpp"
#include "attribute/string.hpp"
#include "guildpoint.pb.h"
#include "rapid_helpers.hpp"
#include "rapidxml-1.13/rapidxml.hpp"
#include "string_helper.hpp"

using namespace std;

string Category::classname() {
    return "MarkerCategory";
}
void Category::init_from_xml(rapidxml::xml_node<>* node, vector<XMLError*>* errors, XMLReaderState* state) {
    for (rapidxml::xml_attribute<>* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()) {
        bool handled = false;

        if (init_xml_attribute(attribute, errors, state)) {
            handled = true;
        }
        // Attempt to parse the attributes and throw away the results. We
        // perform this extra work here to identify if the attribute is
        // part of the particular marker, and to gather any errors there
        // might be with that attribute so they can be correctly attributed
        // to this node instead of the children that inherit the field.
        if (Icon().init_xml_attribute(attribute, errors, state)) {
            this->icon_attributes.push_back(attribute);
            handled = true;
        }
        if (Trail().init_xml_attribute(attribute, errors, state)) {
            this->trail_attributes.push_back(attribute);
            handled = true;
        }

        if (!handled) {
            errors->push_back(new XMLAttributeNameError("Unknown " + this->classname() + " attribute ", attribute));
        }
    }
}

bool Category::init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<XMLError*>* errors, XMLReaderState* state) {
    string attributename;
    attributename = normalize(get_attribute_name(attribute));
    if (attributename == "displayname") {
        xml_attribute_to_string(attribute, errors, state, &(this->display_name), &(this->display_name_is_set));
    }
    else if (attributename == "defaulttoggle") {
        inverted_xml_attribute_to_bool(attribute, errors, state, &(this->is_hidden), &(this->is_hidden_is_set));
    }
    else if (attributename == "isseparator") {
        xml_attribute_to_bool(attribute, errors, state, &(this->is_separator), &(this->is_separator_is_set));
    }
    else if (attributename == "id") {
        xml_attribute_to_unique_id(attribute, errors, state, &(this->menu_id), &(this->menu_id_is_set));
    }
    else if (attributename == "menuid") {
        xml_attribute_to_unique_id(attribute, errors, state, &(this->menu_id), &(this->menu_id_is_set));
    }
    else if (attributename == "name") {
        xml_attribute_to_string(attribute, errors, state, &(this->name), &(this->name_is_set));
    }
    else if (attributename == "tipdescription") {
        xml_attribute_to_string(attribute, errors, state, &(this->tooltip_description), &(this->tooltip_description_is_set));
    }
    else {
        return false;
    }
    return true;
}

rapidxml::xml_node<char>* Category::as_xml(XMLWriterState* state) const {
    rapidxml::xml_node<char>* xml_node = state->doc->allocate_node(rapidxml::node_element, "MarkerCategory");

    if (this->display_name_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("DisplayName", state->doc->allocate_string(val.c_str()))); };
        string_to_xml_attribute(state, &this->display_name, setter);
    }
    if (this->is_hidden_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("DefaultToggle", state->doc->allocate_string(val.c_str()))); };
        bool_to_inverted_xml_attribute(state, &this->is_hidden, setter);
    }
    if (this->is_separator_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("IsSeparator", state->doc->allocate_string(val.c_str()))); };
        bool_to_xml_attribute(state, &this->is_separator, setter);
    }
    if (this->menu_id_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("ID", state->doc->allocate_string(val.c_str()))); };
        unique_id_to_xml_attribute(state, &this->menu_id, setter);
    }
    if (this->name_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("Name", state->doc->allocate_string(val.c_str()))); };
        string_to_xml_attribute(state, &this->name, setter);
    }
    if (this->tooltip_description_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("TipDescription", state->doc->allocate_string(val.c_str()))); };
        string_to_xml_attribute(state, &this->tooltip_description, setter);
    }

    for (const auto& [key, val] : this->children) {
        xml_node->append_node(val.as_xml(state));
    }
    return xml_node;
}

// The following attributes are not yet supported in Burrito
// and are not written to or read from the protobuf file:
//  is_hidden
//  tooltip_description

guildpoint::Category Category::as_protobuf(ProtoWriterState* state) const {
    guildpoint::Category proto_category;
    if (this->display_name_is_set) {
        std::function<void(std::string)> setter = [&proto_category](std::string val) { proto_category.set_name(val); };
        display_name_and_name_to_proto_display_name(this->display_name, state, setter, &(this->name), &(this->name_is_set));
    }
    if (this->is_separator_is_set) {
        std::function<void(bool)> setter = [&proto_category](bool val) { proto_category.set_is_separator(val); };
        bool_to_proto(this->is_separator, state, setter);
    }
    if (this->menu_id_is_set) {
        std::function<void(std::string)> setter = [&proto_category](std::string val) { proto_category.set_id(val); };
        unique_id_to_proto(this->menu_id, state, setter);
    }
    return proto_category;
}

void Category::parse_protobuf(guildpoint::Category proto_category, ProtoReaderState* state) {
    if (proto_category.name() != "") {
        proto_display_name_to_display_name_and_name(proto_category.name(), state, &(this->display_name), &(this->display_name_is_set), &(this->name), &(this->name_is_set));
    }
    if (proto_category.is_separator() != 0) {
        proto_to_bool(proto_category.is_separator(), state, &(this->is_separator), &(this->is_separator_is_set));
    }
    if (proto_category.id() != "") {
        proto_to_unique_id(proto_category.id(), state, &(this->menu_id), &(this->menu_id_is_set));
    }
}
