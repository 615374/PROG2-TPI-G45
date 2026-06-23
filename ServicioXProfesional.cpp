#include <iostream>
#include <cstdio>
#include "ServicioXProfesional.h"
#include "Profesional.h"
#include "Servicio.h"

using namespace std;

// CONSTRUCTOR
ServicioXProfesional::ServicioXProfesional() {
    idProfesional = 0;
    idServicio = 0;
    estado = false;
}

// SETTERS
void ServicioXProfesional::setIdProfesional(int idP) {
    idProfesional = idP;
}

void ServicioXProfesional::setIdServicio(int idS) {
    idServicio = idS;
}

void ServicioXProfesional::setEstado(bool e) {
    estado = e;
}

// GETTERS
int ServicioXProfesional::getIdProfesional() {
    return idProfesional;
}

int ServicioXProfesional::getIdServicio() {
    return idServicio;
}

bool ServicioXProfesional::getEstado() {
    return estado;
}

// METODOS PRINCIPALES
bool ServicioXProfesional::cargar() {
    Profesional prof;
    Servicio serv;
    int pos;

    // DESPLIEGUE DE PROFESIONALES
    cout << "=================================================" << endl;
    cout << "       SELECCIONE UNA PROFESIONAL DEL STAFF       " << endl;
    cout << "=================================================" << endl;
    cout << "0. Volver al Menu Principal / Cancelar alta      " << endl;
    cout << "-------------------------------------------------" << endl;
    pos = 0;
    while (prof.leerDisco(pos)) {
        if (prof.getEstado()) {
            prof.mostrarNombrePorId(prof.getIdProfesional());
            cout << "\n";
        }
        pos++;
    }
    cout << "-------------------------------------------------" << endl;

    // Validacion interactiva del ID de la Profesional
    do {
        cout << "Ingrese el ID de la Profesional elegida: ";
        cin >> idProfesional;

        //Si es 0, sale del bucle de validacion
        if (idProfesional == 0) {
            break;
        }

        if (!prof.buscarPorId(idProfesional)) {
            cout << "[ERROR] El ID ingresado no corresponde a ninguna profesional activa.\n";
        }
    } while (!prof.buscarPorId(idProfesional));

    // Si el usuario tipeo 0, frenamos la funcion
    if (idProfesional == 0) {
        estado = false;
        return false; // Regresa directo al main sin mostrar servicios
    }

    system("cls");

    // DESPLIEGUE DE SERVICIOS
    cout << "=================================================" << endl;
    cout << "          CATALOGO DE SERVICIOS ACTIVOS          " << endl;
    cout << "=================================================" << endl;
    cout << "0. Volver al Menu Principal / Cancelar alta      " << endl; // Sumamos el aviso visual
    cout << "-------------------------------------------------" << endl;
    pos = 0;
    while (serv.leerDisco(pos)) {
        if (serv.getEstado()) {
            serv.mostrarNombrePorId(serv.getIdServicio());
            cout << " - Tipo: " << serv.getTipo() << " ($" << serv.getPrecio() << ")\n";
        }
        pos++;
    }
    cout << "-------------------------------------------------" << endl;

    // Validacion interactiva del ID del Servicio
    do {
        cout << "Ingrese el ID del Servicio a asignar: ";
        cin >> idServicio;

        //Si es 0, sale del bucle de validacion
        if (idServicio == 0) {
            break;
        }

        if (!serv.buscarPorId(idServicio)) {
            cout << "[ERROR] El ID ingresado no corresponde a un servicio activo.\n";
        }
    } while (!serv.buscarPorId(idServicio));

    // Si se arrepintio en el segundo paso, tambien abortamos
    if (idServicio == 0) {
        estado = false;
        return false;
    }

    // Si paso ambos filtros con IDs reales, la relacion es valida
    estado = true;
    return true; // Exito total
}
void ServicioXProfesional::mostrar() {
    if (estado) {
        cout << "-----------------------------------" << endl;
        cout << "ID PROFESIONAL: "
             << idProfesional << endl;
        cout << "ID SERVICIO: "
             << idServicio << endl;
        cout << "-----------------------------------" << endl;
    }
}

// PERSISTENCIA
bool ServicioXProfesional::leerDisco(int pos) {
    FILE* p = fopen("servicioXprofesional.dat", "rb");
    if (p == NULL) return false;

    fseek(p, pos * sizeof(ServicioXProfesional), SEEK_SET);
    bool leyo = fread(this, sizeof(ServicioXProfesional), 1, p);
    fclose(p);

    return leyo;
}

bool ServicioXProfesional::escribirDisco() {
    FILE* p = fopen("servicioXprofesional.dat", "ab");
    if (p == NULL) return false;

    bool escribio = fwrite(this, sizeof(ServicioXProfesional), 1, p);
    fclose(p);

    return escribio;
}
