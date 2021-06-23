# GRAPH Library

> Follow your passion & Success will follow you.
---

## Table of Contents

- [Description](#description)
- [Usage](#usage)
- [Note](#note)
- [Technologies](#technologies)
- [Author Info](#author-info)

---

## Description

The library has many popular algorithms when working with graphs such as:

- [BFS](https://en.wikipedia.org/wiki/Breadth-first_search)
- [Dijkstra](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm)
- [MST](https://en.wikipedia.org/wiki/Minimum_spanning_tree)
- [Coloring](https://en.wikipedia.org/wiki/Graph_coloring)
- and more

The simple thing we need to do is declare functions that are suitable for the purpose of the problem. Besides, the library that supports creating graph images with DOT Language will give us a more intuitive view.

---

## Usage

- Declare

To use this library, add the following header:

```C
#include "graph.h"
```

- Building

To create a graph for your program, definition:

```C
Graph graph = createGraph(hasWeight, isDirected);
```

`hasWeight` is weighted or not
`isDirected` indicates whether this is a directed or undirected graph.
For example:

```C
graph = createGraph(0, 1);
```

i.e. this graph is unweighted and directed

To add an edge between two vertices, you must add that vertex to the graph first through:

```C
addVertex(graph, id, "vertex name");   
```

`vertex name` is at your discretion can be numbers or names of people, trees, cities, ... as long as the two different ids are not named the same

Then you add the edge between the 2 vertices:

```C
addEdge(graph, v1, v2, weight);
```

or for unweighted graphs:

```C
addEdge_uw(graph, v1, v2);
```

- Example

For example, when you want to solve a problem related to [Minimum Spanning Tree Algorithm](https://en.wikipedia.org/wiki/Minimum_spanning_tree) such as:

![MST Picture](https://d2vlcm61l7u1fs.cloudfront.net/media%2F940%2F94099b15-c56f-4e47-be98-33840ef79a4e%2FphpWVn1g3.png)

```C
#include <stdio.h>
#include "graph.h"

int main()
{
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

    // Ok, it might take a little while to create the graph
    // Now you call the MST and drawGraph functions to get a better view
    MST(graph);
    drawGraph(graph);
    
    dropGraph(graph);
    return 0;
}
```

- Compile

First:

```C
gcc -g -c your_name_file.c graph.c

```

Second (Note: 3 files graph.o pqueue.o libfdr.a are required):

```C
gcc -g -o your_name_file your_name_file.o graph.o pqueue.o libfdr.a

```

Finally, run your program:

```C
./your_name_file
```

- Result

File .dot :

```DOT
graph GraphViz
{
 ...
 Stadium [fillcolor = "#00ffff", style = filled];
 UTD [fillcolor = "#00ffff", style = filled];
    ...
 City_Park -- Grocery [style = bold, color = "red", label = "2.00"];
 Grocery -- Restaurant [label = "18.00"];
}
```

and picture:

<img src = "https://raw.githubusercontent.com/Z-techIT/Graph-Library/master/GraphViz000.png" width = "700" height = "500"/>

---

## Note

The library only supports creating images for some functions such as MST, Color, Topological and normal graph.

---

## Technologies

The library is created with:

- C
- DOT

---

## Compatibility

Graph Library has been tested in Linus OS.

---

## Author

- Facebook

>[Hoang Anh Tuan](https://www.facebook.com/profile.php?id=100024788042491)

> [Le Trong Nghia](https://www.facebook.com/Wer.bin.ich.229)

>[Le Viet Hung](https://www.facebook.com/viethung2209)

- [Youtube](https://www.youtube.com/channel/UCJK1BtLEo5dA5OuhqiZRgKA)

---

## Lisence & copyright

(C) Hoang Anh Tuan - HANOI UNIVERSITY OF SCIENCE AND TECHNOLOGY
