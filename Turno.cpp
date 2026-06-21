#include <iostream>
#include <cstdio>
#include <cstring>
#include <ctime>
#include "Turno.h"
#include "Cliente.h"
#include "MenuTurnos.h"

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

//METODOS PRINCIPALES DE LA CLASE

//METODO CARGAR
bool Turno::cargar() {
    cout << "=================================================" << endl;
    cout << "              AGENDAR FECHA DEL TURNO            " << endl;
    cout << "=================================================" << endl;

    Cliente cli;
    if (idCliente > 0) {
        cout << "Clienta: ";
        cli.mostrarNombrePorId(idCliente);
        cout << "\n-------------------------------------------------" << endl;
    }

    int modoFecha;
    int diaT = 0, mesT = 0, anioT = 2026;
    bool fechaValida = false;

    do {
        cout << "SELECCIONE MODO DE CARGA DE FECHA:\n";
        cout << "1. Ver Grilla Semanal en vivo y elegir dia\n";
        cout << "2. Cargar Dia y Mes manualmente\n";
        cout << "0. Cancelar alta de turno\n";
        cout << "-------------------------------------------------\n";
        cout << "Seleccione una opcion: ";
        cin >> modoFecha;

        if (modoFecha == 0) {
            estado = false;
            return false;
        }

        if (modoFecha == 1) {
            system("cls");
            mostrarGrillaSemanalAuto(); // Muestra los 5 dias habiles a partir de hoy

            cout << "-------------------------------------------------\n";
            cout << "Ingrese el NUMERO de dia elegido (ej. 25): ";
            cin >> diaT;

            time_t tActual = time(0);
            bool encontrado = false;
            int diasListados = 0;
            int offset = 0;

            // Buscamos solo en los proximos 5 dias habiles (igual que la grilla)
            while (diasListados < 5) {
                time_t tDia = tActual + (offset * 24 * 60 * 60);
                tm* infoDia = localtime(&tDia);

                if (infoDia->tm_wday >= 1 && infoDia->tm_wday <= 5) {
                    if (infoDia->tm_mday == diaT) {
                        mesT = infoDia->tm_mon + 1;
                        anioT = infoDia->tm_year + 1900;
                        encontrado = true;
                        break;
                    }
                    diasListados++;
                }
                offset++;
                // Evitamos un bucle infinito si el usuario ingresa un numero absurdo
                if (offset > 30) break;
            }

            if (encontrado) {
                fechaValida = true;
                cout << "\n[OK] Fecha detectada: " << (diaT < 10 ? "0" : "") << diaT
                     << "/" << (mesT < 10 ? "0" : "") << mesT << "/" << anioT << endl;
            } else {
                cout << "\n[ERROR] El dia ingresado no figura en la grilla actual. Reintente.\n\n";
                system("pause");
                system("cls");
            }
        }
        else if (modoFecha == 2) {
            cout << "Ingrese dia (1-31): "; cin >> diaT;
            cout << "Ingrese mes (1-12): "; cin >> mesT;
            anioT = 2026;
            fechaValida = true;
        }
        else {
            cout << "[ERROR] Opcion incorrecta.\n";
        }

    } while (!fechaValida);

    // Guardamos la fecha resuelta
    fecha.setDia(diaT);
    fecha.setMes(mesT);
    fecha.setAnio(anioT);

    // Validacion de senia
    do {
        cout << "Ingrese monto de senia: $";
        cin >> sena;
        if (sena < 0) cout << "[ERROR] La senia no puede ser negativa.\n";
    } while (sena < 0);

    idTurno = generarNuevoId();
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

