#include <iostream>
#include "../INI-Parser.hpp"
#include "catch_amalgamated.hpp"

TEST_CASE("TESTING:", "[INI-Parser]") {
	std::string path;
	INIParser parser("test-file");
	SECTION("SECTION 1:") {
		SECTION("VAR 1:") {
			path = "Section1.var1";
			// Output double
			auto value_1_1_1 = parser.GetValue<double>(path);
			std::cout << path << " = " << value_1_1_1 << '\n';
			CHECK(value_1_1_1 == 1.1);
			// Output int
			auto value_1_1_2 = parser.GetValue<int>(path);
			std::cout << path << " = " << value_1_1_2 << '\n';
			CHECK(value_1_1_2 == 1);
			// Output string
			auto value_1_1_3 = parser.GetValue<std::string>(path);
			std::cout << path << " = " << value_1_1_3 << '\n';
			CHECK(value_1_1_3 == "1.1");
		}
		SECTION("VAR 2:") {
			path = "Section1.var2";
			// Output int
			CHECK_THROWS(parser.GetValue<int>(path));

			// Output string
			auto value_1_2_2 = parser.GetValue<std::string>(path);
			std::cout << path << " = " << value_1_2_2 << '\n';
			CHECK(value_1_2_2 == "какая-то строка");
		}
		SECTION("VAR 3:") {
			path = "Section1.var3";
			// Output string
			auto value_1_3_1 = parser.GetValue<std::string>(path);
			std::cout << path << " = " << value_1_3_1 << '\n';
			CHECK(value_1_3_1 == "значение");
		}
	}
	SECTION("SECTION 2:") {
		SECTION("VAR 1:") {
			path = "Section2.var1";
			// Output double
			auto value_2_1_1 = parser.GetValue<double>(path);
			std::cout << path << " = " << value_2_1_1 << '\n';
			CHECK(value_2_1_1 == 1);
			// Output int
			auto value_2_1_2 = parser.GetValue<int>(path);
			std::cout << path << " = " << value_2_1_2 << '\n';
			CHECK(value_2_1_2 == 1);
			// Output string
			auto value_2_1_3 = parser.GetValue<std::string>(path);
			std::cout << path << " = " << value_2_1_3 << '\n';
			CHECK(value_2_1_3 == "1");
		}
		SECTION("VAR 2:") {
			path = "Section2.var2";
			// Output string
			auto value_2_2_1 = parser.GetValue<std::string>(path);
			std::cout << path << " = " << value_2_2_1 << '\n';
			CHECK(value_2_2_1 == "значение_2 3");
		}
		SECTION("VAR 3:") {
			path = "Section2.var3";
			// Output string
			auto value_2_3_1 = parser.GetValue<std::string>(path);
			std::cout << path << " = " << value_2_3_1 << '\n';
			CHECK(value_2_3_1 == "значение_3");
		}
	}
	SECTION("SECTION 3:") {
		SECTION("NO VAR") {
			path = "Section3.var1";
			// Empty section
			CHECK_THROWS_WITH(parser.GetValue<std::string>(path), "In Section3 variables not found");
		}
	}
	SECTION("SECTION 4:") {
		path = "Section4.var1";
		// Check section with value not assigned  
		CHECK_THROWS_WITH(parser.GetValue<int>(path), "The value was not found for the: Section4, var1.\nAvailable variables in this section: [Mode, Vid]");
	}
	SECTION("TRY CHECKING DIFFERENT TYPES OF ACCESS TO THE INI - FILE") {
		path = "Section1";
		CHECK_THROWS_WITH(parser.GetValue<std::string>(path), "Incorrect format, expected format: Section#.Var#!");
	}
}

int main() {
	setlocale(LC_ALL, "RUSSIAN");
	return Catch::Session().run();
}