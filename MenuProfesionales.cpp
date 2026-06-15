#include <iostream>
#include <cstdlib>
#include "MenuProfesionales.h"
#include "Profesional.h"

using namespace std;

void menuProfesionales() {
    int op;
    Profesional aux;
    int pos;

    do {
        cout << "=================================================" << endl;
        cout << "     MODULO: GESTION PROFESIONALES               " << endl;
        cout << "=================================================" << endl;
        cout << "1. Registrar Profesional" << endl;
        cout << "2. Listar Profesionales Activos" << endl;
        cout << "3. Dar de baja Profesional" << endl;
        cout << "0. Volver al Menu Principal" << endl;
        cout << "-------------------------------------------------" << endl;
        cout << "Seleccione una opcion: ";
        cin >> op;
        cout << endl;

        if (op == 1) {
            system("cls");
            if (aux.cargar() == true) {
                if (aux.escribirDisco()) {
                    cout << "\n[OK] Profesional guardado con exito.\n\n";
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
            cout << "=== LISTADO DE PROFESIONALES ===\n";
            while (aux.leerDisco(pos)) {
                aux.mostrar();
                pos++;
            }
            if (pos == 0) {
                cout << "Archivo vacio.\n\n";
            }
            cin.ignore(1000, '\n');
            cout << "\nPresione ENTER para continuar...";
            cin.get();
            system("cls");
        }
        else if (op == 3) {
            system("cls");

            darDeBajaProfesional();

            system("cls");
        }
    } while (op != 0);
    system("cls");
}
