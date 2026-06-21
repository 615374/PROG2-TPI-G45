#include <iostream>
#include <cstring>
#include <cstdio>
#include "Servicio.h"
#include "Profesional.h"
#include "Turno.h"
#include "DetalleTurno.h"
#include "ServicioXProfesional.h"

using namespace std;

// PROTOTIPOS
bool verificarBloqueOcupado(int d, int m, int a, int horaEvaluar);
void mostrarGrillaSemanalAuto();

// Funcion auxiliar para filtrar profesionales por servicio (usando la tabla intermedia)
bool elProfesionalHaceElServicio(int idProf, int idServ) {
    ServicioXProfesional rel;
    int pos = 0;
    while (rel.leerDisco(pos++)) {
        if (rel.getEstado() && rel.getIdProfesional() == idProf && rel.getIdServicio() == idServ) {
            return true;
        }
    }
    return false;
}

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

// METODOS PRINCIPALES DE LA CLASE

//METODO CARGAR
bool DetalleTurno::cargar(int idT, int idS, float precioBase) {
    idTurno = idT; idServicio = idS; precioAlMomento = precioBase;

    Turno turnoPadre;
    int posT = 0, diaF = 0, mesF = 0, anioF = 2026;
    while(turnoPadre.leerDisco(posT++)) {
        if(turnoPadre.getIdTurno() == idT) {
            diaF = turnoPadre.getFecha().getDia();
            mesF = turnoPadre.getFecha().getMes();
            anioF = turnoPadre.getFecha().getAnio();
            break;
        }
    }

    // PANEL 1: HORARIO
    system("cls");
    cout << "=================================================" << endl;
    cout << "           ASIGNAR HORARIO DEL TURNO             " << endl;
    cout << "=================================================" << endl;

    bool horarioValido = false;
    do {
        cout << "Ingrese Hora (10-17) [0 para cancelar]: ";
        cin >> hora;
        if (hora == 0) return false;

        if (hora < 10 || hora > 17) {
            cout << "[ERROR] Fuera de horario comercial.\n";
        } else if (verificarBloqueOcupado(diaF, mesF, anioF, hora)) {
            cout << "[ERROR] Horario ocupado. Consultando grilla...\n";
            system("pause");
            system("cls");
            mostrarGrillaSemanalAuto();
            cout << "\nIngrese Hora nuevamente: ";
        } else {
            horarioValido = true;
        }
    } while (!horarioValido);
    minuto = 0;

    // PANEL 2: PROFESIONAL (CON FILTRO)
    system("cls");
    cout << "=================================================" << endl;
    cout << "           SELECCIONAR PROFESIONAL               " << endl;
    cout << "=================================================" << endl;

    Profesional prof;
    int posP = 0;
    bool hayProf = false;

    cout << "Profesionales aptos para este servicio:" << endl;
    cout << "-------------------------------------------------" << endl;
    while(prof.leerDisco(posP++)) {
        // Filtro: Solo muestra si esta activo y tiene relacion en ServicioXProfesional
        if(prof.getEstado() && elProfesionalHaceElServicio(prof.getIdProfesional(), idS)) {
            cout << "ID [" << prof.getIdProfesional() << "] - "
                 << prof.getApellido() << ", " << prof.getNombre() << endl;
            hayProf = true;
        }
    }

    if (!hayProf) {
        cout << "[AVISO] No hay profesionales asignados a este servicio.\n";
        system("pause");
        return false;
    }

    bool profValido = false;
    do {
        cout << "-------------------------------------------------" << endl;
        cout << "Ingrese ID del Profesional (0 para cancelar): ";
        cin >> idProfesional;
        if (idProfesional == 0) return false;

        // Validar doble: que exista y que tenga la especialidad
        if (prof.buscarPorId(idProfesional) && elProfesionalHaceElServicio(idProfesional, idS)) {
            profValido = true;
        } else {
            cout << "[ERROR] ID inválido o no apto para este servicio.\n";
        }
    } while (!profValido);

    // PANEL 3: OBSERVACIONES
    cin.ignore(1000, '\n');
    cout << "Ingrese Observaciones (opcional): ";
    cin.getline(observaciones, 200);

    estado = true;
    return true;
}

void DetalleTurno::mostrar() {
    if (estado) {
        Servicio serv; Profesional prof;
        cout << "  -> Horario: " << (hora < 10 ? "0" : "") << hora << ":00 hs" << endl;
        cout << "     Servicio: ";
        if (!serv.mostrarNombrePorId(idServicio)) cout << "ID: " << idServicio;
        cout << " | Precio: $" << precioAlMomento << endl;
        cout << "     Especialista: ";
        if (!prof.mostrarNombrePorId(idProfesional)) cout << "ID: " << idProfesional;
        cout << endl;
        if (strlen(observaciones) > 0) cout << "     Notas: " << observaciones << endl;
        cout << "  -------------------------------------------" << endl;
    }
}

// PERSISTENCIA Y METODOS
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
