#include "table.h"

#define _CRT_SECURE_NO_WARNINGS

MyTable::MyTable(RECT clientRect, std::string path)
{
    // �������� ���� ��� �������.
    tablePen = CreatePen(PS_SOLID, 6, RGB(170, 20, 35));
    this->clientRect = clientRect;
    LoadDataFromFile(path);
}

MyTable::~MyTable()
{
    // ������������ ��������: ������� ����� � ����.
    DeleteObject(fontHandle);
    DeleteObject(tablePen);
}

void MyTable::Draw(HWND hWnd)
{
    // ������ ��������� � ��������� ���������� (device context).
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
        // ������� ���� ����� ���������� �������� ������.
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

    // ������ �������������� � ������������ ����� ��� �������.

    // �������������� ����� ��� ��������.
    MoveToEx(hdc, 0, 1, nullptr);
    LineTo(hdc, clientRect.right - clientRect.left, 0);

    // ������������ ����� ����� ���������.
    for (int i = 1; i < columnCount; i++) {
        MoveToEx(hdc, i * (clientRect.right - clientRect.left) / columnCount, 0, nullptr);
        LineTo(hdc, i * (clientRect.right - clientRect.left) / columnCount, tableHeight);
    }

    // ���������� ���������.
    EndPaint(hWnd, &paintStruct);
}

void MyTable::SetClientRect(RECT clientRect)
{
    // ��������� ����� ������ ���������� �������.
    this->clientRect = clientRect;
}

INT MyTable::GetRowCount()
{
    // ���������� ���������� ����� � �������.
    return dataVector.size();
}

INT MyTable::GetColumnCount()
{
    // ���������� ���������� �������� � �������.
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
    // �������� ������ �� �����.
    std::ifstream file(path);
    if (!file)
    {
        return 0; // � ������ ������ �������� �����.
    }

    std::string line;
    while (std::getline(file, line))
    {
        // ���������� ������ �� ���������, ����������� �������� "#".
        std::regex regex{ R"([#][\s]+)" };
        std::sregex_token_iterator it{ line.begin(), line.end(), regex, -1 };
        dataVector.emplace_back(std::vector<std::string>{ it, { } });
    }

    rowCount = GetRowCount();
    columnCount = GetColumnCount();

    file.close();
    return 1; // �������� �������� ������.
}

void MyTable::CreateCustomFont(INT height, INT width, INT weight)
{
    // �������� ���������� ������.
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
    // ��������� ����� �������.
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

    return tableHeight; // ���������� ������ ������� ����� ���������.
}


