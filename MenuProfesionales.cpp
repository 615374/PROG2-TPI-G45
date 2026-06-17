#include <iostream>
#include <cstdlib>
#include <cstring>
#include "MenuProfesionales.h"
#include "Profesional.h"

using namespace std;


// FUNCIONES GLOBALES

// Algoritmo: Filtra y muestra los profesionales activos agrupados por especialidad
void listarProfesionalesPorEspecialidad() {
    char especialidades[100][50];
    int cantidadEspecialidades = 0;
    int opcionEspecialidad;
    Profesional reg;
    int pos = 0;
    bool repetida;
    bool encontrado = false;

    // Armamos la lista de especialidades únicas en memoria
    while (reg.leerDisco(pos)) {
        if (reg.getEstado()) {
            repetida = false;
            for (int i = 0; i < cantidadEspecialidades; i++) {
                if (strcmp(especialidades[i], reg.getEspecialidad()) == 0) {
                    repetida = true;
                }
            }
            if (!repetida) {
                strcpy(especialidades[cantidadEspecialidades], reg.getEspecialidad());
                cantidadEspecialidades++;
            }
        }
        pos++;
    }

    if (cantidadEspecialidades == 0) {
        cout << "[AVISO] No hay especialidades disponibles porque no hay profesionales activos.\n\n";
        return;
    }
    cout << "ESPECIALIDADES DISPONIBLES EN SISTEMA:" << endl;
    cout << "-------------------------------------------------" << endl;
    for (int i = 0; i < cantidadEspecialidades; i++) {
        cout << i + 1 << ". " << especialidades[i] << endl;
    }
    cout << "0. Volver al menu anterior" << endl;
    cout << "-------------------------------------------------" << endl;

    do {
        cout << "Seleccione una especialidad: ";
        cin >> opcionEspecialidad;
        if (opcionEspecialidad < 0 || opcionEspecialidad > cantidadEspecialidades) {
            cout << "[ERROR] Opcion invalida. Reintente.\n";
        }
    } while (opcionEspecialidad < 0 || opcionEspecialidad > cantidadEspecialidades);

    cin.ignore(1000, '\n');

    if (opcionEspecialidad == 0) {
        cout << "\nOperacion cancelada. Volviendo al menu...\n";
        return;
    }

    system("cls");

    cout << "=================================================" << endl;
    cout << "          PANEL DE CONSULTAS Y REPORTES          " << endl;
    cout << "=================================================" << endl;
    cout << "1. Ver Staff de Profesionales Activas" << endl;
    cout << "2. Listar por Especialidad" << endl;
    cout << "3. Listar Ordenadas por Volumen de Servicios" << endl;
    cout << "4. Ver Staff Profesionales Inactivas" << endl;
    cout << "5. Liquidacion de Comisiones" << endl;
    cout << "0. Volver al Menu de Gestion" << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "Opcion seleccionada: 2" << endl << endl;

    pos = 0;
    cout << "=================================================" << endl;
    cout << "  RESULTADOS PARA: " << especialidades[opcionEspecialidad - 1] << endl;
    cout << "=================================================" << endl;

    while (reg.leerDisco(pos)) {
        if (reg.getEstado() && strcmp(reg.getEspecialidad(), especialidades[opcionEspecialidad - 1]) == 0) {
            reg.mostrar();
            encontrado = true;
        }
        pos++;
    }
    if (!encontrado) {
        cout << "No se encontraron profesionales activos con esa especialidad.\n";
    }
}

// Algoritmo: Recorre el archivo mostrando unicamente el staff dado de baja
void listarProfesionalesInactivos() {
    Profesional reg;
    int pos = 0;
    bool hayInactivos = false;
    while (reg.leerDisco(pos)) {
        if (!reg.getEstado()) {
            reg.mostrar();
            hayInactivos = true;
        }
        pos++;
    }
    if (!hayInactivos) {
        cout << "No hay profesionales inactivos para mostrar.\n";
    }
}

// Algoritmo: Filtrara el listado ordenando por volumen de servicios brindados
void listarProfesionalesPorVolumenServicios() {
    cout << " [Proximamente: Se resolvera al avanzar con el modulo Turnos]\n\n";
}

// Algoritmo: Filtrara la liquidacion cruzando comisiones del mes
void liquidacionComisiones() {
    cout << " [Proximamente: Reporte de Liquidacion de Comisiones]\n\n";
}

// SUB-MENU INTERNO: PANEL DE CONSULTAS Y FILTROS DE STAFF
void menuConsultasProfesionales() {
    int op = 1;
    Profesional aux;
    int pos;

    do {
        cout << "=================================================" << endl;
        cout << "          PANEL DE CONSULTAS Y REPORTES          " << endl;
        cout << "=================================================" << endl;
        cout << "1. Ver Staff de Profesionales Activas" << endl;
        cout << "2. Listar por Especialidad" << endl;
        cout << "3. Listar por Volumen de Servicios" << endl;
        cout << "4. Ver Staff Profesionales Inactivas" << endl;
        cout << "5. Liquidacion de Comisiones" << endl;
        cout << "0. Volver al Menu de Gestion" << endl;
        cout << "-------------------------------------------------" << endl;
        cout << "Seleccione una opcion/filtro: ";
        cin >> op;
        cout << endl;

        system("cls");

        cout << "=================================================" << endl;
        cout << "          PANEL DE CONSULTAS Y REPORTES          " << endl;
        cout << "=================================================" << endl;
        cout << "1. Ver Staff de Profesionales Activas" << endl;
        cout << "2. Listar por Especialidad" << endl;
        cout << "3. Listar por Volumen de Servicios" << endl;
        cout << "4. Ver Staff Profesionales Inactivas" << endl;
        cout << "5. Liquidacion de Comisiones" << endl;
        cout << "0. Volver al Menu de Gestion" << endl;
        cout << "-------------------------------------------------" << endl;
        cout << "Opcion seleccionada: " << op << endl << endl;

        if (op == 1) {
            cout << "=================================================" << endl;
            cout << "            STAFF DE PROFESIONALES ACTIVAS       " << endl;
            cout << "=================================================" << endl;
            pos = 0;
            bool hayActivos = false;
            while (aux.leerDisco(pos)) {
                if (aux.getEstado()) {
                    aux.mostrar();
                    hayActivos = true;
                }
                pos++;
            }
            if (pos == 0) {
                cout << "Archivo vacio.\n\n";
            } else if (!hayActivos) {
                cout << "No hay profesionales activos registrados.\n\n";
            }
            cout << "=================================================" << endl << endl;
        }
        else if (op == 2) {
            listarProfesionalesPorEspecialidad();
        }
        else if (op == 3) {
            cout << "=================================================" << endl;
            cout << "      PROFESIONALES POR VOLUMEN DE SERVICIOS     " << endl;
            cout << "=================================================" << endl << endl;
            listarProfesionalesPorVolumenServicios();
            cout << "=================================================" << endl << endl;
        }
        else if (op == 4) {
            cout << "=================================================" << endl;
            cout << "         LISTADO DE PROFESIONALES INACTIVOS      " << endl;
            cout << "=================================================" << endl;
            listarProfesionalesInactivos();
            cout << "=================================================" << endl << endl;
        }
        else if (op == 5) {
            cout << "=================================================" << endl;
            cout << "               LIQUIDACION DE COMISIONES         " << endl;
            cout << "=================================================" << endl << endl;
            liquidacionComisiones();
            cout << "=================================================" << endl << endl;
        }
        else if (op != 0) {
            cout << "[ERROR] Opcion incorrecta. Reintente.\n\n";
        }

        if (op != 0) {
            cout << "Presione ENTER para refrescar o cambiar de filtro...";
            cin.ignore(1000, '\n');
            cin.get();
            system("cls");
        }

    } while (op != 0);
}

// MENU PRINCIPAL: MODULO DE PROFESIONALES
void menuProfesionales() {
    int op;
    Profesional aux;

    do {
        cout << "=================================================" << endl;
        cout << "          MODULO: GESTION DE PROFESIONALES       " << endl;
        cout << "=================================================" << endl;
        cout << "1. Registrar Profesional" << endl;
        cout << "2. Listado de Profesionales y Consultas" << endl;
        cout << "3. Modificar Datos de Profesional" << endl;
        cout << "4. Dar de Baja Profesional" << endl;
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
            menuConsultasProfesionales();
            system("cls");
        }
        else if (op == 3) {
            system("cls");
            modificarProfesional();
            system("cls");
        }
        else if (op == 4) {
            system("cls");
            darDeBajaProfesional();
            system("cls");
        }
    } while (op != 0);

    system("cls");
}
