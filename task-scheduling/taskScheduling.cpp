#include <iostream>
#include <vector>
#include <stack>
#include <random>
#include <string>
#include <ctime>
#include <limits>
#include <cstdlib> // For system()

using namespace std;

// Function prototypes
void addEdge(vector<vector<int>>& adjacencyList, int from, int to);
bool isCyclic(const vector<vector<int>>& adjacencyList, int numVertices);
bool isCyclicUtil(const vector<vector<int>>& adjacencyList, int v, vector<bool>& visited, vector<bool>& recursionStack);
void topologicalSortUtil(const vector<vector<int>>& adjacencyList, int v, vector<bool>& visited, stack<int>& stack, vector<int>& processingOrder, int depth);
vector<int> topologicalSort(const vector<vector<int>>& adjacencyList, int numVertices, vector<int>& processingOrder);
void printGraph(const vector<vector<int>>& adjacencyList, int numVertices);
vector<vector<int>> generateRandomDAG(int n, int m);
void clearScreen();
void displayMenu();
void handleMenuOption(int option, vector<vector<int>>& adjacencyList, int& n);
void pause();

int main() {
    vector<vector<int>> adjacencyList;
    int n = 0; 

    int menuOption;
    do {
        clearScreen();
        displayMenu();
        cout << "Select an option: ";
        cin >> menuOption;

        while (cin.fail() || menuOption < 0 || menuOption > 5) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid option. Please select a valid option (0-5): ";
            cin >> menuOption;
        }

        handleMenuOption(menuOption, adjacencyList, n);

    } while (menuOption != 0);

    return 0;
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear"); 
#endif
}

void displayMenu() {
    cout << "==============================================" << endl;
    cout << "        Task Scheduling System Menu   " << endl;
    cout << "==============================================" << endl;
    cout << "[1] Generate Random DAG" << endl;
    cout << "[2] Display Adjacency List Representation" << endl;
    cout << "[3] Display Topological Sort Process" << endl;
    cout << "[4] Display Stack to Result Transformation" << endl;
    cout << "[5] Summary (include final topological order)" << endl;
    cout << "[0] Exit" << endl;
    cout << "==============================================" << endl;
}

void handleMenuOption(int option, vector<vector<int>>& adjacencyList, int& n) {
    static vector<int> processingOrder;
    static vector<int> topologicalOrder;

    switch (option) {
    case 1: {
        cout << "\nGenerating a new Random DAG..." << endl;
        cout << "\nEnter the number of tasks (N): ";
        cin >> n;
        while (cin.fail() || n <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a positive integer for N: ";
            cin >> n;
        }

        cout << "Enter the number of dependencies (M): ";
        int m;
        cin >> m;
        while (cin.fail() || m < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a non-negative integer for M: ";
            cin >> m;
        }

        adjacencyList = generateRandomDAG(n, m);
        cout << "\nRandom DAG generated successfully!" << endl;
        pause();
        break;
    }
    case 2:
        if (adjacencyList.empty()) {
            cout << "\nNo generated DAG to display." << endl;
        }
        else {
            printGraph(adjacencyList, n);
        }
        pause();
        break;
    case 3:
        if (adjacencyList.empty()) {
            cout << "\nNo generated DAG to display the topological sort process." << endl;
        }
        else {
            cout << "\n[TOPOLOGICAL SORT PROCESS]" << endl << endl;
            processingOrder.clear();
            topologicalOrder = topologicalSort(adjacencyList, n, processingOrder);
        }
        pause();
        break;
    case 4:
        if (topologicalOrder.empty()) {
            cout << "\nNo topological sort result to display." << endl;
        }
        else {
            cout << "\n[STACK TO RESULT TRANSFORMATION]" << endl;
            cout << "Popping from stack (Order added to result):" << endl;

            std::stack<int> stackCopy;
            for (int i = 0; i < topologicalOrder.size(); i++) {
                stackCopy.push(topologicalOrder[i]);
            }

            vector<int> stackContents;
            int step = 1;
            while (!stackCopy.empty()) {
                int current = stackCopy.top();
                stackContents.push_back(current);
                stackCopy.pop();

                cout << "Step " << step++ << ": Popping Task " << current << endl;
                cout << "   Result so far: ";
                for (size_t i = 0; i < stackContents.size(); i++) {
                    cout << stackContents[i];
                    if (i < stackContents.size() - 1) cout << " -> ";
                }
                cout << endl;
            }
        }
        pause();
        break;
    case 5:
        if (topologicalOrder.empty()) {
            cout << "No topological order to summarize." << endl;
        }
        else {
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
            cout << endl;
        }
        pause();
        break;
    case 0:
        cout << "Exiting the program." << endl;
        break;
    default:
        cout << "Invalid option." << endl;
        break;
    }
}

void pause() {
    cout << endl; 
    cout << "Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    cin.get(); 
}

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
            return true; // cycle detected
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
    }
    else {
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
        }
        else {
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

    return processingOrder; // Return the processing order for use in option 4
}

void printGraph(const vector<vector<int>>& adjacencyList, int numVertices) {
    cout << "\nGraph Representation (Adjacency List):" << endl;
    for (int i = 0; i < numVertices; i++) {
        cout << "Task " << i << " -> ";
        for (size_t j = 0; j < adjacencyList[i].size(); j++) {
            cout << adjacencyList[i][j];
            if (j != adjacencyList[i].size() - 1) cout << ", ";
        }
        cout << endl;
    }
}

// Generate random DAG by ensuring edges only go from lower to higher numbered vertices
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

        // ensure DAG by only adding edges from lower to higher numbered vertices
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
