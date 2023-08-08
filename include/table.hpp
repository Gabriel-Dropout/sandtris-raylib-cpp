#ifndef TABLE_HPP
#define TABLE_HPP

#include <vector>

#include "raylib.h"
#include "raylib-cpp.hpp"

struct TableEntry {
	int color, match;
	Color realColor;
};

class Table {
public:
	static constexpr TableEntry emptyEntry = {0, 0, WHITE};

	const int W, H;
	std::vector<std::vector<TableEntry> > table;
	std::vector<int> matchValid;

	raylib::RenderTexture2D sandTexture;
	bool isAutoTextureMode = true;

public:
	Table(int W, int H);

	bool isSafe(int x, int y) const {
	    return 0<=x && x<W && 0<=y && y<H;
	}

	void setAutoTexture() {
		BeginTextureMode(sandTexture);
		isAutoTextureMode = true;
	}
	void resetAutoTexture() {
		EndTextureMode();
		isAutoTextureMode = false;
	}
	raylib::TextureUnmanaged getTexture() {
        return sandTexture.GetTexture();
    }

	/// --- table algorithm --- ///

	void clearTable();

	TableEntry getTableEntry(int x, int y) const ;
	int getTableColor(int x, int y) const ;
	Color getTableRealColor(int x, int y) const ;
	int getTableMatch(int x, int y) const ;

	void setTableEntry(int x, int y, TableEntry v);
	void setTableColor(int x, int y, int v);
	void setTableRealColor(int x, int y, Color c);
	void setTableMatch(int x, int y, int idx);
	void floodFillTable(int x, int y, TableEntry entry);

	void updateTable();

	/// --- matching algorithm --- ///

	void clearMatch();
	bool findMatch(int flag);
	bool appendMatch(int flag);


	void matchNaiveHelper(int x, int y, int flag);
	void matchNaive();

	void matchStackHelper(int x, int y, int flag);
	void matchStack();
};
#endif