#include <iostream>

#define DEBUG_LOG(msg) \
	std::cout \
		<< "DEBUG: " __FILE__ ", " << __LINE__ << ": " \
		<< msg \
		<< std::endl;

int main()
{
	DEBUG_LOG("hello");

	return 0;
}
