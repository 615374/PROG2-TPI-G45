#include <iostream>
#include <cstdlib>
#include <cstring>
#include "MenuTurnos.h"
#include "Turno.h"
#include "Cliente.h"

using namespace std;


// FUNCIONES GLOBALES

// Algoritmo: Busca clientas por coincidencia parcial en el apellido y retorna el ID seleccionado
int buscarClienteParaTurno(const char* apellidoBuscado) {
    Cliente reg;
    int pos = 0;
    bool encontrado = false;

    // Calculamos cuantas letras escribio la recepcionista (ej: si puso "A", tam es 1; si puso "Per", tam es 3)
    int tam = strlen(apellidoBuscado);

    // Si la recepcionista no escribio nada, salimos para evitar que se congele el programa
    if (tam == 0) {
        cout << "\n[ERROR] No se ingreso ningun criterio de busqueda.\n";
        return -1;
    }

    cout << "=================================================" << endl;
    cout << "  CLIENTAS ACTIVAS QUE EMPIEZAN CON: "             << apellidoBuscado << endl;
    cout << "=================================================" << endl;

    while (reg.leerDisco(pos)) {
        // Usamos strncmp para comparar solo los primeros 'tam' caracteres
        if (reg.getEstado() == true && strncmp(reg.getApellido(), apellidoBuscado, tam) == 0) {
            cout << " ID: [" << reg.getIdCliente() << "] - "
                 << reg.getApellido() << ", " << reg.getNombre()
                 << " - Tel: " << reg.getTelefono() << endl;
            encontrado = true;
        }
        pos++;
    }

    if (!encontrado) {
        cout << "\n[AVISO] No se encontraron clientas activas con ese criterio.\n";
        return -1;
    }

    int idSeleccionado;
    cout << "-------------------------------------------------" << endl;
    cout << "Ingrese el ID de la clienta elegida: ";
    cin >> idSeleccionado;

    // Validacion directa contra el disco
    pos = 0;
    Cliente verificador;
    while (verificador.leerDisco(pos)) {
        if (verificador.getIdCliente() == idSeleccionado && verificador.getEstado() == true) {
            return idSeleccionado;
        }
        pos++;
    }

    cout << "\n[ERROR] El ID ingresado es incorrecto o pertenece a una clienta inactiva.\n";
    return -1;
}

// BAJA LOGICA TURNO
bool darDeBajaTurno() {
    int idBuscado;
    Turno reg;
    int pos = 0;
    bool encontrado = false;

    cout << "=================================================" << endl;
    cout << "          SELECCIONE TURNO PARA DAR DE BAJA      " << endl;
    cout << "=================================================" << endl;
    cout << "0. Volver al Menu Principal / Cancelar Baja      " << endl;
    cout << "-------------------------------------------------" << endl;

    // Lista los turnos activos
    while (reg.leerDisco(pos)) {
        if (reg.getEstado() == true) {
            reg.mostrar();
        }
        pos++;
    }

    cout << "Ingrese el ID del turno a dar de baja: ";
    cin >> idBuscado;

    if (idBuscado == 0) {
        cout << "\nOperacion cancelada. Volviendo al menu...\n";
        return false;
    }

    pos = 0;
    FILE* p = fopen("turnos.dat", "rb+");
    if (p == NULL) {
        cout << "\n[ERROR] No se pudo acceder al archivo.\n";
        return true;
    }

    while (fread(&reg, sizeof(Turno), 1, p) == 1) {
        if (reg.getIdTurno() == idBuscado && reg.getEstado() == true) {
            encontrado = true;
            reg.setEstado(false);

            fseek(p, pos * sizeof(Turno), SEEK_SET);
            fwrite(&reg, sizeof(Turno), 1, p);

            cout << "\n[OK] Turno dado de baja correctamente del sistema.\n";
            break;
        }
        pos++;
    }
    fclose(p);

    if (!encontrado) {
        cout << "\n[ERROR] No se encontro ningun turno activo con ese ID.\n";
    }

    cin.ignore(1000, '\n');
    cout << "\nPresione ENTER para continuar...";
    cin.get();

    return true;
}


// SUB-MENU PRINCIPAL: MODULO DE TURNOS
void menuTurnos() {
    int op;
    Turno aux;

    do {
        cout << "=================================================" << endl;
        cout << "           MODULO: GESTION DE TURNOS             " << endl;
        cout << "=================================================" << endl;
        cout << "1. Registrar Turno (Agendar)" << endl;
        cout << "2. Ver Agenda de Turnos Activos" << endl;
        cout << "3. Cancelar Turno (Baja Logica)" << endl;
        cout << "0. Volver al Menu Principal" << endl;
        cout << "-------------------------------------------------" << endl;
        cout << "Seleccione una opcion: ";
        cin >> op;
        cout << endl;

        if (op == 1) {
            system("cls");
            int subOp;
            int idResuelto = -1;

            cout << "=================================================" << endl;
            cout << "          REGISTRAR TURNO: TIPO DE CLIENTA       " << endl;
            cout << "=================================================" << endl;
            cout << "1. Para Clienta NUEVA (Registrar Primero)" << endl;
            cout << "2. Para Clienta HABITUAL (Buscar por Apellido)" << endl;
            cout << "0. Cancelar / Volver" << endl;
            cout << "-------------------------------------------------" << endl;
            cout << "Seleccione una opcion: ";
            cin >> subOp;

            if (subOp == 1) {
                system("cls");
                Cliente nuevoCliente;
                // Registramos a la clienta en caliente
                if (nuevoCliente.cargar() == true) {
                    if (nuevoCliente.escribirDisco()) {
                        cout << "\n[OK] Clienta registrada con éxito en el sistema.\n";
                        idResuelto = nuevoCliente.getIdCliente(); // Capturamos el ID asignado
                    } else {
                        cout << "\n[ERROR] No se pudo guardar la clienta en el disco.\n";
                    }
                }
            }
            else if (subOp == 2) {
                system("cls");
                char apellidoBuscado[50];
                cin.ignore(1000, '\n');

                cout << "=================================================" << endl;
                cout << "          BUSQUEDA DE CLIENTA HABITUAL           " << endl;
                cout << "=================================================" << endl;
                cout << "Ingrese Apellido de la clienta: ";
                cin.getline(apellidoBuscado, 50);

                // Buscamos las coincidencias y obtenemos el ID elegido
                idResuelto = buscarClienteParaTurno(apellidoBuscado);
            }

            if (idResuelto != -1) {
                Turno turnoNuevo;
                turnoNuevo.setIdCliente(idResuelto);

                if (turnoNuevo.cargar() == true) {
                    if (turnoNuevo.escribirDisco()) {
                        cout << "\n[OK] Turno agendado con exito.\n";
                    } else {
                        cout << "\n[ERROR] No se pudo guardar en disco.\n";
                    }
                }
            }
            cin.ignore(1000, '\n');
            cout << "Presione ENTER para continuar..."; cin.get();
            system("cls");
        }
        else if (op == 2) {
            system("cls");
            cout << "=================================================" << endl;
            cout << "           AGENDA DE TURNOS ACTIVOS              " << endl;
            cout << "=================================================" << endl;

            int pos = 0;
            bool hayTurnos = false;

            while (aux.leerDisco(pos)) {
                if (aux.getEstado() == true) {
                    aux.mostrar();
                    hayTurnos = true;
                }
                pos++;
            }

            if (pos == 0) {
                cout << "Archivo de turnos vacio.\n\n";
            } else if (!hayTurnos) {
                cout << "No hay turnos activos agendados para mostrar.\n\n";
            }
            cout << "=================================================" << endl << endl;

            cin.ignore(1000, '\n');
            cout << "Presione ENTER para volver al menu...";
            cin.get();
            system("cls");
        }
        else if (op == 3) {
            system("cls");
            darDeBajaTurno();
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
