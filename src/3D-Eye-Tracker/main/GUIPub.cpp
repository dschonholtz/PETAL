// WindowsProject1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "GUIPub.h"
#include "ShellAPI.h"
#include "DotTrainer.h"
#include <thread> 
#include "DummyPublisher.h"
#include "EyePosDebug.h"
#include<vector>

#define MAX_LOADSTRING 100
#define GWL_HINSTANCE       (-6)

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HWND hWndMain;

// Forward declarations of functions included in this code module:
VOID				GUIStartup(LPCSTR path);
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
HWND CreateButton(int x, int y, int width, int height, string buttonText, int ID, HWND, HWND);
int BCX_Circle(HWND Wnd, int X, int Y, int R, int color, int Fill, HDC DrawHDC);
vector<HWND> buttons;
bool OptiKeyActive = false;

#define ID_OPTIKEYBUTTON 0x8801
HWND optikeyButton;

HWND trainingButton;
#define ID_TRAININGBUTTON 0x8805

HWND loadFromFileButton;
#define ID_LOADFROMFILEBUTTON 0x8809

HWND loadTrainingButton;
#define ID_LOADFROMTRAININGDATAFROMFILEBUTTON 0x8813

PubSubHandler *pubSubHandler;
DotTrainer* dt;
DummyPublisher* dp;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	pubSubHandler = new PubSubHandler();
	dp = new DummyPublisher(pubSubHandler);
	dt = new DotTrainer(pubSubHandler);
	EyePosDebug* eyeDebug = new EyePosDebug(pubSubHandler);
	
	
	
	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_GUIPUB, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}
	bool killSignal = false;
	std::thread eyeTracker(eyeTrackerLoop, pubSubHandler, &killSignal);

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GUIPUB));

	MSG msg;

	MousePosData lastMouse = eyeDebug->GetEyePos();
	MousePosData curMouse = eyeDebug->GetEyePos();
	int mouseDotRadius = 5;
	AddDotToDisplay(lastMouse.x, lastMouse.y, mouseDotRadius);
	
	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		ClearDot(lastMouse.x, lastMouse.y, mouseDotRadius);
		AddDotToDisplay(curMouse.x, curMouse.y, mouseDotRadius);
		lastMouse = curMouse;
		curMouse = eyeDebug->GetEyePos();

		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	killSignal = true;
	eyeTracker.join();
	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GUIPUB));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDI_GUIPUB);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	 hWndMain = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	 int err = GetLastError();

	if (!hWndMain)
	{
		return FALSE;
	}

	optikeyButton = CreateWindow(
		"BUTTON",  // Predefined class; Unicode assumed 
		"Start Optikey",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		50,         // x position 
		50,         // y position 
		150,        // Button width
		150,        // Button height
		hWndMain,     // Parent window
		(HMENU)ID_OPTIKEYBUTTON,
		(HINSTANCE)GetWindowLong(hWndMain, GWL_HINSTANCE),
		NULL);      // Pointer not needed.
	buttons.push_back(optikeyButton);

	trainingButton = CreateWindow(
		"BUTTON",  // Predefined class; Unicode assumed 
		"Start Training",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		50,         // x position 
		200,         // y position 
		150,        // Button width
		150,        // Button height
		hWndMain,     // Parent window
		(HMENU)ID_TRAININGBUTTON,
		(HINSTANCE)GetWindowLong(hWndMain, GWL_HINSTANCE),
		NULL);      // Pointer not needed.
	buttons.push_back(trainingButton);

	loadFromFileButton = CreateWindow(
		"BUTTON",  // Predefined class; Unicode assumed 
		"Load Neural Network From File",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		50,         // x position 
		350,         // y position 
		150,        // Button width
		150,        // Button height
		hWndMain,     // Parent window
		(HMENU)ID_LOADFROMFILEBUTTON,
		(HINSTANCE)GetWindowLong(hWndMain, GWL_HINSTANCE),
		NULL);      // Pointer not needed.
	buttons.push_back(loadFromFileButton);


	loadTrainingButton = CreateWindow(
		"BUTTON",  // Predefined class; Unicode assumed 
		"Load Training Data From File",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		50,         // x position 
		500,         // y position 
		150,        // Button width
		150,        // Button height
		hWndMain,     // Parent window
		(HMENU)ID_LOADFROMTRAININGDATAFROMFILEBUTTON,
		(HINSTANCE)GetWindowLong(hWndMain, GWL_HINSTANCE),
		NULL);      // Pointer not needed.
	buttons.push_back(loadTrainingButton);

	 //CreateButton(50, 200, 100, 100, "Start Training", ID_TRAININGBUTTON, trainingButton, hWndMain);




	ShowWindow(hWndMain, nCmdShow);
	UpdateWindow(hWndMain);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	TCHAR greeting[] = _T("Hello, Windows desktop!");
	LPCSTR outputMsg = greeting;

	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		EventMessage em;
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_OPTIKEYBUTTON:
			if (!OptiKeyActive) {
				GUIStartup((LPCSTR)("C:\\Program Files (x86)\\OptiKey\\OptiKey.exe"));
			}
			OptiKeyActive = true;
			break;
		case ID_TRAININGBUTTON:
			dt->StartThread();
			break;
		case ID_LOADFROMFILEBUTTON:
			em.topic = LoadNeuralNetworkFromFile;
			//em.data = &stuff;
			dp->Publish(em);
			break;
		case ID_LOADFROMTRAININGDATAFROMFILEBUTTON:
			em.topic = LoadTrainingDataFromFile;
			//em.data = &stuff;
			dp->Publish(em);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc,
			5, 5,
			outputMsg, strlen(outputMsg));
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

VOID GUIStartup(LPCSTR path)
{
	std::cout << "in startup\n";
	ShellExecute(NULL, _T("open"), path, NULL, NULL, SW_SHOWDEFAULT);
	std::cout << "after attempt\n";
}

void CreateButton(int x, int y, int width, int height, LPCSTR buttonText, int ID, HWND &button ,HWND &hWndMain) {
	button = CreateWindow(
		"BUTTON",  // Predefined class; Unicode assumed 
		buttonText,      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		x,         // x position 
		y,         // y position 
		width,        // Button width
		height,        // Button height
		hWndMain,     // Parent window
		(HMENU)ID,
		(HINSTANCE)GetWindowLong(hWndMain, GWL_HINSTANCE),
		NULL);      // Pointer not needed.
}

const int DOTRADIUS = 20;
void AddDotToDisplay(int x, int y, int r, int g, int b) {
	BCX_Circle(hWndMain, x, y, DOTRADIUS, RGB(r,g,b), true, NULL);
}

void ClearDot(int x, int y) {
	BCX_Circle(hWndMain, x, y, DOTRADIUS, RGB(255, 255, 255), true, NULL);
}

void AddDotToDisplay(int x, int y, int radius) {
	BCX_Circle(hWndMain, x, y, radius, RGB(255, 0, 0), true, NULL);
}

void ClearDot(int x, int y, int radius) {
	BCX_Circle(hWndMain, x, y, radius, RGB(0, 0, 0), true, NULL);
}

int BCX_Circle(HWND Wnd, int X, int Y, int R, int color, int Fill, HDC DrawHDC)
{
	int a, b = 0;
	if (!DrawHDC)
	{
		DrawHDC = GetDC(Wnd);
		b = 1;
	}
	HPEN   hNPen = CreatePen(PS_SOLID, 1, color);
	HPEN   hOPen = (HPEN)SelectObject(DrawHDC, hNPen);
	HBRUSH hOldBrush;
	HBRUSH hNewBrush;
	if (Fill)
	{
		hNewBrush = CreateSolidBrush(color);
		hOldBrush = (HBRUSH)SelectObject(DrawHDC, hNewBrush);
	}
	else
	{
		hNewBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		hOldBrush = (HBRUSH)SelectObject(DrawHDC, hNewBrush);
	}
	// Win API function
	a = Ellipse(DrawHDC, X - R, Y + R, X + R, Y - R);
	DeleteObject(SelectObject(DrawHDC, hOPen));
	DeleteObject(SelectObject(DrawHDC, hOldBrush));
	if (b) ReleaseDC(Wnd, DrawHDC);
	return a;
}

void HideButtons() {
	for(HWND button : buttons) {
		ShowWindow(button, SW_HIDE);
	}
}

void ShowButtons() {
	for (HWND button : buttons) {
		ShowWindow(button, SW_SHOW);
	}
}
