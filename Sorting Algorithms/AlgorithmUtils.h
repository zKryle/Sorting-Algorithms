#pragma once
#include <iostream>
#include <Windows.h>
class AlgorithmUtils
{
public:
	int getGReference();
	void printArray(int a[], int n);
	void partialBubbleSort(int array[], int size, int* sizePtr);
	void bubbleSort(int array[], int size, int* sizePtr);
	int* genRandomArray(const int size);
	int* genOrderedArray(const int size);
};

