#include <iostream>
#include <cstdio>
#include "GestorTurnos.h"
#include "DetalleTurno.h"
#include "Turno.h"
#include "Servicio.h"
#include "Profesional.h"
#include "ServicioXProfesional.h"

using namespace std;

// IMPLEMENTACION DE FUNCIONES AUXILIARES DE GESTION DE TURNOS Y AGENDA


// Verifica si un profesional realiza un servicio determinado
bool profesionalRealizaServicio(int idProfesional, int idServicio) {
    ServicioXProfesional rel;
    int pos = 0;

    while (rel.leerDisco(pos)) {
        if (rel.getEstado() &&
            rel.getIdProfesional() == idProfesional &&
            rel.getIdServicio() == idServicio) {
            return true;
        }
        pos++;
    }

    return false;
}

// Lista los profesionales habilitados para realizar un servicio
void listarProfesionalesParaServicio(int idServicio) {
    ServicioXProfesional rel;
    Profesional prof;
    int pos = 0;
    bool encontro = false;

    cout << "PROFESIONALES DISPONIBLES PARA ESTE SERVICIO:" << endl;
    cout << "-------------------------------------------------" << endl;

    while (rel.leerDisco(pos)) {
        if (rel.getEstado() && rel.getIdServicio() == idServicio) {
            if (!prof.mostrarNombrePorId(rel.getIdProfesional())) {
            cout << "ID: [" << rel.getIdProfesional() << "] - Profesional no disponible";
        }
        cout << endl;
            encontro = true;
        }
        pos++;
    }

    if (!encontro) {
        cout << "[AVISO] No hay profesionales asignados a este servicio.\n";
    }

    cout << "-------------------------------------------------" << endl;
}

// FUNCIONES AUXILIARES PARA VALIDACIONES DE AGENDA
// Busca un turno activo por ID y devuelve sus datos completos
bool obtenerTurnoPorId(int idTurnoBuscado, Turno &turnoEncontrado) {
    int pos = 0;

    while (turnoEncontrado.leerDisco(pos)) {
        if (turnoEncontrado.getEstado() &&
            turnoEncontrado.getIdTurno() == idTurnoBuscado) {
            return true;
        }
        pos++;
    }

    return false;
}

// Compara si dos fechas tienen el mismo dia, mes y anio
bool fechasIguales(Fecha f1, Fecha f2) {
    return f1.getDia() == f2.getDia() &&
           f1.getMes() == f2.getMes() &&
           f1.getAnio() == f2.getAnio();
}

// Verifica si un profesional ya esta ocupado en la fecha y horario del turno actual
bool profesionalOcupadoEnFechaYHorario(int idProfesional, int hora, int minuto, int idTurnoActual) {
    DetalleTurno det;
    Turno turnoActual;
    Turno turnoDelDetalle;
    int pos = 0;

    if (!obtenerTurnoPorId(idTurnoActual, turnoActual)) {
        return false;
    }

    Fecha fechaActual = turnoActual.getFecha();

    while (det.leerDisco(pos)) {
        if (det.getEstado() &&
            det.getIdProfesional() == idProfesional &&
            det.getHora() == hora &&
            det.getMinuto() == minuto) {

            if (obtenerTurnoPorId(det.getIdTurno(), turnoDelDetalle)) {
                if (fechasIguales(fechaActual, turnoDelDetalle.getFecha())) {
                    return true;
                }
            }
        }
        pos++;
    }

    return false;
}

// Verifica si el mismo turno ya tiene un detalle en ese horario
bool turnoYaTieneDetalleEnHorario(int idTurnoActual, int hora, int minuto) {
    DetalleTurno det;
    int pos = 0;

    while (det.leerDisco(pos)) {
        if (det.getEstado() &&
            det.getIdTurno() == idTurnoActual &&
            det.getHora() == hora &&
            det.getMinuto() == minuto) {
            return true;
        }
        pos++;
    }

    return false;
}

// Muestra horarios disponibles para un profesional en la fecha del turno
void listarHorariosDisponibles(int idProfesional, int idTurnoActual) {
    cout << "HORARIOS DISPONIBLES PARA LA PROFESIONAL:" << endl;
    cout << "-------------------------------------------------" << endl;

    bool hayDisponibles = false;

    for (int h = 8; h <= 20; h++) {
        for (int m = 0; m <= 30; m += 30) {
            if (!profesionalOcupadoEnFechaYHorario(idProfesional, h, m, idTurnoActual)) {
                if (h < 10) cout << "0";
                cout << h << ":";
                if (m < 10) cout << "0";
                cout << m << "  ";
                hayDisponibles = true;
            }
        }
        cout << endl;
    }

    if (!hayDisponibles) {
        cout << "[AVISO] No hay horarios disponibles para esa profesional en esta fecha.\n";
    }

    cout << "-------------------------------------------------" << endl;
}

// Muestra todos los detalles asociados a un turno
void mostrarDetallesPorTurno(int idTurno) {
    DetalleTurno detalle;
    int pos = 0;
    bool encontro = false;

    cout << "SERVICIOS DEL TURNO:" << endl;

    while (detalle.leerDisco(pos)) {
        if (detalle.getEstado() && detalle.getIdTurno() == idTurno) {
            detalle.mostrar();
            encontro = true;
        }
        pos++;
    }
    if (!encontro) {
        cout << "  No hay servicios cargados para este turno." << endl;
    }
}


// Calcula el total de un turno sumando los precios de sus detalles
float calcularTotalTurno(int idTurno) {
    DetalleTurno detalle;
    int pos = 0;
    float total = 0;

    while (detalle.leerDisco(pos)) {
        if (detalle.getEstado() && detalle.getIdTurno() == idTurno) {
            total += detalle.getPrecioAlMomento();
        }
        pos++;
    }
    return total;
}
