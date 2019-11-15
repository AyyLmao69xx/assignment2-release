// Copyright 2019 Zhihao Zhang License MIT

#include "config_parser.hpp"

#include <iostream>
#include <string>
#include <list>

namespace assignment2
{
// Task 3A
ConfigReader::ConfigReader(std::istream & config_file){
    int lineCount = 1;
    int i = 0;
    int counter = 0;
    std::string lineInfo;
	//	Read from config file instead of command line
    while (std::getline(config_file, lineInfo)){
        std::cout << "Line " << lineCount << ": " << lineInfo << '\n';
		// Key and val defined from map
        std::string key = lineInfo.substr(0, lineInfo.find(":"));
        std::string val = lineInfo.substr(lineInfo.find(":") + 1, '\n');

		// Iterate through the key and val arrays and delete all empty values
		i = 0;
        while(i <= key.size){
            if (key[i] != ' '){
                key[counter++] = key[i];
			i++;
            }
        }
		i = 0;
        key[counter] = '\0';
        counter = 0;
		while(i <= val.size){
            if (val[i] != ' '){
				val[counter++] = val[i];
			i++;
			}
        }
		i = 0;
        val[counter] = '\0';
        counter = 0;
        config_[key] = val;

        lineCount++;
    }
	// Go through unordered map and retrieve appropriate key and val
    std::unordered_map<std::string, std::string>::iterator iterator;
	iterator = config_.begin();
	while(iterator != config_.end()){
        std::cout << "key: \"" << iterator->first << "\", value: \"" << iterator->second
                  << "\"" << '\n';
		iterator++;
    }
}
// Task 3B
// Find the required key and return its value
auto ConfigReader::find_config(std::string const & key,
    std::string const & default_value) const -> std::string{
	// Passing in a constant value so need to use a constant iterator
	std::unordered_map<std::string, std::string>::const_iterator constantIterator;
	constantIterator = config_.begin();
	while(constantIterator != config_.end()){
		if (key == constantIterator->first){
			std::cout << key << " || " << constantIterator->second << '\n';
			return constantIterator->second;
        }
		constantIterator++
        }
    return default_value;
}


ConfigParser::ConfigParser(ConfigReader const & config)
    : zid_{config.find_config("zid", std::string{"z5165194"})}
    // TODO(STUDENT): CODE HERE
    , joy_config_{}
    , kinematic_config_{}
    , refresh_period_{}
{
    // TODO(STUDENT): CODE HERE
}

auto ConfigParser::get_zid() const -> std::string { return zid_; }

auto ConfigParser::get_refresh_period() const -> std::chrono::milliseconds
{
    return refresh_period_;
}

auto ConfigParser::get_joystick_config() const -> JoystickConfig
{
    return joy_config_;
}

auto ConfigParser::get_kinematic_config() const -> KinematicLimits
{
    return kinematic_config_;
}
} // namespace assignment2
