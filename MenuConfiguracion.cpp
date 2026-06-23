#include <iostream>
#include <cstdlib>
#include <fstream>

#include "MenuConfiguracion.h"
#include "Cliente.h"
#include "Profesional.h"
#include "Servicio.h"
#include "ServicioXProfesional.h"
#include "Turno.h"
#include "DetalleTurno.h"
#include "Fecha.h"


using namespace std;

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


// SUBMENU CONFIGURACION DEL SISTEMA
void menuConfiguracion() {

    int op;

    do {
        system("cls");

        cout << "==========================================" << endl;
        cout << "      CONFIGURACION DEL SISTEMA" << endl;
        cout << "==========================================" << endl;
        cout << "1) Crear Backup" << endl;
        cout << "2) Restaurar Backup" << endl;
        cout << "3) Exportar Datos a CSV" << endl;
        cout << "0) Volver" << endl;
        cout << "==========================================" << endl;
        cout << "Opcion: ";
        cin >> op;

        switch(op) {

        case 1:
            crearBackup();
            break;
        case 2:
            restaurarBackup();
            break;
        case 3:
            exportarCSV();
            break;
        case 0:
            break;
        default:
            cout << "\n[ERROR] Opcion invalida." << endl;
            cin.ignore(1000, '\n');
            cout << "Presione ENTER para continuar...";
            cin.get();
        }
    } while(op != 0);
  system("cls");
}


// FUNCIONES GLOBALES

// Crea una copia de seguridad de los archivos del sistema
void crearBackup() {

    system("cls");

    cout << "==========================================" << endl;
    cout << "              CREAR BACKUP" << endl;
    cout << "==========================================" << endl;

    system("mkdir backup");

    system("copy clientes.dat backup\\clientes.dat");
    system("copy profesionales.dat backup\\profesionales.dat");
    system("copy servicios.dat backup\\servicios.dat");
    system("copy turnos.dat backup\\turnos.dat");
    system("copy detalles_turnos.dat backup\\detalles_turnos.dat");
    system("copy servicioXprofesional.dat backup\\servicioXprofesional.dat");

    cout << "\nBackup finalizado." << endl;
    cout << "Los archivos fueron copiados en la carpeta backup." << endl;

    cin.ignore(1000, '\n');
    cout << "\nPresione ENTER para continuar...";
    cin.get();
}


// Restaura una copia de seguridad previamente creada
void restaurarBackup() {

    system("cls");

    cout << "==========================================" << endl;
    cout << "            RESTAURAR BACKUP" << endl;
    cout << "==========================================" << endl;

    char confirmar;

    cout << "ATENCION: esta accion reemplazara los archivos actuales." << endl;
    cout << "Desea continuar? (S/N): ";
    cin >> confirmar;

    if (confirmar != 'S' && confirmar != 's') {
        cout << "\nRestauracion cancelada." << endl;

        cin.ignore(1000, '\n');
        cout << "\nPresione ENTER para continuar...";
        cin.get();
        return;
    }

    system("copy backup\\clientes.dat clientes.dat");
    system("copy backup\\profesionales.dat profesionales.dat");
    system("copy backup\\servicios.dat servicios.dat");
    system("copy backup\\turnos.dat turnos.dat");
    system("copy backup\\detalles_turnos.dat detalles_turnos.dat");
    system("copy backup\\servicioXprofesional.dat servicioXprofesional.dat");

    cout << "\nRestauracion finalizada." << endl;
    cout << "Los archivos actuales fueron reemplazados por la copia de seguridad." << endl;

    cin.ignore(1000, '\n');
    cout << "\nPresione ENTER para continuar...";
    cin.get();
    cout << "\nFuncion Restaurar Backup en desarrollo." << endl;

}


// Exporta la informacion del sistema a archivos CSV
// Submenu para exportar informacion a CSV
void exportarCSV() {

    int op;

    do {
        system("cls");

        cout << "==========================================" << endl;
        cout << "             EXPORTAR A CSV" << endl;
        cout << "==========================================" << endl;
        cout << "1) Exportar Clientas" << endl;
        cout << "2) Exportar Profesionales" << endl;
        cout << "3) Exportar Servicios" << endl;
        cout << "4) Exportar Servicio por Profesional" << endl;
        cout << "5) Exportar Turnos" << endl;
        cout << "6) Exportar Detalle de Turnos" << endl;
        cout << "7) Exportar Todo" << endl;
        cout << "0) Volver" << endl;
        cout << "==========================================" << endl;
        cout << "Opcion: ";
        cin >> op;

        switch(op) {
        case 1:
            exportarClientesCSV(true);
            break;
        case 2:
            exportarProfesionalesCSV(true);
            break;
        case 3:
            exportarServiciosCSV(true);
            break;
        case 4:
            exportarServicioXProfesionalCSV(true);
            break;
        case 5:
            exportarTurnosCSV(true);
            break;
        case 6:
            exportarDetalleTurnoCSV(true);
            break;
        case 7:
            exportarTodoCSV();
            break;
        case 0:
            break;
        default:
            cout << "\n[ERROR] Opcion invalida." << endl;
            cin.ignore(1000, '\n');
            cout << "Presione ENTER para continuar...";
            cin.get();
        }
    } while(op != 0);
   system("cls");
}


// Exporta clientas a archivo CSV
void exportarClientesCSV(bool mostrarMensaje) {

    system("mkdir exportaciones >nul 2>nul");

    Cliente cli;
    int pos = 0;

    ofstream archivo("exportaciones\\Clientes.csv");

    archivo << "ID;NOMBRE;APELLIDO;TELEFONO;ESTADO\n";

    while (cli.leerDisco(pos)) {
        archivo << cli.getIdCliente() << ";"
                << cli.getNombre() << ";"
                << cli.getApellido() << ";"
                << cli.getTelefono() << ";"
                << (cli.getEstado() ? "ACTIVO" : "INACTIVO")
                << "\n";
        pos++;
    }

    archivo.close();

    if (mostrarMensaje) {
        cout << "\nClientes.csv generado correctamente." << endl;
        cout << "Ubicacion: carpeta EXPORTACIONES." << endl;

        cin.ignore(1000, '\n');
        cout << "Presione ENTER para continuar...";
        cin.get();
    }
}


// Exporta profesionales a archivo CSV
void exportarProfesionalesCSV(bool mostrarMensaje) {

    system("mkdir exportaciones >nul 2>nul");

    Profesional prof;
    int pos = 0;

    ofstream archivo("exportaciones\\Profesionales.csv");

    archivo << "ID;NOMBRE;APELLIDO;ESPECIALIDAD;COMISION;ESTADO\n";

    while (prof.leerDisco(pos)) {
        archivo << prof.getIdProfesional() << ";"
                << prof.getNombre() << ";"
                << prof.getApellido() << ";"
                << prof.getEspecialidad() << ";"
                << prof.getComision() << ";"
                << (prof.getEstado() ? "ACTIVO" : "INACTIVO")
                << "\n";
        pos++;
    }

    archivo.close();

    if (mostrarMensaje) {
        cout << "\nProfesionales.csv generado correctamente." << endl;
        cout << "Ubicacion: carpeta EXPORTACIONES." << endl;

        cin.ignore(1000, '\n');
        cout << "Presione ENTER para continuar...";
        cin.get();
    }
}


// Exporta servicios a archivo CSV
void exportarServiciosCSV(bool mostrarMensaje) {

    system("mkdir exportaciones >nul 2>nul");

    Servicio serv;
    int pos = 0;

    ofstream archivo("exportaciones\\Servicios.csv");

    archivo << "ID;NOMBRE;TIPO;PRECIO;ESTADO\n";

    while (serv.leerDisco(pos)) {
        archivo << serv.getIdServicio() << ";"
                << serv.getNombre() << ";"
                << serv.getTipo() << ";"
                << serv.getPrecio() << ";"
                << (serv.getEstado() ? "ACTIVO" : "INACTIVO")
                << "\n";
        pos++;
    }

    archivo.close();

    if (mostrarMensaje) {
        cout << "\nServicios.csv generado correctamente." << endl;
        cout << "Ubicacion: carpeta EXPORTACIONES." << endl;

        cin.ignore(1000, '\n');
        cout << "Presione ENTER para continuar...";
        cin.get();
    }
}


// Exporta Servicio por Profesional a archivo CSV
void exportarServicioXProfesionalCSV(bool mostrarMensaje) {

    system("mkdir exportaciones >nul 2>nul");

    ServicioXProfesional rel;
    int pos = 0;

    ofstream archivo("exportaciones\\ServicioXProfesional.csv");

    archivo << "ID_PROFESIONAL;ID_SERVICIO;ESTADO\n";

    while (rel.leerDisco(pos)) {
        archivo << rel.getIdProfesional() << ";"
                << rel.getIdServicio() << ";"
                << (rel.getEstado() ? "ACTIVO" : "INACTIVO")
                << "\n";
        pos++;
    }

    archivo.close();

    if (mostrarMensaje) {
        cout << "\nServicioXProfesional.csv generado correctamente." << endl;
        cout << "Ubicacion: carpeta EXPORTACIONES." << endl;

        cin.ignore(1000, '\n');
        cout << "Presione ENTER para continuar...";
        cin.get();
    }
}


// Exporta turnos a archivo CSV
void exportarTurnosCSV(bool mostrarMensaje) {

    system("mkdir exportaciones >nul 2>nul");

    Turno turno;
    Fecha fecha;
    Fecha fechaLiquidacion;
    int pos = 0;

    ofstream archivo("exportaciones\\Turnos.csv");

    archivo << "ID;ID_CLIENTE;FECHA;SENA;ASISTIO;LIQUIDADO;FECHA_LIQUIDACION;ESTADO\n";

    while (turno.leerDisco(pos)) {

        fecha = turno.getFecha();
        fechaLiquidacion = turno.getFechaLiquidacion();

        archivo << turno.getIdTurno() << ";"
                << turno.getIdCliente() << ";";

        if (fecha.getDia() < 10) archivo << "0";
        archivo << fecha.getDia() << "/";
        if (fecha.getMes() < 10) archivo << "0";
        archivo << fecha.getMes() << "/";
        archivo << fecha.getAnio() << ";";

        archivo << turno.getSena() << ";"
                << (turno.getAsistio() ? "SI" : "NO") << ";"
                << (turno.getLiquidado() ? "SI" : "NO") << ";";

        if (turno.getLiquidado()) {
            if (fechaLiquidacion.getDia() < 10) archivo << "0";
            archivo << fechaLiquidacion.getDia() << "/";
            if (fechaLiquidacion.getMes() < 10) archivo << "0";
            archivo << fechaLiquidacion.getMes() << "/";
            archivo << fechaLiquidacion.getAnio();
        } else {
            archivo << "-";
        }

        archivo << ";"
                << (turno.getEstado() ? "ACTIVO" : "INACTIVO")
                << "\n";

        pos++;
    }

    archivo.close();

    if (mostrarMensaje) {
        cout << "\nTurnos.csv generado correctamente." << endl;
        cout << "Ubicacion: carpeta EXPORTACIONES." << endl;

        cin.ignore(1000, '\n');
        cout << "Presione ENTER para continuar...";
        cin.get();
    }
}


// Exporta Detalle Turno a archivo CSV
void exportarDetalleTurnoCSV(bool mostrarMensaje) {

    system("mkdir exportaciones >nul 2>nul");

    DetalleTurno det;
    int pos = 0;

    ofstream archivo("exportaciones\\DetalleTurno.csv");

    archivo << "ID_TURNO;ID_SERVICIO;ID_PROFESIONAL;HORA;MINUTO;PRECIO_AL_MOMENTO;OBSERVACIONES;ESTADO\n";

    while (det.leerDisco(pos)) {
        archivo << det.getIdTurno() << ";"
                << det.getIdServicio() << ";"
                << det.getIdProfesional() << ";"
                << det.getHora() << ";"
                << det.getMinuto() << ";"
                << det.getPrecioAlMomento() << ";"
                << det.getObservaciones() << ";"
                << (det.getEstado() ? "ACTIVO" : "INACTIVO")
                << "\n";
        pos++;
    }

    archivo.close();

    if (mostrarMensaje) {
        cout << "\nDetalleTurno.csv generado correctamente." << endl;
        cout << "Ubicacion: carpeta EXPORTACIONES." << endl;

        cin.ignore(1000, '\n');
        cout << "Presione ENTER para continuar...";
        cin.get();
    }
}


// Exporta toda la informacion principal a CSV
void exportarTodoCSV() {

    exportarClientesCSV(false);
    exportarProfesionalesCSV(false);
    exportarServiciosCSV(false);
    exportarServicioXProfesionalCSV(false);
    exportarTurnosCSV(false);
    exportarDetalleTurnoCSV(false);

    cout << "\nExportacion completa finalizada." << endl;
    cout << "Los archivos fueron guardados en la carpeta EXPORTACIONES." << endl;

    cin.ignore(1000, '\n');
    cout << "Presione ENTER para continuar...";
    cin.get();
}
