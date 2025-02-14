#include <iostream>
#include <random> 
#include <thread>
#include <atomic>
#include <chrono>
#include <string>

std::atomic<bool> time_up(false);

void timer(int seconds) {
	for (int i = seconds; i > 0; i--) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	time_up = true;
}

int main() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(1, 4);
	std::uniform_int_distribution<> smallint(2, 12);
	std::uniform_int_distribution<> bigint(2, 100);
	int score = 0, time_limit;
	std::string mode;
	std::cout << "Select a mode: (1) Endless   (2) Timed\n";
	std::cin >> mode;
	std::thread timer_thread;
	if (mode == "timed" || mode == "Timed" || mode == "2") {
		int time_limit;
		std::cout << "Enter how much time (in seconds): ";
		std::cin >> time_limit; 
		std::cout << "Timer set for " << time_limit << " seconds\n";
		timer_thread = std::thread(timer, time_limit);
		mode = "2";
	} else if (mode == "endless" || mode == "Endless" || mode == "1") {
		std::cout << "Endless mode selected, will run until incorrect answer\n";
		mode = "1";
	} else {
		std::cout << "Please enter a valid option.";
		return 0; 
	}
	while(!time_up) {
		int a, b, c, op = distrib(gen), ans;
		if (op == 1) {
			a = bigint(gen);
			b = bigint(gen);
			c = a + b;
			std::cout << a << " + " << b << " = ";
		} else if (op == 2) {
			c = bigint(gen);
			b = bigint(gen);
			a = c + b;
			std::cout << a << " - " << b << " = ";
		} else if (op == 3) {
			a = smallint(gen);
			b = bigint(gen);
			c = a * b;
			std::cout << a << " * " << b << " = ";			
		} else if (op == 4) {
			b = smallint(gen);
			c = bigint(gen);
			a = c * b;
			std::cout << a << " / " << b << " = ";	
		}
		std::cin >> ans;
		if (time_up) {
			std::cout << "Your time ran out while you were answering the last question. Final score will not include the last question.\n";
			break;
		}
		if (mode == "1" && c != ans) break;
		else if (c == ans) score++; 
	}
	std::cout << "Score: " << score;
	if (mode == "2" && timer_thread.joinable()) timer_thread.join();
}
