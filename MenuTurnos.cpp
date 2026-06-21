#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <ctime>
#include "MenuTurnos.h"
#include "Turno.h"
#include "Cliente.h"
#include "Profesional.h"
#include "Servicio.h"
#include "DetalleTurno.h"
#include "ServicioXProfesional.h"

using namespace std;

// FUNCIONES GLOBALES DE AUXILIO PARA EL MENÚ

// Funcion auxiliar 1: Revisa si hay un turno activo en esa fecha y hora especifica
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

// Funcion auxiliar 2: Renderiza la grilla semanal vertical
void mostrarGrillaSemanalAuto() {
    time_t tActual = time(0);
    tm* ahora = localtime(&tActual);

    // calculamos la diferencia de dias para posicionarnos en el Lunes de la semana actual
    // ahora->tm_wday va de 0 (Domingo) a 6 (Sábado)
    int corregirALunes = ahora->tm_wday - 1;
    if (ahora->tm_wday == 0) corregirALunes = 6; // Si es domingo, retrocedemos al lunes de la semana que cierra

    cout << "=================================================" << endl;
    cout << "          GRILLA DE DISPONIBILIDAD SEMANAL       " << endl;
    cout << "=================================================" << endl;

    const char* diasNombres[] = {"LUNES", "MARTES", "MIERCOLES", "JUEVES", "VIERNES"};

    // Iteramos de Lunes a Viernes
    for (int i = 0; i < 5; i++) {
        // Desplazamos los días en segundos con respecto al Lunes actual
        time_t tDia = tActual - (corregirALunes - i) * 24 * 60 * 60;
        tm* infoDia = localtime(&tDia);

        int diaCalculado = infoDia->tm_mday;
        int mesCalculado = infoDia->tm_mon + 1;
        int anioCalculado = infoDia->tm_year + 1900;

        cout << "\n-------------------------------------------------" << endl;
        cout << "  " << diasNombres[i] << " " << (diaCalculado < 10 ? "0" : "") << diaCalculado
             << "/" << (mesCalculado < 10 ? "0" : "") << mesCalculado << "/" << anioCalculado << endl;
        cout << "-------------------------------------------------" << endl;

        // Horario comercial de atención restringido: de 09 a 18 hs
        for (int h = 9; h <= 18; h++) {
            bool ocupado = verificarBloqueOcupado(diaCalculado, mesCalculado, anioCalculado, h);

            if (!ocupado) {
                cout << "  [" << (h < 10 ? "0" : "") << h << ":00 hs] --> DISPONIBLE" << endl;
            } else {
                cout << "  [" << (h < 10 ? "0" : "") << h << ":00 hs] --> [OCUPADO]" << endl;
            }
        }
    }
    cout << "=================================================\n" << endl;
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
        cout << "            MODULO: GESTION DE TURNOS            " << endl;
        cout << "=================================================" << endl;
        cout << "1. Registrar Turno (Agendar)"                      << endl;
        cout << "2. Ver Agenda de Turnos Activos"                   << endl;
        cout << "3. Cancelar Turno (Baja Logica)"                   << endl;
        cout << "4. Consultar Disponibilidad Semanal (Grilla)"      << endl;
        cout << "0. Volver al Menu Principal"                       << endl;
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

            // Si logramos resolver el ID de la cliente (Nueva o Habitual) pasamos al tratamiento
            if (idResuelto != -1) {
                cout << "\nPresione ENTER para ir al panel de tratamientos...";
                cin.ignore(1000, '\n');
                cin.get();
                system("cls");

                int idServicioElegido = -1;
                Servicio servAux;

                cout << "=================================================" << endl;
                cout << "        SELECCIONE EL TRATAMIENTO DESEADO        " << endl;
                cout << "=================================================" << endl;
                cout << " TRATAMIENTOS DISPONIBLES:\n";
                cout << "-------------------------------------------------\n";

                int posListado = 0;
                Servicio listador;
                bool hayServicios = false;

                // Listamos los tratamientos activos en el sistema
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

                cout << "Ingrese ID del Servicio deseado (0 para cancelar): ";
                cin >> idServicioElegido;

                // Validacion del servicio seleccionado
                if (idServicioElegido != 0) {
                    int posS = 0;
                    bool servEncontrado = false;
                    float precioBase = 0;

                    while (servAux.leerDisco(posS)) {
                        if (servAux.getIdServicio() == idServicioElegido && servAux.getEstado() == true) {
                            servEncontrado = true;
                            precioBase = servAux.getPrecio();
                            break;
                        }
                        posS++;
                    }

                    if (servEncontrado) {
                        system("cls");
                        cout << "=================================================" << endl;
                        cout << "          SELECCION DE PROFESIONAL HABILITADA    " << endl;
                        cout << "=================================================" << endl;
                        cout << ">> Tratamiento: " << servAux.getNombre() << " ($"  << precioBase << ")\n";
                        cout << "-------------------------------------------------\n";
                        cout << "PROFESIONALES DISPONIBLES PARA ESTE TRATAMIENTO:\n\n";

                        ServicioXProfesional rel;
                        Profesional prof;
                        int posRel = 0;
                        bool hayProfesionales = false;

                        // Recorremos la tabla intermedia buscando quien hace este servicio
                        while (rel.leerDisco(posRel)) {
                            if (rel.getEstado() == true && rel.getIdServicio() == idServicioElegido) {
                                // Si encontramos la relacion, levantamos los datos del profesional
                                if (prof.buscarPorId(rel.getIdProfesional())) {
                                    if (prof.getEstado() == true) {
                                        cout << " ID: [" << prof.getIdProfesional() << "] - "
                                             << prof.getApellido() << ", " << prof.getNombre() << endl;
                                        hayProfesionales = true;
                                    }
                                }
                            }
                            posRel++;
                        }

                        if (!hayProfesionales) {
                            cout << "[AVISO] No hay profesionales configuradas o activas para brindar este servicio.\n";
                            cout << "-------------------------------------------------\n";
                        }
                        else {
                            cout << "-------------------------------------------------\n";
                            int idProfesionalElegido;
                            cout << "Ingrese ID de la Profesional elegida: ";
                            cin >> idProfesionalElegido;

                            // Validamos que el ID ingresado realmente pertenezca a las listadas
                            posRel = 0;
                            bool idValido = false;
                            while (rel.leerDisco(posRel)) {
                                if (rel.getEstado() == true &&
                                    rel.getIdServicio() == idServicioElegido &&
                                    rel.getIdProfesional() == idProfesionalElegido) {
                                    idValido = true;
                                    break;
                                }
                                posRel++;
                            }

                            if (idValido) {
                                system("cls");
                                cout << "=================================================" << endl;
                                cout << "           DATOS DE PRE-RESERVA VALIDADOS        " << endl;
                                cout << "=================================================" << endl;

                                // Levantamos el cliente para mostrar sus datos
                                Cliente cliAux;
                                int posC = 0;
                                while(cliAux.leerDisco(posC++)) {
                                    if(cliAux.getIdCliente() == idResuelto) break;
                                }

                                // Recargamos profesional para extraer nombre completo
                                prof.buscarPorId(idProfesionalElegido);

                                cout << "Clienta: " << cliAux.getNombre() << " " << cliAux.getApellido() << " (ID " << idResuelto << ")" << endl;
                                cout << "Tratamiento: " << servAux.getNombre() << " (ID " << idServicioElegido << ")" << endl;
                                cout << "Profesional: " << prof.getNombre() << " " << prof.getApellido() << " (ID " << idProfesionalElegido << ")" << endl;
                                cout << "-------------------------------------------------\n" << endl;

                                // ====================================================================
                                // PASO 4: (PERSISTENCIA Y DATOS DE FECHA/HORA/SEÑA)
                                // ====================================================================
                                cout << "[SISTEMA] Memoria lista. En el siguiente paso capturamos los datos\n";
                                cout << "          del Turno (Fecha, Hora de llegada y Seña) para escribir en disco.\n";

                            } else {
                                cout << "\n[ERROR] El ID ingresado no corresponde a una profesional habilitada para este servicio.\n";
                            }
                        }
                    } else {
                        cout << "\n[ERROR] El ID de servicio ingresado no existe o esta inactivo.\n";
                    }
                } else {
                    cout << "\nOperacion cancelada. Volviendo al menu...\n";
                }
            } else if (subOp != 0) {
                cout << "\n[AVISO] No se pudo procesar el turno porque no se identifico a la clienta.\n\n";
            }

            cout << "\nPresione ENTER para continuar...";
            cin.ignore(1000, '\n');
            cin.get();
            system("cls");
        }
        else if (op == 2) {
            system("cls");
            cout << "=================================================" << endl;
            cout << "              AGENDA DE TURNOS ACTIVOS           " << endl;
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
