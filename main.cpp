#include <iostream>
#include "Cliente.h"

using namespace std;

int main() {
    // Objeto de la clase Cliente
    Cliente obj;

    cout << "--- PRUEBA DE CARGA DE CLIENTE ---" << endl;

    // Llamamos al método Cargar para cargar datos por teclado
    obj.cargar();

    cout << endl << "--- DATOS INGRESADOS ---" << endl;

    // Mostramos lo que se cargó en memoria
    obj.mostrar();

    return 0;
}
