#include <stdio.h>
#include "graph.h"

int main()
{   
    /*
    int dist[10];
    Graph graph = createGraph(0, 0);
    createRandomGraph(graph, 10);
    //BFS(graph, 0, -1, dist);
    */
    Graph graph = createGraph(1, 0);
    addVertex(graph, 0, "Stadium");
    addVertex(graph, 1, "UTD");
    addVertex(graph, 2, "Gas_Station");
    addVertex(graph, 3, "Home");
    addVertex(graph, 4, "Library");
    addVertex(graph, 5, "Post_Office");
    addVertex(graph, 6, "City_Park");
    addVertex(graph, 7, "Grocery");
    addVertex(graph, 8, "Restaurant");

    addEdge(graph, 0, 1, 10);
    addEdge(graph, 0, 2, 10);
    addEdge(graph, 1, 3, 20);
    addEdge(graph, 1, 4, 4);
    addEdge(graph, 1, 5, 5);
    addEdge(graph, 2, 3, 6);
    addEdge(graph, 2, 6, 3);
    addEdge(graph, 3, 6, 5);
    addEdge(graph, 3, 8, 15);
    addEdge(graph, 4, 5, 1);
    addEdge(graph, 4, 8, 3);
    addEdge(graph, 5, 8, 4);
    addEdge(graph, 6, 7, 2);
    addEdge(graph, 7, 8, 18);
    
    //Ok co the doan tao do thi hoi lau mot chut
    //Bay gio ban goi ham MST va drawGraph de co cai nhin ro hon ve do thi
    
    //deleteVertex(graph, 4);
    //printf("%s\n", isCycle(graph) ? "Hey , here has a cycle" : "Not detect");
    //BFS(graph, 0, -1, dist);
    //DFS(graph, 0, -1);
    //Topological(graph);
    MST(graph);
    //Dijkstra(graph, 0, -1, dist);
    //SCC(graph);
    //Color(graph);
    drawGraph(graph);
    //displayGraph(graph);
    
    dropGraph(graph);
    return 0;
}