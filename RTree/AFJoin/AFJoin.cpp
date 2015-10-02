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
//#define XSize 10

#define TBegin 10

using namespace std;

typedef int ValueType;
int XSize = 10;

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

bool findList(int id, std::deque<int> N, std::deque<int> intersection_list, std::deque<int> visited_list)
{
	if (!findID(N,id) && !findID(intersection_list,id) && !findID(viisted_list,id))
		return true;
	else 
		return false;
}

//determine neighbors
void Neighbors(int id, std::deque<int> intersection_list,std::deque<int> visited_list)
{
   // is id a corner
   if ((id == 0) || (id == XSize-1) || (id == (XSize* XSize) -1) || (id == (XSize-1) * XSize) )
   {
      if (id == 0)
      {
	  if (!findID(N,id+1) && !findID(intersection_list,id+1))
             N.push_back(id+1);
	  if (!findID(N,id+XSize) && !findID(intersection_list,id+XSize))
             N.push_back(id+XSize);
	  if (!findID(N,id+XSize+1) && !findID(intersection_list,id+XSize+1))
             N.push_back(id+XSize+1);
	  return;
      }
      if (id == XSize -1)
      {
	  if (!findID(N,id-1) && !findID(intersection_list,id-1))
             N.push_back(id-1);
	  if (!findID(N,id+XSize) && !findID(intersection_list,id+XSize))
             N.push_back(id+XSize);
	  if (!findID(N,id+XSize-1) && !findID(intersection_list,id+XSize-1))
             N.push_back(id+XSize-1);
	  return;
      }
      if (id == (XSize*XSize)-1)
      {
	  if (!findID(N,id-1) && !findID(intersection_list,id-1))
             N.push_back(id-1);
	  if (!findID(N,id-XSize) && !findID(intersection_list,id-XSize))
             N.push_back(id-XSize);
	  if (!findID(N,id-XSize-1) && !findID(intersection_list,id-XSize-1))
             N.push_back(id-XSize-1);
	  return;
      }
       if (id == (XSize-1)*XSize)
      {
	  if (!findID(N,id+1) && !findID(intersection_list,id+1))
             N.push_back(id+1);
	  if (!findID(N,id-XSize) && !findID(intersection_list,id-XSize))
             N.push_back(id-XSize);
	  if (!findID(N,id-XSize+1) && !findID(intersection_list,id-XSize+1))
             N.push_back(id-XSize+1);
	  return;
      }
   }
   // is id on the X boundary ignoring corners
   if ((0 < id && id < (XSize-1)) || (((XSize -1)*XSize) < id  && id < ((XSize*XSize)-1))) 
   { 
       if (0 < id && id < (XSize -1))
       {
	  if (!findID(N,id-1) && !findID(intersection_list,id-1))
             N.push_back(id-1);
	  if (!findID(N,id+XSize) && !findID(intersection_list,id+XSize))
             N.push_back(id+XSize);
	  if (!findID(N,id+XSize+1) && !findID(intersection_list,id+XSize+1))
             N.push_back(id+XSize+1);
	  if (!findID(N,id+XSize-1) && !findID(intersection_list,id+XSize-1))
             N.push_back(id+XSize -1);
	  if (!findID(N,id+1) && !findID(intersection_list,id+1))
             N.push_back(id+1);
          return;
       }		
       if (((XSize -1)*XSize) < id  && id < ((XSize*XSize)-1))
       {
	  if (!findID(N,id-1) && !findID(intersection_list,id-1))
	     N.push_back(id-1);
	  if (!findID(N,id-XSize) && !findID(intersection_list,id-XSize))
             N.push_back(id-XSize);
	  if (!findID(N,id-XSize+1) && !findID(intersection_list,id-XSize+1))
             N.push_back(id-XSize+1);
	  if (!findID(N,id-XSize-1) && !findID(intersection_list,id-XSize-1))
             N.push_back(id-XSize -1);
	  if (!findID(N,id+1) && !findID(intersection_list,id+1))
             N.push_back(id+1);
	  return;
       }
   }
   // is id on Y boundary ignoring corners
   if ((id % XSize == 0) || (id % XSize == XSize -1)) 
   {
        if (id %XSize == 0)
       {
	  if (!findID(N,id+1) && !findID(intersection_list,id+1))
             N.push_back(id+1);
	  if (!findID(N,id+XSize) && !findID(intersection_list,id+XSize))
             N.push_back(id+XSize);
	  if (!findID(N,id+XSize+1) && !findID(intersection_list,id+XSize+1))
             N.push_back(id+XSize+1);
	  if (!findID(N,id-XSize) && !findID(intersection_list,id-XSize))
             N.push_back(id-XSize);
	  if (!findID(N,id-XSize+1) && !findID(intersection_list,id-XSize+1))
             N.push_back(id-XSize+1);
	  return;
       }		
       if ( id % XSize  == XSize -1)
       {
	  if (!findID(N,id-1) && !findID(intersection_list,id-1))
	     N.push_back(id-1);
	  if (!findID(N,id-XSize) && !findID(intersection_list,id-XSize))
             N.push_back(id-XSize);
	  if (!findID(N,id-XSize-1) && !findID(intersection_list,id-XSize-1))
             N.push_back(id-XSize-1);
	  if (!findID(N,id+XSize-1) && !findID(intersection_list,id+XSize-1))
             N.push_back(id+XSize -1);
	  if (!findID(N,id+XSize) && !findID(intersection_list,id+XSize))
             N.push_back(id+XSize);
	  return;
       }
   }
   // or is id internal; numnhbrs = 8
   for (int i = 0; i < 8; i++)
   {
	  if (!findID(N,id-1) && !findID(intersection_list,id-1))
	     N.push_back(id-1);
	  if (!findID(N,id+1) && !findID(intersection_list,id+1))
	     N.push_back(id+1);
	  if (!findID(N,id-XSize) && !findID(intersection_list,id-XSize))
             N.push_back(id-XSize);
	  if (!findID(N,id-XSize-1) && !findID(intersection_list,id-XSize-1))
             N.push_back(id-XSize-1);
	  if (!findID(N,id-XSize+1) && !findID(intersection_list,id-XSize+1))
             N.push_back(id-XSize+1);
	  if (!findID(N,id+XSize) && !findID(intersection_list,id+XSize))
             N.push_back(id+XSize);
	  if (!findID(N,id+XSize-1) && !findID(intersection_list,id+XSize-1))
             N.push_back(id+XSize-1);
	  if (!findID(N,id+XSize+1) && !findID(intersection_list,id+XSize+1))
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
     int next_element = 0; 
     //for a given cell of T_d findID if it intersects
     Cell search_rect;
	 search_rect  = Cell(T[next_element].min[0],T[next_element].min[1],T[next_element].max[0],T[next_element].max[1]);
     int seed = STree.Search(search_rect.min, search_rect.max, MySearchCallback, NULL);
     cout << "start seed " << seed << "\n";

     
     if (seed >= 0)
     { 
	while (next_element < MeshSize)
	{	
     
        std::queue<int> N_visited;
     	std::deque<int> intersection_list;
		intersection_list.push_back(seed);
			
		cout << "Donor element is " << next_element << "\n";
		//findID Nhbrs of seed
		Neighbors(seed,intersection_list,N_visited);
		while (!N.empty())
		{
		   int id = N.front();
		   cout << "count: " << N.size() << "seed " << seed << " nid " << id << "\n";
		   N.pop_front();
		   N_visited.push_back(id);
		   int *coords = getCellCoords(id);
		   Cell temp(coords[0],coords[1],coords[2],coords[3]);
		   if (Overlap(&search_rect,&temp))
		   {
			std::deque<int>::iterator it = std::find(intersection_list.begin(),intersection_list.end(),id); 
			if (it == intersection_list.end())	
				intersection_list.push_back(id);

			Neighbors(id,intersection_list,N_visted);
		   }
		}
		//get next T_d element
		next_element = next_element + 1;
		int *coords = getCellCoords(next_element);
		search_rect = Cell(coords[0],coords[1],coords[2],coords[3]); 
		for (std::deque<int>::iterator it = intersection_list.begin(); it != intersection_list.end(); ++it)
		{
			//std::cout << ' ' << *it;
			coords = getCellCoords(*it);
			Cell temp(coords[0],coords[1],coords[2],coords[3]);
			if (Overlap(&search_rect,&temp))
			{
			   seed = *it;
			   //cout << "New seed is " << seed << "\n";
			   break;
			}
		}
      }	
      }
  

  
  clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  
  cout << "Search resulted in " << intersects << " intersections and took time " << elapsed_secs << "\n";
}

