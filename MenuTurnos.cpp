#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <ctime>
#include "MenuTurnos.h"
#include "Turno.h"
#include "Cliente.h"
#include "Servicio.h"
#include "DetalleTurno.h"

using namespace std;

// FUNCIONES GLOBALES DE AUXILIO PARA EL MENÚ

// Funcion Auxiliar 1: Revisa si hay un turno activo en esa fecha y hora especifica
bool verificarBloqueOcupado(int d, int m, int a, int horaEvaluar) {
    Turno t;
    DetalleTurno dt;
    int posT = 0;

    // Recorremos las cabeceras de turnos
    while (t.leerDisco(posT)) {
        if (t.getEstado() == true &&
            t.getFecha().getDia() == d &&
            t.getFecha().getMes() == m &&
            t.getFecha().getAnio() == a) {

            // Si encontramos un turno para ese dia, revisamos sus detalles por la hora
            int posDT = 0;
            while (dt.leerDisco(posDT)) {
                if (dt.getEstado() == true &&
                    dt.getIdTurno() == t.getIdTurno() &&
                    dt.getHora() == horaEvaluar) {
                    return true; // Bloque ocupado
                }
                posDT++;
            }
        }
        posT++;
    }
    return false; // Bloque libre
}

// Funcion Auxiliar 2: Renderiza la grilla semanal vertical
void mostrarGrillaSemanalAuto() {
    time_t tActual = time(0);

    cout << "\n=================================================" << endl;
    cout << "        DISPONIBILIDAD: PROXIMOS 5 DIAS HABILES  " << endl;
    cout << "=================================================" << endl;

    int diasListados = 0;
    int offset = 0; // Empezamos desde HOY

    while (diasListados < 5) {
        time_t tDia = tActual + (offset * 24 * 60 * 60);
        tm* infoDia = localtime(&tDia);

        // 1 a 5 son Lunes a Viernes
        if (infoDia->tm_wday >= 1 && infoDia->tm_wday <= 5) {
            int d = infoDia->tm_mday;
            int m = infoDia->tm_mon + 1;
            int a = infoDia->tm_year + 1900;
            const char* nombres[] = {"", "LUNES", "MARTES", "MIERCOLES", "JUEVES", "VIERNES"};

            cout << "\n--- " << nombres[infoDia->tm_wday] << " " << (d < 10 ? "0" : "") << d
                 << "/" << (m < 10 ? "0" : "") << m << "/" << a << " ---" << endl;

            for (int h = 9; h <= 18; h++) {
                bool ocupado = verificarBloqueOcupado(d, m, a, h);
                cout << "  [" << (h < 10 ? "0" : "") << h << ":00 hs] --> "
                     << (ocupado ? "[OCUPADO]" : "DISPONIBLE") << endl;
            }
            diasListados++;
        }
        offset++;
    }
    cout << "=================================================" << endl;
    cout << "Presione ENTER para volver al menu...";
    cin.ignore(1000, '\n');
    cin.get();
}

// Algoritmo: Busca clientas por coincidencia parcial en el apellido y retorna el ID seleccionado
int buscarClienteParaTurno(const char* apellidoBuscado) {
    Cliente reg;
    int pos = 0;
    bool encontrado = false;

    int tam = strlen(apellidoBuscado);

    if (tam == 0) {
        cout << "\n[ERROR] No se ingreso ningun criterio de busqueda.\n";
        return -1;
    }

    cout << "=================================================" << endl;
    cout << "  CLIENTAS ACTIVAS QUE EMPIEZAN CON: " << apellidoBuscado << endl;
    cout << "=================================================" << endl;

    while (reg.leerDisco(pos)) {
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

using namespace std;

//Algoritmo: Lista turnos activos en forma cronologica
void listarTurnosActivosCronologicos() {
    Turno aux;
    int pos = 0, cant = 0;

    // Contamos activos
    while(aux.leerDisco(pos++)) if(aux.getEstado()) cant++;

    if(cant == 0) {
        cout << "No hay turnos activos.\n";
        return;
    }

    // Cargamos y ordenamos
    Turno* lista = new Turno[cant];
    pos = 0; int i = 0;
    while(aux.leerDisco(pos++)) {
        if(aux.getEstado()) lista[i++] = aux;
    }

    // Ordenamiento burbuja por fecha (Año, Mes, Dia)
    for(int j = 0; j < cant - 1; j++) {
        for(int k = 0; k < cant - j - 1; k++) {
            if(lista[k].getFecha().getAnio() > lista[k+1].getFecha().getAnio() ||
              (lista[k].getFecha().getAnio() == lista[k+1].getFecha().getAnio() &&
               lista[k].getFecha().getMes() > lista[k+1].getFecha().getMes()) ||
              (lista[k].getFecha().getAnio() == lista[k+1].getFecha().getAnio() &&
               lista[k].getFecha().getMes() == lista[k+1].getFecha().getMes() &&
               lista[k].getFecha().getDia() > lista[k+1].getFecha().getDia())) {
                Turno temp = lista[k];
                lista[k] = lista[k+1];
                lista[k+1] = temp;
            }
        }
    }

    // Mostrar
    system("cls");
    cout << "--- AGENDA DE TURNOS (ORDEN CRONOLOGICO) ---" << endl;
    for(int j = 0; j < cant; j++) lista[j].mostrar();

    delete[] lista;
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
        cout << "4. Consultar Disponibilidad Semanal (Grilla)"      << endl;
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
            cout << "1. Para Clienta NUEVA (Registrar Primero)"         << endl;
            cout << "2. Para Clienta HABITUAL (Buscar por Apellido)"    << endl;
            cout << "0. Cancelar / Volver" << endl;
            cout << "-------------------------------------------------" << endl;
            cout << "Seleccione una opcion: ";
            cin >> subOp;

            if (subOp == 1) {
                system("cls");
                Cliente nuevoCliente;
                if (nuevoCliente.cargar() == true) {
                    if (nuevoCliente.escribirDisco()) {
                        cout << "\n[OK] Clienta registrada con éxito en el sistema.\n";
                        idResuelto = nuevoCliente.getIdCliente();
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
                cout << "           BUSQUEDA DE CLIENTA HABITUAL          " << endl;
                cout << "=================================================" << endl;
                cout << "Ingrese Apellido (o su primera letra): ";
                cin.getline(apellidoBuscado, 50);

                idResuelto = buscarClienteParaTurno(apellidoBuscado);
            }

            if (idResuelto != -1) {
                cout << "\nPresione ENTER para ingresar los datos del Turno...";
                cin.ignore(1000, '\n');
                cin.get();
                system("cls");

                aux.setIdCliente(idResuelto);

                if (aux.cargar() == true) {
                    if (aux.escribirDisco()) {
                        cout << "\n[OK] Cabecera del Turno agendada con exito.\n\n";

                        int idServicioElegido;
                        Servicio servAux;

                        cout << "=================================================" << endl;
                        cout << "       CARGA DE TRATAMIENTOS PARA EL TURNO       " << endl;
                        cout << "=================================================" << endl;

                        do {
                            cout << "\nSERVICIOS / TRATAMIENTOS DISPONIBLES:\n";
                            cout << "-------------------------------------------------\n";
                            int posListado = 0;
                            Servicio listador;
                            bool hayServicios = false;

                            while (listador.leerDisco(posListado)) {
                                if (listador.getEstado() == true) {
                                    cout << " ID: [" << listador.getIdServicio() << "] - "
                                         << listador.getNombre() << " ($" << listador.getPrecio() << ")\n";
                                    hayServicios = true;
                                }
                                posListado++;
                            }
                            if (!hayServicios) {
                                cout << "[AVISO] No hay servicios activos cargados en el sistema.\n";
                            }
                            cout << "-------------------------------------------------\n";

                            cout << "Ingrese ID del Servicio deseado (0 para finalizar el turno): ";
                            cin >> idServicioElegido;

                            if (idServicioElegido != 0) {
                                int posS = 0;
                                bool servEncontrado = false;
                                float precioBase = 0;

                                while (servAux.leerDisco(posS)) {
                                    if (servAux.getIdServicio() == idServicioElegido && servAux.getEstado() == true) {
                                        servEncontrado = true;
                                        precioBase = servAux.getPrecio();
                                        cout << "\n>> SELECCIONADO: " << servAux.getNombre() << " (Precio base: $" << precioBase << ")\n";
                                        break;
                                    }
                                    posS++;
                                }

                                if (servEncontrado) {
                                    DetalleTurno detAux;
                                    if (detAux.cargar(aux.getIdTurno(), idServicioElegido, precioBase)) {
                                        if (detAux.escribirDisco()) {
                                            cout << "\n[OK] Tratamiento agregado al desglose del turno.\n";
                                        } else {
                                            cout << "\n[ERROR] No se pudo guardar el detalle en disco.\n";
                                        }
                                    }
                                } else {
                                    cout << "[ERROR] El ID de servicio ingresado no existe o esta inactivo.\n\n";
                                }
                                cout << "-------------------------------------------------\n";
                            }

                        } while (idServicioElegido != 0);

                        cout << "\n[SISTEMA] Registro de turno completo finalizado correctamente.\n\n";

                    } else {
                        cout << "\n[ERROR] No se pudo registrar en el archivo de turnos.\n\n";
                    }
                }
            } else if (subOp != 0) {
                cout << "\n[AVISO] No se pudo procesar el turno porque no se identifico a la clienta.\n\n";
            }

            cout << "Presione ENTER para continuar...";
            cin.ignore(1000, '\n');
            cin.get();
            system("cls");
        }
        else if (op == 2) {
            system("cls");
            cout << "=================================================" << endl;
            cout << "             AGENDA DE TURNOS ACTIVOS            " << endl;
            cout << "=================================================" << endl;

            listarTurnosActivosCronologicos();
            cout << "Presione ENTER para volver...";
            cin.ignore(1000, '\n'); cin.get();
            system("cls");
        }
        else if (op == 3) {
            system("cls");
            darDeBajaTurno();
            system("cls");
        }
        else if (op == 4) {
            system("cls");
            mostrarGrillaSemanalAuto();
            cout << "Presione ENTER para volver al menu...";
            cin.ignore(1000, '\n');
            cin.get();
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
