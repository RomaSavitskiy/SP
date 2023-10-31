#include "table.h"

#define _CRT_SECURE_NO_WARNINGS

MyTable::MyTable(RECT clientRect, std::string path)
{
    // Создание пера для таблицы.
    tablePen = CreatePen(PS_SOLID, 6, RGB(170, 20, 35));
    this->clientRect = clientRect;
    LoadDataFromFile(path);
}

MyTable::~MyTable()
{
    // Освобождение ресурсов: удаляем шрифт и перо.
    DeleteObject(fontHandle);
    DeleteObject(tablePen);
}

void MyTable::Draw(HWND hWnd)
{
    // Начало рисования в контексте устройства (device context).
    hdc = BeginPaint(hWnd, &paintStruct);
    int fontHeight = 20;
    int fontWidth = 8;
    CreateCustomFont(fontHeight, fontWidth, 8);
    SelectObject(hdc, fontHandle);
    SelectObject(hdc, tablePen);

    TEXTMETRIC textMetric;
    GetTextMetrics(hdc, &textMetric);

    bool step = false;
    while (DrawCell() > clientRect.bottom - clientRect.top)
    {
        // Очистка окна перед изменением размеров шрифта.
        PatBlt(GetDC(hWnd), 0, 0, clientRect.right, clientRect.bottom, WHITENESS);
        if (fontWidth == 1)
        {
            break;
        }

        fontHeight--;
        if (step)
        {
            fontWidth--;
        }
        step = !step;

        CreateCustomFont(fontHeight, fontWidth, 8);
        SelectObject(hdc, fontHandle);
    }

    int tableHeight = DrawCell();

    // Рисуем горизонтальные и вертикальные линии для таблицы.

    // Горизонтальная линия над таблицей.
    MoveToEx(hdc, 0, 1, nullptr);
    LineTo(hdc, clientRect.right - clientRect.left, 0);

    // Вертикальные линии между столбцами.
    for (int i = 1; i < columnCount; i++) {
        MoveToEx(hdc, i * (clientRect.right - clientRect.left) / columnCount, 0, nullptr);
        LineTo(hdc, i * (clientRect.right - clientRect.left) / columnCount, tableHeight);
    }

    // Завершение рисования.
    EndPaint(hWnd, &paintStruct);
}

void MyTable::SetClientRect(RECT clientRect)
{
    // Установка новых границ клиентской области.
    this->clientRect = clientRect;
}

INT MyTable::GetRowCount()
{
    // Возвращает количество строк в таблице.
    return dataVector.size();
}

INT MyTable::GetColumnCount()
{
    // Возвращает количество столбцов в таблице.
    int column = 0;
    for (int i = 0; i < dataVector.size(); i++)
    {
        if (dataVector[i].size() > column)
        {
            column = dataVector[i].size();
        }
    }
    return column;
}

INT MyTable::LoadDataFromFile(std::string path)
{
    // Загрузка данных из файла.
    std::ifstream file(path);
    if (!file)
    {
        return 0; // В случае ошибки загрузки файла.
    }

    std::string line;
    while (std::getline(file, line))
    {
        // Разделение строки на подстроки, разделенные символом "#".
        std::regex regex{ R"([#][\s]+)" };
        std::sregex_token_iterator it{ line.begin(), line.end(), regex, -1 };
        dataVector.emplace_back(std::vector<std::string>{ it, { } });
    }

    rowCount = GetRowCount();
    columnCount = GetColumnCount();

    file.close();
    return 1; // Успешная загрузка данных.
}

void MyTable::CreateCustomFont(INT height, INT width, INT weight)
{
    // Создание кастомного шрифта.
    LOGFONT lf;
    lf.lfCharSet = DEFAULT_CHARSET;
    lf.lfPitchAndFamily = DEFAULT_PITCH;
    lf.lfHeight = height;
    lf.lfWidth = width;
    lf.lfWeight = weight;
    lf.lfItalic = 1;
    lf.lfUnderline = 0;
    lf.lfStrikeOut = 0;
    lf.lfEscapement = 0;

    fontHandle = CreateFontIndirect(&lf);
}

INT MyTable::DrawCell()
{
    // Рисование ячеек таблицы.
    TEXTMETRIC textMetric;
    GetTextMetrics(hdc, &textMetric);

    int tableHeight = 0;
    int cellWidth = (clientRect.right - clientRect.left) / columnCount;
    int minCellHeight = textMetric.tmHeight;

    for (int i = 0; i < rowCount; i++)
    {
        int currentCellHeight = minCellHeight;
        for (int j = 0; j < dataVector[i].size(); j++)
        {
            std::wstring text = std::wstring(dataVector[i][j].begin(), dataVector[i][j].end());
            RECT rect = { j * cellWidth, tableHeight + minCellHeight / 5, (j + 1) * cellWidth, clientRect.bottom - clientRect.top };
            int tempHeight = DrawText(hdc, text.c_str(), text.size(), &rect, DT_CENTER | DT_WORDBREAK | DT_END_ELLIPSIS);

            if (tempHeight > currentCellHeight)
            {
                currentCellHeight = tempHeight;
            }
        }

        tableHeight += currentCellHeight + minCellHeight / 5;
        MoveToEx(hdc, 0, tableHeight, nullptr);
        LineTo(hdc, clientRect.right - clientRect.left, tableHeight);
    }

    return tableHeight; // Возвращает высоту таблицы после рисования.
}


