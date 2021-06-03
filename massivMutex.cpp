
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <ctime>

std::mutex mutex;

void fillVector(std::vector<int>& massiv, int m) {
	for (int i = 0; i < m; i++)
		massiv.push_back(rand() % 10);
}

void printVector(std::vector<int> massiv) {
	for (int i = 0; i < massiv.size(); i++)
		std::cout << massiv[i] << " ";
	std::cout << std::endl;
}

void sumMassivSingle(std::vector<int> massiv) {
	int sum = 0;
	for (int i = 0; i < massiv.size(); i++)
		sum += massiv[i];
	std::cout << sum << std::endl;
}

void sumMassivMulti(std::vector<int> massiv, int& sum, int start, int step) {
	mutex.lock();
	for (int i = start; i < start + step; i++) {
		sum += massiv[i];
	}
	mutex.unlock();
}

int main()
{
	std::vector<int> massiv;
	int m;
	int n;
	std::cout << "Enter m: ";
	std::cin >> m;
	std::cout << "Enter n: ";
	std::cin >> n;
	fillVector(massiv, m);
	printVector(massiv);

	// Однопоточный
	clock_t startT = clock();
	sumMassivSingle(massiv);
	clock_t endT = clock();
	std::cout <<  "time: " <<endT - startT << std::endl;

	// Многопоточный 
	std::vector<std::thread> threads;
	int sum = 0;
	int start = 0;
	int step = m / n;

	for (int i = 0; i < n; i++) {
		threads.push_back(std::thread(&sumMassivMulti, std::ref(massiv), std::ref(sum), start, step));
		start += step;
	}
	startT = clock();
	for(int i = 0; i < threads.size(); i++)
		threads[i].join();
	endT = clock();
	std::cout << sum << std::endl;
	std::cout << "time: " << endT - startT << std::endl;



	return 0;
}

