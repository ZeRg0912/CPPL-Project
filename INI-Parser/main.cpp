#include <iostream>
#include <sstream>
#include "INI-Parser.hpp"

int main() {
	setlocale(LC_ALL, "RUSSIAN");
	
	try {
		std::ostringstream s;
		s.precision(3);
		INIParser parser("test-file.ini");
		std::string value = parser.GetValue<std::string>("Section2.var2");
		std::cout << value << std::endl;
		s << parser.GetValue<double>("Section1.var1");
		std::cout << s.str() << std::endl;
		value = parser.GetValue<std::string>("Section1.var3");
		std::cout << value << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
	std::cout << "\nEnd of parsing process...\n";
	//system("pause");
	return 0;
}