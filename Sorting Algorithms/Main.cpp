#include <iostream>
#include <algorithm>
#include <cmath>
#include <Windows.h>
#include <tchar.h>
#include <commctrl.h>
#include <Uxtheme.h>
#include "AlgorithmUtils.h"


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

const int WIN_W = 1280, WIN_H = 720, ALG_W = 1140, ALG_H = 620;
RECT rect;
int* array;
int arraySize = 200;
AlgorithmUtils utils = AlgorithmUtils();
HWND hwnd;
bool quit = false;

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
) {
	array = utils.genOrderedArray(arraySize);
	std::random_shuffle(&array[0], &array[arraySize - 1]);
	utils.printArray(array, arraySize);
	//Gen Array ^^^

	const wchar_t CLASS_NAME[] = L"Sorting Algorithms";
	
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); 
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = CLASS_NAME;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Sorting Algorithms"),
			NULL);

		return 1;
	}

	hwnd = CreateWindowExW(WS_EX_OVERLAPPEDWINDOW, CLASS_NAME, L"Sorting Algorithms", (WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME) ^ WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, WIN_W,
		WIN_H, NULL, NULL, hInstance, NULL);

	if (!hwnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindowEx failed!"),
			_T("Sorting Algorithms"),
			NULL);

		return 1;
	}
	
	ShowWindow(hwnd,nCmdShow);
	UpdateWindow(hwnd);

	int tempSize = arraySize;



	MSG msg;
	while (!quit)
	{
		if (tempSize > 0) {
			utils.partialBubbleSort(array, tempSize, &tempSize);
			tempSize--;
			//Sleep(4);
		}
		if (PeekMessageA(&msg, hwnd, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		
	}

	return (int)msg.wParam;
}

int main()
{
	

	array = utils.genOrderedArray(arraySize);

	std::random_shuffle(&array[0], &array[arraySize -1]);

    utils.printArray(array, arraySize);



	return WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOWNORMAL);
}

LRESULT CALLBACK WndProc(
	_In_ HWND   hWnd,
	_In_ UINT   message,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
	{

		hdc = BeginPaint(hWnd, &ps);

		HDC memDC = CreateCompatibleDC(hdc);

		RECT rcClientRect;
		GetClientRect(hwnd, &rcClientRect);

		HBITMAP bmp = CreateCompatibleBitmap(hdc,
			rcClientRect.right - rcClientRect.left,
			rcClientRect.bottom - rcClientRect.top);

		HBITMAP oldBmp = (HBITMAP)SelectObject(memDC, bmp);

		// Drawing Backgrounds.
	
		rect.top = 80;
		rect.left = 50;
		rect.right = WIN_W - rect.left - 20;
		rect.bottom = WIN_H - rect.top - 10;
	
		FillRect(memDC, &rcClientRect, (HBRUSH)GetStockObject(DC_BRUSH));
		SelectObject(memDC, GetStockObject(BLACK_BRUSH));
		RoundRect(memDC, rect.left, rect.top, rect.right, rect.bottom, 20, 20);
		SelectObject(memDC, GetStockObject(DC_BRUSH));
	
		// End of drawing Backgrounds.
	
		// Draw Rectangles.

		SelectObject(memDC, GetStockObject(WHITE_PEN));

		for (int i = 1; i < arraySize + 1; i++) {
			Rectangle(memDC, round(WIN_W - ALG_W - 80 + (((float)ALG_W / (float)arraySize) * i)), 620 - (((float)array[i - 1] / (float)arraySize) * 520), round(WIN_W - ALG_W - 80 - ((float)ALG_W/ (float)arraySize) + (((float)ALG_W / (float)arraySize) * i)), ALG_H);
		}

		SelectObject(memDC, GetStockObject(BLACK_PEN));

		BitBlt(hdc, 0, 0, rcClientRect.right - rcClientRect.left,
			rcClientRect.bottom - rcClientRect.top, memDC, 0, 0, SRCCOPY);

		SelectObject(memDC, oldBmp); 
		DeleteObject(bmp); 
		DeleteDC(memDC);   

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		quit = true;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}


//int size = 0;
//bubbleSort(genRandomArray(size), size, &size);