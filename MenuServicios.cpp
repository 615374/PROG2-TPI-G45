#include <iostream>
#include <cstdlib>
#include <cstring>
#include "MenuServicios.h"
#include "Servicio.h"
#include "DetalleTurno.h"
#include "Turno.h"

using namespace std;

// Algoritmo: lista los servicios activos agrupados por tipo
void listarServiciosPorTipo() {
    Servicio reg;
    int pos = 0, cant = 0;

    // Contamos servicios activos para reservar memoria exacta
    while (reg.leerDisco(pos++)) {
        if (reg.getEstado()) cant++;
    }

    if (cant == 0) {
        cout << " [AVISO] No hay servicios activos registrados.\n\n";
        system("pause");
        return;
    }

    // Usamos memoria dinamica para guardar tipos unicos
    char** tipos = new char*[cant];
    int cantidadTipos = 0;

    pos = 0;
    while (reg.leerDisco(pos++)) {
        if (reg.getEstado()) {
            bool repetido = false;
            for (int i = 0; i < cantidadTipos; i++) {
                if (strcmp(tipos[i], reg.getTipo()) == 0) repetido = true;
            }
            if (!repetido) {
                tipos[cantidadTipos] = new char[30];
                strcpy(tipos[cantidadTipos], reg.getTipo());
                cantidadTipos++;
            }
        }
    }

    // Menu de seleccion
    system("cls");
    cout << "=================================================" << endl;
    cout << "            LISTAR SERVICIOS POR TIPO            " << endl;
    cout << "=================================================" << endl;
    for (int i = 0; i < cantidadTipos; i++) {
        cout << i + 1 << ". " << tipos[i] << endl;
    }
    cout << "0. Volver al menu anterior" << endl;
    cout << "-------------------------------------------------" << endl;

    int opcionTipo;
    do {
        cout << "Seleccione una opcion: ";
        cin >> opcionTipo;
        if (opcionTipo < 0 || opcionTipo > cantidadTipos) {
            cout << "[ERROR] Opcion invalida.\n";
        }
    } while (opcionTipo < 0 || opcionTipo > cantidadTipos);

    // Limpiamos el buffer antes de evaluar la opcion 0
    cin.ignore(1000, '\n');

    if (opcionTipo == 0) {
        for (int i = 0; i < cantidadTipos; i++) delete[] tipos[i];
        delete[] tipos;
        return;
    }

    // Mostrar resultados filtrados
    system("cls");
    cout << "=================================================" << endl;
    cout << "   SERVICIOS DE TIPO: " << tipos[opcionTipo - 1]   << endl;
    cout << "=================================================" << endl;

    bool encontrado = false;
    pos = 0;
    while (reg.leerDisco(pos++)) {
        if (reg.getEstado() && strcmp(reg.getTipo(), tipos[opcionTipo - 1]) == 0) {
            reg.mostrar();
            encontrado = true;
        }
    }

    if (!encontrado) cout << "No se encontraron servicios de este tipo.\n";

    // Limpieza de memoria dinamica y pausa final
    for (int i = 0; i < cantidadTipos; i++) delete[] tipos[i];
    delete[] tipos;

    cout << "\nPresione ENTER para continuar...";
    cin.get();
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

// Algoritmo: Genera un ranking de mayor a menor cruzando los servicios brindados y validados por asistencia
void rankingServiciosMasPedidos() {
    Servicio servReg;
    int posS = 0;
    int totalServicios = 0;

    // Contamos cuantos servicios activos hay en disco para dimensionar la memoria
    while (servReg.leerDisco(posS++)) {
        if (servReg.getEstado()) totalServicios++;
    }

    if (totalServicios == 0) {
        cout << "No hay servicios activos registrados en el sistema.\n\n";
        return;
    }

    // Asignacion dinamica de memoria
    Servicio* listaServicios = new Servicio[totalServicios];
    int* volumenPedidos = new int[totalServicios];

    if (listaServicios == nullptr || volumenPedidos == nullptr) {
        cout << "\n[ERROR] Fallo critico de asignacion de memoria dinamica.\n";
        return;
    }

    // Volcamos registros y contamos ocurrencias en los detalles de turnos cruzados
    posS = 0;
    int idx = 0;
    while (servReg.leerDisco(posS++)) {
        if (servReg.getEstado()) {
            listaServicios[idx] = servReg;
            int contador = 0;

            DetalleTurno dtReg;
            int posDT = 0;
            while (dtReg.leerDisco(posDT++)) {
                if (dtReg.getEstado() && dtReg.getIdServicio() == servReg.getIdServicio()) {
                    Turno tReg;
                    int posT = 0;
                    while (tReg.leerDisco(posT++)) {
                        if (tReg.getEstado() && tReg.getAsistio() && tReg.getIdTurno() == dtReg.getIdTurno()) {
                            contador++;
                            break;
                        }
                    }
                }
            }
            volumenPedidos[idx] = contador;
            idx++;
        }
    }

    // Algoritmo de ordenamiento Burbuja (mayor a menor)
    for (int i = 0; i < totalServicios - 1; i++) {
        for (int j = 0; j < totalServicios - i - 1; j++) {
            if (volumenPedidos[j] < volumenPedidos[j + 1]) {
                int vAux = volumenPedidos[j];
                volumenPedidos[j] = volumenPedidos[j + 1];
                volumenPedidos[j + 1] = vAux;

                Servicio sAux = listaServicios[j];
                listaServicios[j] = listaServicios[j + 1];
                listaServicios[j + 1] = sAux;
            }
        }
    }

    // Renderizado del ranking de los servicios
    cout << "=========================================================================" << endl;
    cout << "                   RANKING DE SERVICIOS MAS DEMANDADOS                   " << endl;
    cout << "=========================================================================" << endl;
    cout << " PUESTO | ID  | SERVICIO                       | TOTAL SOLICITADO        " << endl;
    cout << "-------------------------------------------------------------------------" << endl;

    for (int i = 0; i < totalServicios; i++) {
        char nombreFormateado[50];
        strcpy(nombreFormateado, listaServicios[i].getNombre());

        if (strlen(nombreFormateado) > 30) {
            nombreFormateado[27] = '.';
            nombreFormateado[28] = '.';
            nombreFormateado[29] = '.';
            nombreFormateado[30] = '\0';
        }

        cout << "   #";
        if ((i + 1) < 10) {
            cout << "0";
        }
        cout << (i + 1) << "   | "
             << (listaServicios[i].getIdServicio() < 10 ? "0" : "") << listaServicios[i].getIdServicio() << "  | "
             << nombreFormateado;

        int largoActual = strlen(nombreFormateado);
        for (int espacios = largoActual; espacios < 30; espacios++) {
            cout << " ";
        }

        cout << " | " << volumenPedidos[i] << " veces solicitado" << endl;
    }
    cout << "=========================================================================\n" << endl;

    // Liberacion de memoria dinamica
    delete[] listaServicios;
    delete[] volumenPedidos;
}

// FUNCIONES GLOBALES DE MANTENIMIENTO

void modificarServicio() {
    int idBuscado;
    Servicio reg;
    int pos = 0;
    bool encontrado = false;

    cout << "================================================="  << endl;
    cout << "              SELECCIONE SERVICIO A MODIFICAR    "  << endl;
    cout << "================================================="  << endl;
    cout << "0. Volver al Menu Principal / Cancelar Modificacion"<< endl;
    cout << "-------------------------------------------------"  << endl;

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

            if (!reg.modificar()) {
                cout << "\nModificacion cancelada.\n";
                fclose(p);
                return;
            }
            reg.setIdServicio(idBuscado);
            reg.setEstado(true);

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
    cin.ignore(1000, '\n');
    cin.get();
}

bool darDeBajaServicio() {
    int idBuscado;
    Servicio reg;
    int pos = 0;
    bool encontrado = false;

    cout << "=================================================" << endl;
    cout << "          SELECCIONE UN SERVICIO PARA DAR DE BAJA "<< endl;
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
        if (reg.getIdServicio() == idBuscado && reg.getEstado() == true) {
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

// SUB-MENU INTERNO: PANEL DE CONSULTAS SERVICIOS
void menuConsultasServicios() {
    int op;
    Servicio aux;
    int pos;

    do {
        cout << "=================================================" << endl;
        cout << "         CONSULTAS Y REPORTES: SERVICIOS         " << endl;
        cout << "=================================================" << endl;
        cout << "1. Listar Todos los Servicios Activos            " << endl;
        cout << "2. Listar Servicios Agrupados por Tipo           " << endl;
        cout << "3. Ver Fichas de Servicios Inactivos (Bajas)     " << endl;
        cout << "4. Ver Ranking de Servicios Mas Pedidos          " << endl;
        cout << "0. Volver al Menu anterior" << endl;
        cout << "-------------------------------------------------" << endl;
        cout << "Seleccione una opcion: ";
        cin >> op;
        cout << endl;

        if (op == 1) {
            system("cls");
            pos = 0;
            cout << "=================================================\n";
            cout << "           LISTADO DE SERVICIOS ACTIVOS          \n";
            cout << "=================================================\n";
            while (aux.leerDisco(pos++)) if (aux.getEstado()) aux.mostrar();

            cout << "\nPresione ENTER para continuar...";
            cin.ignore(1000, '\n'); cin.get();
            system("cls");
        }
        else if (op == 2) {
            system("cls");
            listarServiciosPorTipo();
            system("cls");
        }
        else if (op == 3) {
            system("cls");
            cout << "=================================================" << endl;
            cout << "           LISTADO DE SERVICIOS INACTIVOS        " << endl;
            cout << "=================================================" << endl;
            listarServiciosInactivos();
            cout << "\nPresione ENTER para continuar...";
            cin.ignore(1000, '\n'); cin.get();
            system("cls");
        }
        else if (op != 0) {
            cout << "[ERROR] Opcion incorrecta. Reintente.\n\n";
            cin.ignore(1000, '\n');
            cout << "Presione ENTER para continuar...";
            cin.get();
        }
        else if (op == 4) {
            system("cls");
            rankingServiciosMasPedidos();
            cout << "\nPresione ENTER para continuar...";
            cin.ignore(1000, '\n'); cin.get();
            system("cls");
        }
    } while (op != 0);
}

// MENU PRINCIPAL: MODULO SERVICIOS
void menuServicios() {
    int op;
    Servicio aux;

    do {
        cout << "=================================================" << endl;
        cout << "       MODULO: GESTION SERVICIOS                 " << endl;
        cout << "=================================================" << endl;
        cout << "1. Registrar Servicio (Alta)"                      << endl;
        cout << "2. Listado de Servicios y Consultas"               << endl;
        cout << "3. Modificar Datos de Servicio"                    << endl;
        cout << "4. Dar de baja Servicio"                           << endl;
        cout << "0. Volver al Menu Principal"                       << endl;
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
            darDeBajaServicio();
            system("cls");
        }
        else if (op != 0) {
            cout << "[ERROR] Opcion incorrecta. Reintente.\n\n";
            cin.ignore(1000, '\n');
            cout << "Presione ENTER para continuar...";
            cin.get();
            system("cls");
        }
    } while (op != 0);
    system("cls");
}
