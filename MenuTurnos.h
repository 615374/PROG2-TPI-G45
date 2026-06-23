#pragma once

// MENU PRINCIPAL DEL MODULO TURNOS
void menuTurnos();

// BUSQUEDAS Y VALIDACIONES
int buscarClienteParaTurno(const char* apellidoBuscado);
bool verificarBloqueOcupado(int d, int m, int a, int horaEvaluar, int idProfesionalEvaluar);

// DISPONIBILIDAD Y AGENDA
void mostrarGrillaSemanalAuto(int idProfesionalGrilla);
void listarAgendaCronologica();

// MANTENIMIENTO DE TURNOS
bool darDeBajaTurno();
void registrarAsistencia();
void reprogramarTurno();

// CAJA Y LIQUIDACIONES
void cobrarLiquidarTurno();
void submenuCajaLiquidaciones();
void reporteCajaGeneral();
