#include <iostream>
#include <cstdlib>
//#include "Cliente.h"
#include "Profesional.h"  // Lo va a desmarcar María cuando cree su archivo
#include "Servicio.h"     // Lo va a desmarcar María cuando cree su archivo
//#include "ServicioXProfesional.h" //Sugerencia del Profe
// #include "Turno.h"         // Lo va a desmarcar Sol cuando cree su archivo
// #include "DetalleTurno.h"  // Lo va a desmarcar Sol cuando cree su archivo

using namespace std;

// Declaración de prototipos de menús intermedios
void menuClientes();
void menuProfesionales();
void menuServicios();
void menuTurnos();
//void menuServicioXProfesional();

int main() {
    int opcion;
    do {
        cout << "=================================================" << endl;
        cout << "   SISTEMA DE GESTION: CENTRO DE ESTETICA        " << endl;
        cout << "=================================================" << endl;
        cout << "1) Modulo: Gestion de Clientas" << endl;
        cout << "2) Modulo: Gestion de Profesionales (Staff)" << endl;
        cout << "3) Modulo: Catalogo de Servicios y Precios" << endl;
        cout << "4) Modulo: Agenda de Turnos y Detalles" << endl;
      //  cout << "5) Modulo: Servicio por Profesional" << endl;
        cout << "0) Cerrar Sistema" << endl;
        cout << "-------------------------------------------------" << endl;
        cout << "Seleccione una opcion de las anteriores: ";
        cin >> opcion;
        cout << endl;

        system("cls"); //AGREGADO PARA LIMPIAR PANTALLA

        switch (opcion) {
            case 1:
               // menuClientes();
                break;
            case 2:
                menuProfesionales();
                break;
            case 3:
                menuServicios();
                break;
            case 4:
                menuTurnos();
                break;
        //    case 5:
        //        menuServicioXProfesional();
        //        break;
            case 0:
                cout << "Saliendo de la aplicacion de gestion..." << endl;
                break;
            default:
                cout << "Opcion incorrecta. Reintente." << endl;
        }
    } while (opcion != 0);

    return 0;
}


/*
// SUB-MENÚS
void menuClientes() {
    int op;
    Cliente aux;
    int pos;

    do {
        cout << ">>> MODULO: GESTION DE CLIENTAS <<<" << endl;
        cout << "1. Registrar Nueva Clienta" << endl;
        cout << "2. Listar Todas las Clientas Activas" << endl;
        cout << "0. Volver al Menu Principal" << endl;
        cout << "Seleccione una opcion: ";
        cin >> op;
        cout << endl;

        if (op == 1) {
            aux.cargar();
            if (aux.escribirDisco()) {
                cout << "\n[OK] Clienta guardada con exito.\n\n";
            } else {
                cout << "\n[ERROR] No se pudo escribir el archivo.\n\n";
            }
        } else if (op == 2) {
            pos = 0;
            cout << "=== LISTADO DE CLIENTAS ===\n";
            while (aux.leerDisco(pos)) {
                aux.mostrar();
                pos++;
            }
            if (pos == 0) cout << "Archivo vacio.\n\n";
        }
    } while (op != 0);
}

*/
void menuProfesionales() {
    int op;
    Profesional aux;
    int pos;

    do {
        cout << ">>> MODULO: GESTION DE PROFESIONALES <<<" << endl;
        cout << "1. Registrar Profesional" << endl;
        cout << "2. Listar Profesionales Activos" << endl;
        cout << "0. Volver al Menu Principal" << endl;
        cout << "Seleccione una opcion: ";
        cin >> op;
        cout << endl;
        system("cls");

        if (op == 1) {
            aux.cargar();
            if (aux.escribirDisco()) {
                cout << "\n[OK] Profesional guardado con exito.\n\n";
            }
            else {
                cout << "\n[ERROR] No se pudo escribir el archivo.\n\n";
            }
        }
        else if (op == 2) {
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
    } while (op != 0);
}
