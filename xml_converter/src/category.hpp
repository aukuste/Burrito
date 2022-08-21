#pragma once
#include "rapidxml-1.13/rapidxml.hpp"
#include <string>
#include <vector>
#include "parseable.hpp"
#include <map>
#include "icon.hpp"
#include "trail.hpp"

#include "attribute/bool.hpp"
#include "attribute/string.hpp"
using namespace std;

class Category: public Parseable {
    public:
        bool default_visibility;
        string display_name;
        bool is_separator;
        string name;
        string tooltip_name;
        map<string, Category> children;
        Icon default_icon;
        Trail default_trail;

        void init_from_xml(rapidxml::xml_node<>* node, vector<XMLError*> *errors);
        virtual string classname();
        bool init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<XMLError*> *errors);
};