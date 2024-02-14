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

	std::string GetAvailableVar(const std::string& section) const {
		std::ostringstream oss;
		oss << "[";
		bool first = true;
		for (const auto& pair : data.at(section)) {
			if (!first) {
				oss << ", ";
			}
			oss << pair.first;
			first = false;
		}
		oss << "]";
		return oss.str();
	}

	std::string GetAvailableType(const std::string& section, const std::string& key) const {
		std::set<std::string> types;
		std::string value = data.at(section).at(key);
		bool isNum = true;

		bool hasDigit = false;
		bool hasDot = false;
		for (char ch : value) {
			if (isdigit(ch)) {
				hasDigit = true;
			}
			else if (ch == '.') {
				hasDot = true;
			}
			else {
				isNum = false;
				break;
			}
		}
			
		if (isNum && hasDigit && !hasDot) types.insert("int");
		else if (isNum && hasDigit && hasDot) types.insert("double");
		else types.insert("string");

		std::ostringstream oss;
		//oss << "[";
		bool first = true;
		for (const auto& type : types) {
			if (!first) {
				oss << ", ";
			}
			oss << type;
			first = false;
		}
		//oss << "]";
		return oss.str();
	}

public:
	INIParser(const std::string& file_name) {
		std::ifstream file(file_name + ".ini");
		if (!file.is_open()) throw std::runtime_error("Fail to open file " + file_name + ".txt !");

		std::string line;
		std::string current_section;
		int line_number = 0;
		bool section_exists = false;

		while (std::getline(file, line)) {
			line_number++;

			if (line.empty() || line[0] == ';') {
				continue;
			}
			else if (line[0] == '[' && line.back() == ']') {
				if (!current_section.empty() && !section_exists) data[current_section];
				current_section = line.substr(1, line.size() - 2);
				section_exists = false;
			}
			else {
				std::istringstream iss(line);
				std::string key, value;
				std::getline(iss, key, '=');
				std::getline(iss, value);

				value.erase(0, value.find_first_not_of(" \t"));
				value.erase(value.find_last_not_of(" \t") + 1);

				size_t comment_pos = value.find(';');
				if (comment_pos != std::string::npos) {
					value = value.substr(0, comment_pos);
				}

				value.erase(0, value.find_first_not_of(" \t"));
				value.erase(value.find_last_not_of(" \t") + 1);

				data[current_section][key] = value;
				line_numbers[current_section + '.' + key] = line_number;
				section_exists = true;
			}
		}
		file.close();
	}

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