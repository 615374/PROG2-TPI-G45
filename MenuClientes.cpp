#include <iostream>
#include <cstdlib>
#include "MenuClientes.h"
#include "Cliente.h"

using namespace std;

void menuClientes() {
    int op;
    Cliente aux;
    int pos;

    do {
        cout << "=================================================" << endl;
        cout << "     MODULO: GESTION CLIENTAS                    " << endl;
        cout << "=================================================" << endl;
        cout << "1. Registrar Nueva Clienta" << endl;
        cout << "2. Listar Todas las Clientas Activas" << endl;
        cout << "3. Dar de baja Clienta" << endl;
        cout << "0. Volver al Menu Principal" << endl;
        cout << "-------------------------------------------------" << endl;
        cout << "Seleccione una opcion: ";
        cin >> op;
        cout << endl;

        if (op == 1) {
            system("cls");
            if (aux.cargar() == true) {
                if (aux.escribirDisco()) {
                    cout << "\n[OK] Clienta guardada con exito.\n\n";
                } else {
                    cout << "\n[ERROR] No se pudo escribir el archivo.\n\n";
                }
                cin.ignore(1000, '\n');
                cout << "Presione ENTER para continuar...";
                cin.get();
            }
            system("cls");
        }
        else if (op == 2) {
            system("cls");
            pos = 0;
            cout << "=================================================" << endl;
            cout << "               LISTADO DE CLIENTAS               " << endl;
            cout << "=================================================" << endl;
            while (aux.leerDisco(pos)) {
                aux.mostrar();
                pos++;
            }
            if (pos == 0) {
                cout << "Archivo vacio.\n\n";
            }

            cout << "=================================================" << endl;
            cin.ignore(1000, '\n');
            cout << "\nPresione ENTER para continuar...";
            cin.get();
            system("cls");
        }
        else if (op == 3) {
            system("cls");

            darDeBajaCliente();

            system("cls");
        }
    } while (op != 0);

    system("cls");
}
