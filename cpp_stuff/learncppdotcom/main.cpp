#include <iostream>

int five()
{
	return 5;
}

int main()
{
	std::cout << "Hello, World!\n";

	[[maybe_unsued]]constexpr double gravity { 9.8 };
	constexpr int sum { 4 + 5 };
	constexpr int something { sum };

	const int age { 23 };

	constexpr int myAge { age };

	return 0;
}
