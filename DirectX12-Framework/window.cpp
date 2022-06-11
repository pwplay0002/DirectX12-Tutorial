#include "window.h"
#include "Resource.h"

#include "D3D.h"

#include <memory>

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    D3D* _d3d;
    switch (message)
    {
    case WM_PAINT:
    {
        //_d3d->OnInitialize
        //PAINTSTRUCT ps;
        //HDC hdc = BeginPaint(hWnd, &ps);
        //// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        //EndPaint(hWnd, &ps);
        return 0;
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

Window* Window::GetInst()
{
    static std::unique_ptr<Window> window(new Window());
    return window.get();
}

void Window::DestroyInst()
{
}

Window::Window()
{
}

Window::~Window()
{
    DestroyWindow(m_window);
    m_window = nullptr;
}

bool Window::Init(HINSTANCE hInst)
{
    m_hInst = hInst;
    m_width = 300;
    m_height = 300;
    if (!MyRegisterClass()) { DestroyInst(); return FALSE; }
    if (!InitInstance()) return FALSE;

    return true;
}

int Window::MainLoop()
{
    InitInstance();
    MSG msg;

    // 기본 메시지 루프입니다: GetMessage : 메세지가 들어와만 처리를 함. 게임에서 그럴 필요가 없기 때문에 PeekMessage를 사용.
    while (GetMessage(&msg, nullptr, 0, 0)) 
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

int Window::MainLoop(D3D* d3d)
{
    m_currentWinName = d3d->GetTitle();

    d3d->Initialize(this);
    InitInstance();
    UpdateWindow(m_window);
    ShowWindow(m_window, SW_SHOW);

    MSG msg = { 0 };
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            d3d->Update();
        }
    }

    return 0;
}

ATOM Window::MyRegisterClass()
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInst;
    wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_DIRECTX12FRAMEWORK));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOWFRAME + 1);//background color
    wcex.lpszMenuName = NULL;// MAKEINTRESOURCEW(IDC_WINAPI);//menu
    wcex.lpszClassName = L"DX12Tutorial";//레지스터에 등록할 이름
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_DIRECTX12FRAMEWORK));

    return RegisterClassExW(&wcex);
}

BOOL Window::InitInstance()
{
    m_window = CreateWindow(m_winClassName, m_currentWinName, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_hInst, nullptr);

    if (!m_window)
    {
        return FALSE;
    }

    // 실제 윈도우 타이틀바나 메뉴를 포함한 윈도우의 크기를 구해줌.
    RECT rc = { 0, 0, (LONG)m_width, (LONG)m_height };

    //실행해보면 윈도우창 위에 바 크기까지 계산이되어 있음.
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    //그래서 위에서 구해준 크기로 윈도우 클라이언트 영역의 크기를 원하는 크기로 맞춰줌.
    SetWindowPos(m_window, HWND_TOPMOST, 100, 100, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER);

    return TRUE;
}

void Window::SetWidth(UINT width)
{
	m_width = width; 
	RECT rc = { 0, 0, (LONG)m_width, (LONG)m_height };

	SetWindowPos(m_window, HWND_TOPMOST, 100, 100, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER);
}

void Window::SetHeight(UINT height)
{
	m_height = height; 
	RECT rc = { 0, 0, (LONG)m_width, (LONG)m_height };

	SetWindowPos(m_window, HWND_TOPMOST, 100, 100, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER);
}

void Window::SetWindowSize(UINT width, UINT height)
{
	m_width = width, m_height = height; 
	RECT rc = { 0, 0, (LONG)m_width, (LONG)m_height };

	SetWindowPos(m_window, HWND_TOPMOST, 100, 100, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER);
}

void Window::SetWindowSize(std::tuple<UINT, UINT> windowSize)
{
	m_width = std::get<0>(windowSize);
	m_height = std::get<1>(windowSize);
	RECT rc = { 0, 0, (LONG)m_width, (LONG)m_height };

	SetWindowPos(m_window, HWND_TOPMOST, 100, 100, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER);
}
