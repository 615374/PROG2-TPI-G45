#include <iostream>
#include <cstdlib>
#include <cstring>
#include "MenuTurnos.h"
#include "Turno.h"
#include "DetalleTurno.h"
#include "GestorTurnos.h"
#include "Servicio.h"
#include "Cliente.h"
#include "Profesional.h"
#include "ServicioXProfesional.h"

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

// Muestra los turnos activos de una fecha especifica
void listarAgendaPorFecha() {
    int diaBuscado;
    int mesBuscado;
    int anioBuscado;

    cout << "Ingrese dia: ";
    cin >> diaBuscado;

    cout << "Ingrese mes: ";
    cin >> mesBuscado;

    cout << "Ingrese anio: ";
    cin >> anioBuscado;

    Turno turno;
    int pos = 0;
    bool hayTurnos = false;

    while (turno.leerDisco(pos)) {
        if (turno.getEstado()) {
            Fecha fechaTurno = turno.getFecha();

            if (fechaTurno.getDia() == diaBuscado &&
                fechaTurno.getMes() == mesBuscado &&
                fechaTurno.getAnio() == anioBuscado) {

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

                hayTurnos = true;
            }
        }
        pos++;
    }
    if (!hayTurnos) {
        cout << "No hay turnos activos para esa fecha.\n\n";
    }
}


// Muestra la agenda de una profesional para una fecha especifica
void listarAgendaPorProfesional() {
    int idProfesionalBuscado;
    int diaBuscado;
    int mesBuscado;
    int anioBuscado;

    Profesional profesional;
    DetalleTurno detalle;
    Turno turno;

    cout << "Ingrese ID de la profesional: ";
    cin >> idProfesionalBuscado;

    if (!profesional.buscarPorId(idProfesionalBuscado)) {
        cout << "[ERROR] No existe una profesional activa con ese ID.\n\n";
        return;
    }

    cout << "Ingrese dia: ";
    cin >> diaBuscado;

    cout << "Ingrese mes: ";
    cin >> mesBuscado;

    cout << "Ingrese anio: ";
    cin >> anioBuscado;

    int posDetalle = 0;
    bool encontroTurnos = false;

    while (detalle.leerDisco(posDetalle)) {
        if (detalle.getEstado() &&
            detalle.getIdProfesional() == idProfesionalBuscado) {

            int posTurno = 0;

            while (turno.leerDisco(posTurno)) {
                if (turno.getEstado() &&
                    turno.getIdTurno() == detalle.getIdTurno()) {

                    Fecha fechaTurno = turno.getFecha();

                    if (fechaTurno.getDia() == diaBuscado &&
                        fechaTurno.getMes() == mesBuscado &&
                        fechaTurno.getAnio() == anioBuscado) {

                        cout << "-----------------------------------" << endl;
                        cout << "ID TURNO: " << turno.getIdTurno() << endl;
                        cout << "FECHA: ";
                        fechaTurno.mostrar();
                        detalle.mostrar();

                        encontroTurnos = true;
                    }
                    break;
                }
                posTurno++;
            }
        }
        posDetalle++;
    }
    if (!encontroTurnos) {
        cout << "No hay turnos para esa profesional en la fecha indicada.\n\n";
    }
}

// Muestra un resumen de recaudacion, senias y saldos pendientes
void reporteRecaudacionCaja() {
    Turno turno;
    int pos = 0;

    int cantidadTurnosActivos = 0;
    int cantidadTurnosAsistidos = 0;
    int cantidadTurnosPendientes = 0;

    float totalSeniasReservadas = 0;
    float totalServiciosRealizados = 0;
    float totalSaldosPendientes = 0;

    while (turno.leerDisco(pos)) {
        if (turno.getEstado() == true) {
            cantidadTurnosActivos++;
            totalSeniasReservadas += turno.getSena();

            float totalTurno = calcularTotalTurno(turno.getIdTurno());

            if (turno.getAsistio() == true) {
                cantidadTurnosAsistidos++;
                totalServiciosRealizados += totalTurno;

                float saldoTurno = totalTurno - turno.getSena();

                if (saldoTurno < 0) {
                    saldoTurno = 0;
                }

                totalSaldosPendientes += saldoTurno;
            }
            else {
                cantidadTurnosPendientes++;
            }
        }

        pos++;
    }

    if (cantidadTurnosActivos == 0) {
        cout << "No hay turnos activos para calcular caja.\n\n";
        return;
    }

    cout << "TURNOS ACTIVOS: " << cantidadTurnosActivos << endl;
    cout << "TURNOS ASISTIDOS: " << cantidadTurnosAsistidos << endl;
    cout << "TURNOS PENDIENTES DE ASISTENCIA: " << cantidadTurnosPendientes << endl;
    cout << "----------------------------------------" << endl;
    cout << "TOTAL SENIAS RESERVADAS: $" << totalSeniasReservadas << endl;
    cout << "TOTAL SERVICIOS REALIZADOS: $" << totalServiciosRealizados << endl;
    cout << "TOTAL SALDOS PENDIENTES: $" << totalSaldosPendientes << endl;
    cout << "----------------------------------------" << endl;
    cout << "TOTAL ESTIMADO A COBRAR: $" << totalServiciosRealizados + totalSaldosPendientes << endl;
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

// Marca un turno activo como asistido
void marcarAsistenciaTurno() {
    int idBuscado;
    Turno reg;
    int pos = 0;
    bool encontrado = false;

    cout << "Ingrese ID del turno a marcar como asistido (0 para cancelar): ";
    cin >> idBuscado;

    if (idBuscado == 0) {
        cout << "\nOperacion cancelada.\n";
        return;
    }

    while (reg.leerDisco(pos)) {
        if (reg.getIdTurno() == idBuscado && reg.getEstado() == true) {
            encontrado = true;

            cout << "\nTurno encontrado:\n";
            reg.mostrar();

            reg.setAsistio(true);

            FILE* p = fopen("turnos.dat", "rb+");
            if (p == NULL) {
                cout << "\n[ERROR] No se pudo abrir el archivo de turnos.\n";
                return;
            }

            fseek(p, sizeof(Turno) * pos, SEEK_SET);
            fwrite(&reg, sizeof(Turno), 1, p);
            fclose(p);

            cout << "\n[OK] Turno marcado como asistido.\n";
            return;
        }
        pos++;
    }
    if (!encontrado) {
        cout << "\n[ERROR] No se encontro un turno activo con ese ID.\n";
    }
}

// Estructura auxiliar para guardar una opcion de disponibilidad
struct OpcionDisponibilidad {
    Fecha fecha;
    int hora;
    int minuto;
    int idProfesional;
};


// Lista servicios activos y devuelve el ID seleccionado
int seleccionarServicio() {
    Servicio servicio;
    int pos = 0;
    int idServicio;

    cout << "=================================================" << endl;
    cout << "             SERVICIOS DISPONIBLES               " << endl;
    cout << "=================================================" << endl;

    while (servicio.leerDisco(pos)) {
        if (servicio.getEstado()) {
            cout << "[" << servicio.getIdServicio() << "] "
                 << servicio.getNombre()
                 << " - Tipo: " << servicio.getTipo()
                 << " - Precio: $" << servicio.getPrecio()
                 << endl;
        }
        pos++;
    }
    cout << "-------------------------------------------------" << endl;
    cout << "Seleccione ID del servicio (0 para cancelar): ";
    cin >> idServicio;

    if (idServicio == 0) {
        return -1;
    }
    if (!servicio.buscarPorId(idServicio)) {
        cout << "[ERROR] No existe un servicio activo con ese ID.\n";
        return -1;
    }
    return idServicio;
}

// Permite elegir una profesional especifica o cualquier profesional disponible
int seleccionarProfesionalParaServicio(int idServicio, bool &cualquieraDisponible) {
    int opcion;
    int idProfesional;

    cout << "=================================================" << endl;
    cout << "       PROFESIONALES QUE REALIZAN EL SERVICIO    " << endl;
    cout << "=================================================" << endl;

    listarProfesionalesParaServicio(idServicio);

    cout << "1. Elegir una profesional especifica" << endl;
    cout << "2. Cualquiera disponible" << endl;
    cout << "0. Cancelar" << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "Seleccione una opcion: ";
    cin >> opcion;

    if (opcion == 0) {
        return -1;
    }

    if (opcion == 2) {
        cualquieraDisponible = true;
        return -1;
    }

    cualquieraDisponible = false;

    cout << "Ingrese ID de la profesional elegida: ";
    cin >> idProfesional;

    if (!profesionalRealizaServicio(idProfesional, idServicio)) {
        cout << "[ERROR] Esa profesional no realiza el servicio seleccionado.\n";
        return -1;
    }
    return idProfesional;
}

// Verifica si una profesional esta ocupada en una fecha y horario especificos
bool profesionalOcupadoEnFecha(int idProfesional, Fecha fechaBuscada, int hora, int minuto) {
    DetalleTurno detalle;
    Turno turno;
    int posDetalle = 0;

    while (detalle.leerDisco(posDetalle)) {
        if (detalle.getEstado() &&
            detalle.getIdProfesional() == idProfesional &&
            detalle.getHora() == hora &&
            detalle.getMinuto() == minuto) {

            int posTurno = 0;

            while (turno.leerDisco(posTurno)) {
                if (turno.getEstado() &&
                    turno.getIdTurno() == detalle.getIdTurno()) {

                    Fecha fechaTurno = turno.getFecha();

                    if (fechaTurno.getDia() == fechaBuscada.getDia() &&
                        fechaTurno.getMes() == fechaBuscada.getMes() &&
                        fechaTurno.getAnio() == fechaBuscada.getAnio()) {
                        return true;
                    }

                    break;
                }

                posTurno++;
            }
        }

        posDetalle++;
    }

    return false;
}

       // Muestra opciones disponibles para el servicio y profesional seleccionados
// Busca y agrega una opcion disponible dentro de una franja horaria
bool agregarUnaOpcionDeFranja(int idServicio,
                              int idProfesionalElegido,
                              bool cualquieraDisponible,
                              Fecha fechaActual,
                              int horaInicio,
                              int horaFin,
                              OpcionDisponibilidad opciones[],
                              int maxOpciones,
                              int &cantidadOpciones) {
    ServicioXProfesional rel;
    int posRel = 0;

    while (rel.leerDisco(posRel) && cantidadOpciones < maxOpciones) {
        if (rel.getEstado() && rel.getIdServicio() == idServicio) {
            int idProfesionalActual = rel.getIdProfesional();

            if (!cualquieraDisponible && idProfesionalActual != idProfesionalElegido) {
                posRel++;
                continue;
            }

            for (int h = horaInicio; h <= horaFin && cantidadOpciones < maxOpciones; h++) {
                for (int m = 0; m <= 30 && cantidadOpciones < maxOpciones; m += 30) {
                    if (!profesionalOcupadoEnFecha(idProfesionalActual, fechaActual, h, m)) {
                        opciones[cantidadOpciones].fecha = fechaActual;
                        opciones[cantidadOpciones].hora = h;
                        opciones[cantidadOpciones].minuto = m;
                        opciones[cantidadOpciones].idProfesional = idProfesionalActual;

                        cout << "[" << cantidadOpciones + 1 << "] ";

                        if (fechaActual.getDia() < 10) cout << "0";
                        cout << fechaActual.getDia() << "/";
                        if (fechaActual.getMes() < 10) cout << "0";
                        cout << fechaActual.getMes() << "/" << fechaActual.getAnio() << " - ";

                        if (h < 10) cout << "0";
                        cout << h << ":";
                        if (m < 10) cout << "0";
                        cout << m << " hs - ";

                        Profesional prof;
                        prof.mostrarNombrePorId(idProfesionalActual);
                        cout << endl;

                        cantidadOpciones++;
                        return true;
                    }
                }
            }
        }
        posRel++;
    }
    return false;
}

// Muestra opciones disponibles para el servicio y profesional seleccionados
int listarDisponibilidad(int idServicio, int idProfesionalElegido, bool cualquieraDisponible, OpcionDisponibilidad opciones[], int maxOpciones) {
    Fecha fechaInicio;
    int cantidadOpciones = 0;

    cout << "Ingrese fecha desde la cual buscar disponibilidad:" << endl;
    if (!fechaInicio.cargar()) {
        return 0;
    }

    cout << "=================================================" << endl;
    cout << "          DISPONIBILIDAD EN LOS PROXIMOS DIAS    " << endl;
    cout << "=================================================" << endl;

    for (int d = 0; d < 7 && cantidadOpciones < maxOpciones; d++) {
        Fecha fechaActual = fechaInicio;
        fechaActual.sumarDias(d);

        if (!fechaActual.validar()) {
            continue;
        }
        bool agregoEnDia = false;

        // Una opcion de manana
        if (agregarUnaOpcionDeFranja(idServicio, idProfesionalElegido, cualquieraDisponible,
                                     fechaActual, 8, 11, opciones, maxOpciones, cantidadOpciones)) {
            agregoEnDia = true;
        }

        // Una opcion de tarde
        if (agregarUnaOpcionDeFranja(idServicio, idProfesionalElegido, cualquieraDisponible,
                                     fechaActual, 12, 16, opciones, maxOpciones, cantidadOpciones)) {
            agregoEnDia = true;
        }

        // Una opcion de ultima franja
        if (agregarUnaOpcionDeFranja(idServicio, idProfesionalElegido, cualquieraDisponible,
                                     fechaActual, 17, 20, opciones, maxOpciones, cantidadOpciones)) {
            agregoEnDia = true;
        }

        if (agregoEnDia) {
            cout << "-------------------------------------------------" << endl;
        }
    }
    return cantidadOpciones;
}


// Registra un turno completo eligiendo servicio, profesional y disponibilidad
void registrarTurno() {
    int subOp;
    int idCliente = -1;

    cout << "=================================================" << endl;
    cout << "              REGISTRAR NUEVO TURNO              " << endl;
    cout << "=================================================" << endl;
    cout << "1. Para Clienta NUEVA (Registrar Primero)" << endl;
    cout << "2. Para Clienta HABITUAL (Buscar por Apellido)" << endl;
    cout << "0. Cancelar / Volver" << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "Seleccione una opcion: ";
    cin >> subOp;

    if (subOp == 0) {
        return;
    }

    if (subOp == 1) {
        Cliente nuevoCliente;
        system("cls");

        if (nuevoCliente.cargar() && nuevoCliente.escribirDisco()) {
            idCliente = nuevoCliente.getIdCliente();
            cout << "\n[OK] Clienta registrada correctamente.\n";
        }
    }
    else if (subOp == 2) {
        char apellidoBuscado[50];
        cin.ignore(1000, '\n');

        cout << "Ingrese Apellido de la clienta: ";
        cin.getline(apellidoBuscado, 50);

        idCliente = buscarClienteParaTurno(apellidoBuscado);
    }

    if (idCliente == -1) {
        cout << "[AVISO] No se pudo identificar la clienta.\n";
        return;
    }

    system("cls");

    int idServicio = seleccionarServicio();
    if (idServicio == -1) {
        return;
    }

    bool cualquieraDisponible = false;
    int idProfesional = seleccionarProfesionalParaServicio(idServicio, cualquieraDisponible);

    if (!cualquieraDisponible && idProfesional == -1) {
        return;
    }

    cout << "\nSe mostraran las primeras 10 opciones disponibles.\n\n";
    OpcionDisponibilidad opciones[10];
    int cantidadOpciones = listarDisponibilidad(idServicio, idProfesional, cualquieraDisponible, opciones, 10);

    if (cantidadOpciones == 0) {
        cout << "No hay disponibilidad para los criterios seleccionados.\n";
        return;
    }

    int opcionElegida;
    cout << "Seleccione una opcion disponible: ";
    cin >> opcionElegida;

    if (opcionElegida < 1 || opcionElegida > cantidadOpciones) {
        cout << "[ERROR] Opcion invalida.\n";
        return;
    }

    OpcionDisponibilidad seleccion = opciones[opcionElegida - 1];

    Servicio servicio;
    servicio.buscarPorId(idServicio);

    float sena;
    float precioServicio = servicio.getPrecio();

    do {
        cout << "Precio del servicio: $" << precioServicio << endl;
        cout << "Ingrese monto de senia: $";
        cin >> sena;

        if (sena < 0) {
            cout << "[ERROR] La senia no puede ser negativa.\n";
        }
        else if (sena > precioServicio) {
            cout << "[ERROR] La senia no puede superar el precio del servicio.\n";
        }

    } while (sena < 0 || sena > precioServicio);

    cin.ignore(1000, '\n');

    char observaciones[200];
    cout << "Ingrese observaciones/notas (opcional): ";
    cin.getline(observaciones, 200);


    Turno turno;
    if (turno.cargar(idCliente, seleccion.fecha, sena) && turno.escribirDisco()) {
        DetalleTurno detalle;

        if (detalle.cargar(turno.getIdTurno(),
                           idServicio,
                           seleccion.idProfesional,
                           seleccion.hora,
                           seleccion.minuto,
                           servicio.getPrecio(),
                           observaciones) &&
            detalle.escribirDisco()) {

            cout << "\n[OK] Turno registrado correctamente.\n";
        }
        else {
            cout << "\n[ERROR] No se pudo guardar el detalle del turno.\n";
        }
    }
    else {
        cout << "\n[ERROR] No se pudo guardar el turno.\n";
    }
}

// Submenu para administrar turnos ya registrados
void submenuGestionTurnos() {
    int op;

    do {
        cout << "=================================================" << endl;
        cout << "             GESTION DE TURNOS EXISTENTES        " << endl;
        cout << "=================================================" << endl;
        cout << "1. Ver Agenda de Turnos Activos" << endl;
        cout << "2. Marcar Asistencia de Turno" << endl;
        cout << "3. Cancelar Turno (Baja Logica)" << endl;
        cout << "0. Volver al Menu de Turnos" << endl;
        cout << "-------------------------------------------------" << endl;
        cout << "Seleccione una opcion: ";
        cin >> op;

        if (op == 1) {
            system("cls");
            cout << "=================================================" << endl;
            cout << "           AGENDA DE TURNOS ACTIVOS              " << endl;
            cout << "=================================================" << endl;

            Turno aux;
            int pos = 0;
            bool encontrado = false;

            while (aux.leerDisco(pos)) {
                if (aux.getEstado() == true) {
                    aux.mostrar();
                    mostrarDetallesPorTurno(aux.getIdTurno());

                    float totalTurno = calcularTotalTurno(aux.getIdTurno());
                    float saldoPendiente = totalTurno - aux.getSena();

                    if (saldoPendiente < 0) {
                        saldoPendiente = 0;
                    }

                    cout << "TOTAL DEL TURNO: $" << totalTurno << endl;
                    cout << "SALDO PENDIENTE: $" << saldoPendiente << endl;
                    cout << "----------------------------------------" << endl;
                    encontrado = true;
                }
                pos++;
            }

            if (!encontrado) {
                cout << "No hay turnos activos agendados para mostrar.\n\n";
            }

            cout << "\nPresione ENTER para continuar...";
            cin.ignore(1000, '\n');
            cin.get();
            system("cls");
        }
        else if (op == 2) {
            system("cls");
            cout << "=================================================" << endl;
            cout << "              MARCAR ASISTENCIA DE TURNO         " << endl;
            cout << "=================================================" << endl;

            marcarAsistenciaTurno();

            cout << "\nPresione ENTER para continuar...";
            cin.ignore(1000, '\n');
            cin.get();
            system("cls");
        }
        else if (op == 3) {
            system("cls");
            darDeBajaTurno();

            cout << "\nPresione ENTER para continuar...";
            cin.ignore(1000, '\n');
            cin.get();
            system("cls");
        }
        else if (op != 0) {
            cout << "\n[ERROR] Opcion invalida.\n";
            cout << "Presione ENTER para continuar...";
            cin.ignore(1000, '\n');
            cin.get();
            system("cls");
            }
        } while (op != 0);
    system("cls");
}

// Submenu para consultar la agenda con distintos filtros
void submenuConsultasAgenda() {
    int op;

    do {
        cout << "=================================================" << endl;
        cout << "               CONSULTAS DE AGENDA               " << endl;
        cout << "=================================================" << endl;
        cout << "1. Consultar Agenda por Fecha" << endl;
        cout << "2. Consultar Agenda por Profesional" << endl;
        cout << "0. Volver al Menu de Turnos" << endl;
        cout << "-------------------------------------------------" << endl;
        cout << "Seleccione una opcion: ";
        cin >> op;

        if (op == 1) {
            system("cls");
            cout << "=================================================" << endl;
            cout << "              CONSULTA DE AGENDA POR FECHA       " << endl;
            cout << "=================================================" << endl;

            listarAgendaPorFecha();

            cout << "\nPresione ENTER para continuar...";
            cin.ignore(1000, '\n');
            cin.get();
            system("cls");
        }
        else if (op == 2) {
            system("cls");
            cout << "=================================================" << endl;
            cout << "           CONSULTA DE AGENDA POR PROFESIONAL    " << endl;
            cout << "=================================================" << endl;

            listarAgendaPorProfesional();

            cout << "\nPresione ENTER para continuar...";
            cin.ignore(1000, '\n');
            cin.get();
            system("cls");
        }
        else if (op != 0) {
            cout << "\n[ERROR] Opcion invalida.\n";
            cout << "Presione ENTER para continuar...";
            cin.ignore(1000, '\n');
            cin.get();
            system("cls");
        }

    } while (op != 0);
    system("cls");
}


// Submenu para reportes economicos del modulo turnos
void submenuReportesTurnos() {
    int op;

    do {
        cout << "=================================================" << endl;
        cout << "              REPORTES DE TURNOS Y CAJA          " << endl;
        cout << "=================================================" << endl;
        cout << "1. Reporte de Recaudacion y Caja" << endl;
        cout << "0. Volver al Menu de Turnos" << endl;
        cout << "-------------------------------------------------" << endl;
        cout << "Seleccione una opcion: ";
        cin >> op;

        if (op == 1) {
            system("cls");
            cout << "=================================================" << endl;
            cout << "             REPORTE DE RECAUDACION Y CAJA       " << endl;
            cout << "=================================================" << endl;

            reporteRecaudacionCaja();

            cout << "\nPresione ENTER para continuar...";
            cin.ignore(1000, '\n');
            cin.get();
            system("cls");
        }
        else if (op != 0) {
            cout << "\n[ERROR] Opcion invalida.\n";
            cout << "Presione ENTER para continuar...";
            cin.ignore(1000, '\n');
            cin.get();
            system("cls");
        }

    } while (op != 0);
    system("cls");
}



// SUB-MENU PRINCIPAL: MODULO DE TURNOS
// Menu principal del modulo de turnos
void menuTurnos() {
    int op;

    do {
        cout << "=================================================" << endl;
        cout << "              MODULO: GESTION DE TURNOS          " << endl;
        cout << "=================================================" << endl;
        cout << "1. Registrar Nuevo Turno" << endl;
        cout << "2. Gestionar Turnos Existentes" << endl;
        cout << "3. Consultas de Agenda" << endl;
        cout << "4. Reportes de Turnos y Caja" << endl;
        cout << "0. Volver al Menu Principal" << endl;
        cout << "-------------------------------------------------" << endl;
        cout << "Seleccione una opcion: ";
        cin >> op;

        if (op == 1) {
            system("cls");
            registrarTurno();

            cout << "\nPresione ENTER para continuar...";
            cin.ignore(1000, '\n');
            cin.get();
            system("cls");
        }
        else if (op == 2) {
            system("cls");
            submenuGestionTurnos();
        }
        else if (op == 3) {
            system("cls");
            submenuConsultasAgenda();
        }
        else if (op == 4) {
            system("cls");
            submenuReportesTurnos();
        }
        else if (op != 0) {
            cout << "\n[ERROR] Opcion invalida.\n";
            cout << "Presione ENTER para continuar...";
            cin.ignore(1000, '\n');
            cin.get();
            system("cls");
        }

    } while (op != 0);
    system("cls");
}
