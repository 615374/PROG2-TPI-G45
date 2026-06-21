#include <iostream>
#include <cstring>
#include <cstdio>
#include "DetalleTurno.h"
#include "Servicio.h"
#include "Profesional.h"

using namespace std;

// CONSTRUCTOR
DetalleTurno::DetalleTurno() {
    idTurno = 0;
    idServicio = 0;
    idProfesional = 0;
    hora = 0;
    minuto = 0;
    precioAlMomento = 0;
    strcpy(observaciones, "");
    estado = false;
}

// SETTERS
void DetalleTurno::setIdTurno(int id) {
    idTurno = id;
}

void DetalleTurno::setIdServicio(int id) {
    idServicio = id;
}

void DetalleTurno::setIdProfesional(int id) {
    idProfesional = id;
}

void DetalleTurno::setHora(int h) {
    hora = h;
}

void DetalleTurno::setMinuto(int m) {
    minuto = m;
}

void DetalleTurno::setPrecioAlMomento(float p) {
    precioAlMomento = p;
}

void DetalleTurno::setObservaciones(const char* obs) {
    strncpy(observaciones, obs, 199);
    observaciones[199] = '\0';
}

void DetalleTurno::setEstado(bool e) {
    estado = e;
}

//GETTERS
int DetalleTurno::getIdTurno() {
    return idTurno;
}

int DetalleTurno::getIdServicio() {
    return idServicio;
}

int DetalleTurno::getIdProfesional() {
    return idProfesional;
}

int DetalleTurno::getHora() {
    return hora;
}

int DetalleTurno::getMinuto() {
    return minuto;
}

float DetalleTurno::getPrecioAlMomento() {
    return precioAlMomento;
}

const char* DetalleTurno::getObservaciones() {
    return observaciones;
}

bool DetalleTurno::getEstado() {
    return estado;
}

    // METODOS PRINCIPALES

// METODO CARGAR
// Carga el detalle del turno con datos ya seleccionados desde el menu
bool DetalleTurno::cargar(int idT, int idS, int idP, int h, int m, float precioBase, const char* obs) {
    idTurno = idT;
    idServicio = idS;
    idProfesional = idP;
    hora = h;
    minuto = m;
    precioAlMomento = precioBase;
    setObservaciones(obs);
    estado = true;

    return true;
}

// METODO MOSTRAR
void DetalleTurno::mostrar() {
    if (estado) {
        Servicio serv;
        Profesional prof;

        cout << "  -> Horario: " << (hora < 10 ? "0" : "") << hora << ":"
             << (minuto < 10 ? "0" : "") << minuto << " hs" << endl;

        cout << "     Servicio: ";
        if (!serv.mostrarNombrePorId(idServicio)) cout << "ID: " << idServicio;
        cout << " | Precio Cobrado: $" << precioAlMomento << endl;

        cout << "     Especialista: ";
        if (!prof.mostrarNombrePorId(idProfesional)) cout << "ID: " << idProfesional;
        cout << endl;

        if (strlen(observaciones) > 0) {
            cout << "     Notas: " << observaciones << endl;
        }
        cout << "  -------------------------------------------" << endl;
    }
}


// PERSISTENCIA
bool DetalleTurno::leerDisco(int pos) {
    FILE* p = fopen("detalles_turnos.dat", "rb");
    if (p == NULL) return false;
    fseek(p, pos * sizeof(DetalleTurno), SEEK_SET);
    bool leyo = fread(this, sizeof(DetalleTurno), 1, p);
    fclose(p);
    return leyo;
}

bool DetalleTurno::escribirDisco() {
    FILE* p = fopen("detalles_turnos.dat", "ab");
    if (p == NULL) return false;
    bool escribio = fwrite(this, sizeof(DetalleTurno), 1, p);
    fclose(p);
    return escribio;
}
