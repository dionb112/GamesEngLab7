#include <thread>
#include <iostream>
#include <chrono>
#include <mutex>
#include <condition_variable>


std::mutex mutex;

void producer(int buffer, int p, int c) {
	int a[5] = { 10, 20, 30, 40, 50 };
	bool run = true;
	while (run) {
		while (p < 5) {
			std::lock_guard<std::mutex> lock(mutex);
			std::cout << "ID:" << std::this_thread::get_id() << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(2));
			while (!(p == c)) {
				std::cout << "prod spinning" << std::endl;
			}
			buffer = a[p];
			std::cout << "Number:" << a[p] << " dump to buffer" << std::endl;
			p = p + 1;
		}
		std::cout << "prod done" << std::endl;
		std::lock_guard<std::mutex> lock(mutex);
		run = false;
	}
}

void consumer(int buffer, int p, int c) {
	int b[5];
	bool run = true;
	while (run) {
		while (c < 5) {
			std::lock_guard<std::mutex> lock(mutex);
			std::cout << "ID:" << std::this_thread::get_id() << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
			while ((p <= c)) {
				std::cout << "cons spinning" << std::endl;
			}
			b[c] = buffer;
			std::cout << buffer << " dump to b array" << std::endl;
			c = c + 1;
		}
		std::cout << "consumption done" << std::endl;
		run = false;
	}
}

int main(void)
{
	int buffer, p = 0, c = 0;
	std::thread producer(producer, buffer, p, c);
	std::thread consumer(consumer, buffer, p, c);
	producer.join();
	consumer.join();
	std::cin.get();
	return 1;
}
