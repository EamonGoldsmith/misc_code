#include <iostream>
#include "parser.h"

int main()
{
	Parser parser;

	auto result = parser.evaluate("INST");

	if (!result.is_ok()) {
			std::cout << result.reason() << std::endl;
	}

	result = parser.evaluate("THING");

	if (!result.is_ok()) {
			std::cout << result.reason() << std::endl;
	}

	result = parser.evaluate("ABOBA");

	result.match(
		[] {
			std::cout << "yippee!" << std::endl;
		},
		[result] { 
			std::cout << "oh uh :( " << "\n"
			<< result.reason() << std::endl;
		}
	);

	return 0;
}
