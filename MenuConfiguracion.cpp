#include <iostream>
#include <cstdlib>
#include <fstream>

#include "MenuConfiguracion.h"
#include "Cliente.h"
#include "Profesional.h"
#include "Servicio.h"


using namespace std;

//DECLARACION DE FUNCIONES MENU CONFIGURACION
void crearBackup();
void restaurarBackup();
void exportarCSV();


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
void exportarCSV() {

    system("cls");

    Cliente cli;
    Profesional prof;
    Servicio serv;

    int pos;

    ofstream archivoClientes("Clientes.csv");

    archivoClientes << "ID;NOMBRE;APELLIDO;TELEFONO;ESTADO\n";

    pos = 0;
    while (cli.leerDisco(pos)) {
        archivoClientes << cli.getIdCliente() << ";"
                        << cli.getNombre() << ";"
                        << cli.getApellido() << ";"
                        << cli.getTelefono() << ";"
                        << (cli.getEstado() ? "ACTIVO" : "INACTIVO")
                        << "\n";
        pos++;
    }

    archivoClientes.close();

    ofstream archivoProfesionales("Profesionales.csv");

    archivoProfesionales << "ID;NOMBRE;APELLIDO;ESPECIALIDAD;COMISION;ESTADO\n";

    pos = 0;
    while (prof.leerDisco(pos)) {
        archivoProfesionales << prof.getIdProfesional() << ";"
                             << prof.getNombre() << ";"
                             << prof.getApellido() << ";"
                             << prof.getEspecialidad() << ";"
                             << prof.getComision() << ";"
                             << (prof.getEstado() ? "ACTIVO" : "INACTIVO")
                             << "\n";
        pos++;
    }

    archivoProfesionales.close();

    ofstream archivoServicios("Servicios.csv");

    archivoServicios << "ID;NOMBRE;TIPO;PRECIO;ESTADO\n";

    pos = 0;
    while (serv.leerDisco(pos)) {
        archivoServicios << serv.getIdServicio() << ";"
                         << serv.getNombre() << ";"
                         << serv.getTipo() << ";"
                         << serv.getPrecio() << ";"
                         << (serv.getEstado() ? "ACTIVO" : "INACTIVO")
                         << "\n";
        pos++;
    }

    archivoServicios.close();

    cout << "==========================================" << endl;
    cout << "             EXPORTACION CSV" << endl;
    cout << "==========================================" << endl;
    cout << "Exportacion finalizada correctamente." << endl;
    cout << "\nArchivos generados:" << endl;
    cout << "- Clientes.csv" << endl;
    cout << "- Profesionales.csv" << endl;
    cout << "- Servicios.csv" << endl << endl;
    cout << "Los archivos fueron generados en la carpeta del proyecto." << endl;
    cout << "Puede abrirlos con Excel o Bloc de notas." << endl;

    cin.ignore(1000, '\n');
    cout << "\nPresione ENTER para continuar...";
    cin.get();
}
