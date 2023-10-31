#include <windows.h>
#include "resource.h"
#include "table.h"

// ���������� ��������� �� ������ ������ MyTable � ��������� ��� �������� �������� ���������� ������� ����
MyTable* myTable;
RECT clientRect;

// ���������� �������
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
ATOM RegisterCustomClass(HINSTANCE);
HWND CreateAppWindow(HINSTANCE, INT);

// ������� ������� ����������
INT CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
    MSG msg;
    HWND hWnd;

    // ����������� ����������������� ������ ����
    RegisterCustomClass(hInstance);

    // ������������� � �������� ���� ����������
    hWnd = CreateAppWindow(hInstance, nCmdShow);

    msg.message = WM_NULL;
    while (msg.message != WM_QUIT)
    {
        // ��������� � �������� ���������
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            DispatchMessage(&msg);
        }
    }

    // ������������ �������� � ���������� ����������
    delete myTable;
    return (INT)msg.wParam;
}

// ���������� ������� ���������
LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_PAINT:
        // ��������� ����������� ���� � �������������� ������� MyTable
        myTable->Draw(hWnd);
        break;
    case WM_SIZE:
    {
        // ��������� ��������� �������� ����
        GetClientRect(hWnd, &clientRect);
        myTable->SetClientRect(clientRect);
        // ����������� ���� ��� �����������
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }
    case WM_DESTROY:
        // ���������� ����������
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}

// ����������� ����������������� ������ ����
ATOM RegisterCustomClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));
    wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wcex.hInstance = hInstance;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.lpfnWndProc = WndProc;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"CustomAppClass";

    return RegisterClassEx(&wcex);
}

// ������������� ���� ����������
HWND CreateAppWindow(HINSTANCE hInstance, INT nCmdShow)
{
    HWND hWnd = CreateWindow(L"CustomAppClass", L"Lab 2", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, 0, 900, 720, NULL, NULL, hInstance, NULL);
    if (!hWnd)
    {
        return NULL;
    }

    // ��������� �������� ���������� ������� ����
    GetClientRect(hWnd, &clientRect);

    // �������� ������� MyTable � �������� ������ �� ����� "table.txt"
    myTable = new MyTable(clientRect, "table.txt");
    if (!myTable)
    {
        return NULL;
    }

    // ����������� � ���������� ����
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return hWnd;
}


