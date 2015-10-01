//
// Test.cpp
//
// This is a direct port of the C version of the RTree test program.
//

#include <iostream>
#include <ctime>
#include <queue>
#include "RTree.h"

#define MeshSize 10000
#define XSize 100

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

struct Cell S[MeshSize], T[MeshSize], N[MeshSize];

int intersects;
int nhbors_cnt = 0;
//Cell search_rect(6, 4, 9, 6); // search will find above rects that this one overlaps
//Cell search_rect(0, 0, 1, 1); // search will find above rects that this one overlaps


int MySearchCallback(ValueType id, void* arg)
{
  cout << "Hit data rect " << id << "\n";
  //return true;// keep going
  return (int)id; 
}


int* getCellCoords(int id)
{
   int coords[4];
   coords[0] = id % XSize;
   coords[1] = id / XSize;
   coords[2] = coords[0] + 1;
   coords[3] = coords[1] + 1;
   return coords;
}

//determine neighbors
queue Neighbors(int id)
{
   std::queue<int> N;
   // is id a corner
   if ((id == 0) || (id == XSize-1) || (id == (XSize* XSize) -1) || (id == (XSize-1) * XSize) )
   {
      if (id == 0)
      {
          coords = getCellCoords(id+1);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]); 
          nhbors_cnt = nhbors_cnt + 1;
          coords = getCellCoords(id+XSize);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]); 
          nhbors_cnt = nhbors_cnt + 1;
          coords = getCellCoords(id+XSize+1);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]); 
          nhbors_cnt = nhbors_cnt + 1;
      }
      if (id == XSize -1)
      {
          coords = getCellCoords(id-1);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]); 
          nhbors_cnt = nhbors_cnt + 1;
          coords = getCellCoords(id+XSize);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]); 
          nhbors_cnt = nhbors_cnt + 1;
          coords = getCellCoords(id+XSize-1);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]); 
          nhbors_cnt = nhbors_cnt + 1;
      }
      if (id == (XSize*XSize)-1)
      {
          coords = getCellCoords(id-1);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]); 
          nhbors_cnt = nhbors_cnt + 1;
          coords = getCellCoords(id-XSize);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]); 
          nhbors_cnt = nhbors_cnt + 1;
          coords = getCellCoords(id-XSize-1);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]); 
          nhbors_cnt = nhbors_cnt + 1;
      }
       if (id == (XSize-1)*XSize)
      {
          coords = getCellCoords(id+1);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]); 
          nhbors_cnt = nhbors_cnt + 1;
          coords = getCellCoords(id-XSize);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]); 
          nhbors_cnt = nhbors_cnt + 1;
          coords = getCellCoords(id-XSize+1);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]); 
          nhbors_cnt = nhbors_cnt + 1;
      }
   }
   // is id on the X boundary ignoring corners
   if ((0 < id < XSize-1) || (((XSize -1)*XSize) < id  < ((XSize*XSize)-1))) 
   { 
       if (0 < id < XSize -1)
       {
          coords = getCellCoords(id-1);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]);
          nhbors_cnt = nhbors_cnt + 1;
          coords = getCellCoords(id+XSize);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]);
          nhbors_cnt = nhbors_cnt + 1;
          coords = getCellCoords(id+XSize+1);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]);
          nhbors_cnt = nhbors_cnt + 1;
          coords = getCellCoords(id+XSize -1);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]);
          nhbors_cnt = nhbors_cnt + 1;
          coords = getCellCoords(id+1);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]);
          nhbors_cnt = nhbors_cnt + 1;

       }		
       if (((XSize -1)*XSize) < id  < ((XSize*XSize)-1))
       {
	  coords = getCellCoords(id-1);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]);
          nhbors_cnt = nhbors_cnt + 1;
          coords = getCellCoords(id-XSize);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]);
          nhbors_cnt = nhbors_cnt + 1;
          coords = getCellCoords(id-XSize+1);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]);
          nhbors_cnt = nhbors_cnt + 1;
          coords = getCellCoords(id-XSize -1);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]);
          nhbors_cnt = nhbors_cnt + 1;
          coords = getCellCoords(id+1);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]);
          nhbors_cnt = nhbors_cnt + 1;

       }
   }
   // is id on Y boundary ignoring corners
   if ((id % XSize == 0) || (id % XSize == XSize -1)) 
   {
        if (id %XSize == 0)
       {
          coords = getCellCoords(id+1);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]);
          nhbors_cnt = nhbors_cnt + 1;
          coords = getCellCoords(id+XSize);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]);
          nhbors_cnt = nhbors_cnt + 1;
          coords = getCellCoords(id+XSize+1);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]);
          nhbors_cnt = nhbors_cnt + 1;
          coords = getCellCoords(id-XSize);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]);
          nhbors_cnt = nhbors_cnt + 1;
          coords = getCellCoords(id-XSize+1);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]);
          nhbors_cnt = nhbors_cnt + 1;

       }		
       if ( id % XSize  == XSize -1)
       {
	  coords = getCellCoords(id-1);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]);
          nhbors_cnt = nhbors_cnt + 1;
          coords = getCellCoords(id-XSize);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]);
          nhbors_cnt = nhbors_cnt + 1;
          coords = getCellCoords(id-XSize-1);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]);
          nhbors_cnt = nhbors_cnt + 1;
          coords = getCellCoords(id+XSize -1);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]);
          nhbors_cnt = nhbors_cnt + 1;
          coords = getCellCoords(id+XSize);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]);
          nhbors_cnt = nhbors_cnt + 1;

       }
   }
   // or is id internal; numnhbrs = 8
   for (int i = 0; i < 8; i++)
   {
	  coords = getCellCoords(id-1);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]);
          nhbors_cnt = nhbors_cnt + 1;
          coords = getCellCoords(id-XSize);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]);
          nhbors_cnt = nhbors_cnt + 1;
          coords = getCellCoords(id+XSize);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]);
          nhbors_cnt = nhbors_cnt + 1;
          coords = getCellCoords(id-XSize-1);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]);
          nhbors_cnt = nhbors_cnt + 1;
          coords = getCellCoords(id+XSize -1);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]);
          nhbors_cnt = nhbors_cnt + 1;
	  coords = getCellCoords(id+1);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]);
          nhbors_cnt = nhbors_cnt + 1;
          coords = getCellCoords(id-XSize+1);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]);
          nhbors_cnt = nhbors_cnt + 1;
          coords = getCellCoords(id+XSize+1);
          N[nhbors_cnt] = Cell(coords[0],coords[1],coords[2],coords[3]);
          nhbors_cnt = nhbors_cnt + 1;
   }
}
int *add(int *a, int b)
{
 

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


  //AFJoin implementation
  clock_t begin = clock();
  std::queue<int> seed_list
  std::queue<int> N;	 
  /*for (int i = 0; i < XSize; i++)
  {
  for (int j = XSize*i; j < ((i*XSize)+XSize); j++)
  {*/
     //for a given cell of T_d find if it intersects
     Cell search_rect(T[0].min[0],T[0].min[1],T[0].max[0],T[0].max[1]);
     int seed = STree.Search(search_rect.min, search_rect.max, MySearchCallback, NULL);
     seed_list.push(seed);
     if (seed >= 0)
     {
	//find Nhbrs of id
	N = Neighbors(seed); 
	while (N.size() > 0)
	{
	   int id = N.pop();
	   int *coords = getCellCoords(id);
	   Cell temp(coords[0],coords[1],coords[2],coords[3]);
	   if (Overlap(search_rect,temp))
	   {
		seed_list.push(id);
		tmp = Neighbors(id);
		for (int k = 0; k < tmp.size(); k++)
		  N.push(tmp.pop());		
	   }
	}
     }	
  /*}
  }*/

  
  clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  
  cout << "Search resulted in " << intersects << " intersections and took time " << elapsed_secs << "\n";
}

