#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>

#include "graph.h"
#include "dllist.h"
#include "pqueue.h"

#define INFINITIVE_VALUE 10000000
int flag;


Graph createGraph(bool hasWeight, bool isDirected)
{
	Graph g = (Graph)malloc(sizeof(struct _Graph));
	if(g == NULL)
	{
		printf("NULL, can not allocate memory, abort...\n");
		return NULL;
	}
	g->vertices = make_jrb();
	g->edges = make_jrb();
	g->hasWeight = hasWeight;
	g->isDirected = isDirected;
	g->exist = (bool*)calloc(true, sizeof(bool) * 10000);
	g->V = g->E = g->cf = g->max = 0;
	return g;
}

void addVertex(Graph graph, int id, char* name)
{
	JRB vertices = graph->vertices;
	JRB edges = graph->edges;
	JRB tree;
	
	//update vertex
	detailVertex iver = (detailVertex)malloc(sizeof(struct _detailVertex));
	if(iver == NULL) 
	{
	  	printf("NULL, can not allocate memory, abort...\n");
		return;
	}
	iver->name = strdup(name);
	iver->indegreeTree = make_jrb();
	jrb_insert_int(vertices, id, new_jval_v(iver));
	graph->V++;
	//update edge
	tree = make_jrb();
	jrb_insert_int(edges, id, new_jval_v(tree));
	graph->max++;
	graph->exist[id] = true;
}

char *getVertex(Graph graph, int id)
{
	JRB vnode = jrb_find_int(graph->vertices, id);
	if(vnode == NULL) 
		return NULL;
	detailVertex iver = (detailVertex)vnode->val.v;
	return iver->name;
}

void addEdge(Graph graph, int v1, int v2, double wei)
{
	JRB enode = jrb_find_int(graph->edges, v1);
	JRB vnode = jrb_find_int(graph->vertices, v2);
	JRB tree;
	if((enode == NULL) || (vnode == NULL)) 
	{
		printf("Vertex not found\n");
		return;
    }
	tree = (JRB)(enode->val).v;
	jrb_insert_int(tree, v2, new_jval_d(wei));
	detailVertex iver = (detailVertex)vnode->val.v;
	tree = iver->indegreeTree;
	jrb_insert_int(tree, v1, JNULL);
	graph->E++;
	if(!graph->isDirected)
	{
		enode = jrb_find_int(graph->edges, v2);
		vnode = jrb_find_int(graph->vertices, v1);
		if((enode == NULL) || (vnode == NULL)) 
		{
			printf("Vertex not found\n");
			return;
	    }
		tree = (JRB)(enode->val).v;
		jrb_insert_int(tree, v1, new_jval_d(wei));
		detailVertex iver = (detailVertex)vnode->val.v;
		tree = iver->indegreeTree;
		jrb_insert_int(tree, v2, JNULL);
	}
}

void addEdge_uw(Graph graph, int v1, int v2)
{
	addEdge(graph, v1, v2, 0);
	if(graph->isDirected)
		addEdge(graph, v2, v1, 0);
}

int hasEdge(Graph graph, int v1, int v2)
{
	JRB enode = jrb_find_int(graph->edges, v1);
	JRB tree;
	if(enode == NULL) 
	{
		printf("Vertex not found\n");
		return 0;
    }
    tree = (JRB)(enode->val).v;
	if(jrb_find_int(tree,v2) != NULL)
		return 1;
	else return 0;
}

double getEdgeValue(Graph graph, int v1, int v2)
{
	JRB enode = jrb_find_int(graph->edges, v1);
	JRB tree;

	if(enode == NULL) 
	{
		printf("Vertex not found\n");
		return INFINITIVE_VALUE;
    }
    tree = jrb_find_int((JRB)(enode->val).v, v2);

	if(tree == NULL)
		return INFINITIVE_VALUE;
	return (tree->val).d;
}

int indegree(Graph graph, int v, int* output)
{
	JRB vnode = jrb_find_int(graph->vertices, v);
	JRB tree, node;
	int innum = 0;
	
	if(vnode == NULL) 
	{
		printf("Vertex not found\n");
		return 0;
    }
    detailVertex iver = (detailVertex)vnode->val.v;
	tree = iver->indegreeTree;
	jrb_traverse(node, tree)
	{
		output[innum] = node->key.i;
		innum++;
	}
	return innum;
}

int outdegree(Graph graph, int v, int* output)
{
	JRB enode = jrb_find_int(graph->edges, v);
	JRB tree, node;
	int outnum = 0;
	
	if(enode == NULL) 
	{
		printf("Vertex not found\n");
		return 0;
    }

	tree = (JRB)(enode->val).v;
	jrb_traverse(node, tree)
	{
		output[outnum] = node->key.i;
		outnum++;
	}
	return outnum;
}

void deleteEdge(Graph graph, int v1, int v2)
{
	JRB enode = jrb_find_int(graph->edges, v1);
	JRB tree, find;
	if(!enode)
	{
		printf("Vertex not found\n");
		return;
	}
	tree = (JRB)(enode->val).v;
	find = jrb_find_int(tree, v2);
	if(find)
		jrb_delete_node(find);
}

void deleteEdgesRelated(Graph graph, int id)
{
	int V = graph->max, n, i;
	int adj[V];
	n = outdegree(graph, id, adj);
	for(i = 0; i < n; i++)
		deleteEdge(graph, id, adj[i]);
	n = indegree(graph, id, adj);
	for(i = 0; i < n; i++)
		deleteEdge(graph, adj[i], id);
}

void deleteVertex(Graph graph, int id)
{
	graph->exist[id] = 0;
	graph->V--;
	if(id == graph->V - 1)
		for(int i = id; i >= 0; i--)
			if(graph->exist[i])
			{
				graph->max = ++i;
				break;
			}
	deleteEdgesRelated(graph, id);
	JRB vnode = jrb_find_int(graph->vertices, id);
	JRB enode = jrb_find_int(graph->edges, id);
	if(!vnode || !enode)
	{
		printf("Vertex not found!\n");
		return;
	}

	jrb_delete_node(vnode);
	jrb_delete_node(enode);
}

char *strinc(const char *str, int d, int min_width){
    char wk[12];//12:max length of sizeof(int)=4
    char *p;
    int d_len, c = 8;
    d += strtol(&str[c], NULL, 10);
    if(d < 0) d = 0;
    d_len = sprintf(wk, "%0*d.dot", min_width, d);
    p = malloc((c + d_len + 1) * sizeof(char));
    strncpy(p, str, c);
    p[c] = '\0';
    return strcat(p, wk);
}

void buildGraph(Graph graph, int mode)	//1: color algo and 0: other algo
{
	char filename[sizeof "GraphViz100.dot"];
	sprintf(filename, "GraphViz%03d.dot", graph->cf);
	char *tmp = filename;
	int i;
	for(i = 0; i < 100; i++)
	{
		if(access(tmp, F_OK))
			break;
		tmp = strinc(filename, +i, 3);
	}
	strcpy(graph->f_name, tmp);
	graph->gd = fopen(graph->f_name, "w");
	if(!graph->gd)
	{
		printf("Error: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	if(graph->isDirected) 
		fprintf(graph->gd, "digraph GraphViz\n{\n\trankdir = LR;\n");//\tnode [shape = circle, width = 0.4];\n\tnodesep = 1.5;\n");
	else 
		fprintf(graph->gd, "graph GraphViz\n{\n\trankdir = LR;\n");//\tnode [shape = circle, width = 0.4];\n\tnodesep = 1.5;\n");
	if(!mode)
		for(int i = 0; i < graph->max; i++)
			if(graph->exist[i])
				fprintf(graph->gd, "\t%s [fillcolor = \"#00ffff\", style = filled];\n", getVertex(graph, i));
	flag = ftell(graph->gd);
	if(!i)
		return;
	free(tmp);
}

void createRandomGraph(Graph rg, int V)
{
	int u, v, E, i;
	double weight;
	char number[6];
	srand(time(NULL));
	for(i = 0; i < V; i++)
	{
		sprintf(number, "%d", i);
		addVertex(rg, i, number);
	}
	int maxE = V * (V - 1) / 2;
	E = V + rand() % (V + 1);
	for(i = 0; i < E; i++)
	{
		u = rand() % V;
		v = rand() % V;
		if(u == v || hasEdge(rg, u, v) || hasEdge(rg, v, u))
			continue;
		weight = rand() % 1001;
		if(rg->hasWeight)
			addEdge(rg, u, v, weight);
		else
			addEdge_uw(rg, u, v);
	}
	rg->E = E;
}

typedef struct subset
{
	int parent;
	int rank;
}subset;

bool _cyclicUtil(Graph graph, bool *recStack, bool visited[], int v)
{
	if(!visited[v])
	{
		visited[v] = true;
		recStack[v] = true;
		int V = graph->max, adj[V], u, n;
		n = outdegree(graph, v, adj);
		for(u = 0; u < n; u++)
		{
			if(!visited[adj[u]] && _cyclicUtil(graph, recStack, visited, adj[u]))
				return true;
			else if(recStack[adj[u]])
				return true;
		}
	}
	recStack[v] = false;
	return false;
}

int _find(subset subsets[], int i)
{
	if(subsets[i].parent != i)
		subsets[i].parent = _find(subsets, subsets[i].parent);
	return subsets[i].parent;
}

void _union(subset subsets[], int xroot, int yroot)
{
	if(subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if(subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;
    else 
	{
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
	}
}

bool isCycle(Graph graph)
{
	if(graph->isDirected)
	{
		int V = graph->max, i;
		bool visited[V], recStack[V];
		memset(visited, false, sizeof(visited));
		memset(recStack, false, sizeof(recStack));
		for(i = 0; i < V; i++)
			if(_cyclicUtil(graph, recStack, visited, i) && graph->exist[i])
				return true;
		return false;
	}
	else
	{
		int x, y, v, V = graph->max, n, i;
		int *adj = (int*)malloc(V * sizeof(int));
		subset *subsets = (subset*)malloc(V * sizeof(subset));
		bool visited[V];
		memset(visited, false, sizeof(visited));
		for(v = 0; v < V; v++)
		{
			subsets[v].parent = v;
			subsets[v].rank = 0;
		}

		for(v = 0; v < V; v++)
			if(graph->exist[v])
			{
				visited[v] = true;
				n = outdegree(graph, v, adj);
				for(i = 0; i < n; i++)
					if(!visited[adj[i]])
					{
						x = _find(subsets, v);
						y = _find(subsets, adj[i]);
						if(x == y)
							return true;
						_union(subsets, x, y);
					}
			}
		free(adj);
		return false;
	}
}

bool isNegCycleUtil(Graph graph, int src, int dist[])
{
	int V = graph->max, i, n, j, dest, v, weight;
	int *adj = (int*)malloc(V * sizeof(int));

	for(i = 0; i < V; i++)
		dist[i] = INFINITIVE_VALUE;
	dist[src] = true;

	for(v = 1; v <= V - 1; v++)
		for(i = 0; i < V; i++)
			if(graph->exist[i])
			{
				n = outdegree(graph, i, adj);
				for(j = 0; j < n; j++)
				{
					dest = adj[j];
					weight = getEdgeValue(graph, i, dest);
					if(dist[i] != INT_MAX && dist[i] + weight < dist[dest])
						dist[dest] = dist[i] + weight;
				}
			}
	for(i = 0; i < V; i++)
		if(graph->exist[i])
		{
			n = outdegree(graph, i, adj);
			for(j = 0; j < n; j++)
			{
				dest = adj[j];
				weight = getEdgeValue(graph, i, dest);
				if(dist[i] != INT_MAX && dist[i] + weight < dist[dest])
				{
					free(adj);
					return true;
				}
			}
		}	
	free(adj);
	return false;
}

bool isNegativeCycle(Graph graph)
{
	int i, V = graph->max;
	bool visited[V];
	memset(visited, false, sizeof(visited));
	int dist[V];

	for(i = 0; i < V; i++)
	{
		if(!visited[i])
			if(isNegCycleUtil(graph, i, dist))
				return true;
		for(i = 0; i < V; i++)
			if(dist[i] != INT_MAX)
				visited[i] = true;
	}
	return false;
}

void DFSUtil(Graph graph, bool visited[], int v)
{
    int i, n, adj[graph->max];
    visited[v] = true;
    printf("%d ", v);
    n = outdegree(graph, v, adj);
    for(i = 0; i < n; i++)
        if(!visited[adj[i]])
            DFSUtil(graph, visited, adj[i]);
}

void DFS(Graph graph, int src, int dest)
{
	if(!graph->exist[dest] && dest != -1)
	{
		printf("Vertex %d not found\n", dest);
		return;
	}
	if(!graph->exist[src])
	{
		printf("Vertex %d not found\n", src);
		return;
	}
    int V = graph->max, i;
    bool visited[V];
    memset(visited, false, sizeof(visited));
	DFSUtil(graph, visited, src);
	if(!visited[dest] || dest == -1)
	{
		printf("\nNot exist path\n");
		return;
	}
	else printf("\nExist path\n");
}

void BFS(Graph graph, int src, int dest, int *dist)	//Mang dist dung de tinh khoang cach giua dinh nguon va cac dinh trong do thi 
{												//Neu dist[v] = -1 nghia la tu dinh nguon khong the duyet toi dinh do
    if(!graph->exist[dest] && dest != -1)
	{
		printf("Vertex %d not found\n", dest);
		return;
	}
	if(!graph->exist[src])
	{
		printf("Vertex %d not found\n", src);
		return;
	}
	Dllist q = new_dllist(), tmp;				
    int V = graph->max, count, i, u, v, n;
    int *adj = (int*)malloc(V * sizeof(int));
    bool visited[V];
    for(i = 0; i < V; i++)
		dist[i] = -1;
    memset(visited, false, sizeof(visited));
    dll_append(q, new_jval_i(src));
    visited[src] = 1;
	dist[src] = 0;

    while(!dll_empty(q))
    {
        tmp = dll_first(q);
        u = tmp->val.i;
        dll_delete_node(tmp);
        printf("%d ", u);
        if(u == dest)
        {
            free(adj);
			printf("\n");
            return;
        }
        n = outdegree(graph, u, adj);   //n la so dinh ke cua 1 dinh
        for(i = 0; i < n; i++)
        {
            v = adj[i];
            if(!visited[v])
            {
				dist[v] = 1 + dist[u];
                visited[v] = true;
                dll_append(q, new_jval_i(v));
            }
        }
    }
	printf("\n");
    free(adj);
}

void printDist(Graph graph, int dist[], int n)
{
    printf("\nVertex   Distance from Source\n");
    for(int i = 0; i < n; ++i)
		if(graph->exist[i])
        	printf("%d \t\t %d\n", i, dist[i]);
}

void printPath(Graph graph, int parent[], int src)
{
	if(parent[src] == -1)
	{
		printf("%s", getVertex(graph, src));
		return;
	}
	printPath(graph, parent, parent[src]);
	if(graph->exist[src])
		printf(" -> %s", getVertex(graph, src));
}

void Dijkstra(Graph graph, int src, int dest, int *dist)
{
	if(!graph->exist[dest] && dest != -1)
	{
		printf("Vertex %d not found\n", dest);
		return;
	}
	if(!graph->exist[src])
	{
		printf("Vertex %d not found\n", src);
		return;
	}
    int V = graph->max, parent[V];
    int i, u, v, n, weight;
    PQueue *pq = createPQueue(V);
    int *adj = (int*)malloc(sizeof(int) * V);	

    for(v = 0; v < V; v++)
	{
		parent[v] = -1;
		dist[v] = INT_MAX;  
		pq->array[v] = newPQueueNode(v, dist[v]);
		pq->pos[v] = v;
	}
	parent[0] = -1;
    dist[src] = 0;
    decreaseKey(pq, src, dist[src]);
    pq->size = V;

    while(!isEmptyPQueue(pq))
    {
        PQNode *node = extractMin(pq);
        u = node->v;
		if(!graph->exist[u])
			continue;
        n = outdegree(graph, u, adj);
        for(i = 0; i < n; i++)
        {
            v = adj[i];
            weight = getEdgeValue(graph, u, v);		
            if(isInPQueue(pq, v) && dist[u] != INT_MAX && weight + dist[u] < dist[v])
            {
				parent[v] = u;
                dist[v] = dist[u] + weight;
                decreaseKey(pq, v, dist[v]);
            }
        }
    }
    printDist(graph, dist, V);
	if(dest != -1)
		printPath(graph, parent, dest);
	printf("\n");
    free(adj);
}

// Prim's algorithm
void MST(Graph graph)
{
	buildGraph(graph, 0);
    int V = graph->max;
    int parent[V], key[V], i, u, v, weight, n;
	int *adj = (int*)malloc(sizeof(int) * V);
    PQueue *pq = createPQueue(V);

    for(v = 0; v < V; ++v) 
	{
		parent[v] = -1;
		key[v] = INT_MAX;
		pq->array[v] = newPQueueNode(v, key[v]);
		pq->pos[v] = v;
	}
	parent[0] = -1;
    key[0] = 0;
    pq->size = V;
    
	while(!isEmptyPQueue(pq))
	{
		PQNode *node = extractMin(pq);
		u = node->v;
		if(!graph->exist[u])
			continue;
		n = outdegree(graph, u, adj);
		for(i = 0; i < n; i++)
		{
			v = adj[i];
			weight = getEdgeValue(graph, u, v);
			if(isInPQueue(pq, v) && weight < key[v])
			{
				key[v] = weight;
				parent[v] = u;
				decreaseKey(pq, v, key[v]);
			}
		}
	}   
	bool visited[V];
	char c = graph->isDirected ? '>' : '-';
	memset(visited, false, sizeof(visited));
	printf("Minimum spanning tree:\n");
	for(i = 0; i < V; i++)
		if(graph->exist[i])
		{
			if(parent[i] != -1)
				printf("%d -- %d\n", parent[i], i);
			visited[i] = true;
			n = outdegree(graph, i, adj);
			for(u = 0; u < n; u++)
				if(!visited[adj[u]])
				{
					if(i == parent[adj[u]])
						fprintf(graph->gd, "\t%s -%c %s [style = bold, color = \"red\", label = \"%.2lf\"];\n", getVertex(graph, i), c, getVertex(graph, adj[u]), getEdgeValue(graph, i, adj[u]));
					else if(adj[u] == parent[i])
						fprintf(graph->gd, "\t%s -%c %s [style = bold, color = \"red\", label = \"%.2lf\"];\n", getVertex(graph, adj[u]), c, getVertex(graph, i), getEdgeValue(graph, u, adj[u]));
					else 
						fprintf(graph->gd, "\t%s -%c %s [label = \"%.2lf\"];\n", getVertex(graph, i), c, getVertex(graph, adj[u]), getEdgeValue(graph, i, adj[u]));
				}
		}
	fputs("\tlabel = \"Minimum Spanning Tree\";\n", graph->gd);
	free(adj);
}

void TSUtil(Graph g, Dllist *stack, bool visited[], int src)
{
    visited[src] = true;
	int adj[g->max], n, i, v;
    n = outdegree(g, src, adj);
    for(i = 0; i < n; i++)
    {
        v = adj[i];
        if(!visited[v])
            TSUtil(g, stack, visited, v);
    }
    dll_prepend(*stack, new_jval_i(src));
}

void Topological(Graph graph)
{
	buildGraph(graph, 0);
    int i, V = graph->max;
    Dllist stack = new_dllist(), tmp;
    bool visited[V];
    memset(visited, false, sizeof(visited));

    for(i = 0; i < V; i++)
        if(!visited[i] && graph->exist[i])
            TSUtil(graph, &stack, visited, i);
	printf("\nTopological Sort: \n");
    tmp = dll_first(stack);
	fprintf(graph->gd, "\tedge [color = red]\n\t%s", getVertex(graph, tmp->val.i));
	printf("%s ", getVertex(graph, tmp->val.i));
	dll_delete_node(tmp);

    while(!dll_empty(stack))
    {
        tmp = dll_first(stack);
        printf("-> %s ", getVertex(graph, tmp->val.i));
		fprintf(graph->gd, " -> %s", getVertex(graph, tmp->val.i));
        dll_delete_node(tmp);
    }
	fputs(";\n\trankdir = LR;\n", graph->gd);
	fputs("\tlabel = \"Topological Sort\";\n", graph->gd);
	printf("\n");
    free_dllist(stack);
}

Graph getTranspose(Graph graph)
{
	Graph tg = createGraph(graph->hasWeight, graph->isDirected);
	int V = graph->max, adj[V], i, n, u;
	for(i = 0; i < V; i++)
		if(graph->exist[i])
			addVertex(tg, i, getVertex(graph, i));
	for(i = 0; i < V; i++)
		if(graph->exist[i])
		{
			n = outdegree(graph, i, adj);
			for(u = 0; u < n; u++)
				addEdge(tg, adj[u], i, 0);
		}
	return tg;
}

void SCC(Graph graph)
{
	Dllist stack = new_dllist(), tmp;
	int V = graph->max, i;
	bool visited[V];
	memset(visited, false, sizeof(visited));
	for(i = 0; i < V; i++)
		if(!visited[i] && graph->exist[i])
			TSUtil(graph, &stack, visited, i);
	
	Graph tg = getTranspose(graph);
	memset(visited, false, sizeof(visited));
	while(!dll_empty(stack))
	{
		tmp = dll_first(stack);
		i = tmp->val.i;
		if(!visited[i])
		{
			DFSUtil(tg, visited, i);
			printf("\n");
		}
		dll_delete_node(tmp);
	}
	dropGraph(tg);
	free_dllist(stack);
}

void createColor(char color[7])
{
	char alnum[16] = {'a', 'b', 'c', 'd', 'e', 'f', 
					  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
	color[0] = '#';
	for(int i = 1; i < 7; i++)
		color[i] = alnum[rand() % 16];
}

void Color(Graph graph)
{
	int V = graph->max, u, cr, result[V];
	int n, adj[V], i;
	result[0] = 0;
	char color_table[V][8], color[V][8];
	srand(time(NULL));
	strcpy(color_table[0], "#00f000");
	strcpy(color[0], "#00f000");
	for(u = 1; u < V; u++)
	{
		result[u] = -1;
		createColor(color_table[u]);
	}
	bool available[V];
	memset(available, false, sizeof(available));
	for(u = 1; u < V; u++)
		if(graph->exist[u])
		{
			n = outdegree(graph, u, adj);
			for(i = 0; i < n; i++)
				if(result[adj[i]] != -1)
					available[result[adj[i]]] = true;
			for(cr = 0; cr < V; cr++)
				if(available[cr] == false && graph->exist[cr])
					break;
			result[u] = cr;
			strcpy(color[u], color_table[cr]);
			for(i = 0; i < n; i++)
				if(result[adj[i]] != -1)
					available[result[adj[i]]] = false;
		}
	buildGraph(graph, 1);
	for(u = 0; u < V; u++)
		if(graph->exist[u])
			fprintf(graph->gd, "\t%d [fillcolor = \"%s\", style = filled];\n" , u, color[u]);
	memset(available, false, sizeof(available));
	for(u = 0; u < V; u++)
		if(graph->exist[u])
		{
			available[u] = true;
			n = outdegree(graph, u, adj);
			for(i = 0; i < n; i++)
				if(!available[adj[i]])
					fprintf(graph->gd, "\t%d -- %d;\n", u, adj[i]);
		}
	fputs("\tlabel = \"Coloring Algorithm\";\n", graph->gd);
}

bool findAugmentPath(Graph graph, int pairU[], int pairV[], int dist[], int M, int N)
{
	Dllist queue = new_dllist(), tmp;
	int u, v, n, V = graph->max, i;
	int *adj = (int*)malloc(sizeof(int) * V);
	for(u = 1; u <= M; u++)
		if(!pairU[u] && graph->exist[u])
		{
			dist[u] = 0;
			dll_append(queue, new_jval_i(u));
		}
		else dist[u] = INFINITIVE_VALUE;
	
	dist[0] = INFINITIVE_VALUE;
	while(!dll_empty(queue))
	{
		tmp = dll_first(queue);
		u = tmp->val.i;
		dll_delete_node(tmp);
		if(dist[u] < dist[0])
		{
			n = outdegree(graph, u, adj);
			for(i = 0; i < n; i++)
			{
				v = adj[i];
				if(dist[pairV[v]] == INFINITIVE_VALUE)
				{
					dist[pairV[v]] = dist[u] + 1;
					dll_append(queue, new_jval_i(pairV[v]));
				}
			}
		}
	}
	free(adj);
	return dist[0] != INFINITIVE_VALUE;
}

bool createMatch(Graph graph, int dist[], int pairU[], int pairV[], int u)
{
	if(u != 0)
	{
		int n, V = graph->max, i, v;
		int adj[V];
		n = outdegree(graph, u, adj);
		for(i = 0; i < n; i++)
		{
			v = adj[i];
			if(dist[pairV[v]] == INFINITIVE_VALUE)
			{
				dist[pairV[v]] = dist[u] + 1;
				return true;
			}
		}
	}
	dist[u] = INFINITIVE_VALUE;
	return false;
}

int MBM(Graph graph, int M, int N)
{
	int dist[M + 1], pairU[M + 1], pairV[N + 1], u, max_match = 0;
	memset(pairU, 0, sizeof(pairU));
	memset(pairV, 0, sizeof(pairV));
	while(findAugmentPath(graph, pairU, pairV, dist, M, N))
		for(u = 1; u <= M; u++)
			if(pairU[u] && createMatch(graph, dist, pairU, pairV, u) && graph->exist[u])
				max_match++;
	return max_match;
}

void drawGraph(Graph graph)
{
	graph->cf++;
	fputc('}', graph->gd);
	fclose(graph->gd);
	char cmd[200], file_png[50];
	strncpy(file_png, graph->f_name, 11);
	file_png[11] = '\0';
	strcat(file_png, ".pdf");
	sprintf(cmd, "dot -Tpdf %s -o %s", graph->f_name, file_png);
	int state = system(cmd);
	sprintf(cmd, "xdg-open %s", file_png);
	int call = system(cmd);	
}

void displayGraph(Graph graph)
{
	buildGraph(graph, 0);
	int n, u, i, V = graph->max;
	int *adj = (int*)malloc(V * sizeof(int));
	char c = graph->isDirected ? '>' : '-';
	bool visited[V];
	memset(visited, false, sizeof(visited));
	for(i = 0; i < V; i++)
		if(graph->exist[i])
		{
			visited[i] = true;
			n = outdegree(graph, i, adj);
			for(u = 0; u < n; u++)
				if(!visited[adj[u]])
					if(graph->hasWeight)
						fprintf(graph->gd, "\t%s -%c %s [label = \"%.2lf\"];\n", getVertex(graph, i), c, getVertex(graph, adj[u]), getEdgeValue(graph, i, adj[u]));
					else fprintf(graph->gd, "\t%s -%c %s\n", getVertex(graph, i), c, getVertex(graph, adj[u]));
		}
	free(adj);
	drawGraph(graph);
}

void dropGraph(Graph graph)
{
	JRB node, innode;
	detailVertex detailnode;
	
	jrb_traverse(node, graph->edges)
		jrb_free_tree((JRB)jval_v(node->val));

	jrb_free_tree(graph->edges);
	
	jrb_traverse(node, graph->vertices)
	{
		detailnode = (detailVertex)node->val.v;
		free(detailnode->name);
		jrb_free_tree((JRB)detailnode->indegreeTree);
	}
	jrb_free_tree(graph->vertices);
	free(graph->exist);
	free(graph);
}