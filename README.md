
#The Guardians Battles
Este archivo README.md proporciona una visión detallada del código del juego "The Guardians Battles". A continuación, se profundiza en la estructura del código y se explican las principales funciones y características del juego.

##Estructura del Código
###1. Estructuras de Datos
City y Guardian
City: Representa una ciudad con un nombre y las ciudades conectadas a ella.
Guardian: Representa a un guardián con su nombre, poder, maestro y ciudad asociada.
TreeNode y GuardianTreeNode
TreeNode: Es una estructura básica para construir árboles binarios de búsqueda (BST) que se utiliza para organizar jerárquicamente a los guardianes.
GuardianTreeNode: Representa un nodo en el árbol de guardianes, con un guardián y sus aprendices.
UndirectedGraph
Representa un grafo no dirigido que conecta ciudades.
Utiliza una matriz de adyacencia para almacenar las conexiones entre ciudades.
###2. Clases
GuardianTreeNode
Contiene información sobre un guardián y sus aprendices.
Se utiliza para construir árboles jerárquicos de guardianes.
Node
Utilizado en la implementación de árboles binarios de búsqueda (BST).
Almacena un nodo en el árbol con un guardián.
UndirectedGraph
Representa el grafo no dirigido que conecta ciudades.
Contiene funciones para agregar bordes entre ciudades, imprimir información detallada del grafo y obtener información sobre las conexiones.
###3. Funciones de Manipulación de Datos
readCitiesFile y readGuardiansFile
Leen información sobre ciudades y guardianes desde archivos de configuración.
readCitiesFile construye conexiones entre ciudades en el grafo.
readGuardiansFile carga información sobre los guardianes.
insertNode, displayRanking, y displayHierarchy
insertNode: Inserta un nodo en el árbol de jerarquía de guardianes.
displayRanking: Muestra un ranking de candidatos a guardianes con poder entre 90 y 99.
displayHierarchy: Imprime la jerarquía de guardianes representada por el árbol.
###4. Lógica del Juego
int main
La función principal controla el flujo del juego y presenta un menú interactivo al usuario.
Permite al usuario realizar diversas acciones, como ver información sobre guardianes, la jerarquía de poder y el reino, presenciar batallas, y más.
presenciarBatalla
Simula batallas entre guardianes, con una lógica específica para actualizar los puntajes y roles de los guardianes.
El usuario elige un guardián para desafiar a un guardián local, y se determina el resultado de la batalla.
##Uso del Juego
El usuario puede interactuar con el juego seleccionando opciones en el menú principal. Puede explorar la información sobre los guardianes, conocer la jerarquía de poder, participar en batallas simuladas y más.
