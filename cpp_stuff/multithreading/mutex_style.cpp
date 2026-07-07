#include <iostream>
#include <thread>
#include <mutex>

using namespace std::literals::chrono_literals;

std::mutex flag_mutex{};
bool flag = true;

void thread_func()
{
	for (;;) {
		std::cout << "worker: " << std::this_thread::get_id() << std::endl;
		std::this_thread::sleep_for(1s);

		flag_mutex.lock();
		if (!flag) break;
		flag_mutex.unlock();
	}

	std::cout << "worker finishing\n";
}

int main()
{
	std::thread worker(thread_func);
	worker.detach();

	std::this_thread::sleep_for(5s);

	std::cout << "clearing flag\n";

	flag_mutex.lock();
	flag = false;
	flag_mutex.unlock();

	std::cout << "done\n";

	return 0;
}
