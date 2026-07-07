
#include <iostream>
#include <thread>
#include <condition_variable>

using namespace std::literals::chrono_literals;

std::mutex mtx;
std::condition_variable cv;

bool data_ready = false;

void producer()
{
	std::this_thread::sleep_for(2s);

	// lock and auto unlock when out of scope
	std::lock_guard<std::mutex> lock(mtx);

	data_ready = true;

	std::cout << "data produced\n";

	// signal the consumer thread that variable is updated
	cv.notify_one();
}

void consumer()
{
	// guarantees mutex unlock even in case of exception
	// doesn't always lock on creation, used with condition_variable
	std::unique_lock<std::mutex> lock(mtx);

	// cv will block the thread until signalled, will use the provided
	// unique lock to synchronise
	cv.wait(lock, [] { return data_ready; });

	std::cout << "data consumed\n";
}

int main()
{
	std::thread consumer_thread(consumer);
	std::thread producer_thread(producer);

	consumer_thread.join();
	producer_thread.join();

	return 0;
}
