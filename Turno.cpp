#include <iostream>
#include <cstdio>
#include <cstring>
#include "Turno.h"
#include "Cliente.h"

using namespace std;

// CONSTRUCTOR
Turno::Turno() {
    idTurno = 0;
    idCliente = 0;
    sena = 0;
    asistio = false;
    estado = false;
}

// METODO AUXILIAR
int Turno::generarNuevoId() {
    FILE* p = fopen("turnos.dat", "rb");

    if (p == NULL) {
        return 1;
    }

    fseek(p, 0, SEEK_END);
    long bytesTotales = ftell(p);
    fclose(p);

    int cantidadRegistros = bytesTotales / sizeof(Turno);

    return cantidadRegistros + 1;
}

// SETTERS
void Turno::setIdTurno(int id) {
    idTurno = id;
}

void Turno::setIdCliente(int id) {
    idCliente = id;
}

void Turno::setFecha(Fecha f) {
    fecha = f;
}

void Turno::setSena(float s) {
    sena = s;
}

void Turno::setAsistio(bool a) {
    asistio = a;
}

void Turno::setEstado(bool e) {
    estado = e;
}

// GETTERS
int Turno::getIdTurno() {
    return idTurno;
}

int Turno::getIdCliente() {
    return idCliente;
}

Fecha Turno::getFecha() {
    return fecha;
}

float Turno::getSena() {
    return sena;
}

bool Turno::getAsistio() {
    return asistio;
}

bool Turno::getEstado() {
    return estado;
}


// METODOS PRINCIPALES DE LA CLASE
// Carga la cabecera del turno con datos ya seleccionados desde el menu
bool Turno::cargar(int idClienteSeleccionado, Fecha fechaSeleccionada, float senaIngresada) {
    idTurno = generarNuevoId();
    idCliente = idClienteSeleccionado;
    fecha = fechaSeleccionada;
    sena = senaIngresada;
    asistio = false;
    estado = true;

    return true;
}

void Turno::mostrar() {
    if (estado) {
        Cliente cli;
        cout << "-----------------------------------" << endl;
        cout << "ID TURNO: " << idTurno << endl;

        // Muestra apellido y nombre directo en la ficha del turno
        cout << "CLIENTA: ";
        if (!cli.mostrarNombrePorId(idCliente)) {
            cout << "No disponible (ID: " << idCliente << ")";
        }
        cout << endl;

        cout << "FECHA: ";
        fecha.mostrar();
        cout << "SENIA: $" << sena << endl;
        cout << "ASISTIO: " << (asistio ? "SI" : "NO") << endl;
        cout << "-----------------------------------" << endl;
    }
}

bool Turno::buscarPorId(int id) {
    int pos = 0;
    while (leerDisco(pos)) {
        if (idTurno == id && estado == true) {
            return true;
        }
        pos++;
    }
    return false;
}

// PERSISTENCIA
bool Turno::leerDisco(int pos) {
    FILE* p = fopen("turnos.dat", "rb");
    if (p == NULL) return false;

    fseek(p, pos * sizeof(Turno), SEEK_SET);
    bool leyo = fread(this, sizeof(Turno), 1, p);
    fclose(p);

    return leyo;
}

bool Turno::escribirDisco() {
    FILE* p = fopen("turnos.dat", "ab");
    if (p == NULL) return false;

    bool escribio = fwrite(this, sizeof(Turno), 1, p);
    fclose(p);

    return escribio;
}

