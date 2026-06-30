#include <iostream>

int main()
{
	int x[5] = { 0, 1, 2, 4, };

	std::cout << (&x + 1) << std::endl;
	std::cout << (x + 1) << std::endl;

	return 0;
}
