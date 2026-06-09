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
void ServicioXProfesional::cargar() {
    //Agregamos para validar profesional y servicio
    Profesional prof;
    Servicio serv;

    do{ //WHILE DE PROFESIONAL
    cout << "Ingrese ID del Profesional: ";
    cin >> idProfesional;
    if(!prof.buscarPorId(idProfesional)){
       cout << "Ese profesional no existe.\n";
    }
    }while(!prof.buscarPorId(idProfesional));

    do{ //WHILE DE SERVICIO
    cout << "Ingrese ID del Servicio: ";
    cin >> idServicio;
    if(!serv.buscarPorId(idServicio)){
       cout << "Ese servicio no existe.\n";
       }
    }while(!serv.buscarPorId(idServicio));
    estado = true;
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
