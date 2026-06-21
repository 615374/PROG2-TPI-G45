#ifndef MENUTURNOS_H_INCLUDED
#define MENUTURNOS_H_INCLUDED

void menuTurnos();

// Mantenimiento de turnos
bool darDeBajaTurno();
void marcarAsistenciaTurno();

// Consultas de agenda
void listarAgendaPorFecha();
void listarAgendaPorProfesional();

// Reportes de turnos
void reporteRecaudacionCaja();

#endif // MENUTURNOS_H_INCLUDED
