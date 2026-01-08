#include "MainWindow.h"

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		MainWindow* window = (MainWindow*)((LPCREATESTRUCT)lParam)->lpCreateParams;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
		window->SetWindowHandle(hwnd);
		window->OnCreate();
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

MainWindow::MainWindow() : m_hwnd(nullptr), m_isRunning(false), m_wndRect({0,0,0,0})
{

}

MainWindow::~MainWindow()
{
}

bool MainWindow::Init()
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
		return false;

	m_wndRect = { 0, 0, 1920, 1280 };

	AdjustWindowRect(&m_wndRect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, false);

	m_hwnd = CreateWindowEx(NULL, 
		L"LMEWindow", 
		L"D3DX LMEngine", 
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		m_wndRect.right - m_wndRect.left,
		m_wndRect.bottom - m_wndRect.top,
		NULL, NULL, NULL, this);

	if (!m_hwnd)
		return false;

	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);

	m_isRunning = true;
	
	return true;
}

bool MainWindow::Broadcast()
{
	MSG msg;

	this->OnUpdate();

	while (PeekMessageW(&msg, m_hwnd, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Sleep(0);
	
	return false;
}

bool MainWindow::Release() const
{
	if (m_hwnd)
		DestroyWindow(m_hwnd);
	
	return true;
}

bool MainWindow::IsRunnig() const
{
	return m_isRunning;
}

RECT MainWindow::GetClientWindowRect()
{
	return m_wndRect;
}

void MainWindow::SetWindowHandle(HWND hwnd)
{
	this->m_hwnd = hwnd;
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
