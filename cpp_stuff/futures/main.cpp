#include <atomic>
#include <future>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>
#include <optional>

#include <iostream>

enum Status
{
	Command, Ack, Nack
};

struct Message
{
	int id;
	int status;
	std::string content;
};

struct MessagePair
{
	unsigned int retries;

	std::pair<
		Message,
		std::promise<std::optional<Message>>
	> pair;
};

struct MessageList
{
	std::mutex mtx;
	std::vector<MessagePair> msgs;
};

using namespace std::chrono_literals;

void gauge_connect(MessageList& ml);
void gauge_disconnect();
void gauge_fulfill(MessageList& ml, Message msg);
void gauge_retry(MessageList& ml, Message msg);
void gauge_handle_message(MessageList& ml, Message msg);
void gauge_listener_thread(MessageList& ml);
std::future<std::optional<Message>> gauge_send(MessageList& ml, const Message& msg);

std::atomic_bool listening = false;

int main()
{
	MessageList ml{};
	gauge_connect(ml);

	Message test_msg1 = { 1, Command, "Thing" };

	std::future<std::optional<Message>> result = gauge_send(ml, test_msg1);

	while (1) {
		// is a "blank" future still valid?
		if (result.valid()) {
			std::optional<Message> resp = result.get();

			if (resp) {
				std::cout << "response: " << (*resp).id << ", content: " << (*resp).content << std::endl;
			} else {
				std::cout << "the message failed\n";
			}

			result = gauge_send(ml, test_msg1);
		}

		std::this_thread::sleep_for(500ms);
	}
	
	return 0;
}

bool backend_read(Message& msg)
{
	static int i;
	msg = Message { i++, Nack, "Hello" };
	i %= 3;

	std::this_thread::sleep_for(2s);

	std::cout << "read id: " << i << ", content: " << msg.content << std::endl;
	return true;
}

bool backend_write(Message msg)
{
	std::cout << "sending id: " << msg.id << ", content: " << msg.content << std::endl;
	return true;
}

void gauge_connect(MessageList& ml)
{
	std::thread t(gauge_listener_thread, std::ref(ml));
	t.detach();

	listening = true;
}

void gauge_disconnect()
{
	listening = false;
}

void gauge_fulfill(MessageList& ml, Message msg)
{
	auto it = ml.msgs.begin(); 

	for ( ; it != ml.msgs.end(); ) {

		// find message with matching id
		if (it->pair.first.id == msg.id)
		{
			// when found, fulfill promise and remove
			it->pair.second.set_value(msg);
			it = ml.msgs.erase(it);
			return;
		}

		it++;
	}

	if (it == ml.msgs.end()) {
		std::cout << "recieved message doesn't match anything\n";
	}
}

void gauge_retry(MessageList& ml, Message msg)
{
	for (auto it = ml.msgs.begin(); it != ml.msgs.end(); ++it) {

		if (it->pair.first.id == msg.id) {

			std::cout << "re-trying: id: " << msg.id << ", content: " << msg.content << ", retries: " << it->retries << std::endl;

			if (it->retries == 0) {
				it->pair.second.set_value({});
				it = ml.msgs.erase(it);
				return;
			}


			// try re-send the original message
			backend_write(it->pair.first);
			it->retries--;

			return;
		}
	}
}

void gauge_handle_message(MessageList& ml, Message msg)
{
	switch (msg.status)
	{
	case Command:
		std::cout << "received command\n";
		break;

	case Ack:
		gauge_fulfill(ml, msg);
		break;

	case Nack:
		gauge_retry(ml, msg);
		break;

	default:
		std::cerr << "[gauge] received invalid message with status: "
			<< msg.status << std::endl;
	}

	// check timeouts
}

void gauge_listener_thread(MessageList& ml)
{
	while (listening) {
		Message msg{};

		if (!backend_read(msg)) {
			std::cerr << "[gauge] failed to read command\n";
			break;
		}

		std::lock_guard<std::mutex> lock(ml.mtx);
		gauge_handle_message(ml, msg);
	}
}

/*
	there are three ways the message promise can be fulfilled:
	- recieved Ack from gauge backend and has value set by fulfill func
	- timeout check, after 10s the message will be picked up by the check_timeout func
		and will be set as nullopt
	- retry check, after 5 attempts to send the message and recieving a Nack from the gauge
		the value will be set as nullopt
*/
std::future<std::optional<Message>> gauge_send(MessageList& ml, const Message& msg)
{
	backend_write(msg);

	std::promise<std::optional<Message>> prom;
	std::future<std::optional<Message>> fut = prom.get_future();

	{
		std::lock_guard<std::mutex> lock(ml.mtx);
		ml.msgs.push_back( { 5, std::pair { msg, std::move(prom) } } );
	}

	return std::move(fut);
}
