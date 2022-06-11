#include "window.h"
#include "Resource.h"

#include "D3D.h"

#include <memory>

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �뵵: �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���ø����̼� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
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
        //// TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�...
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

    // �⺻ �޽��� �����Դϴ�: GetMessage : �޼����� ���͸� ó���� ��. ���ӿ��� �׷� �ʿ䰡 ���� ������ PeekMessage�� ���.
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
    wcex.lpszClassName = L"DX12Tutorial";//�������Ϳ� ����� �̸�
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

    // ���� ������ Ÿ��Ʋ�ٳ� �޴��� ������ �������� ũ�⸦ ������.
    RECT rc = { 0, 0, (LONG)m_width, (LONG)m_height };

    //�����غ��� ������â ���� �� ũ����� ����̵Ǿ� ����.
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    //�׷��� ������ ������ ũ��� ������ Ŭ���̾�Ʈ ������ ũ�⸦ ���ϴ� ũ��� ������.
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
