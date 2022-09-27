#include <iostream>
#include <thread>
#include <algorithm>
#include <cmath>
#include <Windows.h>
#include <tchar.h>
#include <commctrl.h>
#include <Uxtheme.h>
#include "AlgorithmUtils.h"


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

const int MY_TIMERID1 = 100;
RECT rect;
int* array;
int arraySize = 100;
AlgorithmUtils utils = AlgorithmUtils(); 
bool quit = false;
std::atomic<bool> killThreads = false, paint = false;
std::thread threadPaint;
std::thread threadAlgo;

//Buttons:
HWND playButton;


void algo() {
	utils.bubbleSort(array, arraySize, &arraySize);
	return;
}

void paintFunc(HWND hWnd) {
	while (true) {
		if (paint.load(std::memory_order_acquire)) {
				paint.store(false, std::memory_order_release);
				PAINTSTRUCT ps;
				HDC hdc;

				hdc = BeginPaint(hWnd, &ps);

				HDC memDC = CreateCompatibleDC(hdc);

				RECT rcClientRect;
				GetClientRect(hWnd, &rcClientRect);

				HBITMAP bmp = CreateCompatibleBitmap(hdc,
					rcClientRect.right - rcClientRect.left,
					rcClientRect.bottom - rcClientRect.top);

				HBITMAP oldBmp = (HBITMAP)SelectObject(memDC, bmp);

				// Drawing Backgrounds.

				rect.top = 80;
				rect.left = 50;
				rect.right = rcClientRect.right - rect.left;
				rect.bottom = rcClientRect.bottom - rect.top + 32;

				FillRect(memDC, &rcClientRect, (HBRUSH)GetStockObject(DC_BRUSH));
				SelectObject(memDC, GetStockObject(BLACK_BRUSH));
				RoundRect(memDC, rect.left, rect.top, rect.right, rect.bottom, 20, 20);
				SelectObject(memDC, GetStockObject(DC_BRUSH));

				// End of drawing Backgrounds.

				// Draw Rectangles.

				SelectObject(memDC, GetStockObject(BLACK_PEN));

				HBRUSH RED_BRUSH = CreateSolidBrush(RGB(255, 0, 0));
				HPEN RED_PEN = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));

				for (int i = 1; i < arraySize + 1; i++) {
					if (utils.getGReference() == array[i] || ((i - 1) >= 0 && utils.getGReference() == array[i - 1]) || ((i + 1) < arraySize && utils.getGReference() == array[i + 1])) {
						if (arraySize > 250) {
							SelectObject(memDC, RED_PEN);
						}
						SelectObject(memDC, RED_BRUSH);
					}
					else {
						SelectObject(memDC, GetStockObject(WHITE_BRUSH));
						if (arraySize > 250) {
							SelectObject(memDC, GetStockObject(WHITE_PEN));
						}
					}
					Rectangle(memDC, ceil(rect.left + 10 - ((float)(rect.right - rect.left - 19) / (float)arraySize) + (((float)(rect.right - rect.left - 20) / (float)arraySize) * i)), rect.bottom - 10 - (((float)array[i - 1] / (float)arraySize) * (rcClientRect.bottom - rect.top - 68)), ceil(rect.left + 10 + (((float)(rect.right - rect.left - 20) / (float)arraySize) * i)), rect.bottom - 10);
					}

				SelectObject(memDC, GetStockObject(BLACK_PEN));

				BitBlt(hdc, 0, 0, rcClientRect.right - rcClientRect.left,
					rcClientRect.bottom - rcClientRect.top, memDC, 0, 0, SRCCOPY);

				SelectObject(memDC, oldBmp);
				DeleteObject(bmp);
				DeleteObject(RED_BRUSH);
				DeleteObject(RED_PEN);
				DeleteDC(memDC);

				EndPaint(hWnd, &ps);
				
			}
		if (killThreads.load(std::memory_order_acquire)) break;
	}
	return;
}

HWND createBmpButton(HWND parentHwnd, int width, int height, int posX, int posY) {
	return CreateWindow(L"BUTTON", NULL, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_OWNERDRAW, posX, posY,
		width, height, parentHwnd, NULL, (HINSTANCE)GetWindowLongPtr(parentHwnd, GWLP_HINSTANCE), NULL);    
}

void writeRegSizePos(HWND hWnd) {
	WINDOWPLACEMENT wp = { 0 };
	wp.length = sizeof(wp);
	GetWindowPlacement(hWnd, &wp);
	HKEY hkey;
	RECT clientRect = wp.rcNormalPosition;
	int upperLeftX = clientRect.left, upperLeftY = clientRect.top, lowerRightX = clientRect.right, lowerRightY = clientRect.bottom;
	RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\zKryle\\SortingAlgorithms", 0, KEY_SET_VALUE, &hkey);
	RegSetValueExA(hkey, "upperLeftX", 0, REG_DWORD, (const BYTE*)&upperLeftX, sizeof(upperLeftX));
	RegSetValueExA(hkey, "upperLeftY", 0, REG_DWORD, (const BYTE*)&upperLeftY, sizeof(upperLeftY));
	RegSetValueExA(hkey, "lowerRightX", 0, REG_DWORD, (const BYTE*)&lowerRightX, sizeof(lowerRightX));
	RegSetValueExA(hkey, "lowerRightY", 0, REG_DWORD, (const BYTE*)&lowerRightY, sizeof(lowerRightY));
	RegCloseKey(hkey);
}

void loadRegSizePos(HWND hWnd) {
	WINDOWPLACEMENT wp = { 0 };
	wp.length = sizeof(wp);
	GetWindowPlacement(hWnd, &wp);
	HKEY hkey;
	RECT clientRect = wp.rcNormalPosition;
	DWORD keyWasOpened;
	DWORD size = sizeof(int);
	int upperLeftX, upperLeftY, lowerRightX, lowerRightY;
	RegCreateKeyExA(HKEY_CURRENT_USER, "Software\\zKryle\\SortingAlgorithms", 0, NULL, NULL, KEY_CREATE_SUB_KEY | KEY_READ, NULL, &hkey, &keyWasOpened);
	if (keyWasOpened = REG_OPENED_EXISTING_KEY) {
		RegGetValueA(hkey, NULL, "upperLeftX", RRF_RT_DWORD, NULL, &upperLeftX, &size);
		RegGetValueA(hkey, NULL, "upperLeftY", RRF_RT_DWORD, NULL, &upperLeftY, &size);
		RegGetValueA(hkey, NULL, "lowerRightX", RRF_RT_DWORD, NULL, &lowerRightX, &size);
		RegGetValueA(hkey, NULL, "lowerRightY", RRF_RT_DWORD, NULL, &lowerRightY, &size);
		clientRect.left = upperLeftX; clientRect.top = upperLeftY;
		clientRect.right = lowerRightX; clientRect.bottom = lowerRightY;
		wp.rcNormalPosition = clientRect;
		SetWindowPlacement(hWnd, &wp);
	}
	RegCloseKey(hkey);
}

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
) {
	array = utils.genOrderedArray(arraySize);
	std::random_shuffle(&array[0], &array[arraySize - 1]);
	//utils.printArray(array, arraySize);
	//Gen Array ^^^

	const wchar_t CLASS_NAME[] = L"Sorting Algorithms";
	
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
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

	HWND hwnd = CreateWindowExW(WS_EX_OVERLAPPEDWINDOW, CLASS_NAME, L"Sorting Algorithms", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT, 740,
		560, NULL, NULL, hInstance, NULL);

	if (!hwnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindowEx failed!"),
			_T("Sorting Algorithms"),
			NULL);

		return 1;
	}

	loadRegSizePos(hwnd);
	
	ShowWindow(hwnd,nCmdShow);
	UpdateWindow(hwnd);

	threadPaint = std::thread(paintFunc, hwnd);
	threadAlgo = std::thread(algo);

	MSG msg;

	msg.message = 0;

	while (WM_QUIT != msg.message)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0) //Or use an if statement
		{
			if (msg.message != WM_PAINT) {
			
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			InvalidateRect(hwnd, NULL, FALSE);
			UpdateWindow(hwnd);
		}
		InvalidateRect(hwnd, NULL, FALSE);
		UpdateWindow(hwnd);
	}

	return (int)msg.wParam;
}


int main()
{
	return WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOWNORMAL);
}



LRESULT CALLBACK WndProc(
	_In_ HWND   hWnd,
	_In_ UINT   message,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {
	switch (message)
	{
	case WM_CREATE:
	{
		//playButton = createBmpButton(hWnd, 60, 60, 0, 0);

		//HDC hdcScreen = GetDC(NULL);
		//
		//// Create a device context
		//HDC hdcBmp = CreateCompatibleDC(hdcScreen);
		//
		//// Create a bitmap and attach it to the device context we created above...
		//HBITMAP bmp = CreateCompatibleBitmap(hdcScreen, 60, 60);
		//HBITMAP hbmOld = (HBITMAP) (SelectObject(hdcBmp, bmp));
		//
		//// Now, you can draw into bmp using the device context hdcBmp...
		//RECT r = { 0, 0, 60, 60 };
		//FillRect(hdcBmp, &r, (HBRUSH)(GetStockObject(BLACK_BRUSH)));
		//// etc...
		//
		//// Clean up the GDI objects we've created.
		//SelectObject(hdcBmp, hbmOld);
		//DeleteDC(hdcBmp);
		//ReleaseDC(NULL, hdcScreen);
		//
		//SendMessage(playButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);

		break;
	}
	case WM_SIZE: {
		//RECT buttonRc = {}, winRc = {};
		//GetClientRect(hWnd, &winRc);
		//GetClientRect(playButton, &buttonRc);
		//MoveWindow(playButton, (winRc.right / 2) - ((buttonRc.right - buttonRc.left)/2), 2, buttonRc.right - buttonRc.left, buttonRc.bottom - buttonRc.top, 1);
		//
		break;
	}
	case WM_PAINT:
		paint.store(true);
		break;
	case WM_ENTERSIZEMOVE:
		SetTimer(hWnd, MY_TIMERID1, 0, NULL);
		return DefWindowProc(hWnd, message, wParam, lParam);
	case WM_TIMER:
		InvalidateRect(hWnd, NULL, FALSE);
		UpdateWindow(hWnd);
		break;
	case WM_EXITSIZEMOVE:
		KillTimer(hWnd, MY_TIMERID1);
		return DefWindowProc(hWnd, message, wParam, lParam);
	case WM_CLOSE: {
		killThreads.store(true);
		threadAlgo.join();
		threadPaint.join();
		writeRegSizePos(hWnd);
		return DestroyWindow(hWnd);
	}
	case WM_DESTROY:
		quit = true;
		PostQuitMessage(0);
		break;
	case WM_ERASEBKGND: 
		return 1; 
	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
		lpMMI->ptMinTrackSize.x = 740;
		lpMMI->ptMinTrackSize.y = 560;
		return 0;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
