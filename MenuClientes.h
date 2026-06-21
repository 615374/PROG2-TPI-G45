#ifndef MENUCLIENTES_H
#define MENUCLIENTES_H

void menuClientes();

// Mantenimiento de clientas
void modificarCliente();
bool darDeBajaCliente();

// Consultas y reportes de clientas
void listarClientasPorApellido();
void listarClientasPorFrecuencia();
void listarClientasConSaldos();
void historialTratamientosPorClienta();

// Busqueda auxiliar para elegir clienta sin memorizar ID
int seleccionarClientePorApellido();

#endif // MENUCLIENTES_H
