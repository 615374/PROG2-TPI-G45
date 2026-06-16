#include <iostream>
#include <cstdlib>
#include <cstring>
#include "MenuClientes.h"
#include "Cliente.h"

using namespace std;


// FUNCIONES GLOBALES

// Algoritmo: Ordena en memoria las clientas activas por Apellido (A-Z) y las muestra
void listarClientasPorApellido() {
    Cliente aux;
    int pos = 0;
    int cantidadActivas = 0;

    // Contamos cuantas clientas activas reales hay en el archivo
    while (aux.leerDisco(pos)) {
        if (aux.getEstado()) {
            cantidadActivas++;
        }
        pos++;
    }

    if (cantidadActivas == 0) {
        cout << " [AVISO] No hay clientas activas registradas para mostrar.\n\n";
        return;
    }

    // Creamos un array dinamico en memoria para guardar las activas
    Cliente* vectorClientes = new Cliente[cantidadActivas];
    if (vectorClientes == NULL) {
        cout << " [ERROR] Memoria insuficiente para ordenar.\n\n";
        return;
    }

    // Volcamos las clientas del disco al array
    pos = 0;
    int index = 0;
    while (aux.leerDisco(pos)) {
        if (aux.getEstado()) {
            vectorClientes[index] = aux;
            index++;
        }
        pos++;
    }

    // Algoritmo de Burbuja clasico comparando Apellidos (A-Z)
    for (int i = 0; i < cantidadActivas - 1; i++) {
        for (int j = 0; j < cantidadActivas - i - 1; j++) {
            if (strcmp(vectorClientes[j].getApellido(), vectorClientes[j + 1].getApellido()) > 0) {
                Cliente temp = vectorClientes[j];
                vectorClientes[j] = vectorClientes[j + 1];
                vectorClientes[j + 1] = temp;
            }
        }
    }

    // Mostramos el array ya ordenado en pantalla
    for (int i = 0; i < cantidadActivas; i++) {
        vectorClientes[i].mostrar();
    }
    cout << endl;

    // Liberamos la memoria dinamica del array obligatoriamente
    delete[] vectorClientes;
}

// Algoritmo: Filtrara el listado cruzando datos con la clase Turno
void listarClientasPorFrecuencia() {
    cout << " [Proximamente: Se resolvera al avanzar con el modulo Turnos]\n\n";
}

// Algoritmo: Filtrara las clientas que posean deudas registradas
void listarClientasConSaldos() {
    cout << " [Proximamente: Reporte de Cuentas Corrientes]\n\n";
}


// SUB-MENU INTERNO: PANEL DE CONSULTAS Y FILTROS CLIENTAS
void menuConsultasClientes() {
    int op = 1; // Arranca por defecto listado A-Z

    do {
        cout << "=================================================" << endl;
        cout << "          PANEL DE CONSULTAS Y FILTROS           " << endl;
        cout << "=================================================" << endl;
        cout << "1. Ver Listado de Clientas Activas (A-Z)" << endl;
        cout << "2. Filtrar por Frecuencia de Visitas" << endl;
        cout << "3. Filtrar por Saldos Pendientes" << endl;
        cout << "0. Volver al Menu de Gestion" << endl;
        cout << "-------------------------------------------------" << endl;
        cout << "Seleccione una opcion/filtro: ";
        cin >> op;
        cout << endl;

        system("cls");

        cout << "=================================================" << endl;
        cout << "          PANEL DE CONSULTAS Y FILTROS           " << endl;
        cout << "=================================================" << endl;
        cout << "1. Ver Listado de Clientas Activas (A-Z)" << endl;
        cout << "2. Filtrar por Frecuencia de Visitas" << endl;
        cout << "3. Filtrar por Saldos Pendientes" << endl;
        cout << "0. Volver al Menu de Gestion" << endl;
        cout << "-------------------------------------------------" << endl;
        cout << "Opcion seleccionada: " << op << endl << endl;

        if (op == 1) {
            cout << "=================================================" << endl;
            cout << "        LISTADO DE CLIENTAS ACTIVAS (A-Z)       " << endl;
            cout << "=================================================" << endl;
            listarClientasPorApellido();
            cout << "=================================================" << endl << endl;
        }
        else if (op == 2) {
            cout << "=================================================" << endl;
            cout << "        CLIENTAS POR FRECUENCIA DE VISITAS       " << endl;
            cout << "=================================================" << endl << endl;
            listarClientasPorFrecuencia();
            cout << "=================================================" << endl << endl;
        }
        else if (op == 3) {
            cout << "=================================================" << endl;
            cout << "          CLIENTAS CON SALDOS PENDIENTES         " << endl;
            cout << "=================================================" << endl << endl;
            listarClientasConSaldos();
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

// MENU PRINCIPAL: MODULO DE CLIENTAS
void menuClientes() {
    int op;
    Cliente aux;

    do {
        cout << "=================================================" << endl;
        cout << "            MODULO: GESTION CLIENTAS             " << endl;
        cout << "=================================================" << endl;
        cout << "1. Registrar Nueva Clienta" << endl;
        cout << "2. Listado de Clientas y Consultas" << endl;
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
            menuConsultasClientes();
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
