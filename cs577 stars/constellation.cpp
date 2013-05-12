#include <iostream>
#include <vector>
#include <queue>

#include "EasyBMP.h"
#include "EasyBMP_Geometry.h"
#include "EasyBMP_Font.h"

using namespace std;


const int MAX_EDGE = 100;
const int MAX_NODE = 100;


typedef struct{
	int pixel_height;
	int pixel_width;
	char *name;
}Pictureinfo;

typedef struct{
	int v;
	int t;
	int weight;
	bool isMST;
}Edge;

typedef struct{
	double position_width, position_height;
	int region_width, region_height;
	int father;
	int intensity;
	int mst_father;

}Node;


bool operator<(const Edge& a, const Edge& b) {
	return a.weight > b.weight;
}

priority_queue <Edge> edges;
priority_queue <Edge> finaledges;
Node nodes[MAX_NODE];
Pictureinfo pic;

int edgeNum;
int nodeNum;

/*
 Quicksort
*/
int exchange(Edge *a,Edge *b)
{
 Edge t;
 t = *a;
 *a = *b;
 *b = t;
 return 0;
}
int partition(Edge*edges,int p,int r)
{
  int i = p-1,j = p;
  
  for(;j<r;j++)
  {
   if(edges[j].weight <= edges[r].weight)
   {
    i++;
    exchange(edges+i,edges+j);
   }
  }
  exchange(&edges[i+1],&edges[r]);
  return i+1;
}
int quick_sort(Edge edges[],int p,int r)
{
 if(p < r)
 {
  int q = partition(edges,p,r);
  quick_sort(edges,p,q-1);
  quick_sort(edges,q+1,r);
 }
 return 0;
}


int find(int x)
{
	//if(father[x]!=x) father[x]=find(father[x]);
	if (nodes[x].father != x)
		nodes[x].father = find(nodes[x].father);
	return nodes[x].father;
}


void MST_Kruskal()
{
	 while (!edges.empty())
    {
        Edge CurrentEdge = edges.top();
		int a, b;

		a = find(CurrentEdge.t);
		b = find(CurrentEdge.v);
		if ( a != b) 
			{
				CurrentEdge.isMST = true;
				nodes[a].father = b;
				finaledges.push(CurrentEdge);
			}
        edges.pop();
    }

}

void Inputdata()
{
 
}

void GenerateConstellation(int widthnum, int heightnum){
	int regionwidth = pic.pixel_width / widthnum;
	int regionheight = pic.pixel_height / heightnum;
	int i,j;
	for (i = 0; i< nodeNum; i++)
	{
		nodes[i].region_height = nodes[i].position_height / heightnum ;
		nodes[i].region_width = nodes[i].position_width / widthnum;
		nodes[i].father = i;
	}

	
	for (i = 0; i< nodeNum; i++)
		for (j = i+1; j<nodeNum; j++)
		{
			int dif_width = abs(nodes[i].region_width - nodes[j].region_width);
			int dif_height = abs(nodes[i].region_height - nodes[j].region_height);
			if  (dif_width <= 1 || dif_height <= 1)
			{
				int distance = sqrt((nodes[i].position_height - nodes[j].position_height)*(nodes[i].position_height - nodes[j].position_height)+(nodes[i].position_width - nodes[j].position_width)*(nodes[i].position_width - nodes[j].position_width));
				// not consider star priority temporarily.
				//int intensity_average = (nodes[i].intensity + nodes[j].intensity) / 2;
				//int a,b;
				//a = find(i);
				//b = find(j);
				//nodes[a].father = b;
				Edge temp;
				temp.t = i;
				temp.v = j;
				temp.weight = distance;
				temp.isMST = false;
				edges.push(Edge(temp));
			}
		}

	MST_Kruskal();
	


}

void OutputImage()
{
	BMP AnImage;
	AnImage.ReadFromFile("sample.bmp");
	RGBApixel FontColor;
	FontColor.Red = 255; FontColor.Green = 0; FontColor.Blue = 0;
	 while (!finaledges.empty())
    {
        Edge CurrentEdge = finaledges.top();
		DrawLine(AnImage, nodes[CurrentEdge.t].position_width, nodes[CurrentEdge.t].position_height, nodes[CurrentEdge.v].position_width, nodes[CurrentEdge.v].position_height, FontColor);
        edges.pop();
    }
	AnImage.WriteToFile("copied.bmp");
}

int main()
{
	Inputdata();
	GenerateConstellation( 10, 10);
	OutputImage();
	system("pause");
	return 0;
}
