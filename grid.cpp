#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include "pprint.hpp"

using namespace std;

struct Coord {
  unsigned int x, y;
};

struct Rect {
  int x1, y1, x2, y2;

  friend ostream& operator<< (ostream& stream, const Rect& r) {
    stream << "(" << r.x1 << ", " << r.y1 << ")->(" << r.x2 << ", " << r.y2 << ")";
    return stream;
  }
};

struct Map {
  unsigned int x, y;
  vector<vector<char>> grid;

  char safeGet(int xPos, int yPos) const {
    if (xPos >= (int)x || xPos < 0 || yPos >= (int)y || yPos < 0) return '0';
    else return grid[xPos][yPos];
  }

  void fill(int xPos, int yPos) {
    if (xPos >= (int)x || xPos < 0 || yPos >= (int)y || yPos < 0) {
      cout << endl << "ERROR: Out of bounds fill " << xPos << ' ' << yPos << endl;
      exit(0);
    }
    if (grid[xPos][yPos] != '1') {
      cout << endl << "ERROR: Can't fill rect " << xPos << ' ' << yPos << endl;
      exit(0);
    }
    grid[xPos][yPos]='2';
  }

  friend ostream& operator<< (ostream& stream, const Map& m) {
    for (auto l: m.grid) stream << l << endl;
    return stream;
  }
};

Map readMap(string filename) {
  Map m;
  ifstream in(filename.c_str());
  in >> m.x >> m.y;
  m.grid = vector<vector<char>>(m.y, vector<char>(m.x, ' '));
  char c;
  for (unsigned int y = 0; y < m.y; ++y) {
    for (unsigned int x = 0; x < m.x; ++x) {
      in >> c;
      m.grid[y][x] = c;
    }
  }
  return m;
}

bool isDone(const Map& m) {
  for (unsigned int x = 0; x < m.x; ++x) {
    for (unsigned int y = 0; y < m.y; ++y) {
      if (m.safeGet(x, y) == '1') return false;
    }
  }
  return true;
}

bool isSingleWidth(const Map& m, int x, int y) {
  if ((m.safeGet(x + 1, y) != '1' && m.safeGet(x - 1, y) != '1') ||
      (m.safeGet(x, y + 1) != '1' && m.safeGet(x, y - 1) != '1')) return true;
  return false;
}

Rect expandSingle(Map& m, int x, int y) {
  int y1 = y, y2 = y;
  int x1 = x, x2 = x;
  m.fill(x,y);
  if (m.safeGet(x + 1, y) != '1' && m.safeGet(x - 1, y) != '1') {
    while (m.safeGet(x, y1+1) == '1') { y1++; m.fill(x, y1); }
    while (m.safeGet(x, y2-1) == '1') { y2--; m.fill(x, y2);}
  }
  if (m.safeGet(x, y + 1) != '1' && m.safeGet(x, y - 1) != '1') {
    while (m.safeGet(x1+1, y) == '1') { x1++; m.fill(x1,y); } 
    while (m.safeGet(x2-1, y) == '1') { x2--; m.fill(x2,y); }
  }
  Rect r = {x1, y1, x2, y2};
  return r;
}

// x, y should be top left corner
// and since it's not a single, we know it's at least a 2x2
Rect expandRect(Map &m, int x, int y) {
  int y1 = y, y2 = y+1;
  int x1 = x, x2 = x+1;
  m.fill(x1, y1); m.fill(x1+1, y1); m.fill(x1, y1+1); m.fill(x1+1, y1+1);
  while (true) {
    // Try to grow right
    bool validRight = true, validDown = true;
    for (int dY = y1; dY <= y2; ++dY) {
      if (m.safeGet(x2+1, dY) != '1') { validRight = false; break; }
    }
    if (validRight) {
      x2++;
      for (int dY = y1; dY <= y2; ++dY) m.fill(x2,dY);
    }

    // Grow down
    for (int dX = x1; dX <= x2; ++dX) {
      if (m.safeGet(dX, y2+1) != '1') { validDown = false; break; }
    }
    if (validDown) {
      y2++;
      for (int dX = x1; dX <= x2; ++dX) m.fill(dX,y2);
    }

    if (!validDown && !validRight) break;
  }
  Rect r = {x1, y1, x2, y2};
  return r;
}

Coord getSingle(const Map& m) {
  Coord out = {0, 0};
  for (unsigned int x = 0; x < m.x; ++x) {
    for (unsigned int y = 0; y < m.y; ++y) {
      if (m.grid[x][y] == '1') {
        if (isSingleWidth(m,(int)x,(int)y)) {
          out.x = x; out.y = y;
          return out;
        }
      } 
    }
  }
  return out;
}

Coord getAnyCornerSpot(const Map& m) {
  Coord out = {0, 0};
  for (unsigned int y = 0; y < m.y; ++y) {
    for (unsigned int x = 0; x < m.x; ++x) {
      if (m.grid[x][y] == '1') {
        out.x = x; out.y = y;
        return out;
      } 
    }
  }
  return out;
}

int main() {
  Map m = readMap("in2.txt");

  int counter = 0;
  while (!isDone(m)) {
    cout << endl << "-- Counter " << counter << " --" << endl;
    Coord singleCoord = getSingle(m); cout << "Coord: " << singleCoord.x << ' ' << singleCoord.y << endl;
    if (singleCoord.x == 0 && singleCoord.y == 0) {
      cout << "Can't find single row" << endl;
      Coord any = getAnyCornerSpot(m); cout << "Expanding: " << any.x << ' ' << any.y << endl;
      Rect rect = expandRect(m, any.x, any.y); cout << rect << endl;
    } else {
      Rect single = expandSingle(m, singleCoord.x, singleCoord.y); cout << single << endl;
    }
    cout << m;
    counter++;
  }  
}