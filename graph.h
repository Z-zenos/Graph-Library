#ifndef graph_h
#define graph_h

#define INFINITIVE_VALUE 10000000

#include "jrb.h"
#include <stdbool.h>

typedef struct _detailVertex
{
	char *name;
	JRB indegreeTree;
} *detailVertex;


typedef struct _Graph
{
	bool hasWeight;
	bool isDirected;
	int V;
	int E;
	int max;
	bool *exist;
	char f_name[50];
	FILE *gd;
	int cf;					//cf for counting number of calling drawGraph function
	JRB edges;
	JRB vertices;
} *Graph;

Graph  createGraph(bool hasWeight, bool isDirected);						// Create normal graph
void   addVertex(Graph graph, int id, char* name);							// Add a vertex to graph		
char   *getVertex(Graph graph, int id);										// Get name of vertex
void   addEdge(Graph graph, int v1, int v2, double weight);					// Add edge has weighted
void   addEdge_uw(Graph graph, int v1, int v2);								// Add edge unweighted
double getEdgeValue(Graph graph, int v1, int v2);							// Get weight. Neu la unweighted graph thi return 0
int    hasEdge(Graph graph, int v1, int v2);								// Chech edge
void   deleteVertex(Graph graph, int id);									// Delete a vertex
void   deleteEdge(Graph graph, int v1, int v2);								// Delete a edge
int    indegree(Graph graph, int v, int* output);							// Danh sach va so cac dinh ke co canh den mot dinh v
int	   outdegree(Graph graph, int v, int* output);							// Danh sach va so cac dinh ke co canh den tu dinh v

extern void createRandomGraph(Graph graph, int V);					// Create random graph
extern bool isCycle(Graph graph);									// Check graph has cycle or not. Use for directed or undirected graph
extern bool isNegativeCycle(Graph graph);							
extern void DFS(Graph graph, int src, int dest);					//Depth First Search
extern void BFS(Graph graph, int src, int dest, int *dist);			//Breadth First Search
extern void printDist(Graph graph, int *dist, int n);							
extern void Dijkstra(Graph graph, int src, int dest, int *dist);	// Dijkstra Shortest Path Algorithm
extern void MST(Graph graph);										// Minimum Spanning Tree
extern void Topological(Graph graph);								// Topological Sort
extern void SCC(Graph graph);										// Strongly Connected Component
extern void Color(Graph graph);										// Coloring Algorithm
extern int  MBM(Graph graph, int M, int N);							// Maximum Bipartite Matching
extern void drawGraph(Graph graph);									// Export picture for graph, use for calling functions
extern void displayGraph(Graph graph);								// Export picture for graph, use for normal graph
extern void dropGraph(Graph graph);									// Drop Graph

#endif