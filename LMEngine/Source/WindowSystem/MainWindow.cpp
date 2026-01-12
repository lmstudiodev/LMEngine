#include "stdafx.h"
#include "MainWindow.h"

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		break;
	}
	case WM_SIZE:
	{
		MainWindow* window = (MainWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (window)
			window->OnSize();
		break;
	}
	case WM_DESTROY:
	{
		MainWindow* window = (MainWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->OnDestroy();
		::PostQuitMessage(0);
		break;
	}
	case WM_SETFOCUS:
	{
		MainWindow* window = (MainWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if(window)
			window->OnFocus();
		break;
	}
	case WM_KILLFOCUS:
	{
		MainWindow* window = (MainWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->OnKillFocus();
		break;
	}
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

MainWindow::MainWindow() : m_hwnd(nullptr), m_isRunning(false), m_wndRect({0,0,0,0}), m_isInitialized(false)
{
	WNDCLASSEX wc{};
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = L"LMEWindow";
	wc.lpszMenuName = L"";
	wc.style = NULL;
	wc.lpfnWndProc = WndProc;

	if (!RegisterClassEx(&wc))
		throw std::exception("[LMEngine Error] Register Window Class failed.");

	m_wndRect = { 0, 0, 1920, 1280 };

	AdjustWindowRect(&m_wndRect, WS_OVERLAPPEDWINDOW, false);

	m_hwnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
		L"LMEWindow",
		L"D3DX LMEngine",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		m_wndRect.right - m_wndRect.left,
		m_wndRect.bottom - m_wndRect.top,
		NULL, NULL, NULL, NULL);

	if (!m_hwnd)
		throw std::exception("[LMEngine Error] Window creation failed.");

	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);

	m_isRunning = true;
}

MainWindow::~MainWindow()
{

}

void MainWindow::Broadcast()
{
	MSG msg;

	if (!this->m_isInitialized)
	{
		SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
		this->OnCreate();
		
		this->m_isInitialized = true;
	}

	this->OnUpdate();

	while (PeekMessageW(&msg, m_hwnd, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Sleep(1);
}

bool MainWindow::IsRunnig()
{
	if (m_isRunning)
		Broadcast();

	return m_isRunning;
}

RECT MainWindow::GetClientWindowRect()
{
	RECT rc;

	GetClientRect(this->m_hwnd, &rc);

	return rc;
}

RECT MainWindow::GetScreenSize()
{
	RECT rc;
	rc.right = GetSystemMetrics(SM_CXSCREEN);
	rc.bottom = GetSystemMetrics(SM_CYSCREEN);
	return rc;
}

void MainWindow::OnDestroy()
{
	m_isRunning = false;
}

void MainWindow::OnCreate()
{
	
}

void MainWindow::OnUpdate()
{

}


void MainWindow::OnSize()
{

}
