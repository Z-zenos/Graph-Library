/*
Libraries for priority queue using min heap
Reference: https://www.geeksforgeeks.org/binary-heap/
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pqueue.h"

 
PQNode *newPQueueNode(int v, int dist)
{
    PQNode *PQueueNode = (PQNode*)malloc(sizeof(PQNode));
    PQueueNode->v = v;
    PQueueNode->dist = dist;
    return PQueueNode;
}
 
//Function to create a Min Heap
PQueue* createPQueue(int capacity)
{
    PQueue *minHeap = (PQueue*)malloc(sizeof(PQueue));
    minHeap->pos = (int*)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (PQNode**)malloc(capacity * sizeof(PQNode*));
    return minHeap;
}
 
//Function to swap two nodes of min heap.Needed for min heapify
void swapPQueueNode(PQNode **a, PQNode **b)
{
    PQNode* t = *a;
    *a = *b;
    *b = t;
}
 
// Function to heapify at given idx. 
// This function also updates position of nodes when they are swapped.
// Position is needed for decreaseKey()
void PQueueify(PQueue *minHeap, int idx)
{
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;
 
    if(left < minHeap->size && minHeap->array[left]->dist < minHeap->array[smallest]->dist)
        smallest = left;
 
    if(right < minHeap->size && minHeap->array[right]->dist < minHeap->array[smallest]->dist)
        smallest = right;
 
    if(smallest != idx)
    {
        PQNode *smallestNode = minHeap->array[smallest];
        PQNode *idxNode = minHeap->array[idx];

        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;
 
        swapPQueueNode(&minHeap->array[smallest], &minHeap->array[idx]);
        PQueueify(minHeap, smallest);
    }
}
 
//Function to check if the given minHeap is ampty or not
int isEmptyPQueue(PQueue *minHeap)
{
    return minHeap->size == 0;
}
 
//Function to extract minimum node from heap
PQNode* extractMin(PQueue *minHeap)
{
    if(isEmptyPQueue(minHeap))
        return NULL;
 
    PQNode *root = minHeap->array[0];
    PQNode *lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;
 
    minHeap->pos[root->v] = minHeap->size-1;
    minHeap->pos[lastNode->v] = 0;
 
    --minHeap->size;
    PQueueify(minHeap, 0);
    return root;
}
 
// Function to decreasy dist value of a given vertex v. 
// This function uses pos[] of min heap to get the current index of node in min heap
void decreaseKey(PQueue *minHeap, int v, int dist)
{
    int i = minHeap->pos[v];
    minHeap->array[i]->dist = dist;
    while(i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist)
    {
        minHeap->pos[minHeap->array[i]->v] = (i-1)/2;
        minHeap->pos[minHeap->array[(i-1)/2]->v] = i;
        swapPQueueNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}
 
// A utility function to check if a given vertex 'v' is in min heap or not
bool isInPQueue(PQueue *minHeap, int v)
{
    if(minHeap->pos[v] < minHeap->size)
        return true;
    return false;
}
