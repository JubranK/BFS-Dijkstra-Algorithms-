#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>


#define MinQueueSize 5
#define Max 15
#define inf 9999

struct QueueRecord{ // a struct for the queue
    int Capacity;
    int Front;
    int Rear;
    int Size;
    int*Array;
};
typedef struct QueueRecord *Queue;


typedef struct { // a struct for the stack
    int items[Max];
    int top;
} Stack;

typedef struct { // a struct for the heap (min heap)
    int* array;
    int size;
    int capacity;
} MinHeap;


//------------------Functions prototype--------------------

int IsEmpty(Queue);
int IsFull(Queue);
Queue CreateQueue(int);
void MakeEmpty(Queue);
void DisposeQueue(Queue);
int Succ(int,Queue);
void Enqueue(int,Queue);
int Front(Queue);
void Dequeue(Queue);
int FrontAndDequeue(Q);
Stack CreateStack();
int IsEmptyStack(Stack);
void Push(int,Stack*);
int Pop(Stack*);

int mapCityToIndex(char*);

MinHeap* createMinHeap(int);
int isEmptyHeap(MinHeap*);
void swap(int*,int*);
void heapifyUp(MinHeap*,int);
void heapifyDown(MinHeap*,int);
void insert(MinHeap*,int);
int extractMin(MinHeap*);


void BFS(int[Max][Max],int[Max],int[Max],int,int);
void Dijkstra(int[Max][Max],int[Max],int ,int);
void BFSFile(FILE*,int[Max][Max],int[Max],int[Max],int,int);
void DijkstraFile(FILE*,int[Max][Max],int[Max],int,int);
void Menu(int [Max][Max],int [Max][Max],int[Max],int[Max],int[Max],char[100],char[100]);


//-------The main function-----------------

int main()
{
    char sourceCityByTheUser[100];
    char DestinationCityByTheUser[100];

    int AdjMatrix[Max][Max];// BFS adjacency matrix and initially(before reading the file) there no adjacent cities.
    for(int i=0;i<Max;i++){
       for(int j=0;j<Max;j++){
          AdjMatrix[i][j]=0;
        }
    }

     int AdjMatrixDijk[Max][Max];//Dijkstra adjacency matrix and initially(before reading the file) there no adjacent cities.
    for(int i=0;i<Max;i++){
       for(int j=0;j<Max;j++){
          AdjMatrixDijk[i][j]=0;
        }
    }

    int visited[Max]={0}; // visited matrix to know which city is already visited once and initially all city are not visisted so all values are zeros.


    int Distance[Max];  // distance matrix for BFS and initially infinity
    for (int i = 0; i < Max; i++) {
        Distance[i] = inf;
    }

    int DistanceDijk[Max];  // distance matrix for Dijkstra and initially infinity
    for (int i = 0; i < Max; i++) {
        DistanceDijk[i] = inf;
    }

    printf("----------The menu----------"); // calling the menu function .
    Menu(AdjMatrix,AdjMatrixDijk,visited,Distance,DistanceDijk,sourceCityByTheUser,DestinationCityByTheUser);
    return 0;
}

//------------------FUNCTIONS------------------------------

// I used the queues in the BFS algorithm and used these function from queue.
int IsEmpty( Queue Q ){ // Is empty routine for the queue , check if the given queue is empty.
    return Q->Size == 0;
}

int IsFull( Queue Q ){  // Is full routine for the queue
    return Q->Size == Q->Capacity;
}

Queue CreateQueue( int MaxElements ){ // creat queue routine
    Queue Q;

    if( MaxElements < MinQueueSize ) printf("Queue size is too small\n");
        Q = (Queue)malloc(sizeof( struct QueueRecord ));

    if( Q == NULL) printf("Out of space");
        Q->Array = (int*)malloc(sizeof(int) * MaxElements);

    if( Q->Array == NULL ) printf("Out of space");
        Q->Capacity = MaxElements;

    MakeEmpty( Q );

    return Q;
}

void MakeEmpty( Queue Q){     // make empty routine
    Q->Size = 0;
    Q->Front = 1;
    Q->Rear = 0;
}

void DisposeQueue( Queue Q ){ // Dispose queue routine
    if( Q != NULL ){
        free( Q->Array );
        free( Q );
    }
}

int Succ( int Value, Queue Q ){ // Succ routine
    if( ++Value == Q->Capacity )
    Value = 0;

    return Value;
}

void Enqueue( int X, Queue Q ){ // Enqueue routine , remove the first element were added (FIFO).
    if( IsFull( Q ) )
        printf("Full Queue");
    else{
        Q->Size++;
        Q->Rear = Succ( Q->Rear, Q );
        Q->Array[ Q->Rear ] = X;
    }
}

int Front( Queue Q ){ // Front routine
    if( !IsEmpty( Q ) )
        return Q->Array[ Q->Front ];

    printf("Empty Queue!");
    return 0;
}

void Dequeue( Queue Q ){    // Dequeue routine
    if( IsEmpty( Q ) )
        printf("Empty Queue!");
    else{
        Q->Size--;
        Q->Front = Succ( Q->Front, Q );
    }
}

int FrontAndDequeue( Queue Q ){ // Front and Dequeue routine it return  the front and remove it from the queue (Dequeue).
    int X = 0;
    if( IsEmpty( Q ) )
        printf("Empty Queue!");
    else{
        Q->Size--;
        X = Q->Array[ Q->Front ];
        Q->Front = Succ( Q->Front, Q );
}
return X;
}


// I used the stacks(with queue) in the BFS algorithm and used these function from stack.
Stack CreateStack() {      // creat stack routine
    Stack S;
    S.top = -1;
    return S;
}

int IsEmptyStack(Stack S) { // Is empty routine , check if the given stack is empty.
    if (S.top == -1)
        return 1;
    else
        return 0;
}

void Push(int value, Stack* S) { // push routine (insert to the stack at top)
    if (S->top == Max - 1)
        printf("Stack is full");
    else {
        S->top++;
        S->items[S->top] = value;
    }
}

int Pop(Stack* S) {// pop routine (remove the last element were added ,LIFO ).
    int item;
    if (IsEmptyStack(*S)) {
        printf("Stack is empty");
        item = -1;
    } else {
        item = S->items[S->top];
        S->top--;
    }
    return item;
}


int mapCityToIndex(char* source) { // mapping each city to its index value (for adj matrix and To make it easier to deal with them ).
    if (strcmp(source, "Akka") == 0) {
        return 0;
    } else if (strcmp(source, "Haifa") == 0) {
        return 1;
    } else if (strcmp(source, "Nazareth") == 0) {
        return 2;
    }else if (strcmp(source, "Jenin") == 0) {
        return 3;
    }else if (strcmp(source, "Bethlehem") == 0) {
        return 4;
    }else if (strcmp(source, "Hebron") == 0) {
        return 5;
    }else if (strcmp(source, "Jerusalem") == 0) {
        return 6;
    }else if (strcmp(source, "Jaffa") == 0) {
        return 7;
    }else if (strcmp(source, "Jericho") == 0) {
        return 8;
    }else if (strcmp(source, "Tulkarem") == 0) {
        return 9;
    }else if (strcmp(source, "Qalqilya") == 0) {
        return 10;
    }else if (strcmp(source, "Tubas") == 0) {
        return 11;
    }else if (strcmp(source, "Nablus") == 0) {
        return 12;
    }else if (strcmp(source, "Ramallah") == 0) {
        return 13;
    }else if (strcmp(source, "Salfit") == 0) {
        return 14;
    }

}

// I used the heap in Dijkstra algorithm (we need to dequeue the element with the min value , so heap instead of regular queue).
MinHeap* createMinHeap(int capacity) { // Creat min heap routine
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->capacity = capacity;
    heap->size = 0;
    heap->array = (int*)malloc(capacity * sizeof(int));
    return heap;
}

int isEmptyHeap(MinHeap* heap) { // Is empty heap routine, to check if the given heap is empty .
    return heap->size == 0;
}

void swap(int* a, int* b) { // Swap routine.
    int temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(MinHeap* heap, int index) { //Heapify up routine.
    int parent = (index - 1) / 2;

    if (parent >= 0 && heap->array[parent] > heap->array[index]) {
        swap(&heap->array[parent], &heap->array[index]);
        heapifyUp(heap, parent);
    }
}

void heapifyDown(MinHeap* heap, int index) { //Heapify down routine.
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int smallest = index;

    if (left < heap->size && heap->array[left] < heap->array[smallest]) {
        smallest = left;
    }

    if (right < heap->size && heap->array[right] < heap->array[smallest]) {
        smallest = right;
    }

    if (smallest != index) {
        swap(&heap->array[smallest], &heap->array[index]);
        heapifyDown(heap, smallest);
    }
}

void insert(MinHeap* heap, int value) { // insert routine , to insert the element in right place in the min heap .
    if (heap->size == heap->capacity) { // heap is full
        return;
    }
    // not full continue
    heap->array[heap->size] = value;
    heapifyUp(heap, heap->size);
    heap->size++;
}

int extractMin(MinHeap* heap) { // Extract min routine , dequeue the first element which is the min element.
    if (heap->size == 0) { // heap is empty
        return -1;
    }

    // not empty , extract the min
    int min = heap->array[0];
    heap->array[0] = heap->array[heap->size - 1];
    heap->size--;
    heapifyDown(heap, 0);

    return min;
}


void BFS(int adj[Max][Max],int visited[Max], int Distance[Max], int source, int destination) {
    Queue Q = CreateQueue(Max);
    Stack S = CreateStack(); // a stack for printing the path in the right order

    Enqueue(source, Q);
    visited[source] = 1;
    Distance[source] = 0;
    int current;
    int counter = 0;
    int NumOfVertices=0;

    // Array to store the previous node for each node
    int prev[Max];
    for (int i = 0; i < Max; i++) {
        prev[i] = -1;
    }

    while (!IsEmpty(Q)) { // while the queue is not empty
        source = FrontAndDequeue(Q);

        if (source == destination) { // Check if the current node is the destination
            current = destination;

            while (current != -1) {
                Push(current, &S);// push it into the stack to be later printed in the right position.
                current = prev[current];
            }

            while (!IsEmptyStack(S)) {
                NumOfVertices++;// increment the number of vertices by 1

                current = Pop(&S);// pop it from the stack and check its index value to print the city name.
                if (current == 0) {
                    printf("Akka ");
                } else if (current == 1) {
                    printf("Haifa ");
                } else if (current == 2) {
                    printf("Nazareth ");
                } else if (current == 3) {
                    printf("Jenin ");
                } else if (current == 4) {
                    printf("Bethlehem ");
                } else if (current == 5) {
                    printf("Hebron ");
                } else if (current == 6) {
                    printf("Jerusalem ");
                } else if (current == 7) {
                    printf("Jaffa ");
                } else if (current == 8) {
                    printf("Jericho ");
                } else if (current == 9) {
                    printf("Tulkarem ");
                } else if (current == 10) {
                    printf("Qalqilya ");
                } else if (current == 11) {
                    printf("Tubas ");
                } else if (current == 12) {
                    printf("Nablus ");
                } else if (current == 13) {
                    printf("Ramallah ");
                } else if (current == 14) {
                    printf("Salfit ");
                }
            }
            printf(",And the shortest distance is %d Edge/Edges (since its BFS). \n",NumOfVertices-1);

            return;
        }

        for (int i = 0; i < Max; i++) {
            if (adj[source][i] == 1 && visited[i] == 0) {
                Distance[i] = Distance[source] + 1;
                Enqueue(i, Q);
                visited[i] = 1;
                prev[i] = source; // previous node for node i
            }
        }
    }
    printf("Destination not found!\n");
}

void Dijkstra(int adj[Max][Max], int Distance[Max],int source, int destination) {
    int Parent[Max]; // to track the road
    MinHeap* Heap = createMinHeap(Max);

    insert(Heap, source); // Insert the source to the heap and set distance = 0.
    Distance[source] = 0;

    int OriginalSource = source; // Save the original source.

    while (!isEmptyHeap(Heap)) {
        source = extractMin(Heap);
        for (int i = 0; i < Max; i++) {
            if (adj[source][i] != 0 && Distance[source] + adj[source][i] < Distance[i]) {
                Distance[i] = Distance[source] + adj[source][i]; // Compute the new distance.
                Parent[i] = source; // Update the parent of vertex i.

                insert(Heap, i); // Insert/update the heap.
            }
        }
    }

    free(Heap->array);
    free(Heap);

    // Mapping of cities to their indices
    char* cities[15] = {
        "Akka", "Haifa", "Nazareth", "Jenin", "Bethlehem",
        "Hebron", "Jerusalem", "Jaffa", "Jericho", "Tulkarem",
        "Qalqilya", "Tubas", "Nablus", "Ramallah", "Salfit"
    };

    printf("Minimum distance from %s to %s: %d\n", cities[OriginalSource], cities[destination], Distance[destination]);
    if(Distance[destination]==inf){
        printf("There is no path since the distance is inf\n");
    }else{
        printf("Path: ");

        int currentVertex = destination;
        while (currentVertex != OriginalSource) {
            printf("(%dKm)",Distance[currentVertex]-Distance[Parent[currentVertex]]);// edge distance
            printf("%s <- ", cities[currentVertex]);
            currentVertex = Parent[currentVertex];
        }
        printf("%s\n", cities[OriginalSource]);

    }
}


// Functions for print the information in the file (they are same as above , but print the result in the file)
void BFSFile(FILE* inp,int adj[Max][Max], int visited[Max], int Distance[Max], int source, int destination) {

    Queue Q = CreateQueue(Max);
    Stack S = CreateStack();

    Enqueue(source, Q);
    visited[source] = 1;
    Distance[source] = 0;
    int current;
    int counter = 0;
    int NumOfVertices = 0;

    // Array to store the previous node for each node
    int prev[Max];
    for (int i = 0; i < Max; i++) {
        prev[i] = -1;
    }

    while (!IsEmpty(Q)) {
        source = FrontAndDequeue(Q);

        if (source == destination) { // Check if the current node is the destination
            current = destination;

            while (current != -1) {
                Push(current, &S);
                current = prev[current];
            }

            while (!IsEmptyStack(S)) {
                NumOfVertices++;

                current = Pop(&S); // pop it from the stack and check its index value to print the city name.
                if (current == 0) {
                    fprintf(inp, "Akka ");
                } else if (current == 1) {
                    fprintf(inp, "Haifa ");
                } else if (current == 2) {
                    fprintf(inp, "Nazareth ");
                } else if (current == 3) {
                    fprintf(inp, "Jenin ");
                } else if (current == 4) {
                    fprintf(inp, "Bethlehem ");
                } else if (current == 5) {
                    fprintf(inp, "Hebron ");
                } else if (current == 6) {
                    fprintf(inp, "Jerusalem ");
                } else if (current == 7) {
                    fprintf(inp, "Jaffa ");
                } else if (current == 8) {
                    fprintf(inp, "Jericho ");
                } else if (current == 9) {
                    fprintf(inp, "Tulkarem ");
                } else if (current == 10) {
                    fprintf(inp, "Qalqilya ");
                } else if (current == 11) {
                    fprintf(inp, "Tubas ");
                } else if (current == 12) {
                    fprintf(inp, "Nablus ");
                } else if (current == 13) {
                    fprintf(inp, "Ramallah ");
                } else if (current == 14) {
                        fprintf(inp, "Salfit ");
                }
            }
            fprintf(inp,",And the shortest distance is %d Edge/Edges (since its BFS). \n", NumOfVertices - 1); // number of edge is less than the number of vertices by 1
        }

        for (int i = 0; i < Max; i++) {
            if (adj[source][i] == 1 && visited[i] == 0) {
                Distance[i] = Distance[source] + 1;
                Enqueue(i, Q);
                visited[i] = 1;
                prev[i] = source; // previous node for node i
            }
        }
    }

    fprintf(inp,"Destination not found!\n");
}

void DijkstraFile(FILE*inp,int adj[Max][Max], int Distance[Max], int source, int destination) {
    int Parent[Max]; // to track the road
    MinHeap* Heap = createMinHeap(Max);

    insert(Heap, source); // Insert the source to the heap and set distance = 0.
    Distance[source] = 0;

    int OriginalSource = source; // Save the original source.

    while (!isEmptyHeap(Heap)) {
        source = extractMin(Heap);
        for (int i = 0; i < Max; i++) {
            if (adj[source][i] != 0 && Distance[source] + adj[source][i] < Distance[i]) {
                Distance[i] = Distance[source] + adj[source][i]; // Compute the new distance.
                Parent[i] = source; // Update the parent of vertex i.

                insert(Heap, i); // Insert/update the heap.
            }
        }
    }

    free(Heap->array);
    free(Heap);

    // Mapping of cities to their indices.
    char* cities[15] = {
        "Akka", "Haifa", "Nazareth", "Jenin", "Bethlehem",
        "Hebron", "Jerusalem", "Jaffa", "Jericho", "Tulkarem",
        "Qalqilya", "Tubas", "Nablus", "Ramallah", "Salfit"
    };

    fprintf(inp, "Minimum distance from %s to %s: %d\n", cities[OriginalSource], cities[destination], Distance[destination]);
    if(Distance[destination]==inf){
        fprintf(inp,"There is no path since the distance is inf\n");
    }else{
        fprintf(inp, "Path: ");

        int currentVertex = destination;
        while (currentVertex != OriginalSource) {
            fprintf(inp,"(%dKm)",Distance[currentVertex]-Distance[Parent[currentVertex]]);
            fprintf(inp, "%s <- ", cities[currentVertex]);
            currentVertex = Parent[currentVertex];
        }
            fprintf(inp, "%s\n", cities[OriginalSource]);

        }
}



// The menu function
void Menu(int AdjMatrix[Max][Max],int AdjMatrixDijk[Max][Max],int visited[Max],int Distance[Max],int DistanceDijk[Max],char sourceCityByTheUser[100],char DestinationCityByTheUser[100]) {
        int choice;
        printf("\nChoose One Of The following Options:\n");
        printf("1) Load the Cities.\n");
        printf("2) Enter source.\n");
        printf("3) Enter destination.\n");
        printf("4) Exit.\n");
        printf("Your Choice: ");
        scanf("%d",&choice);
        printf("\n");
        if(choice==1){
            FILE* inp = fopen("Cities.txt","r");

            if (inp == NULL) {
                printf("Failed to open the file.\n");
            }else{
                char source[100];
                char destination[100];
                int distance;
                int SourceMap;
                int DestinationMap;

                while (fscanf(inp, "%s\t%s\t%dkm\n",source,destination,&distance) == 3) {

                    /*Note that :
                    for adj matrices
                    Akka -> 0
                    Haifa-> 1
                    Nazareth-> 2
                    Jenin-> 3
                    Bethlehem-> 4
                    Hebron-> 5
                    Jerusalem-> 6
                    Jaffa-> 7
                    Jericho-> 8
                    Tulkarem-> 9
                    Qalqilya-> 10
                    Tubas-> 11
                    Nablus-> 12
                    Ramallah->13
                    Salfit->14

                    */

                    SourceMap=mapCityToIndex(source);// will map the city name to its index in the adj matrix

                    DestinationMap=mapCityToIndex(destination);// will map the city name to its index in the adj matrix

                    AdjMatrix[SourceMap][DestinationMap]=1; // they are adjacent
                    AdjMatrixDijk[SourceMap][DestinationMap]=distance; // they are adjacent and store the distance value from file

                    // Now the adj matrices are ready
                }
                fclose(inp);

                printf("Done!The cities were loaded and the graph were created!\n");
                Menu(AdjMatrix,AdjMatrixDijk,visited,Distance,DistanceDijk,sourceCityByTheUser,DestinationCityByTheUser);
            }
        }else if(choice == 2) {
            printf("Enter source:");
            scanf("%s",sourceCityByTheUser);
            printf("\n");
            if(strcmp(sourceCityByTheUser, "Akka") != 0 && strcmp(sourceCityByTheUser, "Haifa") != 0 && strcmp(sourceCityByTheUser, "Nazareth") != 0 && strcmp(sourceCityByTheUser, "Jenin") != 0 && strcmp(sourceCityByTheUser, "Bethlehem") != 0 && strcmp(sourceCityByTheUser, "Hebron") != 0 && strcmp(sourceCityByTheUser, "Jerusalem") != 0 && strcmp(sourceCityByTheUser, "Jaffa") != 0 && strcmp(sourceCityByTheUser, "Jericho") != 0 && strcmp(sourceCityByTheUser, "Tulkarem") != 0 && strcmp(sourceCityByTheUser, "Qalqilya") != 0 && strcmp(sourceCityByTheUser, "Tubas") != 0 && strcmp(sourceCityByTheUser, "Nablus") != 0 && strcmp(sourceCityByTheUser, "Ramallah") != 0 && strcmp(sourceCityByTheUser, "Salfit") != 0) {
                printf("----The source %s is not in the file(Cities.txt)----\n", sourceCityByTheUser);
            } else {
                Menu(AdjMatrix, AdjMatrixDijk, visited, Distance, DistanceDijk, sourceCityByTheUser, DestinationCityByTheUser);
            }



        }else if (choice == 3) {
            printf("Enter destination:");
            scanf("%s",DestinationCityByTheUser);
            printf("\n");
            printf("Your source city :%s\n",sourceCityByTheUser);
            printf("Your destination city :%s\n\n",DestinationCityByTheUser);

            int SourceAsInt=mapCityToIndex(sourceCityByTheUser);  // convert the cities from user to its index value
            int DestinationAsInt=mapCityToIndex(DestinationCityByTheUser);

            //BFS
            printf("BFS:");
            BFS(AdjMatrix,visited,Distance,SourceAsInt,DestinationAsInt);
            printf("\n");


            //Dijkstra
            printf("Dijkstra:");
            Dijkstra(AdjMatrixDijk,DistanceDijk,SourceAsInt,DestinationAsInt);
            printf("\n");


            // to redefine them as inf
            for (int i = 0; i < Max; i++) {
                Distance[i] = inf;
            }


            for (int i = 0; i < Max; i++) {
                DistanceDijk[i] = inf;
            }

            Menu(AdjMatrix,AdjMatrixDijk,visited,Distance,DistanceDijk,sourceCityByTheUser,DestinationCityByTheUser);

        }else if (choice == 4) {

            FILE* inp = fopen("shortest_distance.txt", "w");
            if(inp!=NULL){

                int SourceAsInt=mapCityToIndex(sourceCityByTheUser);  // convert the cities from user to its index value
                int DestinationAsInt=mapCityToIndex(DestinationCityByTheUser);

                //fprintf(inp,"BFS:");
                //BFSFile(inp,AdjMatrix,visited,Distance,SourceAsInt,DestinationAsInt);

                //fprintf(inp,"Dijkstra:");
                DijkstraFile(inp,AdjMatrixDijk,DistanceDijk,SourceAsInt,DestinationAsInt);

                fclose(inp);
                printf("Done! check the shortest_distance.txt file!\n");
            }else
                printf("Can not find the shortest_distance.txt file !\n");

        }else
            printf("-----Invalid choice------\n");

}
