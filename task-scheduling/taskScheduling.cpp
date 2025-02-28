#include <iostream>
#include <vector>
#include <stack>
#include <random>
#include <string>
#include <ctime>

using namespace std;

// function prototypes
void addEdge(vector<vector<int>>& adjacencyList, int from, int to);
bool isCyclic(const vector<vector<int>>& adjacencyList, int numVertices);
bool isCyclicUtil(const vector<vector<int>>& adjacencyList, int v, vector<bool>& visited, vector<bool>& recursionStack);
void topologicalSortUtil(const vector<vector<int>>& adjacencyList, int v, vector<bool>& visited, stack<int>& stack, vector<int>& processingOrder, int depth);
vector<int> topologicalSort(const vector<vector<int>>& adjacencyList, int numVertices, vector<int>& processingOrder);
void printGraph(const vector<vector<int>>& adjacencyList, int numVertices);
vector<vector<int>> generateRandomDAG(int n, int m);

void addEdge(vector<vector<int>>& adjacencyList, int from, int to) {
    for (int vertex : adjacencyList[from]) {
        if (vertex == to) return; 
    }
    adjacencyList[from].push_back(to);
}

bool isCyclic(const vector<vector<int>>& adjacencyList, int numVertices) {
    vector<bool> visited(numVertices, false);
    vector<bool> recursionStack(numVertices, false);
    
    for (int i = 0; i < numVertices; i++) {
        if (!visited[i] && isCyclicUtil(adjacencyList, i, visited, recursionStack)) {
            return true;
        }
    }
    return false;
}

// DFS-based cycle detection
bool isCyclicUtil(const vector<vector<int>>& adjacencyList, int v, vector<bool>& visited, vector<bool>& recursionStack) {
    visited[v] = true;
    recursionStack[v] = true;
    
    for (int neighbor : adjacencyList[v]) {
        if (!visited[neighbor]) {
            if (isCyclicUtil(adjacencyList, neighbor, visited, recursionStack)) {
                return true;
            }
        }
        else if (recursionStack[neighbor]) {
            return true; // cycle detected (guide question 2)
        }
    }
    
    recursionStack[v] = false;
    return false;
}

// DFS-based topological sort
void topologicalSortUtil(const vector<vector<int>>& adjacencyList, int v, vector<bool>& visited, 
                         stack<int>& stack, vector<int>& processingOrder, int depth) {
    cout << "Task " << v << " | Visiting..." << endl;
    visited[v] = true;
    processingOrder.push_back(v);
    
    cout << "Task " << v << " | Exploring neighbors: ";
    if (adjacencyList[v].empty()) {
        cout << "none (leaf node)" << endl;
    } else {
        for (size_t i = 0; i < adjacencyList[v].size(); i++) {
            cout << adjacencyList[v][i];
            if (i < adjacencyList[v].size() - 1) cout << ", ";
        }
        cout << endl;
    }
    
    for (int neighbor : adjacencyList[v]) {
        if (!visited[neighbor]) {
            cout << "Task " << v << " | -> [Task " << neighbor << "] (unvisited)" << endl;
            topologicalSortUtil(adjacencyList, neighbor, visited, stack, processingOrder, depth + 1);
        } else {
            cout << "Task " << v << " | -> [Task " << neighbor << "] (already visited)" << endl;
        }
    }
    
    stack.push(v);  // push to stack after all descendants are processed
    cout << "Task " << v << " | Finished (all descendants processed), pushing to stack" << endl;
}

vector<int> topologicalSort(const vector<vector<int>>& adjacencyList, int numVertices, vector<int>& processingOrder) {
    stack<int> stack;
    vector<bool> visited(numVertices, false);
    
    // find all starting points for DFS
    for (int i = 0; i < numVertices; i++) {
        if (!visited[i]) {
            cout << "Starting new DFS from Task " << i << " (not yet visited)" << endl;
            topologicalSortUtil(adjacencyList, i, visited, stack, processingOrder, 0);
            cout << endl;
        }
    }
    
    cout << "\n[STACK TO RESULT TRANSFORMATION]\n" << endl;
    cout << "Stack (bottom to top): ";
    
    std::stack<int> stackCopy = stack;
    vector<int> stackContents;
    while (!stackCopy.empty()) {
        stackContents.push_back(stackCopy.top());
        stackCopy.pop();
    }
    
    for (int i = stackContents.size() - 1; i >= 0; i--) {
        cout << stackContents[i];
        if (i > 0) cout << " | ";
    }
    cout << endl;
    
    // create final topological order by popping from stack
    vector<int> result;
    cout << "\nPopping from stack (Order added to result):" << endl;
    int step = 1;
    while (!stack.empty()) {
        int current = stack.top();
        result.push_back(current);
        stack.pop();
        
        cout << "Step " << step++ << ": Popping Task " << current << endl;
        cout << "   Result so far: ";
        for (size_t i = 0; i < result.size(); i++) {
            cout << result[i];
            if (i < result.size() - 1) cout << " -> ";
        }
        cout << endl;
    }
    
    return result;
}

void printGraph(const vector<vector<int>>& adjacencyList, int numVertices) {
    cout << "Graph Representation (Adjacency List):" << endl;
    for (int i = 0; i < numVertices; i++) {
        cout << "Task " << i << " -> ";
        for (size_t j = 0; j < adjacencyList[i].size(); j++) {
            cout << adjacencyList[i][j];
            if (j != adjacencyList[i].size() - 1) cout << ", ";
        }
        cout << endl;
    }
}

// generate random DAG by ensuring edges only go from lower to higher numbered vertices
vector<vector<int>> generateRandomDAG(int n, int m) {
    int maxEdges = (n * (n - 1)) / 2;
    if (m > maxEdges) {
        m = maxEdges;
        cout << "Number of edges limited to maximum possible: " << maxEdges << endl;
    }
    
    vector<vector<int>> adjacencyList(n);
    mt19937 rng(static_cast<unsigned>(time(nullptr))); // mersenne twister rng with seed from current time
    uniform_int_distribution<int> dist(0, n - 1);
    
    int edgesAdded = 0;
    while (edgesAdded < m) {
        int from = dist(rng);
        int to = dist(rng);
        
        // ensure DAG by only adding edges from lower to higher numbered vertices (guide question 1)
        if (from != to && from < to) {
            bool edgeExists = false;
            for (int v : adjacencyList[from]) {
                if (v == to) {
                    edgeExists = true;
                    break;
                }
            }
            
            if (!edgeExists) {
                addEdge(adjacencyList, from, to);
                edgesAdded++;
            }
        }
    }
    
    return adjacencyList;
}

int main() {
    int n, m;
    cout << "Enter the number of tasks (N): ";
    cin >> n;
    
    if (n <= 0) {
        n = 1;
        cout << "Using minimum number of tasks: 1" << endl;
    }
    
    cout << "Enter the number of dependencies (M): ";
    cin >> m;
    
    vector<vector<int>> adjacencyList = generateRandomDAG(n, m);
    
    cout << "\n[ADJACENCY LIST REPRESENTATION]" << endl << endl;
    printGraph(adjacencyList, n);
    
    if (isCyclic(adjacencyList, n)) {
        cout << "Error: The generated graph contains a cycle!" << endl;
        return 1;
    }
    
    cout << "\n[TOPOLOGICAL SORT PROCESS]" << endl << endl;
    
    vector<int> processingOrder;
    vector<int> topologicalOrder = topologicalSort(adjacencyList, n, processingOrder);
    
    cout << "\n[SUMMARY]" << endl;
    cout << "\nDFS Visiting sequence:" << endl;
    for (size_t i = 0; i < processingOrder.size(); i++) {
        cout << "Step " << i + 1 << ": Task " << processingOrder[i] << endl;
    }
    
    cout << "\n[FINAL TOPOLOGICAL ORDER]" << endl;
    for (size_t i = 0; i < topologicalOrder.size(); i++) {
        cout << "Task " << topologicalOrder[i];
        if (i < topologicalOrder.size() - 1) cout << " -> ";
    }
    cout << endl << endl;
    
    return 0;
}