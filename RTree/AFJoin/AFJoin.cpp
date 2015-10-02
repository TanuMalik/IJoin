//
// Test.cpp
//
// This is a direct port of the C version of the RTree test program.
//

#include <iostream>
#include <ctime>
#include <deque>
#include <algorithm>
#include "RTree.h"

#define MeshSize 100
#define XSize 10

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
std::deque<int> N;
std::deque<int> seed_list;

int intersects;
int nhbors_cnt = 0;
//Cell search_rect(6, 4, 9, 6); // search will findID above rects that this one overlaps
//Cell search_rect(0, 0, 1, 1); // search will findID above rects that this one overlaps


int MySearchCallback(ValueType id, void* arg)
{
  cout << "Hit data rect " << id << "\n";
  //return true;// keep going
  return (int)id; 
}

bool Overlap(Cell* a, Cell *b)
{
  int NUMDIMS = 2;
  for(int index=0; index < NUMDIMS; ++index)
  {
    if (a->min[index] > b->max[index] ||
        b->min[index] > a->max[index])
    {
      return false;
    }
  }
  return true;
}

int* getCellCoords(int id)
{
   int *coords;
   coords = (int *)malloc(sizeof(int) *4);
   coords[0] = id % XSize;
   coords[1] = id / XSize;
   coords[2] = coords[0] + 1;
   coords[3] = coords[1] + 1;
   return coords;
}


bool findID(std::deque<int>L,int id)
{
    std::deque<int>::iterator it;
    it = std::find(L.begin(),L.end(),id);
    if (it == L.end())
	return false;
    else
	return true;
}

//determine neighbors
void Neighbors(int id)
{
   // is id a corner
   if ((id == 0) || (id == XSize-1) || (id == (XSize* XSize) -1) || (id == (XSize-1) * XSize) )
   {
      if (id == 0)
      {
	  if (!findID(N,id+1) && !findID(seed_list,id+1))
             N.push_back(id+1);
	  if (!findID(N,id+XSize) && !findID(seed_list,id+XSize))
             N.push_back(id+XSize);
	  if (!findID(N,id+XSize+1) && !findID(seed_list,id+XSize+1))
             N.push_back(id+XSize+1);
	  return;
      }
      if (id == XSize -1)
      {
	  if (!findID(N,id-1) && !findID(seed_list,id-1))
             N.push_back(id-1);
	  if (!findID(N,id+XSize) && !findID(seed_list,id+XSize))
             N.push_back(id+XSize);
	  if (!findID(N,id+XSize-1) && !findID(seed_list,id+XSize-1))
             N.push_back(id+XSize-1);
	  return;
      }
      if (id == (XSize*XSize)-1)
      {
	  if (!findID(N,id-1) && !findID(seed_list,id-1))
             N.push_back(id-1);
	  if (!findID(N,id-XSize) && !findID(seed_list,id-XSize))
             N.push_back(id-XSize);
	  if (!findID(N,id-XSize-1) && !findID(seed_list,id-XSize-1))
             N.push_back(id-XSize-1);
	  return;
      }
       if (id == (XSize-1)*XSize)
      {
	  if (!findID(N,id+1) && !findID(seed_list,id+1))
             N.push_back(id+1);
	  if (!findID(N,id-XSize) && !findID(seed_list,id-XSize))
             N.push_back(id-XSize);
	  if (!findID(N,id-XSize+1) && !findID(seed_list,id-XSize+1))
             N.push_back(id-XSize+1);
	  return;
      }
   }
   // is id on the X boundary ignoring corners
   if ((0 < id < (XSize-1)) || (((XSize -1)*XSize) < id  < ((XSize*XSize)-1))) 
   { 
       if (0 < id < (XSize -1))
       {
	  if (!findID(N,id-1) && !findID(seed_list,id-1))
             N.push_back(id-1);
	  if (!findID(N,id+XSize) && !findID(seed_list,id+XSize))
             N.push_back(id+XSize);
	  if (!findID(N,id+XSize+1) && !findID(seed_list,id+XSize+1))
             N.push_back(id+XSize+1);
	  if (!findID(N,id+XSize-1) && !findID(seed_list,id+XSize-1))
             N.push_back(id+XSize -1);
	  if (!findID(N,id+1) && !findID(seed_list,id+1))
             N.push_back(id+1);
          return;
       }		
       if (((XSize -1)*XSize) < id  < ((XSize*XSize)-1))
       {
	  if (!findID(N,id-1) && !findID(seed_list,id-1))
	     N.push_back(id-1);
	  if (!findID(N,id-XSize) && !findID(seed_list,id-XSize))
             N.push_back(id-XSize);
	  if (!findID(N,id-XSize+1) && !findID(seed_list,id-XSize+1))
             N.push_back(id-XSize+1);
	  if (!findID(N,id-XSize-1) && !findID(seed_list,id-XSize-1))
             N.push_back(id-XSize -1);
	  if (!findID(N,id+1) && !findID(seed_list,id+1))
             N.push_back(id+1);
	  return;
       }
   }
   // is id on Y boundary ignoring corners
   if ((id % XSize == 0) || (id % XSize == XSize -1)) 
   {
        if (id %XSize == 0)
       {
	  if (!findID(N,id+1) && !findID(seed_list,id+1))
             N.push_back(id+1);
	  if (!findID(N,id+XSize) && !findID(seed_list,id+XSize))
             N.push_back(id+XSize);
	  if (!findID(N,id+XSize+1) && !findID(seed_list,id+XSize+1))
             N.push_back(id+XSize+1);
	  if (!findID(N,id-XSize) && !findID(seed_list,id-XSize))
             N.push_back(id-XSize);
	  if (!findID(N,id-XSize+1) && !findID(seed_list,id-XSize+1))
             N.push_back(id-XSize+1);
	  return;
       }		
       if ( id % XSize  == XSize -1)
       {
	  if (!findID(N,id-1) && !findID(seed_list,id-1))
	     N.push_back(id-1);
	  if (!findID(N,id-XSize) && !findID(seed_list,id-XSize))
             N.push_back(id-XSize);
	  if (!findID(N,id-XSize-1) && !findID(seed_list,id-XSize-1))
             N.push_back(id-XSize-1);
	  if (!findID(N,id+XSize-1) && !findID(seed_list,id+XSize-1))
             N.push_back(id+XSize -1);
	  if (!findID(N,id+XSize) && !findID(seed_list,id+XSize))
             N.push_back(id+XSize);
	  return;
       }
   }
   // or is id internal; numnhbrs = 8
   for (int i = 0; i < 8; i++)
   {
	  if (!findID(N,id-1) && !findID(seed_list,id-1))
	     N.push_back(id-1);
	  if (!findID(N,id+1) && !findID(seed_list,id+1))
	     N.push_back(id+1);
	  if (!findID(N,id-XSize) && !findID(seed_list,id-XSize))
             N.push_back(id-XSize);
	  if (!findID(N,id-XSize-1) && !findID(seed_list,id-XSize-1))
             N.push_back(id-XSize-1);
	  if (!findID(N,id-XSize+1) && !findID(seed_list,id-XSize+1))
             N.push_back(id-XSize+1);
	  if (!findID(N,id+XSize) && !findID(seed_list,id+XSize))
             N.push_back(id+XSize);
	  if (!findID(N,id+XSize-1) && !findID(seed_list,id+XSize-1))
             N.push_back(id+XSize-1);
	  if (!findID(N,id+XSize+1) && !findID(seed_list,id+XSize+1))
             N.push_back(id+XSize+1);
	  return;
   }
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
  /*for (int i = 0; i < XSize; i++)
  {
  for (int j = XSize*i; j < ((i*XSize)+XSize); j++)
  {*/
     //for a given cell of T_d findID if it intersects
     Cell search_rect(T[0].min[0],T[0].min[1],T[0].max[0],T[0].max[1]);
     int seed = STree.Search(search_rect.min, search_rect.max, MySearchCallback, NULL);
     cout << "start seed " << seed << "\n";

     std::deque<int>::iterator it = std::find(seed_list.begin(),seed_list.end(),seed);
     if (it == seed_list.end())	
     	seed_list.push_back(seed);

     if (seed >= 0)
     {
	//findID Nhbrs of id
	Neighbors(seed);
	while (!N.empty())
	{
	   int id = N.front();
	   cout << "count: " << N.size() << " id " << id << "\n";
	   N.pop_front();
	   int *coords = getCellCoords(id);
	   Cell temp(coords[0],coords[1],coords[2],coords[3]);
	   if (Overlap(&search_rect,&temp))
	   {
     		std::deque<int>::iterator it = std::find(seed_list.begin(),seed_list.end(),id); 
     		if (it == seed_list.end())	
     			seed_list.push_back(id);

		Neighbors(id);
	   }
	}
     }	
  /*}
  }*/

  
  clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  
  cout << "Search resulted in " << intersects << " intersections and took time " << elapsed_secs << "\n";
}

