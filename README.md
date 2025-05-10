# Dijkstra's Algorithm with Cycle Detection

## Overview

This project implements **Dijkstra's algorithm** to compute the shortest paths from a user-selected start vertex within a graph. It reads graph data from an input file and outputs a summary table displaying the shortest paths to all other vertices. The project also incorporates cycle detection using the **Depth-First Search (DFS)** algorithm. The graph is represented using an adjacency matrix, and various data structures such as vectors, stacks, and queues are used to manage and process the graph.

## Problem Description

The input consists of a text file containing edges and their weights, formatted as follows: Origin; Destination; Trip-length. 

The program will:

1. Build a weighted graph from the input file.
2. Prompt the user to select a start vertex.
3. Apply **Dijkstra's algorithm** to compute the shortest paths from the start vertex.
4. Detect if the graph contains any cycles using the **DFS** algorithm.
5. Output a summary table showing the shortest paths, along with cycle detection results.

## Graph Class

The **Graph class** provides a flexible representation of the graph data structure and supports various operations like adding vertices, adding edges, and retrieving adjacency information. It uses templates to handle different types of vertices. The key methods used in this implementation are:

- **AddVertex()**: Adds a vertex to the graph.
- **AddEdge()**: Adds an edge between two vertices with a specified weight.
- **WeightIs()**: Retrieves the weight of the edge between two vertices.
- **GetToVertices()**: Retrieves the adjacent vertices of a given vertex.
- **MakeEmpty()**: Clears all vertices and edges from the graph.
- **IsEmpty()**: Checks if the graph is empty.
- **IsFull()**: Checks if the graph is full (maximum number of vertices).
- **ClearMarks()**: Clears all marks associated with vertices.
- **MarkVertex()**: Marks a specific vertex.
- **IsMarked()**: Checks if a vertex is marked.

### Client Algorithm

1. **Graph Construction**: The program reads from an input file, parsing each line to extract edge data and build the graph.
2. **Start Vertex Selection**: The user selects a start vertex. The program validates this input to ensure the vertex exists.
3. **Dijkstra's Algorithm**: The algorithm calculates the shortest path from the selected start vertex to all other vertices in the graph.
4. **Cycle Detection**: The program uses DFS to detect if a cycle exists in the graph.
5. **Output**: The program prints a summary table of shortest paths and indicates whether the graph contains a cycle.

## Helper Methods

Several helper methods are utilized to simplify and modularize the code:

- **initializeArrayInfo()**: Initializes the four parallel arrays used in Dijkstra's algorithm.
- **getIndex()**: Returns the index of a given vertex in the vertex list.
- **printSummaryTableRow()**: Prints a row of the summary table.
- **findMinDistance()**: Finds the vertex with the minimum distance among unmarked vertices.
- **isValidVertex()**: Checks if a vertex is valid (exists in the graph).
- **allVisited()**: Checks if all vertices have been visited during Dijkstra's algorithm.

## Input File Example (ginfile2.dat)

The graph data is represented in an adjacency matrix format as shown below:
```
      V1   V2   V3   V4   V5   V6   V7
  ---------------------------------------
V1 |  0    2    4    1    0    0    0  
V2 |  2    0    0    3    10   0    0  
V3 |  4    0    0    2    0    5    0  
V4 |  1    3    2    0    2    8    4  
V5 |  0    10   0    2    0    0    6  
V6 |  0    0    5    8    0    0    1  
V7 |  0    0    0    4    6    1    0  
```
## Compile & Run Instructions
```
c++ dijkstra.cpp
./a.out ginfile1.dat                        # for single-file input 
./a.out ginfile1.dat ginfile2.dat           # for multi-file input 
```


## Step by step guide to the shortest paths from V1 using Dijkstra’s Algorithm--

1. First, we should set up a table to store our vertices, mark (a boolean representation of whether this vertex has been visited), distance, and previous (where the vertex was last).
2. Mark the starting vertex (V1) as visited (represented by T0), set its distance to 0, and set the previous to be blank. 
3. Now we can get greedy! Continue this step until the entire graph is covered. Identify the smallest unmarked vertex. Label this as marked and label the distance and previous of its adjacent vertices. Keep in mind that marked vertices should not be touched. Also, distance must include the previous distance and should be updated if a smaller distance is found.

Let’s begin the algorithm: 
We can label V1 as T0 and examine its adjacent vertices. Based on the input file, it can be seen that V1 has adjacent vertices of V2 (distance of 2), V3 (4), and V4 (1). We can now label the distances and previous for V2, V3, and V4, while leaving all of them unmarked. 

The current smallest non-marked vertex is now V4 with a distance of 1. We can mark this as T1 and examine the adjacent vertices of V4. Based on the input file, V4 has adjacent vertices of V1 (distance of 1), V2 (3), V3 (2), V5 (2), V6 (8), and V7 (4). We can now label the distances and previous for V3, V5, V6, and V7, while leaving the marked vertices (V1) and vertices with smaller distances (V2) unmarked. The updated distance for V3 will include prior distances, so it will be 1+2 = 3. Furthermore, the distances for V5, V6, and V7 must include V4’s distance, so they will respectively be 3, 9, and 5.

Now, the current smallest non-marked vertex will be V2 with its distance of 2. We can mark this as T2 and examine the adjacent vertices of V2. Based on the input file, V2 has adjacent vertices of V1 (2), V4 (3), and V5 (10). V1 is already marked so it should not be touched. Furthermore, V4 and V5 have smaller distances, so they do not need to be updated.

The next smallest non-marked vertex is a tie between V3 (3) and V5 (3). Since the distances are the same, it does not matter which of these vertices we choose to look at first. This step by step guide will mark V3 as T3 and examine the adjacent vertices of V3. Based on the input file, V3 has adjacent vertices of V1 (4), V4 (2), and V6 (5). V1 is already marked so it should not be touched. Furthermore, V4 and V6 have smaller distances, so they do not need to be updated.

V5 is the next smallest non-marked vertex with a distance of 3. We can mark this vertex as T4 and examine its adjacent vertices. Based on the input file, V5 has adjacent vertices of V2 (10), V4 (2), and V7 (6). However, V2, V4, and V7 all have smaller distances, so they do not need to be updated. 

The next smallest non-marked vertex is V7 with a distance of 5. We can mark V7 as T5 and examine its adjacent vertices. Based on the input file, V7 has adjacent vertices of V4 (4), V5 (5), and V6 (1). V4 and V5 already have smaller distances so they do not need to be updated. The updated distance for V6 will include prior distances, so it will be 5+1 = 6 and its previous will be set to V7. 

The last non-marked vertex is V6 with a distance of 6. We can mark V6 as T6 and examine its adjacent vertices. Based on the input file, V6 has adjacent vertices of V3 (5), V4 (8), and V7 (1). However, V3, V4, and V7 all have smaller distances, so they do not need to be updated. 

All vertices have now been marked, meaning that the entire graph has been convered! Dijkstra’s algorithm is now complete and we can see the shorted paths from V1 to all other vertices in the graph. For example, it can be seen from the summary table that the shortest distance from V1 to V7 is 5. 
```
----------------------------------------------------------------------------------------
Vertex                 Mark                Distance                 Previous
                
V1                     T0                  0                        N/A                      
V4                     T2                  1                        V1                       
V2                     T3                  2                        V1                       
V5                     T1                  3                        V4                       
V3                     T4                  3                        V4                       
V7                     T6                  5                        V4                       
V6                     T5                  6                        V7                       
----------------------------------------------------------------------------------------
```

## License
 No license has been provided for this project
