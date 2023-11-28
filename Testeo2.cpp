#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

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

// Funciones para leer archivos
void readCitiesFile(vector<City>& citiesVector) {
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
        }
        citiesFile.close();
    } else {
        cerr << "No se pudo abrir el archivo cities.txt" << endl;
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

void displayRanking(TreeNode* root) {
    if (root == nullptr) return;

    displayRanking(root->left);
    
    // Agregar condición para imprimir solo guardianes con poder entre 90 y 99
    if (root->guardian.power >= 90 && root->guardian.power < 100) {
        cout << "Nombre: " << root->guardian.name << ", Poder: " << root->guardian.power << endl;
    }
    
    displayRanking(root->right);
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
        // Puedes agregar más información aquí si es necesario
    } else {
        cout << "Numero de guardian no valido." << endl;
    }
}

int main() {
	vector<City> citiesVector;
	vector<Guardian> guardians;
	TreeNode* rankingTree = nullptr;
	
	readCitiesFile(citiesVector);
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
                cout <<"\n\nJerarquia\n";
                displayHierarchy(rankingTree, 0);

                break;

            case 2:
                // Lógica para ver la lista de candidatos
                cout << "\nRanking de candidatos a Guardianes (Poder entre 90 y 99)\n" << endl;
    			displayRanking(rankingTree);
                break;

            case 3:
                // Lógica para ver al guardian
                cout << "Mostrar al guardian" << endl;
                verInformacionGuardian(guardians);
                
                break;

            case 4:
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

            case 5:
            	
                cout << "Presenciar una batalla" << endl;

                break;
            
            case 6:
            	// Lógica para presenciar una batalla
            	cout << "Saliendo del programa." << endl;
            	deleteTree(rankingTree);
            	break;

            default:
                cout << "Opcion no valida. Inténtelo de nuevo." << endl;
        }

    } while (opcion != 6);

    return 0;
}