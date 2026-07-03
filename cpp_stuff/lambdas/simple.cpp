#include <iostream>

int main()
{
	auto res = [](int x) {
		return x + x;
	};

	std::cout << res(5) << std::endl;

	return 0;
}
