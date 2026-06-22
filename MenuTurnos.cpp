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
#include "Fecha.h"

using namespace std;

// FUNCIONES GLOBALES DE SOPORTE Y LOGICA DE NEGOCIO

//-------------------------------------
// VERIFICAR BLOQUE HORARIO OCUPADO
//-------------------------------------

// Revisa si hay un turno activo en esa fecha y hora especifica
bool verificarBloqueOcupado(int d, int m, int a, int horaEvaluar) {
    Turno t;
    DetalleTurno dt;
    int posT = 0;

    while (t.leerDisco(posT)) {
        if (t.getEstado() == true &&
            t.getFecha().getDia() == d &&
            t.getFecha().getMes() == m &&
            t.getFecha().getAnio() == a) {

            int posDT = 0;
            while (dt.leerDisco(posDT)) {
                if (dt.getEstado() == true &&
                    dt.getIdTurno() == t.getIdTurno() &&
                    dt.getHora() == horaEvaluar) {
                    return true;
                }
                posDT++;
            }
        }
        posT++;
    }
    return false;
}

//-----------------------------------------------
// GRILLA DE DISPONIBILIDAD SEMANAL INTERACTIVA
//-----------------------------------------------

//Renderiza la grilla de la semana entrante si es fin de semana
void mostrarGrillaSemanalAuto() {
    time_t tActual = time(0);
    tm* ahora = localtime(&tActual);

    // Si es sabado (6) o domingo (0), nos paramos en la semana que entra
    int corregirALunes = ahora->tm_wday - 1;
    if (ahora->tm_wday == 0) corregirALunes = -1; // Apunta al lunes de mañana
    else if (ahora->tm_wday == 6) corregirALunes = -2; // Apunta al lunes siguiente

    cout << "=================================================" << endl;
    cout << "         GRILLA DE DISPONIBILIDAD SEMANAL        " << endl;
    cout << "=================================================" << endl;

    const char* diasNombres[] = {"LUNES", "MARTES", "MIERCOLES", "JUEVES", "VIERNES"};

    for (int i = 0; i < 5; i++) {
        time_t tDia = tActual - (corregirALunes - i) * 24 * 60 * 60;
        tm* infoDia = localtime(&tDia);

        int diaCalculado = infoDia->tm_mday;
        int mesCalculado = infoDia->tm_mon + 1;
        int anioCalculado = infoDia->tm_year + 1900;

        cout << "\n-------------------------------------------------" << endl;
        cout << "  " << diasNombres[i] << " " << (diaCalculado < 10 ? "0" : "") << diaCalculado
             << "/" << (mesCalculado < 10 ? "0" : "") << mesCalculado << "/" << anioCalculado << endl;
        cout << "-------------------------------------------------" << endl;

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

//------------------------------------------------------------
// FILTRADO DE CLIENTAS ACTIVAS POR COINCIDENCIA DE APELLIDO
//------------------------------------------------------------

// Busca clientas por coincidencia parcial en el apellido y retorna el ID seleccionado
int buscarClienteParaTurno(const char* apellidoBuscado) {
    Cliente reg;
    int pos = 0;
    bool encontrado = false;

    int tam = strlen(apellidoBuscado);

    if (tam == 0) {
        cout << "\n[ERROR] No se ingreso ningun criterio de busqueda.\n";
        return -1;
    }

    system("cls");

    cout << "=================================================" << endl;
    cout << "  CLIENTAS ACTIVAS QUE EMPIEZAN CON: "             << apellidoBuscado << endl;
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
    cout << "Ingrese el ID de la clienta elegida (0 para cancelar): ";
    cin >> idSeleccionado;

    // Opcion de 0 para cancelar / retornar al menu
    if (idSeleccionado == 0) {
        cout << "\nOperacion cancelada por el usuario.\n";
        return -1;
    }

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

//-----------------------------------------------------
// GENERACION CRONOLOGICA DE AGENDA (MEMORIA DINAMICA)
//-----------------------------------------------------

// Extrae, ordena cronologicamente y lista la agenda diaria con horario de llegada
void listarAgendaCronologica() {
    Turno tReg;
    int pos = 0;
    int totalTurnosActivos = 0;

    // Contamos cuantos turnos activos hay para dimensionar la memoria exacta
    while (tReg.leerDisco(pos++)) {
        if (tReg.getEstado() == true) {
            totalTurnosActivos++;
        }
    }

    if (totalTurnosActivos == 0) {
        cout << "No hay turnos activos agendados para mostrar.\n\n";
        return;
    }

    // Creamos los arrays dinamicos en memoria
    Turno* listaTurnos = new Turno[totalTurnosActivos];
    int* horasArribo = new int[totalTurnosActivos];
    int* minsArribo = new int[totalTurnosActivos];

    if (listaTurnos == nullptr || horasArribo == nullptr || minsArribo == nullptr) {
        cout << "\n[ERROR] Fallo critico de asignacion de memoria dinamica.\n";
        return;
    }

    pos = 0;
    int idx = 0;
    // Volcamos los registros del disco a las estructuras en memoria y calculamos la hora de llegada
    while (tReg.leerDisco(pos++)) {
        if (tReg.getEstado() == true) {
            listaTurnos[idx] = tReg;

            DetalleTurno dtReg;
            int posDT = 0;
            int horaMinima = 99;
            int minMinimo = 99;

            while (dtReg.leerDisco(posDT++)) {
                if (dtReg.getEstado() == true && dtReg.getIdTurno() == tReg.getIdTurno()) {
                    if (dtReg.getHora() < horaMinima || (dtReg.getHora() == horaMinima && dtReg.getMinuto() < minMinimo)) {
                        horaMinima = dtReg.getHora();
                        minMinimo = dtReg.getMinuto();
                    }
                }
            }

            horasArribo[idx] = (horaMinima == 99) ? 0 : horaMinima;
            minsArribo[idx] = (minMinimo == 99) ? 0 : minMinimo;
            idx++;
        }
    }

    // Algoritmo de Ordenamiento cronologico (Burbuja)
    for (int i = 0; i < totalTurnosActivos - 1; i++) {
        for (int j = 0; j < totalTurnosActivos - i - 1; j++) {
            bool intercambiar = false;
            Fecha f1 = listaTurnos[j].getFecha();
            Fecha f2 = listaTurnos[j+1].getFecha();

            if (f1.getAnio() > f2.getAnio()) intercambiar = true;
            else if (f1.getAnio() == f2.getAnio()) {
                if (f1.getMes() > f2.getMes()) intercambiar = true;
                else if (f1.getMes() == f2.getMes()) {
                    if (f1.getDia() > f2.getDia()) intercambiar = true;
                    else if (f1.getDia() == f2.getDia()) {
                        if (horasArribo[j] > horasArribo[j+1]) intercambiar = true;
                        else if (horasArribo[j] == horasArribo[j+1] && minsArribo[j] > minsArribo[j+1]) intercambiar = true;
                    }
                }
            }

            if (intercambiar) {
                // Intercambio de Turnos
                Turno tAux = listaTurnos[j];
                listaTurnos[j] = listaTurnos[j+1];
                listaTurnos[j+1] = tAux;

                // Intercambio paralelo de horarios de arribo
                int hAux = horasArribo[j];
                horasArribo[j] = horasArribo[j+1];
                horasArribo[j+1] = hAux;

                int mAux = minsArribo[j];
                minsArribo[j] = minsArribo[j+1];
                minsArribo[j+1] = mAux;
            }
        }
    }

    // Renderizado estetico de la agenda diaria con detalle de servicio/s y profesional/es
    cout << "========================================================================================================================" << endl;
    cout << " ID | FECHA      | HORA  | CLIENTA            | TRATAMIENTO                         | PROFESIONAL  | SENIA  | ASIS | LIQ " << endl;
    cout << "========================================================================================================================" << endl;

    for (int i = 0; i < totalTurnosActivos; i++) {
        Cliente cli;
        char nombreClienta[40] = "No disp.";

        // Recuperamos el nombre de la clienta
        int posC = 0;
        while (cli.leerDisco(posC++)) {
            if (cli.getIdCliente() == listaTurnos[i].getIdCliente()) {
                strcpy(nombreClienta, cli.getApellido());
                strcat(nombreClienta, ", ");
                strcat(nombreClienta, cli.getNombre());
                if (strlen(nombreClienta) > 18) {
                    nombreClienta[15] = '.'; nombreClienta[16] = '.'; nombreClienta[17] = '.'; nombreClienta[18] = '\0';
                }
                break;
            }
        }

        // Imprimimos los datos fijos de la Cabecera del Turno
        // ID
        cout << " " << (listaTurnos[i].getIdTurno() < 10 ? "0" : "") << listaTurnos[i].getIdTurno() << " | ";

        // Fecha
        cout << (listaTurnos[i].getFecha().getDia() < 10 ? "0" : "") << listaTurnos[i].getFecha().getDia() << "/"
             << (listaTurnos[i].getFecha().getMes() < 10 ? "0" : "") << listaTurnos[i].getFecha().getMes() << "/"
             << listaTurnos[i].getFecha().getAnio() << " | ";

        // Hora
        cout << (horasArribo[i] < 10 ? "0" : "") << horasArribo[i] << ":"
             << (minsArribo[i] < 10 ? "0" : "") << minsArribo[i] << " | ";

        // Clienta
        cout << nombreClienta;
        int largoC = strlen(nombreClienta);
        for (int e = largoC; e < 18; e++) cout << " ";
        cout << " | ";

        // Buscamos e imprimimos los tratamientos asociados
        DetalleTurno dtReg;
        int posDT = 0;
        bool primerServicio = true;

        while (dtReg.leerDisco(posDT++)) {
            if (dtReg.getEstado() == true && dtReg.getIdTurno() == listaTurnos[i].getIdTurno()) {

                // Buscamos datos del Servicio
                Servicio serv;
                int posS = 0;
                char nombreServicio[40] = "Desconocido";
                while (serv.leerDisco(posS++)) {
                    if (serv.getIdServicio() == dtReg.getIdServicio()) {
                        strcpy(nombreServicio, serv.getNombre());
                        if (strlen(nombreServicio) > 34) {
                            nombreServicio[31] = '.'; nombreServicio[32] = '.'; nombreServicio[33] = '.'; nombreServicio[34] = '\0';
                        }
                        break;
                    }
                }

                // Buscamos datos del Profesional
                Profesional prof;
                int posP = 0;
                char apellidoProf[15] = "Sin Asig.";
                while (prof.leerDisco(posP++)) {
                    if (prof.getIdProfesional() == dtReg.getIdProfesional()) {
                        strcpy(apellidoProf, prof.getApellido());
                        if (strlen(apellidoProf) > 11) {
                            apellidoProf[9] = '.'; apellidoProf[10] = '.'; apellidoProf[11] = '\0';
                        }
                        break;
                    }
                }

                // Si es el primer servicio del turno, va en la misma linea horizontal
                if (primerServicio) {
                    cout << nombreServicio;
                    for (int e = strlen(nombreServicio); e < 35; e++) cout << " ";
                    cout << " | " << apellidoProf;
                    for (int e = strlen(apellidoProf); e < 12; e++) cout << " ";

                    // Completamos la fila horizontal con los estados finales
                    cout << " | $" << (int)listaTurnos[i].getSena();
                    cout << (listaTurnos[i].getSena() < 10000 ? "  " : " ");
                    cout << " | " << (listaTurnos[i].getAsistio() ? "SI  " : "NO  ");
                    cout << " | " << (listaTurnos[i].getLiquidado() ? "SI " : "NO ");
                    cout << endl;
                    primerServicio = false;
                }
                // Si es un segundo o tercer servicio del mismo turno, se desglosa abajo en forma de lista
                else {
                    cout << "    |            |       |                    | ";
                    cout << nombreServicio;
                    for (int e = strlen(nombreServicio); e < 35; e++) cout << " ";
                    cout << " | " << apellidoProf;
                    for (int e = strlen(apellidoProf); e < 12; e++) cout << " ";
                    cout << " |       |      |    " << endl;
                }
            }
        }

        // Si un turno se quedo sin desgloses cargados, cerramos la linea de forma balanceada
        if (primerServicio) {
            cout << "Sin tratamientos registrados        | Sin Asig.    | $"
                 << (int)listaTurnos[i].getSena() << "  | NO   | NO " << endl;
        }
    }
    cout << "========================================================================================================================" << endl << endl;

    // Liberacion obligatoria de la memoria dinamica
    delete[] listaTurnos;
    delete[] horasArribo;
    delete[] minsArribo;
}

//------------------------------------
// REGISTRAR ASISTENCIA (DAR PRESENTE)
//------------------------------------
void registrarAsistencia() {
    int idBuscado;
    Turno reg;
    int pos = 0;
    bool encontrado = false;

    cout << "=================================================" << endl;
    cout << "          REGISTRAR ASISTENCIA DE CLIENTA        " << endl;
    cout << "=================================================" << endl;

    // Mostramos todos los turnos activos ordenados cronologicamente
    listarAgendaCronologica();

    cout << "Ingrese el ID del Turno para dar el PRESENTE (0 para cancelar): ";
    cin >> idBuscado;

    if (idBuscado == 0) return;

    FILE* p = fopen("turnos.dat", "rb+");
    if (p == NULL) {
        cout << "\n[ERROR] No se pudo acceder al archivo.\n";
        return;
    }

    while (fread(&reg, sizeof(Turno), 1, p) == 1) {
        if (reg.getIdTurno() == idBuscado && reg.getEstado() == true) {
            encontrado = true;
            reg.setAsistio(true); // Marcamos el presente

            fseek(p, pos * sizeof(Turno), SEEK_SET);
            fwrite(&reg, sizeof(Turno), 1, p);

            cout << "\n[OK] Asistencia registrada. ¡Clienta en el centro de estetica!\n";
            break;
        }
        pos++;
    }
    fclose(p);

    if (!encontrado) cout << "\n[ERROR] No se encontro ningun turno activo con ese ID.\n";
}

//--------------------------------------------------------
// RE-PROGRAMAR HORARIO DE ATENCION (CON CAMBIO EN CASCADA)
//--------------------------------------------------------

void reprogramarTurno() {
    int idBuscado;
    Turno reg;
    int pos = 0;
    bool encontrado = false;

    cout << "=================================================" << endl;
    cout << "          RE-PROGRAMAR HORARIO DE TURNO          " << endl;
    cout << "=================================================" << endl;

    // Desplegamos la lista para saber a quien vamos a re-programar
    listarAgendaCronologica();

    cout << "Ingrese el ID del Turno a modificar (0 para volver): ";
    cin >> idBuscado;

    if (idBuscado == 0) return;

    FILE* p = fopen("turnos.dat", "rb+");
    if (p == NULL) {
        cout << "\n[ERROR] No se pudo acceder al archivo de cabeceras.\n";
        return;
    }

    while (fread(&reg, sizeof(Turno), 1, p) == 1) {
        if (reg.getIdTurno() == idBuscado && reg.getEstado() == true) {
            encontrado = true;
            break;
        }
        pos++;
    }

    if (!encontrado) {
        cout << "\n[ERROR] No se encontro ningun turno activo con ese ID.\n";
        fclose(p);
        return;
    }

    // Liberamos de forma inmediata los horarios viejos en detalles_turnos.dat
    DetalleTurno dt;
    FILE* pDT = fopen("detalles_turnos.dat", "rb+");
    if (pDT != NULL) {
        int posDT = 0;
        while (fread(&dt, sizeof(DetalleTurno), 1, pDT) == 1) {
            if (dt.getIdTurno() == idBuscado && dt.getEstado() == true) {
                dt.setEstado(false);
                fseek(pDT, posDT * sizeof(DetalleTurno), SEEK_SET);
                fwrite(&dt, sizeof(DetalleTurno), 1, pDT);
                fseek(pDT, (posDT + 1) * sizeof(DetalleTurno), SEEK_SET); // Sincroniza puntero de lectura
            }
            posDT++;
        }
        fclose(pDT);
    }

    // Desplegamos la agenda interactiva y capturamos la nueva fecha
    system("cls");
    mostrarGrillaSemanalAuto(); // Grilla semanal actualizada

    int diaElegido, mesElegido, anioElegido, horaElegida, minElegida;
    cout << "CONFECCION DEL NUEVO HORARIO PARA EL TURNO Nro [" << idBuscado << "]:\n";
    cout << "Ingrese el Dia elegido: "; cin >> diaElegido;
    cout << "Ingrese el Mes elegido: "; cin >> mesElegido;
    cout << "Ingrese el Anio elegido: "; cin >> anioElegido;
    cout << "Ingrese la Hora elegida (09 a 18): "; cin >> horaElegida;
    cout << "Ingrese los Minutos elegidos: "; cin >> minElegida;

    // Validamos disponibilidad en el disco
    if (horaElegida < 9 || horaElegida > 18 || verificarBloqueOcupado(diaElegido, mesElegido, anioElegido, horaElegida)) {
        cout << "\n[ERROR] Horario ocupado o fuera de rango comercial. Operacion cancelada.\n";
        cout << "        (Los horarios previos se restauraran de forma segura al salir).\n";

        // Restauramos el estado viejo por seguridad si fallo la validacion
        pDT = fopen("detalles_turnos.dat", "rb+");
        if (pDT != NULL) {
            int posDT = 0;
            while (fread(&dt, sizeof(DetalleTurno), 1, pDT) == 1) {
                if (dt.getIdTurno() == idBuscado && dt.getEstado() == false) {
                    dt.setEstado(true);
                    fseek(pDT, posDT * sizeof(DetalleTurno), SEEK_SET);
                    fwrite(&dt, sizeof(DetalleTurno), 1, pDT);
                    fseek(pDT, (posDT + 1) * sizeof(DetalleTurno), SEEK_SET);
                }
                posDT++;
            }
            fclose(pDT);
        }
        fclose(p);
        return;
    }

    // Si el nuevo horario esta libre, impactamos de verdad ambos archivos
    Fecha fNueva;
    fNueva.setDia(diaElegido); fNueva.setMes(mesElegido); fNueva.setAnio(anioElegido);
    reg.setFecha(fNueva);

    fseek(p, pos * sizeof(Turno), SEEK_SET);
    fwrite(&reg, sizeof(Turno), 1, p);
    fclose(p);

    // Grabamos los nuevos datos horarios en el desglose
    pDT = fopen("detalles_turnos.dat", "rb+");
    if (pDT != NULL) {
        int posDT = 0;
        while (fread(&dt, sizeof(DetalleTurno), 1, pDT) == 1) {
            if (dt.getIdTurno() == idBuscado) {
                dt.setHora(horaElegida);
                dt.setMinuto(minElegida);
                dt.setEstado(true); // Se vuelve a activar en la nueva celda

                fseek(pDT, posDT * sizeof(DetalleTurno), SEEK_SET);
                fwrite(&dt, sizeof(DetalleTurno), 1, pDT);
                fseek(pDT, (posDT + 1) * sizeof(DetalleTurno), SEEK_SET);
            }
            posDT++;
        }
        fclose(pDT);
    }

    cout << "\n[OK] Turno re-programado con exito. Grilla actualizada de inmediato.\n";
}

// BAJA LOGICA TURNO (conectado con detalle turno en cascada)
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
            reg.setEstado(false); // Baja logica de cabecera

            fseek(p, pos * sizeof(Turno), SEEK_SET);
            fwrite(&reg, sizeof(Turno), 1, p);

            // BAJA EN CASCADA EN DETALLES_TURNOS.DAT
            DetalleTurno dtReg;
            FILE* pDT = fopen("detalles_turnos.dat", "rb+");
            if (pDT != NULL) {
                int posDT = 0;
                while (fread(&dtReg, sizeof(DetalleTurno), 1, pDT) == 1) {
                    if (dtReg.getIdTurno() == idBuscado && dtReg.getEstado() == true) {
                        dtReg.setEstado(false); // Desactivamos los desgloses para liberar la grilla
                        fseek(pDT, posDT * sizeof(DetalleTurno), SEEK_SET);
                        fwrite(&dtReg, sizeof(DetalleTurno), 1, pDT);
                        fseek(pDT, (posDT + 1) * sizeof(DetalleTurno), SEEK_SET); // Sincroniza puntero
                    }
                    posDT++;
                }
                fclose(pDT);
            }

            cout << "\n[OK] Turno y desgloses dados de baja. Horarios liberados en la grilla.\n";
            break;
        }
        pos++;
    }
    fclose(p);

    if (!encontrado) cout << "\n[ERROR] No se encontro ningun turno activo con ese ID.\n";

    cin.ignore(1000, '\n');
    cout << "\nPresione ENTER para continuar...";
    cin.get();
    return true;
}

//-----------------------------------------------------
// REGISTRAR COBRO EN CAJA (LIQUIDAR TURNOS ASISTIDOS)
//-----------------------------------------------------
void cobrarLiquidarTurno() {
    int idBuscado;
    Turno reg;
    int pos = 0;
    bool encontrado = false;

    cout << "=================================================" << endl;
    cout << "          LIQUIDACION Y COBRO DE TURNOS          " << endl;
    cout << "=================================================" << endl;

    listarAgendaCronologica();

    cout << "Ingrese el ID del Turno a COBRAR (0 para cancelar): ";
    cin >> idBuscado;

    if (idBuscado == 0) return;

    FILE* p = fopen("turnos.dat", "rb+");
    if (p == NULL) {
        cout << "\n[ERROR] No se pudo acceder al archivo de cabeceras.\n";
        return;
    }

    while (fread(&reg, sizeof(Turno), 1, p) == 1) {
        if (reg.getIdTurno() == idBuscado && reg.getEstado() == true) {
            encontrado = true;

            if (reg.getAsistio() == false) {
                cout << "\n[ERROR] El turno no figura como ASISTIDO. Registre la asistencia primero.\n";
                fclose(p);
                return;
            }

            if (reg.getLiquidado() == true) {
                cout << "\n[AVISO] Este turno ya fue cobrado y liquidado previamente.\n";
                fclose(p);
                return;
            }

            DetalleTurno dt;
            FILE* pDT = fopen("detalles_turnos.dat", "rb");
            float precioTotalServicios = 0;

            if (pDT != NULL) {
                while (fread(&dt, sizeof(DetalleTurno), 1, pDT) == 1) {
                    if (dt.getIdTurno() == idBuscado && dt.getEstado() == true) {
                        precioTotalServicios += dt.getPrecioAlMomento();
                    }
                }
                fclose(pDT);
            }

            float saldoAPagar = precioTotalServicios - reg.getSena();

            system("cls");
            cout << "=================================================" << endl;
            cout << "               TICKET DE COBRO COMPLETO          " << endl;
            cout << "=================================================" << endl;
            cout << " TURNO Nro:        [" << idBuscado << "]"          << endl;
            cout << " TOTAL SERVICIOS:  $" << precioTotalServicios      << endl;
            cout << " SENIA ENTREGADA:  -$" << reg.getSena()            << endl;
            cout << "-------------------------------------------------" << endl;
            cout << " TOTAL A ABONAR:   $" << saldoAPagar << endl;
            cout << "=================================================" << endl;

            char confirma;
            cout << "¿Confirma el pago en efectivo/tarjeta? (S/N): ";
            cin >> confirma;

            if (confirma == 'S' || confirma == 's') {
                reg.setLiquidado(true);
                Fecha hoy; // Usa el constructor
                hoy.setDia(22);
                hoy.setMes(6);
                hoy.setAnio(2026);
                reg.setFechaLiquidacion(hoy);

                fseek(p, pos * sizeof(Turno), SEEK_SET);
                fwrite(&reg, sizeof(Turno), 1, p);
                cout << "\n[OK] ¡Cobro exitoso! El turno paso a estado LIQUIDADO.\n";
            } else {
                cout << "\nOperacion cancelada. No se registro el ingreso de dinero.\n";
            }
            break;
        }
        pos++;
    }
    fclose(p);

    if (!encontrado) cout << "\n[ERROR] No se encontro ningun turno activo con ese ID.\n";
}

// SUB-MENU PRINCIPAL: MODULO DE TURNOS

void menuTurnos() {
    int op;
    Turno aux;

    do {
        cout << "=================================================" << endl;
        cout << "            MODULO: GESTION DE TURNOS            " << endl;
        cout << "=================================================" << endl;
        cout << "1. Registrar Turno (Agendar)"                       << endl;
        cout << "2. Ver Agenda de Turnos Activos"                    << endl;
        cout << "3. Registrar Asistencia (Dar Presente)"            << endl;
        cout << "4. Re-programar Turno (Cambiar Fecha/Hora)"        << endl;
        cout << "5. Cancelar Turno (Baja Logica)"                   << endl;
        cout << "6. Consultar Disponibilidad Semanal (Grilla)"      << endl;
        cout << "7. Registrar Cobro en Caja (Liquidar Turno)"       << endl;
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
                        cout << "\n[OK] Clienta registrada con exito in el sistema.\n";
                        idResuelto = nuevoCliente.getIdCliente();
                    } else {
                        cout << "\n[ERROR] No se pudo guardar la clienta in el disco.\n";
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
                cout << "\nPresione ENTER para ir al panel de tratamientos...";
                cin.ignore(1000, '\n');
                cin.get();

                //SELECCION DE TRATAMIENTO
                bool flujoTurnoActivo = true;
                while (flujoTurnoActivo) {
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
                    while (listador.leerDisco(posListado)) {
                        if (listador.getEstado() == true) {
                            cout << " ID: [" << listador.getIdServicio() << "] - "
                                 << listador.getNombre() << " ($" << listador.getPrecio() << ")\n";
                        }
                        posListado++;
                    }
                    cout << " 0. Cancelar y salir al menu principal\n";
                    cout << "-------------------------------------------------\n";
                    cout << "Ingrese ID del Servicio deseado: ";
                    cin >> idServicioElegido;

                    if (idServicioElegido == 0) {
                        flujoTurnoActivo = false;
                        break;
                    }

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

                    if (!servEncontrado) {
                        cout << "\n[ERROR] ID de servicio invalido. Presione ENTER para reintentar...";
                        cin.ignore(1000, '\n'); cin.get();
                        continue;
                    }

                    // SELECCION DE PROFESIONAL
                    bool profesionalValida = false;
                    int idProfesionalElegido = 0;

                    while (!profesionalValida) {
                        system("cls");
                        cout << "=================================================" << endl;
                        cout << "          SELECCION DE PROFESIONAL HABILITADA    " << endl;
                        cout << "=================================================" << endl;
                        cout << ">> Tratamiento: " << servAux.getNombre() << " ($"  << precioBase << ")\n";
                        cout << "-------------------------------------------------\n";
                        cout << "PROFESIONALES DISPONIBLES:\n";

                        ServicioXProfesional rel;
                        Profesional prof;
                        int posRel = 0;
                        bool hayProfesionales = false;

                        int IDsMostrados[100] = {0};
                        int cantMostrados = 0;

                        while (rel.leerDisco(posRel)) {
                            if (rel.getEstado() == true && rel.getIdServicio() == idServicioElegido) {
                                bool yaMostrado = false;
                                for (int i = 0; i < cantMostrados; i++) {
                                    if (IDsMostrados[i] == rel.getIdProfesional()) {
                                        yaMostrado = true;
                                        break;
                                    }
                                }

                                if (!yaMostrado) {
                                    if (prof.buscarPorId(rel.getIdProfesional()) && prof.getEstado() == true) {
                                        cout << "  ID: [" << prof.getIdProfesional() << "] - "
                                             << prof.getApellido() << ", " << prof.getNombre() << "\n";
                                        hayProfesionales = true;

                                        if (cantMostrados < 100) {
                                            IDsMostrados[cantMostrados] = rel.getIdProfesional();
                                            cantMostrados++;
                                        }
                                    }
                                }
                            }
                            posRel++;
                        }

                        cout << "\n-------------------------------------------------\n";
                        cout << " 0. Volver al panel de tratamientos\n";
                        cout << "-------------------------------------------------\n";
                        cout << "Ingrese ID de la Profesional elegida: ";
                        cin >> idProfesionalElegido;

                        if (idProfesionalElegido == 0) {
                            break;
                        }

                        // Validacion con objeto aux.
                        ServicioXProfesional relAux;
                        int posCheck = 0;
                        while (relAux.leerDisco(posCheck)) {
                            if (relAux.getEstado() == true &&
                                relAux.getIdServicio() == idServicioElegido &&
                                relAux.getIdProfesional() == idProfesionalElegido) {
                                profesionalValida = true;
                                break;
                            }
                            posCheck++;
                        }

                        if (!profesionalValida) {
                            cout << "\n[ERROR] La profesional no atiende este servicio o el ID es incorrecto.\n";
                            cout << "Presione ENTER para reintentar...";
                            cin.ignore(1000, '\n'); cin.get();
                        }
                    }

                    if (idProfesionalElegido == 0) continue;

                    // SELECCION DE FECHA Y HORARIO
                    bool fechaAsignadaCorrectamente = false;
                    while (!fechaAsignadaCorrectamente) {
                        system("cls");
                        cout << "=================================================" << endl;
                        cout << "            DATOS DE PRE-RESERVA VALIDADOS       " << endl;
                        cout << "=================================================" << endl;

                        Cliente cliAux;
                        int posC = 0;
                        while(cliAux.leerDisco(posC++)) {
                            if(cliAux.getIdCliente() == idResuelto) break;
                        }
                        Profesional profAux;
                        profAux.buscarPorId(idProfesionalElegido);

                        cout << "Clienta: " << cliAux.getNombre() << " " << cliAux.getApellido() << " (ID " << idResuelto << ")" << endl;
                        cout << "Tratamiento: " << servAux.getNombre() << " (ID " << idServicioElegido << ")" << endl;
                        cout << "Profesional: " << profAux.getNombre() << " " << profAux.getApellido() << " (ID " << idProfesionalElegido << ")" << endl;
                        cout << "-------------------------------------------------\n" << endl;

                        int opAgenda;
                        int diaElegido = 0, mesElegido = 0, anioElegido = 2026;
                        int horaElegida = 0, minElegido = 0;

                        cout << "ASIGNACION DE FECHA Y HORARIO:" << endl;
                        cout << "1. Consultar Bloque de Disponibilidad Semanal (Grilla)" << endl;
                        cout << "2. Elegir Fecha y Hora Manualmente" << endl;
                        cout << "0. Volver a la seleccion de Profesional" << endl;
                        cout << "-------------------------------------------------" << endl;
                        cout << "Seleccione una opcion: ";
                        cin >> opAgenda;

                        if (opAgenda == 0) {
                            break;
                        }

                        if (opAgenda == 1) {
                            system("cls");
                            mostrarGrillaSemanalAuto();
                            cout << "CONFECCION DEL HORARIO:\n";

                            Fecha fValidar;
                            if (fValidar.cargar() == false) continue;

                            diaElegido = fValidar.getDia();
                            mesElegido = fValidar.getMes();
                            anioElegido = fValidar.getAnio();

                            cout << "Ingrese la Hora (Ej 15): "; cin >> horaElegida;
                            cout << "Ingrese los Minutos (Ej 0 o 30): "; cin >> minElegido;
                        }
                        else if (opAgenda == 2) {
                            system("cls");
                            cout << "=================================================" << endl;
                            cout << "            INGRESO MANUAL DE FECHA Y HORA       " << endl;
                            cout << "=================================================" << endl;

                            Fecha fValidar;
                            if (fValidar.cargar() == false) continue;

                            diaElegido = fValidar.getDia();
                            mesElegido = fValidar.getMes();
                            anioElegido = fValidar.getAnio();

                            cout << "Ingrese la Hora (09 a 18): "; cin >> horaElegida;
                            cout << "Ingrese los Minutos (00 a 59): "; cin >> minElegido;
                        }

                        if (diaElegido == 0) continue;

                        // VALIDACION DE PASADO ABSOLUTO CONTRA LA HORA DE LA MAQUINA
                        time_t tSincro = time(0);
                        tm* infoActual = localtime(&tSincro);

                        int tAnio = infoActual->tm_year + 1900;
                        int tMes = infoActual->tm_mon + 1;
                        int tDia = infoActual->tm_mday;
                        int tHora = infoActual->tm_hour;
                        int tMin = infoActual->tm_min;

                        bool esFechaPasada = false;
                        if (anioElegido < tAnio) esFechaPasada = true;
                        else if (anioElegido == tAnio) {
                            if (mesElegido < tMes) esFechaPasada = true;
                            else if (mesElegido == tMes) {
                                if (diaElegido < tDia) esFechaPasada = true;
                                else if (diaElegido == tDia) {
                                    if (horaElegida < tHora) esFechaPasada = true;
                                    else if (horaElegida == tHora && minElegido < tMin) esFechaPasada = true;
                                }
                            }
                        }

                        if (esFechaPasada) {
                            cout << "\n[ERROR] No se pueden registrar turnos en el pasado (Fecha u Hora ya vencidas).\n";
                            cout << "Presione ENTER para reintentar...";
                            cin.ignore(1000, '\n'); cin.get();
                        }
                        else if (horaElegida < 9 || horaElegida > 18 || minElegido < 0 || minElegido > 59) {
                            cout << "\n[ERROR] Horario fuera del rango comercial. Presione ENTER...";
                            cin.ignore(1000, '\n'); cin.get();
                        }
                        else if (verificarBloqueOcupado(diaElegido, mesElegido, anioElegido, horaElegida)) {
                            cout << "\n[ERROR] El bloque horario ya esta ocupado. Presione ENTER...";
                            cin.ignore(1000, '\n'); cin.get();
                        }
                        else {
                            aux.setIdCliente(idResuelto);
                            Fecha fAux;
                            fAux.setDia(diaElegido); fAux.setMes(mesElegido); fAux.setAnio(anioElegido);
                            aux.setFecha(fAux);

                            float montoSena;
                            do {
                                cout << "Ingrese el monto de la senia: $";
                                cin >> montoSena;
                                if (montoSena < 0) cout << "[ERROR] La senia no puede ser negativa.\n";
                            } while (montoSena < 0);
                            aux.setSena(montoSena);
                            aux.setAsistio(false);
                            aux.setEstado(true);

                            FILE* fTurnosCount = fopen("turnos.dat", "rb");
                            int cantT = 0;
                            if (fTurnosCount != NULL) {
                                fseek(fTurnosCount, 0, SEEK_END);
                                cantT = ftell(fTurnosCount) / sizeof(Turno);
                                fclose(fTurnosCount);
                            }
                            int idTurnoAsignado = cantT + 1;
                            aux.setIdTurno(idTurnoAsignado);

                            if (aux.escribirDisco()) {
                                DetalleTurno detAux;
                                detAux.setIdTurno(idTurnoAsignado);
                                detAux.setIdServicio(idServicioElegido);
                                detAux.setIdProfesional(idProfesionalElegido);
                                detAux.setHora(horaElegida);
                                detAux.setMinuto(minElegido);
                                detAux.setPrecioAlMomento(precioBase);
                                detAux.setEstado(true);
                                detAux.setObservaciones("Alta inicial desde el panel central");

                                if (detAux.escribirDisco()) {
                                    system("cls");
                                    cout << "=================================================" << endl;
                                    cout << "            TURNO REGISTRADO CON EXITO!          " << endl;
                                    cout << "=================================================" << endl;
                                    cout << "[OK] Cabecera agendada en Turno Nro: " << idTurnoAsignado << endl;
                                    cout << "[OK] Desglose de servicios guardado correctamente."<< endl;
                                    cout << "-------------------------------------------------" << endl;
                                    fechaAsignadaCorrectamente = true;
                                    flujoTurnoActivo = false;
                                }
                            }
                        }
                    }
                }
            }
            cout << "\nPresione ENTER para continuar...";
            cin.ignore(1000, '\n'); cin.get();
            system("cls");
        }
        else if (op == 2) {
            system("cls");

            cout << "========================================================================================================================" << endl;
            cout << "                                         AGENDA DE TURNOS ACTIVOS (CRONOLOGICA)                                         " << endl;
            cout << "========================================================================================================================" << endl;
            listarAgendaCronologica();
            cin.ignore(1000, '\n'); cout << "\nPresione ENTER para volver..."; cin.get();
            system("cls");
        }
        else if (op == 3) {
            system("cls");
            registrarAsistencia();
            cin.ignore(1000, '\n'); cout << "\nPresione ENTER para continuar..."; cin.get();
            system("cls");
        }
        else if (op == 4) {
            system("cls");
            reprogramarTurno();
            cin.ignore(1000, '\n'); cout << "\nPresione ENTER para continuar..."; cin.get();
            system("cls");
        }
        else if (op == 5) {
            system("cls");
            darDeBajaTurno();
            system("cls");
        }
        else if (op == 6) {
            system("cls");
            mostrarGrillaSemanalAuto();
            cout << "Presione ENTER para volver al menu...";
            cin.ignore(1000, '\n'); cin.get();
            system("cls");
        }
        else if (op == 7) {
            system("cls");
            cobrarLiquidarTurno();
            cin.ignore(1000, '\n'); cout << "\nPresione ENTER para continuar..."; cin.get();
            system("cls");
        }
    } while (op != 0);

    system("cls");
}
