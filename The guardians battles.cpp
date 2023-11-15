#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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

// Función principal del menú
int main() {
    vector<City> citiesVector;
    vector<Guardian> guardians;

    readCitiesFile(citiesVector);
    readGuardiansFile(guardians);

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
            	system("cls");
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
                system("cls");
                break;

            case 3:
            	system("cls");
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
                system("cls");
                break;

            case 5:
            	system("cls");
                cout << "Saliendo del programa." << endl;
                break;

            default:
                cout << "Opcion no valida. Inténtelo de nuevo." << endl;
        }

    } while (opcion != 5);

    return 0;
}


