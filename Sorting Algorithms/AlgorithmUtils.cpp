#include "AlgorithmUtils.h"
using std::cout;
using std::endl;

extern std::atomic<bool> killThreads;
std::atomic<int> g_reference;

void AlgorithmUtils::printArray(int a[], int n) {
	cout << "[ ";
	for (int i = 0; i < n; i++)
		cout << a[i] << ((i != n - 1) ? ", " : " ");
	cout << "]";
	cout << endl;
}

void AlgorithmUtils::partialBubbleSort(int array[], int size, int* sizePtr) {
	if (size != 0) {
		int reference = array[0];
		for (int i = 1; i < size; i++) {
			if (reference < array[i]) {
				reference = array[i];
			}
			else {
				int swapValue = array[i];
				array[i] = reference;
				array[i - 1] = swapValue;
			}
		}
	}
	else cout << "Cannot sort Array of size 0!" << endl;
}

void AlgorithmUtils::bubbleSort(int array[], int size, int* sizePtr) {
	if (size != 0) {
		int reference = array[0];
		int iterations = 1;
		for (int i = 1; i < size; i++) {
			if (reference < array[i]) {
				reference = array[i];
				iterations++;
			}
			else {
				int swapValue = array[i];
				array[i] = reference;
				array[i - 1] = swapValue;
			}
			g_reference = reference;
			if (killThreads) break;
			Sleep(4);
		}
		if (iterations < size && !killThreads) {
			bubbleSort(array, size - 1, sizePtr);
		} else g_reference = 0;
	}
}

int* AlgorithmUtils::genRandomArray(const int size) {
	srand((unsigned)time(NULL));

	int* array = new int[size];
	for (int i = 0; i < size; i++) {
		array[i] = rand() % size;
	}

	return array;
}

int* AlgorithmUtils::genOrderedArray(const int size) {
	int* array = new int[size];
	for (int i = 0; i < size; i++) {
		array[i] = i + 1;
	}

	return array;
}
int AlgorithmUtils::getGReference() {
	return g_reference.load();
}
