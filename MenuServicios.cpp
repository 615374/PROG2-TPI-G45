#include <iostream>
#include <cstdlib>
#include <cstring>
#include "MenuServicios.h"
#include "Servicio.h"

using namespace std;

// FUNCIONES GLOBALES

// Algoritmo: lista los servicios activos agrupados por tipo
void listarServiciosPorTipo() {
    char tipos[100][30];
    int cantidadTipos = 0;
    int opcionTipo;
    Servicio reg;
    int pos = 0;
    bool repetido;
    bool encontrado = false;

    while (reg.leerDisco(pos)) {
        if (reg.getEstado()) {
            repetido = false;
            for (int i = 0; i < cantidadTipos; i++) {
                if (strcmp(tipos[i], reg.getTipo()) == 0) {
                    repetido = true;
                }
            }
            if (!repetido) {
                strcpy(tipos[cantidadTipos], reg.getTipo());
                cantidadTipos++;
            }
        }
        pos++;
    }

    if (cantidadTipos == 0) {
        cout << " [AVISO] No hay tipos disponibles porque no hay servicios activos.\n\n";
        return;
    }
    cout << " TIPOS DISPONIBLES:" << endl;
    cout << "-------------------------------------------------" << endl;
    for (int i = 0; i < cantidadTipos; i++) {
        cout << i + 1 << ". " << tipos[i] << endl;
    }
    cout << "0. Volver al menu anterior" << endl;
    cout << "-------------------------------------------------" << endl;

    do {
        cout << "Seleccione un tipo: ";
        cin >> opcionTipo;
        if (opcionTipo < 0 || opcionTipo > cantidadTipos) {
            cout << "[ERROR] Opcion invalida.\n";
        }
    } while (opcionTipo < 0 || opcionTipo > cantidadTipos);

    cin.ignore(1000, '\n');

    if (opcionTipo == 0) {
        cout << "\nVolviendo al menu...\n";
        return;
    }

    pos = 0;
    cout << endl;
    cout << "=================================================" << endl;
    cout << "  SERVICIOS DE TIPO: " << tipos[opcionTipo - 1]    << endl;
    cout << "=================================================" << endl;


    while (reg.leerDisco(pos)) {
        if (reg.getEstado() == true && strcmp(reg.getTipo(), tipos[opcionTipo - 1]) == 0) {
            reg.mostrar();
            encontrado = true;
        }
        pos++;
    }
    if (!encontrado) {
        cout << "No se encontraron servicios activos con ese tipo.\n";
    }
}

// Algoritmo: lista los servicios inactivos recorriendo el archivo
void listarServiciosInactivos() {
    Servicio reg;
    int pos = 0;
    bool hayInactivos = false;

    while (reg.leerDisco(pos)) {
        if (reg.getEstado() == false) {
            cout << "-----------------------------------"  << endl;
            cout << "ID SERVICIO: " << reg.getIdServicio() << endl;
            cout << "NOMBRE: " << reg.getNombre()          << endl;
            cout << "TIPO: " << reg.getTipo()              << endl;
            cout << "PRECIO: $" << reg.getPrecio()         << endl;
            cout << "-----------------------------------"  << endl;

            hayInactivos = true;
        }
        pos++;
    }
    if (!hayInactivos) {
        cout << "No hay servicios inactivos para mostrar.\n" << endl;
    }
}

// FUNCIONES GLOBALES DE MANTENIMIENTO

// Algoritmo: Busca un ID activo, permite reingresar datos y los sobrescribe en el archivo servicios.dat
void modificarServicio() {
    int idBuscado;
    Servicio reg;
    int pos = 0;
    bool encontrado = false;

    cout << "=================================================" << endl;
    cout << "             SELECCIONE SERVICIO A MODIFICAR     " << endl;
    cout << "=================================================" << endl;
    cout << "0. Volver al Menu Principal / Cancelar Modificacion" << endl;
    cout << "-------------------------------------------------" << endl;

    // Listamos todos los servicios activos para que el usuario elija
    while (reg.leerDisco(pos)) {
        if (reg.getEstado() == true) {
            cout << "ID: [" << reg.getIdServicio() << "] - "
                 << reg.getNombre() << " - Tipo: " << reg.getTipo()
                 << " - Precio: $" << reg.getPrecio() << endl;
        }
        pos++;
    }

    cout << "=================================================" << endl;
    cout << "Ingrese el ID del servicio a modificar: ";
    cin >> idBuscado;

    if (idBuscado == 0) {
        cout << "\nOperacion cancelada. Volviendo al menu...\n";
        return;
    }

    pos = 0;
    FILE* p = fopen("servicios.dat", "rb+");
    if (p == NULL) {
        cout << "\n[ERROR] No se pudo acceder al archivo.\n";
        return;
    }

    while (fread(&reg, sizeof(Servicio), 1, p) == 1) {
        if (reg.getIdServicio() == idBuscado && reg.getEstado() == true) {
            encontrado = true;

            cout << "\nServicio encontrado. Reingrese los datos:\n\n";
            cout << "ID SERVICIO: " << idBuscado << endl;

            // Invocamos el metodo de la clase para cargar y validar variables en RAM
            if (!reg.modificar()) {
                cout << "\nModificacion cancelada.\n";
                fclose(p);
                return;
            }
            reg.setIdServicio(idBuscado);
            reg.setEstado(true);

            // Volvemos el puntero una posicion atras y reescribimos en disco
            fseek(p, pos * sizeof(Servicio), SEEK_SET);
            fwrite(&reg, sizeof(Servicio), 1, p);
            cout << "\n[OK] Servicio modificado correctamente.\n";
            cin.ignore(1000, '\n');
            break;
        }
        pos++;
    }
    fclose(p);

    if (!encontrado) {
        cout << "\n[ERROR] No se encontro ningun servicio activo con ese ID.\n";
    }

    cout << "\nPresione ENTER para continuar...";
    cin.get();
}

//BAJA LOGICA SERVICIO
bool darDeBajaServicio() {
    int idBuscado;
    Servicio reg;
    int pos = 0;
    bool encontrado = false;

    cout << "=================================================" << endl;
    cout << "        SELECCIONE UN SERVICIO PARA DAR DE BAJA  " << endl;
    cout << "=================================================" << endl;
    cout << " 0. Volver al Menu Principal / Cancelar Baja     " << endl;
    cout << "-------------------------------------------------" << endl;

    while (reg.leerDisco(pos)) {
        if (reg.getEstado() == true) {
            cout << "ID: [" << reg.getIdServicio() << "] - "
                 << reg.getNombre()
                 << " ($" << reg.getPrecio() << ")"
                 << endl;
        }
        pos++;
    }
    cout << "=================================================" << endl;
    cout << "Ingrese el ID del servicio a dar de baja: ";
    cin >> idBuscado;

    //Si el usuario presiona 0, corta la funcion
    if (idBuscado == 0) {
        cout << "\nOperacion cancelada. Volviendo al menu...\n";
        return false;
    }

    pos = 0;
    FILE* p = fopen("servicios.dat", "rb+");
    if (p == NULL) {
        cout << "\n[ERROR] No se pudo acceder al archivo.\n";
        return true;
    }
    while (fread(&reg, sizeof(Servicio), 1, p) == 1) {
        if (reg.getIdServicio() == idBuscado &&
            reg.getEstado() == true) {
            encontrado = true;
            reg.setEstado(false);
            fseek(p, pos * sizeof(Servicio), SEEK_SET);
            fwrite(&reg, sizeof(Servicio), 1, p);
            cout << "\n[OK] Servicio dado de baja correctamente.\n";
            break;
        }
        pos++;
    }
    fclose(p);
    if (!encontrado) {
        cout << "\n[ERROR] No se encontro ningun servicio activo con ese ID.\n";
    }

    cin.ignore(1000, '\n');
    cout << "\nPresione ENTER para continuar...";
    cin.get();

    return true;
}


// SUB-MENUS INTERMEDIOS Y CONSULTAS

// Menu secundario de reportes y listados especificos
void menuConsultasServicios() {
    int op;
    Servicio aux;
    int pos;

    do {
        cout << "=================================================" << endl;
        cout << "         CONSULTAS Y REPORTES: SERVICIOS         " << endl;
        cout << "=================================================" << endl;
        cout << "1. Listar Todos los Servicios Activos"             << endl;
        cout << "2. Listar Servicios Agrupados por Tipo"            << endl;
        cout << "3. Ver Fichas de Servicios Inactivos (Bajas)"      << endl;
        cout << "0. Volver al Menu anterior" << endl;
        cout << "-------------------------------------------------" << endl;
        cout << "Seleccione una opcion: ";
        cin >> op;
        cout << endl;

        if (op == 1) {
            system("cls");
            pos = 0;
            cout << "=================================================\n";
            cout << "           LISTADO DE SERVICIOS ACTIVOS           \n";
            cout << "=================================================\n";
            while (aux.leerDisco(pos)) {
                aux.mostrar();
                pos++;
            }
            if (pos == 0) {
                cout << "Archivo de servicios vacio.\n\n";
            }
            cin.ignore(1000, '\n');
            cout << "\nPresione ENTER para continuar...";
            cin.get();
            system("cls");
        }
        else if (op == 2) {
            system("cls");
            cout << "=================================================" << endl;
            cout << "             LISTADO DE SERVICIOS POR TIPO       " << endl;
            cout << "=================================================" << endl;
            listarServiciosPorTipo();
            cout << "\nPresione ENTER para continuar...";
            cin.get();
            system("cls");
        }
        else if (op == 3) {
            system("cls");
            cout << "=================================================" << endl;
            cout << "             LISTADO DE SERVICIOS INACTIVOS      " << endl;
            cout << "=================================================" << endl;
            listarServiciosInactivos();
            cout << "\nPresione ENTER para continuar...";
            cin.ignore(1000, '\n');
            cin.get();
            system("cls");
        }
    } while (op != 0);
}

// SUB-MENU PRINCIPAL: MODULO DE SERVICIOS
void menuServicios() {
    int op;
    Servicio aux;

    do {
        cout << "=================================================" << endl;
        cout << "     MODULO: GESTION SERVICIOS                   " << endl;
        cout << "=================================================" << endl;
        cout << "1. Registrar Servicio (Alta)" << endl;
        cout << "2. Listado de Servicios y Consultas" << endl;
        cout << "3. Modificar Datos de Servicio" << endl;
        cout << "4. Dar de baja Servicio" << endl;
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
            menuConsultasServicios();
            system("cls");
        }
        else if (op == 3) {
            system("cls");
            modificarServicio();
            system("cls");
        }
        else if (op == 4) {
            system("cls");
            if (darDeBajaServicio()) {

            }
            system("cls");
        }
    } while (op != 0);

    system("cls");
}
