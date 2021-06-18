#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "graph.h"
#include "graph.c"

int main()
{
    
    Graph g = createGraph(0, 0);
    addVertex(g, 0, "0");
    addVertex(g, 1, "1");
    addVertex(g, 2, "2");
    addVertex(g, 3, "3");
    addVertex(g, 4, "4");
    addVertex(g, 5, "5");
    addVertex(g, 6, "6");
    addVertex(g, 7, "7");
    addVertex(g, 8, "8");
    //addVertex(g, 9, "9");
    //addVertex(g, 10, "10");

    addEdge(g, 0, 1, 22);
    addEdge(g, 0, 2, 9);
    addEdge(g, 0, 3, 12);
    addEdge(g, 1, 2, 25);
    addEdge(g, 1, 5, 7);
    addEdge(g, 1, 7, 34);
    addEdge(g, 2, 3, 4);
    addEdge(g, 2, 4, 65);
    addEdge(g, 2, 5, 42);
    addEdge(g, 3, 4, 13);
    addEdge(g, 3, 8, 30);
    addEdge(g, 4, 5, 18);
    addEdge(g, 4, 6, 23);
    addEdge(g, 5, 6, 2);
    addEdge(g, 5, 7, 10);
    addEdge(g, 6, 7, 8);
    addEdge(g, 6, 8, 21);
    addEdge(g, 7, 8, 19);
    //addEdge(g, 9, 10, 10);
    

    int *dist = (int*)malloc(sizeof(int) * g->V);
    //BFS(g, 3, 2, dist);
    //if(dist[2] != -1)
    //    printf("Exist path between 3 and 2\n");
    //else printf("Not found path\n");
    //Khong co cach nao de khong ghi de du lieu trong file 
    Dijkstra(g, 0, 5, dist);
    drawGraph(g);
    //DFS(g, 0, 8);
    MST(g);
    drawGraph(g);
    //Topological(g);
    //drawGraph(g);    

    //printf("%s", isCycle(g) ? "Graph has cycle\n" : "Graph hasn't cycle\n");
    /*
    Graph graph = createGraph(0, 1);
    addVertex(graph, 0, "0");
    addVertex(graph, 1, "1");
    addVertex(graph, 2, "2");
    addVertex(graph, 3, "3");
    addVertex(graph, 4, "4");

    addEdge(graph, 1, 0, 0);
    addEdge(graph, 0, 2, 0);
    addEdge(graph, 2, 1, 0);
    addEdge(graph, 0, 3, 0);
    addEdge(graph, 3, 4, 0);
    SCC(graph);
    */
    //Color(g);
    //drawGraph(g);
    free(dist);
    dropGraph(g);
    return 0;
}