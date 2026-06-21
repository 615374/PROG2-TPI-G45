#pragma once

#include "Fecha.h"
#include "Turno.h"

// FUNCIONES AUXILIARES DE GESTION DE TURNOS Y AGENDA

// Muestra todos los detalles asociados a un turno
void mostrarDetallesPorTurno(int idTurno);

// Calcula el total de un turno sumando los precios de sus detalles
float calcularTotalTurno(int idTurno);

// Verifica si un profesional realiza un servicio determinado
bool profesionalRealizaServicio(int idProfesional, int idServicio);

// Lista los profesionales habilitados para realizar un servicio
void listarProfesionalesParaServicio(int idServicio);

// Busca un turno activo por ID y devuelve sus datos completos
bool obtenerTurnoPorId(int idTurnoBuscado, Turno &turnoEncontrado);

// Compara si dos fechas tienen el mismo dia, mes y anio
bool fechasIguales(Fecha f1, Fecha f2);

// Verifica si un profesional ya esta ocupado en la fecha y horario del turno actual
bool profesionalOcupadoEnFechaYHorario(int idProfesional, int hora, int minuto, int idTurnoActual);

// Verifica si el mismo turno ya tiene un detalle en ese horario
bool turnoYaTieneDetalleEnHorario(int idTurnoActual, int hora, int minuto);

// Muestra horarios disponibles para un profesional en la fecha del turno
void listarHorariosDisponibles(int idProfesional, int idTurnoActual);
