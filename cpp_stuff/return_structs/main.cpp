#include <iostream>
#include <string>

int main()
{
	std::string a = "\x03";
	std::string b = "hello";
	std::string c = " world\n";

	std::cout << b << a << c;

	return 0;
}
