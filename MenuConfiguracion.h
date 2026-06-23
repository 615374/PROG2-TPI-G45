#ifndef MENUCONFIGURACION_H_INCLUDED
#define MENUCONFIGURACION_H_INCLUDED

void menuConfiguracion();

//DECLARACION DE FUNCIONES MENU CONFIGURACION
void crearBackup();
void restaurarBackup();
void exportarCSV();

void exportarClientesCSV(bool mostrarMensaje);
void exportarProfesionalesCSV(bool mostrarMensaje);
void exportarServiciosCSV(bool mostrarMensaje);
void exportarServicioXProfesionalCSV(bool mostrarMensaje);
void exportarTurnosCSV(bool mostrarMensaje);
void exportarDetalleTurnoCSV(bool mostrarMensaje);
void exportarTodoCSV();

#endif // MENUCONFIGURACION_H_INCLUDED
