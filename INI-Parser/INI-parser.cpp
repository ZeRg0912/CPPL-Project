#include "INI-Parser.hpp"

std::string INIParser::GetAvailableVar(const std::string& section) const {
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

std::string INIParser::GetAvailableType(const std::string& section, const std::string& key) const {
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

INIParser::INIParser(const std::string& file_name) {
	std::ifstream file(file_name);
	if (!file.is_open()) throw std::runtime_error("Fail to open file " + file_name + " !");

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