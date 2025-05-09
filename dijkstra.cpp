#include <iostream>
#include <fstream>
#include <iomanip> // for setw
#include <vector>
#include <stack>
#include <string>
#include <algorithm>
#include <limits.h>
#include "graph.h"
#include "queue.h"

using namespace std;

// function prototypes
Graph<string> buildGraphFromFile(const string& filename, vector<string>& verticesList);
string startVertex(const vector<string>& verticesList);
void initializeArrayInfo(const vector<string>& verticesList, string vertexNames[], bool visited[], int distance[], string previousVertex[]);
int getIndex(const vector<string>& verticesList, const string& vertex);
void printSummaryTableRow(const string& vertex, int distance, const string& previousVertex);
int findMinDistance(const vector<string>& verticesList, bool visited[], int distance[], int numVertices);
bool isValidVertex(const vector<string>& verticesList, const string& vertex);
void dijkstra(Graph<string>& graph, const string& startVertex, const vector<string>& verticesList);
bool allVisited(bool visited[], int numVertices);
bool hasCycle(Graph<string>& graph, const vector<string>& verticesList);
bool DFS(Graph<string>& graph, const string& vertex, bool visited[], bool inStack[], stack<string>& verticesStack, const vector<string>& verticesList);

// main method 
int main(int argc, char *argv[]) {
	if(argc != 2) {
		cerr << "Usage: " << argv[0] << endl;
		return 1;
	}

	string filename = argv[1];
	vector<string> verticesList;
	Graph<string> myGraph = buildGraphFromFile(filename, verticesList);
	
	string start = startVertex(verticesList);
	
	dijkstra(myGraph, start, verticesList);
	
	hasCycle(myGraph, verticesList);
	
	return 0;
}

// method to print out sorted list of vertices and take user input of starting vertex 
string startVertex(const vector<string>& verticesList) {
	cout << "^^^^^^^^^^^^^^^^ DIJKSTRA’S ALGORITHM ^^^^^^^^^^^^^^^^" << endl;
	cout << "A Weighted Graph Has Been Built For These " << verticesList.size() << " Cities:\n" << endl;

	vector<string> sortedVertices = verticesList;
	sort(sortedVertices.begin(), sortedVertices.end());

	for(int i = 0; i < sortedVertices.size(); ++i) { // prints verticesList in sorted order (three per line)
		cout << setw(15) << left << sortedVertices[i];
		if((i + 1) % 3 == 0) {
			cout << endl;
		}
	}
	cout << endl;

	string startVertex;
	cout << "\nPlease input your starting vertex: ";
	cin >> startVertex;

	while (!(isValidVertex(verticesList, startVertex))) { // allow user to reinput startVertex if invalid
		cout << startVertex << "is an invalid vertex. Please enter a valid vertex: ";
		cin >> startVertex;
	}

	cout << "------------------------------------------------------------------" << endl;
	return startVertex;
}

// boolean method to determine if vertex is valid (cheks to see if it is in verticesList)
bool isValidVertex(const vector<string>& verticesList, const string& startVertex) {
	return find(verticesList.begin(), verticesList.end(), startVertex) != verticesList.end();
}

// method to initialize array information for 4 parallel arrays
void initializeArrayInfo(const vector<string>& verticesList, string vertexNames[], bool visited[], int distance[], string previousVertex[]) {
	for (int i = 0; i < verticesList.size(); ++i) {  
                vertexNames[i] = verticesList[i];
                visited[i] = false;
                distance[i] = INT_MAX;
                previousVertex[i] = "";
        }
}

// method to return the index number of a given vertex 
int getIndex(const vector<string>& verticesList, const string& vertex) {
	for (size_t i = 0; i < verticesList.size(); ++i) {
		if (verticesList[i] == vertex) {
			return i;
		}
	}
	return -1;
}

// method to print one row of Dijkstra's Summary Table 
void printSummaryTableRow(const string& vertex, int distance, const string& previousVertex) {
	cout << setw(25) << left << vertex << setw(25) << distance << setw(25) << previousVertex << endl;
}

// method to find the minimum distance among the current unmarked vertices 
int findMinDistance(const vector<string>& verticesList, bool visited[], int distance[], int numVertices) {
	int minDistance = INT_MAX;
	int minIndex = -1;
	for (int i = 0; i < numVertices; ++i) {
		if (!visited[i] && (distance[i] < minDistance)) {
			minDistance = distance[i];
			minIndex = i;
		}
	} 
	return minIndex;
}

// method to build graph from input file 
Graph<string> buildGraphFromFile(const string& filename, vector<string>& verticesList) {
	Graph<string> graph(50);
	ifstream inputFile(filename.c_str());

	if (!inputFile) {
		cerr << "Error: Unable to open input file." << endl;
		exit(1);
	}

	string origin; string destination;
	int tripLength;
	while (getline(inputFile, origin, ';') && getline(inputFile, destination, ';') && inputFile >> tripLength) {
		inputFile.ignore();
		if (!isValidVertex(verticesList, origin)) {
			graph.AddVertex(origin);
			verticesList.push_back(origin);
		}
		if (!isValidVertex(verticesList, destination)) {
			graph.AddVertex(destination);
			verticesList.push_back(destination);
		}	
		graph.AddEdge(origin, destination, tripLength);
	}	
	
	inputFile.close();
	
	return graph;
} 

// method to check if all vertices have been visited 
bool allVisited(bool visited[], int numVertices) {
	for(int i = 0; i < numVertices; ++i) {
		if (!visited[i]) {
			return false;
		}
	}
	return true;
}

// method to handle Dijkstra's algorithm logic 
void dijkstra(Graph<string>& graph, const string& startVertex, const vector<string>& verticesList) {
	int numVertices = verticesList.size();	
	string vertexNames[numVertices]; // to represent different vertices
	bool visited[numVertices]; // to "mark" vertices as visited 
	int distance[numVertices]; // to represent total distance to reach vertex
	string previousVertex[numVertices]; // to represent previous vertex visited
	
	initializeArrayInfo(verticesList, vertexNames, visited, distance, previousVertex);	

	// determine index of starting vertex 
	int startVertexIndex = getIndex(verticesList, startVertex);
	if(startVertexIndex == -1) {
		cerr << "Error: Start vertex not found." << endl;
		exit(1);
	}
	visited[startVertexIndex] = true;
	distance[startVertexIndex] = 0;
	previousVertex[startVertexIndex] = "N/A";
	
	cout << setw(25) << left << "Vertex" << setw(25) << "Distance" << setw(25) << "Previous\n" << endl;
	printSummaryTableRow(vertexNames[startVertexIndex], distance[startVertexIndex], previousVertex[startVertexIndex]);	
	
	string currentVertex = startVertex;
	int currentIndex = getIndex(verticesList, currentVertex);
	Queue<string> adjacentVertices;

	while (!allVisited(visited, numVertices)) { // until all vertices are marked 
		graph.GetToVertices(currentVertex, adjacentVertices); // determine adjacent vertices 	
		while (!adjacentVertices.isEmpty()) {
			string adjacentVertex = adjacentVertices.dequeue();
			int adjacentIndex = getIndex(verticesList, adjacentVertex);
			int weight = graph.WeightIs(currentVertex, adjacentVertex);
			if ((!visited[adjacentIndex]) && (distance[adjacentIndex] > distance[currentIndex] + weight)) { // reset distance value 
				distance[adjacentIndex] = distance[currentIndex] + weight;
				previousVertex[adjacentIndex] = vertexNames[currentIndex]; 
			}
		}
		// set currentVertex to an unmarked vertex wiith the minimum distance of all unmarked vertices 
		previousVertex[currentIndex] = currentVertex;
		currentIndex = findMinDistance(verticesList, visited, distance, numVertices);
		currentVertex = vertexNames[currentIndex];
		if(currentIndex == -1) { // all remaining vertices are unreachable
			break;
		}				
		printSummaryTableRow(vertexNames[currentIndex], distance[currentIndex], previousVertex[currentIndex]);
		visited[currentIndex] = true;
	} 

	cout << "\n------------------------------------------------------------------" << endl;
}

// method to serve as entry point for cycle detection
bool hasCycle(Graph<string>& graph, const vector<string>& verticesList) {
	int numVertices = verticesList.size();
	bool visited[numVertices]; // boolean array to track visited vertices
	bool inStack[numVertices]; // boolean array to track vertices in the stack
	stack<string> verticesStack; // stack to keep track of vertices in current traversal

	// for all vertices, initialize visited and inStack arrays to false 
	for(int i = 0; i < verticesList.size(); i++) { 
		visited[i] = false;
		inStack[i] = false;
		verticesStack.push(verticesList[i]);
	}

	while (!verticesStack.empty()) { // for all vertices in the graph 
		string vertex = verticesStack.top();
		verticesStack.pop();

		if ((!visited[getIndex(verticesList, vertex)]) && (DFS(graph, vertex, visited, inStack, verticesStack, verticesList))) { // if vertex has not been visited (all vertices), perform DFS traversal 
			cout << "The graph contains a cycle." << endl;
			return true;
		} 
	}
	
	cout << "The graph does not contain a cycle." << endl; // if no cycle is found after traversing all vertices 
	return false;
}

// method to perform depth first search traversal on given vertex and recursively explore adjacent vertices 
bool DFS(Graph<string>& graph, const string& vertex, bool visited[], bool inStack[], stack<string>& verticesStack, const vector<string>& verticesList) {
	// mark current vertex as visited and inStack, push to verticesStack
	visited[getIndex(verticesList, vertex)] = true;
	inStack[getIndex(verticesList, vertex)] = true;
	verticesStack.push(vertex);
	
	Queue<string> adjacentVertices;
	graph.GetToVertices(vertex, adjacentVertices); // get ajacent vertices of the current vertex 

	while (!adjacentVertices.isEmpty()) { // iterate through all adjacent vertices 
		string adjacentVertex = adjacentVertices.dequeue();
		int adjacentIndex = getIndex(verticesList, adjacentVertex);
		// if adjacent vertex has not been visited, recursively call DFS
		if (!visited[adjacentIndex]) {
			if(DFS(graph, adjacentVertex, visited, inStack, verticesStack, verticesList)) {
				return true;
			}
		} else if (inStack[adjacentIndex]) { // if adjacent vertex is in the current traversal path, a cycle exists 
			return true;
		}
	}

	inStack[getIndex(verticesList, vertex)] = false; // mark the current vertex 
	verticesStack.pop(); // remove current vertex from stack 
	return false; // no cycle found 
}
	
