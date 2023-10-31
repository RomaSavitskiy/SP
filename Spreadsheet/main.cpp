#include <windows.h>
#include "resource.h"
#include "table.h"

// Объявление указателя на объект класса MyTable и структуры для хранения размеров клиентской области окна
MyTable* myTable;
RECT clientRect;

// Объявление функций
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
ATOM RegisterCustomClass(HINSTANCE);
HWND CreateAppWindow(HINSTANCE, INT);

// Главная функция приложения
INT CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
    MSG msg;
    HWND hWnd;

    // Регистрация пользовательского класса окна
    RegisterCustomClass(hInstance);

    // Инициализация и создание окна приложения
    hWnd = CreateAppWindow(hInstance, nCmdShow);

    msg.message = WM_NULL;
    while (msg.message != WM_QUIT)
    {
        // Обработка и отправка сообщений
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            DispatchMessage(&msg);
        }
    }

    // Освобождение ресурсов и завершение приложения
    delete myTable;
    return (INT)msg.wParam;
}

// Обработчик оконных сообщений
LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_PAINT:
        // Отрисовка содержимого окна с использованием объекта MyTable
        myTable->Draw(hWnd);
        break;
    case WM_SIZE:
    {
        // Обработка изменения размеров окна
        GetClientRect(hWnd, &clientRect);
        myTable->SetClientRect(clientRect);
        // Инвалидация окна для перерисовки
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }
    case WM_DESTROY:
        // Завершение приложения
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}

// Регистрация пользовательского класса окна
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

// Инициализация окна приложения
HWND CreateAppWindow(HINSTANCE hInstance, INT nCmdShow)
{
    HWND hWnd = CreateWindow(L"CustomAppClass", L"Lab 2", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, 0, 900, 720, NULL, NULL, hInstance, NULL);
    if (!hWnd)
    {
        return NULL;
    }

    // Получение размеров клиентской области окна
    GetClientRect(hWnd, &clientRect);

    // Создание объекта MyTable и загрузка данных из файла "table.txt"
    myTable = new MyTable(clientRect, "table.txt");
    if (!myTable)
    {
        return NULL;
    }

    // Отображение и обновление окна
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return hWnd;
}


