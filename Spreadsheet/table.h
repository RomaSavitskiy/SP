#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <regex>

class MyTable
{
private:
    HDC hdc;
    PAINTSTRUCT paintStruct;
    RECT clientRect;
    std::vector<std::vector<std::string>> dataVector;
    HPEN tablePen;
    HFONT fontHandle;
    INT rowCount;
    INT columnCount;

    INT GetRowCount();
    INT GetColumnCount();
    INT LoadDataFromFile(std::string path);
    void CreateCustomFont(INT height, INT width, INT weight);
    INT DrawCell();

public:
    MyTable(RECT clientRect, std::string path);
    ~MyTable();

    void Draw(HWND hWnd);
    void SetClientRect(RECT clientRect);
};


