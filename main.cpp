#include <iostream>
#include <cstdlib>
#include "main.h"
//#include "Cliente.h"
#include "Profesional.h"
//#include "Servicio.h"
//#include "ServicioXProfesional.h"

using namespace std;

int main(){
    int opcion;
    do {
        cout << "=================================================" << endl;
        cout << "   SISTEMA DE GESTION: CENTRO DE ESTETICA        " << endl;
        cout << "=================================================" << endl;
        cout << "1) Modulo: Gestion de Clientas" << endl;
        cout << "2) Modulo: Gestion de Profesionales (Staff)" << endl;
        cout << "3) Modulo: Catalogo de Servicios y Precios" << endl;
        cout << "4) Modulo: Agenda de Turnos y Detalles" << endl;
        cout << "5) Modulo: Servicio por Profesional" << endl;
        cout << "0) Cerrar Sistema" << endl;
        cout << "-------------------------------------------------" << endl;
        cout << "Seleccione una opcion de las anteriores: ";
        cin >> opcion;
        cout << endl;

        system("cls"); // Limpieza de pantalla al seleccionar

        switch (opcion) {
            case 1:
              //  menuClientes();
                break;
            case 2:
                menuProfesionales();
                break;
            case 3:
               // menuServicios();
                break;
            case 4:
               // menuTurnos();
                break;
            case 5:
               // menuServicioXProfesional();
                break;
            case 0:
                cout << "Saliendo de la aplicacion de gestion..." << endl;
                break;
            default:
                cout << "Opcion incorrecta. Reintente." << endl;
        }
    } while (opcion != 0);
}

// ===============================================
//         SUB-MENÚS DEL BACKOFFICE
// ===============================================

/*
// Modulo Clientes
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

            system("cls");
        }
    } while (op != 0);

    system("cls");
}

*/

// Modulo Profesionales
void menuProfesionales() {
    int op;
    Profesional aux;
    int pos;

    do {
        cout << ">>> MODULO: GESTION DE PROFESIONALES <<<" << endl;

        cout << "\n--- ALTAS ---" << endl;
        cout << "1. Registrar Profesional" << endl;

        cout << "\n--- CONSULTAS ---" << endl;
        cout << "2. Listar Profesionales Activos" << endl;
        cout << "3. Listar Profesionales por Especialidad" << endl;
        cout << "4. Listar Profesionales Inactivos" << endl;

        cout << "\n--- MANTENIMIENTO ---" << endl;
        cout << "5. Modificar Profesional" << endl;
        cout << "6. Dar de Baja Profesional" << endl;

/*
        cout << "\n--- REPORTES ---" << endl;
        cout << "7. Volumen de Servicios por Profesional" << endl;
        cout << "8. Liquidacion de Comisiones" << endl;
*/

        cout << "\n0. Volver al Menu Principal" << endl;
        cout << "Seleccione una opcion: ";
        cin >> op;

        if (op == 1) {
            system("cls");

        if (aux.cargar() == true) {
            if (aux.escribirDisco()) {
                cout << "\n[OK] Profesional guardado con exito.\n\n";
            }
            else {
                cout << "\n[ERROR] No se pudo escribir el archivo.\n\n";
            }
            cin.ignore(1000, '\n'); //pq venimos de cargar
        }
        else {
            cout << "\nCarga cancelada. No se guardo el profesional.\n\n";
        }
            //si ya cancele no queda nada en el buffer
            cout << "Presione ENTER para continuar...";
            cin.get();
            system("cls");
        }
        else if (op == 2) {
            system("cls");

            pos = 0;
            bool hayActivos = false;
            cout << "=== LISTADO DE PROFESIONALES ACTIVOS ===\n";
            while (aux.leerDisco(pos)) {
                if (aux.getEstado()) {
                    aux.mostrar();
                    hayActivos = true;
                }
                pos++;
            }
            if (pos == 0) {
                cout << "Archivo vacio.\n\n";
            }
            else if (!hayActivos) {
                cout << "No hay profesionales activos para mostrar.\n\n";
            }
            cin.ignore(1000, '\n');
            cout << "\nPresione ENTER para continuar...";
            cin.get();
            system("cls");
        }
        else if (op == 3) {
            system("cls");

            listarProfesionalesPorEspecialidad();

            cout << "\nPresione ENTER para continuar...";
            cin.get();
            system("cls");
        }
        else if (op == 4){
            system("cls");

            listarProfesionalesInactivos();

            cin.ignore(1000, '\n');
            cout << "\nPresione ENTER para continuar...";
            cin.get();

            system("cls");
        }
        else if (op == 5){
            system("cls");
            modificarProfesional();

            cout << "\nPresione ENTER para continuar...";
            cin.get();

        system("cls");
        }
        else if (op == 6) {
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

/*
// Modulo Servicios
void menuServicios() {
    int op;
    Servicio aux;
    int pos;

    do {
        cout << "=================================================" << endl;
        cout << "     MODULO: GESTION SERVICIOS                   " << endl;
        cout << "=================================================" << endl;
        cout << "1. Registrar Servicio" << endl;
        cout << "2. Listar Servicios Activos" << endl;
        cout << "3. Dar de baja Servicio"  << endl;
        cout << "0. Volver al Menu Principal" << endl;
        cout << "-------------------------------------------------" << endl;
        cout << "Seleccione una opcion: ";
        cin >> op;
        cout << endl;

        if (op == 1) {
            system("cls");
            if (aux.cargar() == true) {
                if (aux.escribirDisco()) {
                    cout << "\n[OK] Servicio guardado con exito.\n\n";
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

            system("cls");
        }
    } while (op != 0);
    system("cls");
}

// Modulo Tabla Intermedia Servicio X Profesional
void menuServicioXProfesional() {
    int op;
    ServicioXProfesional aux;

    do {
        cout << "=================================================" << endl;
        cout << "     MODULO: SERVICIOS POR PROFESIONAL           " << endl;
        cout << "=================================================" << endl;
        cout << "1. Registrar relacion Profesional-Servicio" << endl;
        cout << "2. Listar servicios por profesional" << endl;
        cout << "0. Volver al Menu Principal" << endl;
        cout << "-------------------------------------------------" << endl;
        cout << "Seleccione una opcion: ";
        cin >> op;
        cout << endl;

        if (op == 1) {
            system("cls");
            if (aux.cargar() == true) {
                if (aux.escribirDisco()) {
                    cout << "\nRelacion guardada correctamente.\n\n";
                } else {
                    cout << "\n[ERROR] No se pudo guardar la relacion.\n\n";
                }
                cin.ignore(1000, '\n');
                cout << "Presione ENTER para continuar...";
                cin.get();
            }
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
*/
