#include <iostream>
#include <optional>
#include <string>

std::optional<int> positive_integer(int a)
{
	if (a > 0) {
		return a;
	} else {
		return {};
	}
}

void test(int a)
{
	std::optional<int> result = positive_integer(a);

	if (result) {
		std::cout << a << " is pass\n";
	} else {
		std::cout << a << " is fail\n";
	}
}

int main()
{
	test(0);
	test(1);
	test(400);
	test(-3);
	test(-90);

	return 0;
}
