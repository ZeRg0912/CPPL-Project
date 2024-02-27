#pragma once
#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <sstream>

class INIParser {
private:
	// Section < Var, Value> 
	std::map<std::string, std::map<std::string, std::string>> data;
	std::map<std::string, int> line_numbers;

	std::string GetAvailableVar(const std::string& section) const;

	std::string GetAvailableType(const std::string& section, const std::string& key) const;

public:
	INIParser(const std::string& file_name);

	template<typename T>
	T GetValue(const std::string& section, const std::string& key) const {
		if (data.count(section) == 0) throw std::runtime_error(section + " not found");
		if (data.at(section).empty()) throw std::runtime_error("In " + section + " variables not found");
		
		if (data.count(section) && data.at(section).count(key)) {
			std::string value_str = data.at(section).at(key);
			if (value_str.empty()) throw std::runtime_error(
				"Value not assigned for: "
				+ section
				+ ", "
				+ key
			);
			std::istringstream iss(data.at(section).at(key));
			T value;
			if (!(iss >> value))
			{
				throw std::runtime_error(
					"Conversion is not allowed for the "
					+ section
					+ ", "
					+ key
					+ ".\n"
					+ "Incorrect format on the line: "
					+ std::to_string(line_numbers.at(section + '.' + key))
					+ ".\n"
					+ "Available types in this variable: "
					+ GetAvailableType(section, key)
				);
			}
			return value;
		}
		else {
			throw std::runtime_error(
				"The value was not found for the: "
				+ section
				+ ", "
				+ key
				+ ".\n"
				+ "Available variables in this section: "
				+ GetAvailableVar(section)
			);
		}
	}

	template<>
	std::string GetValue(const std::string& section, const std::string& key) const {
		if (data.count(section) == 0) throw std::runtime_error(section + " not found");
		if (data.at(section).empty()) throw std::runtime_error("In " + section + " variables not found");

		if (data.count(section) && data.at(section).count(key)) {
			return data.at(section).at(key);
		}
		else {
			throw std::runtime_error(
				"The value was not found for the: "
				+ section
				+ ", "
				+ key
				+ ".\n"
				+ "Available variables in this section: "
				+ GetAvailableVar(section)
			);
		}
	}

	template<typename T>
	T GetValue(const std::string& section_key) const {
		size_t dot_pos = section_key.find('.');
		if (dot_pos == std::string::npos) throw std::runtime_error("Incorrect format, expected format: Section#.Var#!");
		std::string section = section_key.substr(0, dot_pos);
		std::string key = section_key.substr(dot_pos + 1);
		return GetValue<T>(section, key);
	}
};