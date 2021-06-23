#ifndef _P_QUEUE_
#define _P_QUEUE

// Structure to represent a min heap node
typedef struct PriorityQueueNode
{
    int  v;
    int dist;
}PQNode;
 
// Structure to represent a min heap
typedef struct PQueue
{
    int size;    
    int capacity;
    int *pos;   
    PQNode **array;
}PQueue;

extern PQNode *newPQueueNode(int v, int dist);
extern PQueue* createPQueue(int capacity);
extern void swapPQueueNode(PQNode **a, PQNode **b);
extern void PQueueify(PQueue *minHeap, int idx);
extern int isEmptyPQueue(PQueue *minHeap);
extern PQNode* extractMin(PQueue *minHeap);     
extern void decreaseKey(PQueue *minHeap, int v, int dist);
extern bool isInPQueue(PQueue *minHeap, int v);


#endif