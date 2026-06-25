#include <iostream>
#include <cstdlib>
#include <cstring>
#include "Cliente.h"
#include "MenuClientes.h"
#include "Turno.h"
#include "DetalleTurno.h"

using namespace std;


/// FUNCIONES GLOBALES DE MANTENIMIENTO
void modificarCliente() {
    int idBuscado;
    Cliente reg;
    int pos = 0;
    bool encontrado = false;

    cout << "=================================================" << endl;
    cout << "           SELECCIONE CLIENTA A MODIFICAR        " << endl;
    cout << "=================================================" << endl;
    cout << "0. Volver al Menu Principal / Cancelar Modificacion" << endl;
    cout << "-------------------------------------------------" << endl;

    // Listamos todas las clientas activas para que la recepcionista elija el ID
    while (reg.leerDisco(pos)) {
        if (reg.getEstado() == true) {
            cout << "ID: [" << reg.getIdCliente() << "] - "
                 << reg.getApellido() << ", " << reg.getNombre()
                 << " - Tel: " << reg.getTelefono() << endl;
        }
        pos++;
    }
    cout << "=================================================" << endl;
    cout << "Ingrese el ID de la clienta a modificar: ";
    cin >> idBuscado;

    if (idBuscado == 0) {
        cout << "\nOperacion cancelada. Volviendo al menu...\n";
        return;
    }

    pos = 0;
    FILE* p = fopen("clientes.dat", "rb+");
    if (p == NULL) {
        cout << "\n[ERROR] No se pudo acceder al archivo.\n";
        return;
    }
    // Buscamos el registro en el archivo de forma binaria
    while (fread(&reg, sizeof(Cliente), 1, p) == 1) {
        if (reg.getIdCliente() == idBuscado && reg.getEstado() == true) {
            encontrado = true;

            cout << "\nClienta encontrada. Reingrese los datos:\n\n";
            cout << "ID CLIENTA: " << idBuscado << endl;

            // Invocamos el metodo para cargar las variables
            if (!reg.modificar()) {
                cout << "\nModificacion cancelada.\n";
                fclose(p);
                return;
            }

            // Aseguramos que conserve su ID original y siga activa
            reg.setIdCliente(idBuscado);
            reg.setEstado(true);

            // Volvemos para atras el puntero del archivo y reescribimos el registro
            fseek(p, pos * sizeof(Cliente), SEEK_SET);
            fwrite(&reg, sizeof(Cliente), 1, p);

            cout << "\n[OK] Clienta modificada correctamente.\n";
            break;
        }
        pos++;
    }
    fclose(p);

    if (!encontrado) {
        cout << "\n[ERROR] No se encontro ninguna clienta activa con ese ID.\n";
    }

    cout << "\nPresione ENTER para continuar...";
    cin.get();
}

/// BAJA LOGICA
bool darDeBajaCliente() {
    int idBuscado;
    Cliente reg;
    int pos = 0;
    bool encontrado = false;

    cout << "=================================================" << endl;
    cout << "        SELECCIONE UNA CLIENTA PARA DAR DE BAJA  " << endl;
    cout << "=================================================" << endl;
    cout << " 0. Volver al Menu Principal / Cancelar baja" << endl;
    cout << "-------------------------------------------------" << endl;

    // Recorremos el archivo completo y mostramos las activas
    while (reg.leerDisco(pos)) {
        if (reg.getEstado() == true) {
            cout << " ID: [" << reg.getIdCliente() << "] - "
                 << reg.getApellido() << ", " << reg.getNombre() << endl;
        }
        pos++;
    }
    cout << "=================================================" << endl;
    cout << "Ingrese el ID de la clienta a eliminar: ";
    cin >> idBuscado;

    // Si presiona 0, vuelve al menu
    if (idBuscado == 0) {
        return false;
    }

    // Reiniciamos la posicion para buscar y sobreescribir
    pos = 0;
    FILE* p = fopen("clientes.dat", "rb+");
    if (p == NULL) {
        cout << "\n[ERROR] No se pudo acceder al archivo de clientes.\n";
        return true;
    }

    while (fread(&reg, sizeof(Cliente), 1, p) == 1) {
        if (reg.getIdCliente() == idBuscado && reg.getEstado() == true) {
            encontrado = true;
            reg.setEstado(false);

            fseek(p, pos * sizeof(Cliente), SEEK_SET);
            fwrite(&reg, sizeof(Cliente), 1, p);

            cout << "\n[OK] Clienta dada de baja correctamente del sistema.\n";
            break;
        }
        pos++;
    }
    fclose(p);

    if (!encontrado) {
        cout << "\n[ERROR] No se encontro ninguna clienta activa con el ID: " << idBuscado << endl;
    }
    cin.ignore(1000, '\n');
    cout << "\nPresione ENTER para continuar...";
    cin.get();

    return true;
}


/// FUNCIONES GLOBALES DE LISTADOS Y REPORTES

/// Algoritmo: Ordena en memoria las clientas activas por Apellido (A-Z) y las muestra
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


/// Algoritmo: Filtra el listado cruzando datos con la clase Turno

// Estructura auxiliar local para el ordenamiento por frecuencia
struct RegistroFrecuencia {
    int idCliente;
    int cantidadTurnos;
};

void listarClientasPorFrecuencia() {
    Cliente cReg;
    int posC = 0;
    int totalClientas = 0;

    // Contamos cuantas clientas activas hay en disco para dimensionar la memoria exacta
    while (cReg.leerDisco(posC++)) {
        if (cReg.getEstado() == true) {
            totalClientas++;
        }
    }
    if (totalClientas == 0) {
        cout << "\n[AVISO] No hay clientas activas registradas en el sistema.\n";
        return;
    }

    // Creamos el array dinamico para guardar temporalmente los IDs y sus contadores
    RegistroFrecuencia* listaFrecuencia = new RegistroFrecuencia[totalClientas];
    if (listaFrecuencia == nullptr) {
        cout << "\n[ERROR] Fallo crítico de asignación de memoria dinámica.\n";
        return;
    }

    // Volcamos las clientas activas al array inicializando los turnos en 0
    posC = 0;
    int idx = 0;
    while (cReg.leerDisco(posC++)) {
        if (cReg.getEstado() == true) {
            listaFrecuencia[idx].idCliente = cReg.getIdCliente();
            listaFrecuencia[idx].cantidadTurnos = 0;
            idx++;
        }
    }

    // Cruzamos datos: Recorremos turnos.dat acumulando las citas de cada una
    Turno tReg;
    int posT = 0;
    while (tReg.leerDisco(posT++)) {
        if (tReg.getEstado() == true) {
            for (int i = 0; i < totalClientas; i++) {
                if (listaFrecuencia[i].idCliente == tReg.getIdCliente()) {
                    listaFrecuencia[i].cantidadTurnos++;
                    break;
                }
            }
        }
    }

    //Algoritmo de Ordenamiento: Metodo de la burbuja (de Mayor a Menor)
    for (int i = 0; i < totalClientas - 1; i++) {
        for (int j = 0; j < totalClientas - i - 1; j++) {
            if (listaFrecuencia[j].cantidadTurnos < listaFrecuencia[j+1].cantidadTurnos) {
                // Intercambio completo de la estructura
                RegistroFrecuencia aux = listaFrecuencia[j];
                listaFrecuencia[j] = listaFrecuencia[j+1];
                listaFrecuencia[j+1] = aux;
            }
        }
    }

    // Renderizado en pantalla con las mas frecuentes arriba
    cout << "=================================================" << endl;
    cout << "          LISTADO: CLIENTAS POR FRECUENCIA       " << endl;
    cout << "=================================================" << endl;
    cout << " ID   | Apellido y Nombre          | Citas Asignadas " << endl;
    cout << "-------------------------------------------------" << endl;

    for (int i = 0; i < totalClientas; i++) {
        Cliente cliAux;
        int posBusqueda = 0;
        char nombreCompleto[100] = "";

        // Buscamos el registro del cliente en disco para recuperar el nombre para el panel
        while (cliAux.leerDisco(posBusqueda++)) {
            if (cliAux.getIdCliente() == listaFrecuencia[i].idCliente) {
                strcpy(nombreCompleto, cliAux.getApellido());
                strcat(nombreCompleto, ", ");
                strcat(nombreCompleto, cliAux.getNombre());
                break;
            }
        }

        cout << "[" << (listaFrecuencia[i].idCliente < 10 ? "0" : "") << listaFrecuencia[i].idCliente << "] | "
             << nombreCompleto;

        // Formateador de espacios para alinear las columnas
        int largoTexto = strlen(nombreCompleto);
        for (int e = largoTexto; e < 26; e++) cout << " ";

        cout << " | " << listaFrecuencia[i].cantidadTurnos << " turno(s)" << endl;
    }
    cout << "=================================================" << endl << endl;

    // Liberacion de memoria dinamica
    delete[] listaFrecuencia;
}

/// Algoritmo: Listar clientas con saldos pendientes (agendados futuros / y en gabinete x liquidar)
void listarClientasConSaldos() {
    Cliente cli;
    int posC = 0;
    bool hayDeudas = false;

    cout << "=================================================" << endl;
    cout << "       REPORTES DE CUENTAS CORRIENTES: SALDOS    " << endl;
    cout << "=================================================" << endl;

    // Recorremos cada clienta activa del sistema
    while (cli.leerDisco(posC++)) {
        if (cli.getEstado() == true) {
            float deudatotalClienta = 0;

            Turno tur;
            int posT = 0;

            // Buscamos todos los turnos activos que tengan saldos pendientes (NO liquidados)
            while (tur.leerDisco(posT++)) {
                if (tur.getEstado() == true &&
                    tur.getIdCliente() == cli.getIdCliente() &&
                    tur.getLiquidado() == false) { // Si no esta liquidado, es un saldo pendiente

                    // Calculamos el costo total del servicio
                    DetalleTurno det;
                    int posD = 0;
                    float precioServicio = 0;
                    while (det.leerDisco(posD++)) {
                        if (det.getEstado() == true && det.getIdTurno() == tur.getIdTurno()) {
                            precioServicio += det.getPrecioAlMomento();
                        }
                    }

                    // El saldo pendiente es el total del tratamiento menos la seña que ya dejo
                    float saldoPendiente = precioServicio - tur.getSena();
                    if (saldoPendiente > 0) {
                        deudatotalClienta += saldoPendiente;
                    }
                }
            }

            // Si la clienta acumula saldos (ya sea por turnos futuros o de hoy), la listamos
            if (deudatotalClienta > 0) {
                if (!hayDeudas) {
                    cout << " ID   | Clienta                    | Saldo Pendiente " << endl;
                    cout << "-------------------------------------------------" << endl;
                    hayDeudas = true;
                }

                cout << " [" << (cli.getIdCliente() < 10 ? "0" : "") << cli.getIdCliente() << "] | "
                     << cli.getApellido() << ", " << cli.getNombre();

                // Formateador de espacios para alinear las columnas en la consola
                int largoNombre = strlen(cli.getApellido()) + strlen(cli.getNombre()) + 2;
                for (int e = largoNombre; e < 26; e++) cout << " ";

                cout << " | $" << deudatotalClienta << endl;
            }
        }
    }
    if (!hayDeudas) {
        cout << "\n[OK] No se registran saldos pendientes de cobro en este momento.\n";
    }
    cout << "=================================================" << endl << endl;
}


/// SUB-MENU INTERNO: PANEL DE CONSULTAS Y FILTROS CLIENTAS
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
            system("cls");
            cout << "=================================================" << endl;
            cout << "        LISTADO DE CLIENTAS ACTIVAS (A-Z)        " << endl;
            cout << "=================================================" << endl;
            listarClientasPorApellido();
            cout << "=================================================" << endl << endl;
        }
        else if (op == 2) {
            system("cls");
            cout << "=================================================" << endl;
            cout << "        CLIENTAS POR FRECUENCIA DE VISITAS       " << endl;
            cout << "=================================================" << endl << endl;
            listarClientasPorFrecuencia();
            cout << "=================================================" << endl << endl;
        }
        else if (op == 3) {
            system("cls");
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

/// MENU PRINCIPAL: MODULO DE CLIENTAS
void menuClientes() {
    int op;
    Cliente aux;

    do {
        cout << "=================================================" << endl;
        cout << "            MODULO: GESTION CLIENTAS             " << endl;
        cout << "=================================================" << endl;
        cout << "1. Registrar Nueva Clienta (Alta)" << endl;
        cout << "2. Listado de Clientas y Consultas" << endl;
        cout << "3. Modificar Datos de Clienta" << endl;
        cout << "4. Dar de baja Clienta (Baja Logica)" << endl;
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
            modificarCliente();
            system("cls");
        }
        else if (op == 4) {
            system("cls");
            darDeBajaCliente();
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
