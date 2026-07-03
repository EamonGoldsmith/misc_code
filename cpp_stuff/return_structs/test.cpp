#include <iostream>
#include <memory>

struct return_struct {
	char arr[5];
	int num;
};

std::unique_ptr<return_struct> foo()
{
	// create local struct
	return_struct r{
		"ABCD",
		5,
	};

	// make shared ptr copy
	auto shared_r = std::make_unique<return_struct>(r);

	return shared_r;
}

int main()
{
	// get struct reference from func
	std::shared_ptr<return_struct> sref = foo();
	auto r = sref.get();

	// see whats in it
	std::cout << r->arr << std::endl;
	std::cout << r->num << std::endl;
		
	return 0;
}
