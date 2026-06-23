#pragma once

// MENU PRINCIPAL DEL MODULO TURNOS
void menuTurnos();

// BUSQUEDAS Y VALIDACIONES
int buscarClienteParaTurno(const char* apellidoBuscado);
bool verificarBloqueOcupado(int d, int m, int a, int horaEvaluar, int idProfesionalEvaluar);
int buscarTurnoClienteEnFecha(int idCliente, int d, int m, int a);
bool verificarClienteOcupada(int idCliente, int d, int m, int a, int horaEvaluar, int minutoEvaluar);



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
