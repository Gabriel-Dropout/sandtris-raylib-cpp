#include <vector>
#include <stack>
#include <algorithm>

#include "raylib.h"
#include "raylib-cpp.hpp"

#include "Table.hpp"

Table::Table(int W, int H): W(W), H(H), table(H, std::vector<TableEntry>(W)) {
	sandTexture = raylib::RenderTexture2D(W, H);
	SetTextureFilter(sandTexture.GetTexture(), TEXTURE_FILTER_POINT);
    BeginTextureMode(sandTexture);
        ClearBackground(WHITE);
    EndTextureMode();
}

/// --- table algorithm --- ///

void Table::clearTable() {
	table = std::vector<std::vector<TableEntry> >(H, std::vector<TableEntry>(W));
	if(isAutoTextureMode) ClearBackground(WHITE);
}

TableEntry Table::getTableEntry(int x, int y) const {
    if(isSafe(x, y)) return table[y][x];
    return {0, 0, WHITE};
}
int Table::getTableColor(int x, int y) const {
    if(isSafe(x, y)) return table[y][x].color;
    return 0;
}
Color Table::getTableRealColor(int x, int y) const {
    if(isSafe(x, y)) return table[y][x].realColor;
    return WHITE;
}
int Table::getTableMatch(int x, int y) const {
    if(isSafe(x, y)) return table[y][x].match;
    return 0;
}

void Table::setTableEntry(int x, int y, TableEntry v) {
    if(isSafe(x, y)) {
        table[y][x] = v;
        if(isAutoTextureMode) DrawRectangle(x, y, 1, 1, table[y][x].realColor);
    }
}
void Table::setTableColor(int x, int y, int v) {
    if(isSafe(x, y)) {
        table[y][x].color = v;
    }
}
void Table::setTableRealColor(int x, int y, Color c) {
    if(isSafe(x, y)) {
        table[y][x].realColor = c;
        if(isAutoTextureMode) DrawRectangle(x, y, 1, 1, table[y][x].realColor);
    }
}
void Table::setTableMatch(int x, int y, int idx) {
    if(isSafe(x, y)) {
        table[y][x].match = idx;
    }
}

void Table::floodFillTable(int x, int y, TableEntry entry) {
    if(!isSafe(x, y)) return;  // just for safety
    if(table[y][x].color == entry.color) return;  // prevent infinite loop

    int prev = table[y][x].color;
    setTableEntry(x, y, entry);

    static const int dx[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    static const int dy[8] = {-1, -1, 0, 1, 1, 1, 0, -1};

    for(int i = 0; i < 8; i++) {
        int nx = x + dx[i], ny = y + dy[i];
        if(isSafe(nx, ny) && table[ny][nx].color == prev) floodFillTable(nx, ny, entry);
    }
}

void Table::updateTable() {
    for(int i=H-2; i>=0; i--) for(int j=0; j<W; j++) {
        if(table[i][j].color != 0) {
            if(table[i+1][j].color == 0) {
                setTableEntry(j, i+1, table[i][j]);
                setTableEntry(j, i, emptyEntry);
            }else if(isSafe(j+1, i+1) && table[i+1][j+1].color == 0 && table[i][j+1].color == 0) {
                setTableEntry(j+1, i+1, table[i][j]);
                setTableEntry(j, i, emptyEntry);
            }else if(isSafe(j-1, i+1) && table[i+1][j-1].color == 0 && table[i][j-1].color == 0) {
                setTableEntry(j-1, i+1, table[i][j]);
                setTableEntry(j, i, emptyEntry);
            }
        }
    }
}

/// --- matching algorithm --- ///

void Table::clearMatch() {
    // use range-based for loop
    for(auto &row: table)
        for(auto &entry: row)
            entry.match = 0;
	matchValid.clear();
}
bool Table::findMatch(int flag) {
    return std::find(matchValid.begin(), matchValid.end(), flag) != matchValid.end();
}
bool Table::appendMatch(int flag) {
    if(!findMatch(flag)) {
        matchValid.push_back(flag);
        return true;
    }
    return false;
}


void Table::matchNaive() {
    clearMatch();

    for(int i=0; i<H; i++) {
        if(table[i][0].color != 0 && table[i][0].match == 0)
            matchNaiveHelper(0, i, i+1);
    }
    for(int i=0; i<H; i++) {
        if(table[i][W-1].match != 0) appendMatch(table[i][W-1].match);
    }
}
void Table::matchNaiveHelper(int x, int y, int flag) {
    if(!isSafe(x, y)) return;  // just for safety

    table[y][x].match = flag;
    
    static const int dx[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    static const int dy[8] = {-1, -1, 0, 1, 1, 1, 0, -1};

    for(int i = 0; i < 8; i++) {
        int nx = x + dx[i], ny = y + dy[i];
        if(isSafe(nx, ny) && table[ny][nx].color == table[y][x].color && table[ny][nx].match == 0)
            matchNaiveHelper(nx, ny, flag);
    }
}

void Table::matchStack() {
    clearMatch();

    for(int i=0; i<H; i++) {
        if(table[i][0].color != 0 && table[i][0].match == 0)
            matchStackHelper(0, i, i+1);
    }
    for(int i=0; i<H; i++) {
        if(table[i][W-1].match != 0) appendMatch(table[i][W-1].match);
    }
}
void Table::matchStackHelper(int x, int y, int flag) {
    if(!isSafe(x, y)) return;  // just for safety

    static const int dx[8] = {0, 1, 1, 1, 0, -1, -1, -1}; // relative neighbor x coordinates
    static const int dy[8] = {-1, -1, 0, 1, 1, 1, 0, -1}; // relative neighbor y coordinates

    std::stack<std::pair<int, int> > stack;
    stack.push({x, y});
    while(!stack.empty()) {
        auto [x, y] = stack.top();
        stack.pop();
        table[y][x].match = flag;

        for(int i = 0; i < 8; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if(isSafe(nx, ny) && table[ny][nx].color == table[y][x].color && table[ny][nx].match == 0)
                stack.push({nx, ny});
        }
    }
}