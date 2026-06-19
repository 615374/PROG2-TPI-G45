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
bool Turno::cargar() {

    cout << "=================================================" << endl;
    cout << "              REGISTRAR NUEVO TURNO              " << endl;
    cout << "=================================================" << endl;
    cout << "0. Volver al Menu Principal / Cancelar alta      " << endl;
    cout << "-------------------------------------------------" << endl;

    Cliente cli;
    bool idValido = false;

    // Si el idCliente ya se asigno en el menu (es mayor a 0), saltamos el pedido por teclado
    if (idCliente > 0) {
        cout << "ID CLIENTA ASOCIADA AL TURNO: " << idCliente << " (";
        cli.mostrarNombrePorId(idCliente);
        cout << ")" << endl << endl;
    }
    else {
        // Si por alguna razon arranca en 0, lo pide como antes
        do {
            cout << "Ingrese ID de la clienta: ";
            cin >> idCliente;

            if (idCliente == 0) {
                estado = false;
                return false;
            }

            if (!cli.buscarPorId(idCliente)) {
                cout << "[ERROR] No existe ninguna clienta activa con el ID ingresado.\n\n";
            } else {
                idValido = true;
            }
        } while (!idValido);
    }

    idTurno = generarNuevoId();
    cout << "ID TURNO ASIGNADO AUTOMATICAMENTE: " << idTurno << endl;

    // Carga de la clase Fecha asociada
    if (!fecha.cargar()) {
        estado = false;
        return false;
    }

    // Validacion del monto de la seña
    do {
        cout << "Ingrese monto de senia: $";
        cin >> sena;

        if (sena < 0) {
            cout << "[ERROR] La senia no puede ser un valor negative.\n";
        }
    } while (sena < 0);

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

