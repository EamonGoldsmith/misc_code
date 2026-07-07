#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
	std::vector<int> v = {5, 1, 8, 3, 9, 2};

	std::sort(v.begin(), v.end(), [] (const int &a, const int &b) {
		return a > b;
	});

	for (int x : v) {
		std::cout << x << " ";
	}
	std::cout << std::endl;

	return 0;
}
