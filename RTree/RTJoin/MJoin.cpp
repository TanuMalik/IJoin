//
// Test.cpp
//
// This is a direct port of the C version of the RTree test program.
//

#include <iostream>
#include <ctime>
#include "RTree.h"

#define MeshSize 4999696
#define XSize 2236

#define TBegin 10

using namespace std;

typedef int ValueType;

struct Cell
{
  Cell()  {}

  Cell(int a_minX, int a_minY, int a_maxX, int a_maxY)
  {
    min[0] = a_minX;
    min[1] = a_minY;

    max[0] = a_maxX;
    max[1] = a_maxY;
  }


  int min[2];
  int max[2];
};

struct Cell S[MeshSize], T[MeshSize];

int intersects;

//Cell search_rect(6, 4, 9, 6); // search will find above rects that this one overlaps
//Cell search_rect(0, 0, 1, 1); // search will find above rects that this one overlaps


bool MySearchCallback(ValueType id, void* arg)
{
  //cout << "Hit data rect " << id << "\n";
  return true; // keep going
}


int main()
{
  typedef RTree<ValueType, int, 2, float> IndexTree;
  IndexTree STree;
  int nhits;


  // Assign cells to Mesh S
  for(int i=0; i < MeshSize; i++)
  {
     int min_x = i % XSize;
     int min_y = i / XSize;
     //cout << i << "= (" << min_x << "," << min_y << ",),(" << min_x + 1 << "," << min_y + 1 << ")\n";
     S[i] = Cell(min_x,min_y,min_x+1,min_y+1);
  }

  /*for (int i = 0; i < XSize; i++)
  {
  for (int j = XSize*i; j < ((i*XSize)+XSize); j++)
  {
     cout << S[j].min[0] << "," << S[j].max[0] << ";";

  }
  cout << ";\n";
  }*/

  for(int i=0; i<MeshSize; i++)
  {
    STree.Insert(S[i].min, S[i].max, i); // Note, all values including zero are fine in this version
  }

  // Assign cells to Mesh T
  for(int i=0; i < MeshSize; i++)
  {
     int min_x = i % XSize;
     int min_y = i / XSize;
     //cout << i << "= (" << min_x << "," << min_y << ",),(" << min_x + 1 << "," << min_y + 1 << ")\n";
     T[i] = Cell(min_x,min_y,min_x+1,min_y+1);
  }

  clock_t begin = clock();
  for (int i = 0; i < XSize; i++)
  {
  for (int j = XSize*i; j < ((i*XSize)+XSize); j++)
  {
     //for each cell of T fnd if it intersects
     Cell search_rect(T[j].min[0],T[j].min[1],T[j].max[0],T[j].max[1]);
     nhits = STree.Search(search_rect.min, search_rect.max, MySearchCallback, NULL);
     if (nhits > 0)
	intersects++;
  }
  }

  
  clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  
  cout << "Search resulted in " << intersects << " intersections and took time " << elapsed_secs << "\n";
}

