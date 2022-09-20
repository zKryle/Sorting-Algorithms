#include "AlgorithmUtils.h"
using std::cout;
using std::endl;

extern HWND hwnd;

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
			InvalidateRect(hwnd, NULL, FALSE);
			UpdateWindow(hwnd);
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
		}
		if (iterations < size) {
			bubbleSort(array, size - 1, sizePtr);
		}
		else if (iterations == size) {
			cout << "Sorted Array is: " << endl;
			printArray(array, *sizePtr);
		}
	}
	else cout << "Cannot sort Array of size 0!" << endl;
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
