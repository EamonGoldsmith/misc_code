#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
	std::vector<int> v = {5, 1, 8, 3, 9, 2};

	auto it = std::find_if(v.begin(), v.end(), [] (const int &a) {
		return a % 3 == 0;
	});

	if (it != v.end()) std::cout << *it << std::endl;
	else std::cout << "no such element\n";

	return 0;
}
