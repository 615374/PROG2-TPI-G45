#include <iostream>
#include <cstdlib>
#include "Cliente.h"
#include "Profesional.h"

using namespace std;

// DECLARACIÓN DE FUNCIONES DE MENÚS (Para que main sepa que existen abajo)
void menuClientes();
void menuProfesionales();
void menuServicios();
void menuTurnos();
void menuServicioXProfesional();

int main() {
    int opcion;

    // Incluimos las directivas de cabeceras necesarias para los prototipos de las funciones
    do {
        cout << "===============================================" << endl;
        cout << "       SISTEMA DE GESTION: CENTRO DE ESTETICA  " << endl;
        cout << "===============================================" << endl;
        cout << "1. Modulo Clientes" << endl;
        cout << "2. Modulo Profesionales" << endl;
        cout << "3. Modulo Servicios" << endl;
        cout << "4. Modulo Turnos" << endl;
        cout << "5. Modulo Configurar Staff (Servicio x Profesional)" << endl;
        cout << "0. Salir de la Aplicacion" << endl;
        cout << "-----------------------------------------------" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cout << endl;

        switch (opcion) {
            case 1:
                menuClientes();
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
            case 5:
                menuServicioXProfesional();
                break;
            case 0:
                cout << "Saliendo de la aplicacion de gestion..." << endl;
                break;
            default:
                cout << "Opcion incorrecta. Reintente." << endl;
        }
    } while (opcion != 0);

    return 0;
}

// ===============================================
//         SUB-MENÚS DEL BACKOFFICE
// ===============================================

// Modulo Clientes - Gisela
void menuClientes() {
    int op;
    Cliente aux;
    int pos;

    do {
        cout << ">>> MODULO: GESTION DE CLIENTAS <<<" << endl;
        cout << "1. Registrar Nueva Clienta" << endl;
        cout << "2. Listar Todas las Clientas Activas" << endl;
        cout << "3. Dar de baja Clienta" << endl;
        cout << "0. Volver al Menu Principal" << endl;
        cout << "Seleccione una opcion: ";
        cin >> op;
        cout << endl;

        if (op == 1) {
            system("cls");
            aux.cargar();
            if (aux.escribirDisco()) {
                cout << "\n[OK] Clienta guardada con exito.\n\n";
            } else {
                cout << "\n[ERROR] No se pudo escribir el archivo.\n\n";
            }
            cin.ignore(1000, '\n');
            cout << "Presione ENTER para continuar...";
            cin.get();
            system("cls");
        }
        else if (op == 2) {
            system("cls");
            pos = 0;
            cout << "=== LISTADO DE CLIENTAS ===\n";
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

            darDeBajaCliente();

            cin.ignore(1000, '\n');
            cout << "\nPresione ENTER para continuar...";
            cin.get();
            system("cls");
        }
    } while (op != 0);
    system("cls");
}

// Modulo Profesionales - Sol
void menuProfesionales() {
    int op;
    Profesional aux;
    int pos;

    do {
        cout << ">>> MODULO: GESTION DE PROFESIONALES <<<" << endl;
        cout << "1. Registrar Profesional" << endl;
        cout << "2. Listar Profesionales Activos" << endl;
        cout << "3. Dar de baja Profesional" << endl;
        cout << "0. Volver al Menu Principal" << endl;
        cout << "Seleccione una opcion: ";
        cin >> op;
        cout << endl;

        if (op == 1) {
            system("cls");
            aux.cargar();
            if (aux.escribirDisco()) {
                cout << "\n[OK] Profesional guardado con exito.\n\n";
            }
            else {
                cout << "\n[ERROR] No se pudo escribir el archivo.\n\n";
            }
            cin.ignore(1000, '\n');
            cout << "Presione ENTER para continuar...";
            cin.get();
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

            cin.ignore(1000, '\n');
            cout << "\nPresione ENTER para continuar...";
            cin.get();
            system("cls");
        }
    } while (op != 0);
    system("cls");
}

// Modulo Servicios Sol
void menuServicios() {
    int op;
    Servicio aux;
    int pos;

    do {
        cout << ">>> MODULO: GESTION DE SERVICIOS <<<" << endl;
        cout << "1. Registrar Servicio" << endl;
        cout << "2. Listar Servicios Activos" << endl;
        cout << "3. Dar de baja Servicio"  << endl;
        cout << "0. Volver al Menu Principal" << endl;
        cout << "Seleccione una opcion: ";
        cin >> op;
        cout << endl;

        if (op == 1) {
            system("cls");
            aux.cargar();
            if (aux.escribirDisco()) {
                cout << "\n[OK] Servicio guardado con exito.\n\n";
            }
            else {
                cout << "\n[ERROR] No se pudo escribir el archivo.\n\n";
            }
            cin.ignore(1000, '\n');
            cout << "Presione ENTER para continuar...";
            cin.get();
            system("cls");
        }
        else if (op == 2) {
            system("cls");
            pos = 0;
            cout << "=== LISTADO DE SERVICIOS ===\n";
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
        else if(op == 3){
            system("cls");

            darDeBajaServicio();
            cin.ignore(1000, '\n');
            cout << "\nPresione ENTER para continuar...";
            cin.get();
            system("cls");
        }
    } while (op != 0);
    system("cls");
}

// Modulo Tabla Intermedia Servicio X Profesional Sol y Gise
void menuServicioXProfesional() {
    int op;
    ServicioXProfesional aux;

    do {
        cout << ">>> MODULO: SERVICIO POR PROFESIONAL <<<" << endl;
        cout << "1. Registrar relacion Profesional-Servicio" << endl;
        cout << "2. Listar servicios por profesional" << endl;
        cout << "0. Volver al Menu Principal" << endl;
        cout << "Seleccione una opcion: ";
        cin >> op;
        cout << endl;

        if (op == 1) {
            system("cls");
            aux.cargar();
            if (aux.escribirDisco()) {
                cout << "\nRelacion guardada correctamente.\n\n";
            }
            else {
                cout << "\n[ERROR] No se pudo guardar la relación.\n\n";
            }
            system("pause");
            system("cls");
        }
        else if (op == 2) {
            system("cls");
            Profesional prof;
            Servicio serv;
            ServicioXProfesional rel;

            int posProf = 0;
            int posRel;
            bool tieneServicios;
            bool hayRelaciones = false;

            cout << "=================================================" << endl;
            cout << "        SERVICIOS ASIGNADOS POR PROFESIONAL      " << endl;
            cout << "=================================================" << endl << endl;

            while (prof.leerDisco(posProf)) {
                if (prof.getEstado()) {
                    tieneServicios = false;
                    posRel = 0;

                    while (rel.leerDisco(posRel)) {
                        if (rel.getEstado() && rel.getIdProfesional() == prof.getIdProfesional()) {
                            if (!tieneServicios) {
                                cout << "PROFESIONAL: ";
                                prof.mostrarNombrePorId(prof.getIdProfesional());
                                cout << endl;
                                cout << "SERVICIOS:" << endl;
                                tieneServicios = true;
                                hayRelaciones = true;
                            }
                            cout << "   - ";
                            serv.mostrarNombrePorId(rel.getIdServicio());
                            cout << endl;
                        }
                        posRel++;
                    }
                    if (tieneServicios) {
                        cout << "-------------------------------------------------" << endl;
                    }
                }
                posProf++;
            }
            if (!hayRelaciones) {
                cout << "No hay relaciones cargadas.\n\n";
            }
            cin.ignore(1000, '\n');
            cout << "\nPresione ENTER para continuar...";
            cin.get();
            system("cls");
        }
    } while (op != 0);
    system("cls");
}

// Modulo Agenda de Turnos Cascada
void menuTurnos() {
    cout << "Modulo en desarrollo. Disponible proximamente.\n\n";
    system("pause");
    system("cls");
}
