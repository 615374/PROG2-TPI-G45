#include <iostream>
#include <cstdlib>
#include <cstring>
#include "MenuClientes.h"
#include "Cliente.h"
#include "Turno.h"
#include "DetalleTurno.h"

using namespace std;


// FUNCIONES GLOBALES DE LISTADOS Y REPORTES

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

// Algoritmo: Filtra clientas ordenadas por frecuencia de visitas, (cruzado con turnos)
void listarClientasPorFrecuencia() {
    Cliente cliente;
    Turno turno;
    int posCliente = 0;
    int cantidadClientas = 0;

    // Cuenta cuantas clientas activas existen
    while (cliente.leerDisco(posCliente)) {
        if (cliente.getEstado()) {
            cantidadClientas++;
        }
        posCliente++;
    }

    if (cantidadClientas == 0) {
        cout << "No hay clientas activas registradas.\n\n";
        return;
    }

    Cliente* vectorClientas = new Cliente[cantidadClientas];
    int* vectorCantidadTurnos = new int[cantidadClientas];

    if (vectorClientas == NULL || vectorCantidadTurnos == NULL) {
        cout << "[ERROR] Memoria insuficiente para generar el listado.\n\n";
        delete[] vectorClientas;
        delete[] vectorCantidadTurnos;
        return;
    }

    // Carga clientas activas y calcula su cantidad de turnos
    posCliente = 0;
    int indice = 0;

    while (cliente.leerDisco(posCliente)) {
        if (cliente.getEstado()) {
            vectorClientas[indice] = cliente;

            int cantidadTurnos = 0;
            int posTurno = 0;

            while (turno.leerDisco(posTurno)) {
                if (turno.getEstado() &&
                    turno.getIdCliente() == cliente.getIdCliente()) {
                    cantidadTurnos++;
                }
                posTurno++;
            }
            vectorCantidadTurnos[indice] = cantidadTurnos;
            indice++;
        }

        posCliente++;
    }

    // Ordena de mayor a menor segun la cantidad de turnos
    for (int i = 0; i < cantidadClientas - 1; i++) {
        for (int j = 0; j < cantidadClientas - i - 1; j++) {
            if (vectorCantidadTurnos[j] < vectorCantidadTurnos[j + 1]) {
                int auxCantidad = vectorCantidadTurnos[j];
                vectorCantidadTurnos[j] = vectorCantidadTurnos[j + 1];
                vectorCantidadTurnos[j + 1] = auxCantidad;

                Cliente auxCliente = vectorClientas[j];
                vectorClientas[j] = vectorClientas[j + 1];
                vectorClientas[j + 1] = auxCliente;
            }
        }
    }

    bool hayTurnos = false;
    for (int i = 0; i < cantidadClientas; i++) {
        if (vectorCantidadTurnos[i] > 0) {
            cout << "-----------------------------------" << endl;
            cout << "CLIENTA: " << vectorClientas[i].getApellido()
                 << ", " << vectorClientas[i].getNombre()
                 << " [" << vectorClientas[i].getIdCliente() << "] "
                 << endl;
            cout << "CANTIDAD DE TURNOS REGISTRADOS: " << vectorCantidadTurnos[i] << endl;
            cout << "-----------------------------------" << endl;

            hayTurnos = true;
        }
    }
    if (!hayTurnos) {
        cout << "No hay clientas con turnos registrados.\n\n";
    }

    delete[] vectorClientas;
    delete[] vectorCantidadTurnos;
}

// Algoritmo: Filtrara las clientas que posean deudas registradas
void listarClientasConSaldos() {
    Turno turno;
    Cliente cliente;
    int pos = 0;
    bool encontroSaldos = false;

    while (turno.leerDisco(pos)) {
        if (turno.getEstado()) {
            float totalTurno = calcularTotalTurno(turno.getIdTurno());
            float saldoPendiente = totalTurno - turno.getSena();

            if (saldoPendiente > 0) {
                cout << "-----------------------------------" << endl;
                cout << "ID TURNO: " << turno.getIdTurno() << endl;

                cout << "CLIENTA: ";
                if (!cliente.mostrarNombrePorId(turno.getIdCliente())) {
                    cout << "No disponible (ID: " << turno.getIdCliente() << ")";
                }
                cout << endl;

                cout << "TOTAL DEL TURNO: $" << totalTurno << endl;
                cout << "SENIA ABONADA: $" << turno.getSena() << endl;
                cout << "SALDO PENDIENTE: $" << saldoPendiente << endl;
                cout << "-----------------------------------" << endl;

                encontroSaldos = true;
            }
        }
        pos++;
    }
    if (!encontroSaldos) {
        cout << "No hay clientas con saldos pendientes.\n\n";
    }
}

// Permite buscar una clienta por apellido y devuelve el ID seleccionado
int seleccionarClientePorApellido() {
    Cliente cliente;
    char apellidoBuscado[50];
    int pos = 0;
    bool encontro = false;

    cin.ignore(1000, '\n');

    cout << "Ingrese apellido o inicio del apellido de la clienta: ";
    cin.getline(apellidoBuscado, 50);

    if (strlen(apellidoBuscado) == 0) {
        cout << "[ERROR] No se ingreso ningun criterio de busqueda.\n";
        return -1;
    }

    cout << "=================================================" << endl;
    cout << "       CLIENTAS ACTIVAS ENCONTRADAS              " << endl;
    cout << "=================================================" << endl;

    while (cliente.leerDisco(pos)) {
        if (cliente.getEstado() &&
            strncmp(cliente.getApellido(), apellidoBuscado, strlen(apellidoBuscado)) == 0) {

            cout << "ID: [" << cliente.getIdCliente() << "] - "
                 << cliente.getApellido() << ", "
                 << cliente.getNombre()
                 << " - Tel: " << cliente.getTelefono()
                 << endl;

            encontro = true;
        }

        pos++;
    }
    if (!encontro) {
        cout << "[AVISO] No se encontraron clientas con ese criterio.\n";
        return -1;
    }

    int idSeleccionado;
    cout << "-------------------------------------------------" << endl;
    cout << "Ingrese el ID de la clienta elegida: ";
    cin >> idSeleccionado;

    Cliente verificador;
    pos = 0;

    while (verificador.leerDisco(pos)) {
        if (verificador.getEstado() &&
            verificador.getIdCliente() == idSeleccionado) {
            return idSeleccionado;
        }
        pos++;
    }
    cout << "[ERROR] El ID ingresado no corresponde a una clienta activa.\n";
    return -1;
}

// Muestra historial de turnos y tratamientos de una clienta
void historialTratamientosPorClienta() {
    Cliente cliente;
    Turno turno;
    int posTurno  = 0;
    bool encontroHistorial = false;

    // Selecciona la clienta sin exigir que el usuario recuerde el ID
    int idClienteBuscado = seleccionarClientePorApellido();

    if (idClienteBuscado == -1) {
        return;
    }

    cout << "=================================================" << endl;
    cout << "            HISTORIAL DE TRATAMIENTOS            " << endl;
    cout << "=================================================" << endl;
    cout << "CLIENTA: ";
    cliente.mostrarNombrePorId(idClienteBuscado);
    cout << endl;

    while (turno.leerDisco(posTurno)) {
        if (turno.getEstado() &&
            turno.getIdCliente() == idClienteBuscado) {

            turno.mostrar();
            mostrarDetallesPorTurno(turno.getIdTurno());

            float totalTurno = calcularTotalTurno(turno.getIdTurno());
            float saldoPendiente = totalTurno - turno.getSena();

            if (saldoPendiente < 0) {
                saldoPendiente = 0;
            }

            cout << "TOTAL DEL TURNO: $" << totalTurno << endl;
            cout << "SALDO PENDIENTE: $" << saldoPendiente << endl;
            cout << "-----------------------------------" << endl;

            encontroHistorial = true;
        }
        posTurno++;
    }
    if (!encontroHistorial) {
        cout << "La clienta no tiene turnos registrados.\n\n";
    }
}

// FUNCIONES GLOBALES DE MANTENIMIENTO
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
            cin.ignore(1000, '\n');
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

//BAJA LOGICA
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
        cout << "4. Historial de Tratamientos por Clienta" << endl;
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
        cout << "4. Historial de Tratamientos por Clienta" << endl;
        cout << "0. Volver al Menu de Gestion" << endl;
        cout << "-------------------------------------------------" << endl;
        cout << "Opcion seleccionada: " << op << endl << endl;

        if (op == 1) {
            cout << "=================================================" << endl;
            cout << "        LISTADO DE CLIENTAS ACTIVAS (A-Z)        " << endl;
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
                else if (op == 4) {
            cout << "=================================================" << endl;
            cout << "       HISTORIAL DE TRATAMIENTOS POR CLIENTA     " << endl;
            cout << "=================================================" << endl << endl;
            historialTratamientosPorClienta();
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
