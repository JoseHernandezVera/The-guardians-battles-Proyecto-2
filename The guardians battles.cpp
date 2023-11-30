#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdlib>
#include <ctime>

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
    TreeNode* left;    // Maestro
    TreeNode* right;   // Aprendiz
    bool isMaster;     // Bandera que indica si el nodo es un maestro
    TreeNode(Guardian g, bool isMaster) : guardian(g), left(nullptr), right(nullptr), isMaster(isMaster) {}
};

class GuardianTreeNode {
public:
    Guardian guardian;
    vector<GuardianTreeNode*> apprentices;
    GuardianTreeNode* master;

    GuardianTreeNode(const Guardian& g) : guardian(g), master(nullptr) {}
};

class Node {
public:
    Guardian guardian;
    Node* left;
    Node* right;

    Node(Guardian g) : guardian(g), left(nullptr), right(nullptr) {}
};

class UndirectedGraph {
private:
    unordered_map<string, int> cityIndexMap;
    vector<vector<int>> adjacencyMatrix;
    vector<string> cities;
    unordered_map<string, GuardianTreeNode*> cityGuardianTrees;
    unordered_map<string, GuardianTreeNode*> cityMasterTrees;

public:
    UndirectedGraph() = default;

    void addEdge(const string& src, const string& dest, int weight) {
        int srcIndex = getIndex(src);
        int destIndex = getIndex(dest);

        adjacencyMatrix[srcIndex][destIndex] = weight;
        adjacencyMatrix[destIndex][srcIndex] = weight; 
    }
	Node* deleteNodeFromBST(Node* root, const Guardian& guardian) {
    if (root == nullptr) {
        return root;
    } else if (guardian.power < root->guardian.power) {
        root->left = deleteNodeFromBST(root->left, guardian);
    } else if (guardian.power > root->guardian.power) {
        root->right = deleteNodeFromBST(root->right, guardian);
    } else {
        if (root->left == nullptr && root->right == nullptr) {
            delete root;
            root = nullptr;
        } else if (root->left == nullptr) {
            Node* temp = root;
            root = root->right;
            delete temp;
        } else if (root->right == nullptr) {
            Node* temp = root;
            root = root->left;
            delete temp;
        } else {
            Node* temp = findMin(root->right);
            root->guardian = temp->guardian;
            root->right = deleteNodeFromBST(root->right, temp->guardian);
        }
    }
    return root;
}

	Node* findMin(Node* root) {
    	if (root == nullptr) {
        	return nullptr;
    	}
    	while (root->left != nullptr) {
    	    root = root->left;
    	}
    	return root;
	}

    size_t getNumberOfCities() const {
        return cities.size();
    }

    // Obtener el nombre de una ciudad por su índice
    string getCityName(size_t index) const {
        if (index < cities.size()) {
            return cities[index];
        }
        return "";
    }

    int getIndex(const string& city) {
        if (cityIndexMap.find(city) == cityIndexMap.end()) {
            int newIndex = cities.size();
            cities.push_back(city);
            cityIndexMap[city] = newIndex;

            // Ajustar la matriz de adyacencia al tamaño correcto
            for (auto& row : adjacencyMatrix) {
                row.resize(cities.size(), 0);
            }
            adjacencyMatrix.resize(cities.size(), vector<int>(cities.size(), 0));

            return newIndex;
        }
        return cityIndexMap[city];
    }
    //Funcion para imprimir la ciudad ->Sus conexiones
    void printDetailedGraph() {
        cout << "\nCiudades y sus conexiones:\n" << endl;
        for (size_t i = 0; i < cities.size(); ++i) 
        {
            cout << cities[i] << ", Sus conexiones: ";
            bool hasConnection = false;
            for (size_t j = 0; j < cities.size(); ++j) {
                if (adjacencyMatrix[i][j] != 0) {
                    if (hasConnection) {
                        cout << ", ";
                    }
                    cout << cities[j];
                    hasConnection = true;
                }
            }
            if (!hasConnection) {
                cout << "Ninguna conexion";
            }
            cout << endl;
        }
    }

    void printCityConnections(const string& city) {
        int cityIndex = getIndex(city);

        cout << city << " conectada a: ";
        bool hasConnection = false;
        for (size_t i = 0; i < cities.size(); ++i) {
            if (adjacencyMatrix[cityIndex][i] != 0) {
                cout << cities[i] << " | ";
                hasConnection = true;
            }
        }
        if (!hasConnection) {
            cout << "Ninguna conexion";
        }
        cout << endl;
    }
    void addGuardianToCity(const string& city, const Guardian& guardian) {
        if (cityGuardianTrees.find(city) == cityGuardianTrees.end()) {
            cityGuardianTrees[city] = new GuardianTreeNode(guardian);
        } else {
            addApprentice(cityGuardianTrees[city], new GuardianTreeNode(guardian));
        }
    }

    void printGuardiansInCity(const string& city, const vector<Guardian>& guardians) {
        cout << "Guardianes en la ciudad de " << city << ":" << endl;

        if (cityGuardianTrees.find(city) != cityGuardianTrees.end()) {
            GuardianTreeNode* cityGuardianTree = cityGuardianTrees[city];

            // Imprimir el maestro y sus aprendices en la ciudad
            cout << "Maestro: " << cityGuardianTree->guardian.name << endl;
            for (const auto& apprentice : cityGuardianTree->apprentices) {
                cout << "- " << apprentice->guardian.name << endl;
            }

            // Imprimir los maestros de los aprendices en la misma ciudad
            for (const auto& apprentice : cityGuardianTree->apprentices) {
                for (const auto& guardian : guardians) {
                    if (guardian.name == apprentice->guardian.master && guardian.city == city) {
                        cout << "Maestro de " << apprentice->guardian.name << ": " << guardian.name << endl;
                        break;
                    }
                }
            }
        } else {
            cout << "No hay guardianes en esta ciudad." << endl;
        }
    }
    vector<string> getConnectedCities(const string& city) {
        vector<string> connectedCities;
        int cityIndex = getIndex(city);

        cout << "Ciudades conectadas a " << city << ":" << endl;
        for (size_t i = 0; i < cities.size(); ++i) {
            if (adjacencyMatrix[cityIndex][i] != 0) {
                connectedCities.push_back(cities[i]);
                cout << cities[i] << endl;
            }
        }

        return connectedCities;
    }
  
     // Función para obtener el árbol general de guardianes de una ciudad específica
    GuardianTreeNode* getMasterTreeOfCity(const string& city) {
        if (cityMasterTrees.find(city) != cityMasterTrees.end()) {
            return cityMasterTrees[city];
        }
        return nullptr;
    }
    
    bool isConnected(const string& cityA, const string& cityB) {
        int indexA = getIndex(cityA);
        int indexB = getIndex(cityB);

        if (adjacencyMatrix[indexA][indexB] != 0 || adjacencyMatrix[indexB][indexA] != 0) {
            cout << cityA << " y " << cityB << " estan conectadas directamente." << endl;
            return true;
        } else {
            cout << cityA << " y " << cityB << " no estan conectadas directamente." << endl;
            return false;
        }
    }
    
   void printGuardiansInCityWithOpponent(const string& city, vector<Guardian>& guardians, Node*& powerTree) {
    cout << "Guardianes en la ciudad de " << city << ":" << endl;

       Guardian chosenGuardian;
        if (cityGuardianTrees.find(city) != cityGuardianTrees.end()) {
            GuardianTreeNode* cityGuardianTree = cityGuardianTrees[city];

            // Imprimir el maestro y sus aprendices en la ciudad
            cout << "Maestro: " << cityGuardianTree->guardian.name << endl;
            for (const auto& apprentice : cityGuardianTree->apprentices) {
                cout << "- " << apprentice->guardian.name << endl;
            }

            cout << "-------------------------------------" << endl;
            cout << "Guardianes disponibles para la batalla:" << endl;

            // Mostrar la lista de guardianes disponibles con su índice
            cout << "1) " << cityGuardianTree->guardian.name << "(Maestro)" << endl;
            int index = 2;
            for (const auto& apprentice : cityGuardianTree->apprentices) {
                cout << index << ") " << apprentice->guardian.name << "(Aprendiz)" << endl;
                index++;
            }

            cout << "Elige el numero del guardian para la batalla: ";
            int chosenOpponentIndex;
            cin >> chosenOpponentIndex;

            int totalGuardiansInCity = cityGuardianTree->apprentices.size() + 1;

            if (chosenOpponentIndex >= 1 && chosenOpponentIndex <= totalGuardiansInCity) {
                int battleResult = 0;
                if (chosenOpponentIndex == 1) {
                    // Lógica para pelear con el Maestro
                    cout << "Has elegido pelear contra el Maestro: " << cityGuardianTree->guardian.name << "!" << endl;
                    // Asignar puntos al elegir pelear contra el Maestro
                    battleResult = 5;
                    chosenGuardian = cityGuardianTree->guardian;
                } else {
                    // Lógica para pelear con el Aprendiz
                    chosenGuardian = cityGuardianTree->apprentices[chosenOpponentIndex - 2]->guardian;
                    cout << "Has elegido pelear contra: " << chosenGuardian.name << endl;
                    // Asignar puntos al elegir pelear contra un Aprendiz
                    battleResult = 3;
                }

                // Actualizar el árbol de poder si se proporciona
                if (powerTree != nullptr) {
                    powerTree = deleteNodeFromBST(powerTree, chosenGuardian);
                }
            } else {
                cout << "Numero de guardián no válido." << endl;
            }
        } else {
            cout << "No hay guardianes en esta ciudad." << endl;
        }
    }
    private:
        // Función para insertar un aprendiz bajo un maestro en el árbol general
        void addApprentice(GuardianTreeNode* master, GuardianTreeNode* apprentice) {
            apprentice->master = master;
            master->apprentices.push_back(apprentice);
        }
};

// Funciones para leer archivos
void readCitiesFile(UndirectedGraph& graph, std::vector<City>& citiesVector) {
    ifstream citiesFile("cities.conf.txt");

    if (citiesFile.is_open()) {
        string line;
        while (getline(citiesFile, line)) {
            size_t pos = line.find(",");
            string cityName = line.substr(0, pos);
            string connectedCitiesStr = line.substr(pos + 1);
            vector<string> connectedCities;

            size_t commaPos;
            while ((commaPos = connectedCitiesStr.find(",")) != string::npos) {
                connectedCities.push_back(connectedCitiesStr.substr(0, commaPos));
                connectedCitiesStr = connectedCitiesStr.substr(commaPos + 1);
            }

            connectedCities.push_back(connectedCitiesStr);

            City city;
            city.name = cityName;
            city.connectedCities = connectedCities;

            citiesVector.push_back(city);

            // Agregar las conexiones al grafo
            for (const auto& connectedCity : connectedCities) {
                graph.addEdge(cityName, connectedCity, 1);  // Peso de conexión 1
            }
        }
        citiesFile.close();
    } else {
        cerr << "No se pudo abrir el archivo cities.conf" << endl;
    }
}

void readGuardiansFile(vector<Guardian>& guardians) {
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
        }
        guardiansFile.close();
    } else {
        cerr << "No se pudo abrir el archivo guardians.txt" << endl;
    }
}

void insertNode(TreeNode*& root, Guardian guardian) {
    if (!root) {
        root = new TreeNode(guardian, true); // El primer guardián es siempre un maestro
        return;
    }

    if (guardian.master == root->guardian.name) {
        insertNode(root->left, guardian); // Insertar como aprendiz
    } else if (guardian.power < root->guardian.power) {
        insertNode(root->right, guardian);
    } else {
        insertNode(root->left, guardian);
    }
}

void displayRanking(const vector<Guardian>& guardians) {
    cout << "\nRanking de candidatos a Guardianes (Poder entre 90 y 99)\n" << endl;
    for (const auto& guardian : guardians) {
        if (guardian.power >= 90 && guardian.power < 100) {
            cout << "Nombre: " << guardian.name << ", Poder: " << guardian.power << endl;
        }
    }
}

void displayHierarchy(TreeNode* root, int nivel) {
    if (root) {
        for (int i = 0; i < nivel; ++i) {
            cout << "  ";
        }
        cout << "- " << root->guardian.name << " (" << root->guardian.city << ")\n";

        // Llamada recursiva para imprimir los hermanos del guardián actual
        displayHierarchy(root->left, nivel + 1);
        // Llamada recursiva para imprimir los aprendices del guardián actual
        displayHierarchy(root->right, nivel + 1);
    }
}

void deleteTree(TreeNode* root) {
    if (root) {
        deleteTree(root->left);
        deleteTree(root->right);
        delete root;
        root = nullptr;
    }
}

void verInformacionGuardian(const vector<Guardian>& guardians) {
    cout << "Guardianes disponibles:" << endl;
    for (int i = 0; i < guardians.size(); ++i) {
        cout << i + 1 << ". " << guardians[i].name << endl;
    }

    int chosenGuardianIndex;
    cout << "Elige el numero del guardian para ver su informacion: ";
    cin >> chosenGuardianIndex;

    if (chosenGuardianIndex >= 1 && chosenGuardianIndex <= guardians.size()) {
        Guardian chosenGuardian = guardians[chosenGuardianIndex - 1];
        cout << "\nInformacion del guardian seleccionado:\n" << endl;
        cout << "Nombre: " << chosenGuardian.name << endl;
        cout << "Poder: " << chosenGuardian.power << endl;
        cout << "Maestro: " << chosenGuardian.master << endl;
        cout << "Ciudad: " << chosenGuardian.city << endl;
    } else {
        cout << "Numero de guardian no valido." << endl;
    }
}

void conocerReino(UndirectedGraph& graph, const vector<Guardian>& guardians) {
    cout << "1. Ver la lista de ciudades y sus caminos" << endl;
    cout << "2. Conocer los caminos de una ciudad especifica" << endl;
    cout << "3. Conocer caminos entre ciudades" << endl;
    cout << "4. Agregar nuevos caminos entre ciudades" << endl;

    int opcion;
    cout << "Ingresa tu eleccion: ";
    cin >> opcion;

    switch (opcion) {
        case 1:
            graph.printDetailedGraph();
            break;
        case 2:
            cout << "\nCiudades disponibles:" << endl;
            for (size_t i = 0; i < graph.getNumberOfCities(); ++i) {
                cout << i + 1 << ". " << graph.getCityName(i) << endl;
            }

            int cityNumber;
            cout << "\nIngresa el numero de la ciudad para conocer sus caminos ";
            cin >> cityNumber;

            if (cityNumber >= 1 && cityNumber <= graph.getNumberOfCities()) {
                string cityToCheck = graph.getCityName(cityNumber - 1);
                graph.printCityConnections(cityToCheck);
            } else {
                cout << "Numero de ciudad invalido." << endl;
            }
            break;
        case 3:
		    cout << "\nCiudades disponibles:" << endl;
		    for (size_t i = 0; i < graph.getNumberOfCities(); ++i) {
		        cout << i + 1 << ". " << graph.getCityName(i) << endl;
		    }
		
		    int cityNumberA, cityNumberB;
		    cout << "\nIngresa el numero de la primera ciudad: ";
		    cin >> cityNumberA;
		    cout << "Ingresa el numero de la segunda ciudad: ";
		    cin >> cityNumberB;
		
		    if (cityNumberA >= 1 && cityNumberA <= graph.getNumberOfCities() &&
		        cityNumberB >= 1 && cityNumberB <= graph.getNumberOfCities()) {
		        string cityA = graph.getCityName(cityNumberA - 1);
		        string cityB = graph.getCityName(cityNumberB - 1);
		
		        int weight = 1;
		
		        graph.isConnected(cityA, cityB);
		    } else {
		        cout << "Numero de ciudad(es) invalido(s)." << endl;
		    }
		    break;
        case 4:
            cout << "\nAgregar un nuevo camino entre ciudades" << endl;

            cout << "Ciudades disponibles:" << endl;
            for (size_t i = 0; i < graph.getNumberOfCities(); ++i) {
                cout << i + 1 << ". " << graph.getCityName(i) << endl;
            }
            
            cout << "\nIngresa el numero de la primera ciudad: ";
            cin >> cityNumberA;
            cout << "Ingresa el numero de la segunda ciudad: ";
            cin >> cityNumberB;

            if (cityNumberA >= 1 && cityNumberA <= graph.getNumberOfCities() &&
                cityNumberB >= 1 && cityNumberB <= graph.getNumberOfCities()) {
                string cityA = graph.getCityName(cityNumberA - 1);
                string cityB = graph.getCityName(cityNumberB - 1);

                int weight = 1;

                graph.addEdge(cityA, cityB, weight);
            } else {
                cout << "Numero de ciudad(es) invalido(s)." << endl;
            }
            break;
        default:
            cout << "Opcion invalida. Por favor, elige una opcion valida." << endl;
            break;
    }
}
// Función para presenciar una batalla con la lógica especificada
void presenciarBatalla(UndirectedGraph& graph, vector<Guardian>& guardians) {
    srand(time(0));  // Inicializar la semilla del generador de numeros aleatorios

    // Filtrar guardianes con menos de 90 puntos de poder y excluyendo al Gran Maestro
    vector<Guardian> eligibleGuardians;
    for (const auto& guardian : guardians) {
        if (guardian.power < 90 && guardian.name != "Gran Maestro") {
            eligibleGuardians.push_back(guardian);
        }
    }

    // Imprimir guardianes elegibles para la batalla
    cout << "\nGuardianes para Batalla (con menos de 90 puntos de poder y excluyendo al Gran Maestro):" << endl;
    for (size_t i = 0; i < eligibleGuardians.size(); ++i) {
        cout << i + 1 << ". " << eligibleGuardians[i].name << " - Poder: " << eligibleGuardians[i].power << " - Ciudad: " << eligibleGuardians[i].city << endl;
    }

    // Solicitar al usuario que ingrese el índice del guardián para la batalla
    int chosenGuardianIndex;
    cout << "Ingrese el numero del guardián que desea para la batalla: ";
    cin >> chosenGuardianIndex;

    // Verificar si el índice es válido
    if (chosenGuardianIndex >= 1 && chosenGuardianIndex <= static_cast<int>(eligibleGuardians.size())) {
        Guardian chosenGuardian = eligibleGuardians[chosenGuardianIndex - 1];
        cout << "\nHas elegido pelear con: " << chosenGuardian.name << " - Poder: " << chosenGuardian.power << " - Ciudad: " << chosenGuardian.city << endl;

        // Filtrar guardianes locales que pertenecen a la misma ciudad que el retador
        vector<Guardian> localGuardians;
        for (const auto& guardian : guardians) {
            if (guardian.city == chosenGuardian.city && guardian.name != chosenGuardian.name) {
                localGuardians.push_back(guardian);
            }
        }

        // Imprimir guardianes locales para la batalla
        cout << "\nGuardianes Locales para Batalla en la Ciudad " << chosenGuardian.city << ":" << endl;
        for (size_t i = 0; i < localGuardians.size(); ++i) {
            cout << i + 1 << ". " << localGuardians[i].name << " - Poder: " << localGuardians[i].power << " - Ciudad: " << localGuardians[i].city << endl;
        }

        // Solicitar al usuario que elija el campeón local
        int localChampionIndex;
        cout << "Elige el numero del guardián local para la batalla: ";
        cin >> localChampionIndex;

        // Verificar si el índice del campeón local es válido
        if (localChampionIndex >= 1 && localChampionIndex <= static_cast<int>(localGuardians.size())) {
            Guardian localChampion = localGuardians[localChampionIndex - 1];

            // Simular la batalla y calcular el resultado mediante un numero aleatorio
            bool retadorWins = rand() % 10 < 4;  // Probabilidad de 40% de ganar

            // Actualizar puntajes y roles según la lógica especificada
            if (retadorWins) {
                int battleResult = (localChampion.name == "Maestro") ? 5 : 3;
                chosenGuardian.power += battleResult;

                if (battleResult > 0) {
                    // El retador supera al maestro
                    if (chosenGuardian.power > guardians[0].power) {
                        guardians[0] = chosenGuardian;
                        cout << "El retador ha superado al Gran Maestro y ahora es el nuevo Gran Maestro" << endl;
                    }

                    // Reducir puntaje a guardianes derrotados
                    for (auto& guardian : guardians) {
                        if (guardian.name != chosenGuardian.name) {
                            guardian.power = max(0, guardian.power - 1);
                        }
                    }
                }
            } else {
                cout << "El retador ha perdido la batalla contra " << localChampion.name << endl;
            }

            // Verificar si hay más de 4 guardianes con poder entre 90 y 99
            int candidatesCount = 0;
            for (const auto& guardian : guardians) {
                if (guardian.power >= 90 && guardian.power < 100) {
                    candidatesCount++;
                }
            }

            // Si hay más de 4, imprimir a los candidatos a caballero
            if (candidatesCount > 4) {
                cout << "\nCandidatos a Caballero:\n" << endl;
                for (const auto& guardian : guardians) {
                    if (guardian.power >= 90 && guardian.power < 100) {
                        cout << "- " << guardian.name << " - Poder: " << guardian.power << endl;
                    }
                }
            }
        } else {
            cout << "Numero del guardián local no válido. Batalla cancelada." << endl;
        }
    } else {
        cout << "Numero de guardián no válido. Batalla cancelada." << endl;
    }
}

int main() {
	vector<City> citiesVector;
	vector<Guardian> guardians;
	TreeNode* rankingTree = nullptr;
	UndirectedGraph graph;
	
    readCitiesFile(graph, citiesVector);
	readGuardiansFile(guardians);
    int opcion;

    
    for (const auto& guardian : guardians) {
        insertNode(rankingTree, guardian);
    }
	
	
	do{
	    // Menú principal
	    cout << "\nMenu\n\n";
        cout << "1. Mostar datos" << endl;
        cout << "2. Ver la lista de candidatos" << endl;
        cout << "3. Ver al guardian" << endl;
        cout << "4. Conocer el reino" << endl;
        cout << "5. Presenciar una Batalla" << endl;
        cout << "6. Salir" << endl;
        cout << "\nIngrese su opcion: ";
        cin >> opcion;

        switch (opcion) {
        	
            case 1:
                //Mostrar todos los datos
                cout << "\nInformacion de los Guardianes\n" << endl;
                for (const auto& guardian : guardians) {
                    cout << "Nombre: " << guardian.name << " | Poder: " << guardian.power
                         << " | Maestro: " << guardian.master << " | Ciudad: " << guardian.city << endl;
                }
                
                cout << "\n\nInformacion de las Ciudades\n";
                
                for (const auto& city : citiesVector) {
                    cout << "Nombre: " << city.name << " | Conectadas: ";
                    for (const auto& connectedCity : city.connectedCities) {
                        cout << connectedCity << " ";
                    }
                    cout << endl;
                }
                cout <<"\n\nJerarquia\n";
                displayHierarchy(rankingTree, 0);

                break;

            case 2:
                // Lógica para ver la lista de candidatos
			    displayRanking(guardians);
                break;

            case 3:
                // Lógica para ver al guardian
                cout << "Mostrar al guardian" << endl;
                verInformacionGuardian(guardians);
                
                break;

            case 4:
            	// Lógica para conocer el reino
            	cout << "\nInformacion del reino\n" << endl;
                conocerReino(graph, guardians);
                break;

            case 5:
            	// Lógica para presenciar una batalla
                cout << "Presenciar una batalla" << endl;
			    presenciarBatalla(graph, guardians);
                break;
            
            case 6:
            	cout << "Saliendo del programa." << endl;
            	deleteTree(rankingTree);
            	break;

            default:
                cout << "Opcion no valida. Inténtelo de nuevo." << endl;
        }

    } while (opcion != 6);

    return 0;
}