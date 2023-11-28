#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>

using namespace std;

// Estructuras de datos para ciudades y guardianes
struct City {
    string name;
    vector<string> connectedCities;
};

struct Guardian {
    string name;
    int power;
    string master;
    string city;
};

struct TreeNode {
    Guardian guardian;
    TreeNode* left;
    TreeNode* right;

    TreeNode(const Guardian& g) : guardian(g), left(nullptr), right(nullptr) {}
};

// Estructura para representar el grafo dirigido
struct Graph {
    unordered_map<string, vector<string>> adjacencyList;
    TreeNode* binaryTreeRoot;

    // Constructor para inicializar la raíz del árbol binario
    Graph() : binaryTreeRoot(nullptr) {}

    // Funciones para gestionar el árbol binario
    void addBinaryTreeNode(const Guardian& guardian);
    TreeNode* addBinaryTreeNodeRecursive(TreeNode* node, const Guardian& guardian);
    TreeNode* searchBinaryTreeNode(int power);
    TreeNode* searchBinaryTreeNodeRecursive(TreeNode* node, int power);
    void traverseBinaryTreePreorder(TreeNode* node);
    void traverseBinaryTreeInorder(TreeNode* node);  // Agregada aquí
    void traverseBinaryTreePostorder(TreeNode* node);  // Agregada aquí

    // Funciones para gestionar el grafo
    void addEdge(const string& city1, const string& city2);
    bool hasEdge(const string& city1, const string& city2);
    bool hasPath(const string& startCity, const string& endCity);
};

void Graph::addEdge(const string& city1, const string& city2) {
    adjacencyList[city1].push_back(city2);
    adjacencyList[city2].push_back(city1);
}

bool Graph::hasEdge(const string& city1, const string& city2) {
    auto it = adjacencyList.find(city1);
    if (it != adjacencyList.end()) {
        return std::find(it->second.begin(), it->second.end(), city2) != it->second.end();
    }
    return false;
}

bool Graph::hasPath(const string& startCity, const string& endCity) {
    unordered_map<string, bool> visited;
    queue<string> q;
    q.push(startCity);

    while (!q.empty()) {
        string currentCity = q.front();
        q.pop();

        if (currentCity == endCity) {
            return true;  // Se encontró un camino
        }

        if (visited[currentCity]) {
            continue;  // Evitar ciclos
        }

        visited[currentCity] = true;

        auto it = adjacencyList.find(currentCity);
        if (it != adjacencyList.end()) {
            for (const string& neighbor : it->second) {
                q.push(neighbor);
            }
        }
    }

    return false;  // No se encontró un camino
}

void Graph::addBinaryTreeNode(const Guardian& guardian) {
    binaryTreeRoot = addBinaryTreeNodeRecursive(binaryTreeRoot, guardian);
}

TreeNode* Graph::addBinaryTreeNodeRecursive(TreeNode* node, const Guardian& guardian) {
    if (node == nullptr) {
        return new TreeNode(guardian);
    }

    if (guardian.power < node->guardian.power) {
        node->left = addBinaryTreeNodeRecursive(node->left, guardian);
    } else {
        node->right = addBinaryTreeNodeRecursive(node->right, guardian);
    }

    return node;
}

TreeNode* Graph::searchBinaryTreeNode(int power) {
    return searchBinaryTreeNodeRecursive(binaryTreeRoot, power);
}

TreeNode* Graph::searchBinaryTreeNodeRecursive(TreeNode* node, int power) {
    if (node == nullptr || node->guardian.power == power) {
        return node;
    }

    if (power < node->guardian.power) {
        return searchBinaryTreeNodeRecursive(node->left, power);
    }

    return searchBinaryTreeNodeRecursive(node->right, power);
}

void Graph::traverseBinaryTreePreorder(TreeNode* node) {
    if (node != nullptr) {
        // Procesar el nodo actual antes de sus hijos
        traverseBinaryTreePreorder(node->right);  // Recorrer el subárbol derecho primero
        cout << "Nombre: " << node->guardian.name << " | Poder: " << node->guardian.power << endl;
        traverseBinaryTreePreorder(node->left);   // Luego recorrer el subárbol izquierdo
    }
}

void Graph::traverseBinaryTreeInorder(TreeNode* node) {
    if (node != nullptr) {
        traverseBinaryTreeInorder(node->left);
        cout << "Nombre: " << node->guardian.name << " | Poder: " << node->guardian.power << endl;
        traverseBinaryTreeInorder(node->right);
    }
}

void Graph::traverseBinaryTreePostorder(TreeNode* node) {
    if (node != nullptr) {
        traverseBinaryTreePostorder(node->left);
        traverseBinaryTreePostorder(node->right);
        cout << "Nombre: " << node->guardian.name << " | Poder: " << node->guardian.power << endl;
    }
}

// Funciones para leer archivos
void readCitiesFile(vector<City>& citiesVector, Graph& graph) {
    ifstream citiesFile("cities.conf.txt");

    if (citiesFile.is_open()) {
        string line;
        while (getline(citiesFile, line)) {
            size_t pos = line.find(", ");
            string cityName = line.substr(0, pos);
            string connectedCitiesStr = line.substr(pos + 2);
            vector<string> connectedCities;

            size_t commaPos;
            while ((commaPos = connectedCitiesStr.find(", ")) != string::npos) {
                connectedCities.push_back(connectedCitiesStr.substr(0, commaPos));
                connectedCitiesStr = connectedCitiesStr.substr(commaPos + 2);
            }

            connectedCities.push_back(connectedCitiesStr);

            City city;
            city.name = cityName;
            city.connectedCities = connectedCities;

            citiesVector.push_back(city);

            // Agregar aristas al grafo
            for (const auto& connectedCity : connectedCities) {
                graph.addEdge(cityName, connectedCity);
            }
        }
        citiesFile.close();
    } else {
        cerr << "No se pudo abrir el archivo cities.txt" << endl;
    }
}

void readGuardiansFile(vector<Guardian>& guardians, Graph& graph) {
    ifstream guardiansFile("guardians.conf.txt");

    if (guardiansFile.is_open()) {
        string line;
        while (getline(guardiansFile, line)) {
            size_t pos1 = line.find(",");
            size_t pos2 = line.find(",", pos1 + 1);
            size_t pos3 = line.find(",", pos2 + 1);

            string guardianName = line.substr(0, pos1);
            int power = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
            string master = line.substr(pos2 + 1, pos3 - pos2 - 1);
            string city = line.substr(pos3 + 1);

            Guardian guardian;
            guardian.name = guardianName;
            guardian.power = power;
            guardian.master = master;
            guardian.city = city;

            guardians.push_back(guardian);

            // Agregar al árbol binario
            graph.addBinaryTreeNode(guardian);
        }
        guardiansFile.close();
    } else {
        cerr << "No se pudo abrir el archivo guardians.txt" << endl;
    }
}

int main() {
    vector<City> citiesVector;
    vector<Guardian> guardians;
    Graph graph;

    readCitiesFile(citiesVector, graph);
    readGuardiansFile(guardians, graph);

    int opcion;

    do {
        // Menú principal
        cout << "\nMenu:\n" << endl;
        cout << "1. Ver la lista de candidatos" << endl;
        cout << "2. Ver al guardian" << endl;
        cout << "3. Conocer el reino" << endl;
        cout << "4. Presenciar una Batalla" << endl;
        cout << "5. Salir" << endl;
        cout << "\nIngrese su opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                // Lógica para ver la lista de candidatos
                cout << "\nInformacion de los Guardianes\n" << endl;
                for (const auto& guardian : guardians) {
                    cout << "Nombre: " << guardian.name << " | Poder: " << guardian.power
                         << " | Maestro: " << guardian.master << " | Ciudad: " << guardian.city << endl;
                }
                break;

            case 2:
                // Lógica para ver al guardian
                cout << "Mostrar al guardian" << endl;

                    // Recorrer el árbol binario en preorden
				    cout << "\nRecorrido del arbol binario en preorden:\n";
				    graph.traverseBinaryTreePreorder(graph.binaryTreeRoot);
				
				    // Recorrer el árbol binario en inorden
				    cout << "\nRecorrido del arbol binario en inorden:\n";
				    graph.traverseBinaryTreeInorder(graph.binaryTreeRoot);
				
				    // Recorrer el árbol binario en postorden
				    cout << "\nRecorrido del arbol binario en postorden:\n";
				    graph.traverseBinaryTreePostorder(graph.binaryTreeRoot);
                break;

            case 3:
                // Lógica para conocer el reino
                cout << "\nInformacion del reino\n" << endl;
                for (const auto& city : citiesVector) {
                    cout << "Nombre: " << city.name << " | Conectadas: ";
                    for (const auto& connectedCity : city.connectedCities) {
                        cout << connectedCity << " ";
                    }
                    cout << endl;
                }
                break;

            case 4:
                // Lógica para presenciar una batalla
                cout << "Presenciar una batalla" << endl;
                break;

            case 5:
                cout << "Saliendo del programa." << endl;
                break;

            default:
                cout << "Opcion no valida. Inténtelo de nuevo." << endl;
        }
    } while (opcion != 5);

    return 0;
}