#include <iostream>
#include <cstring>
#include <cstdio>
#include "DetalleTurno.h"
#include "Servicio.h"
#include "Profesional.h"

using namespace std;

//CONSTRUCTOR
DetalleTurno::DetalleTurno() {
    idTurno = 0;
    idServicio = 0;
    idProfesional = 0;
    hora = 0;
    minuto = 0;
    precioMomento = 0;

    strcpy(observaciones, "");

    estado = false;
}

//SETTERS
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

void DetalleTurno::setPrecioMomento(float p) {
    precioMomento = p;
}

void DetalleTurno::setObservaciones(const char* obs) {
    strncpy(observaciones, obs, 199);
    observaciones[199] = '\0';
}

void DetalleTurno::setEstado(bool e) {
    estado = e;

//GETTERS

}int DetalleTurno::getIdTurno() {
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

float DetalleTurno::getPrecioMomento() {
    return precioMomento;
}

const char* DetalleTurno::getObservaciones() {
    return observaciones;
}

bool DetalleTurno::getEstado() {
    return estado;
}

      //METODOS PRINCIPALES
//METODO CARGAR
bool DetalleTurno::cargar(int idTurnoRecibido) {
    Servicio serv;
    Profesional prof;
    int pos = 0;
    bool hayServicios = false;
    bool hayProfesionales = false;

    idTurno = idTurnoRecibido;

    cout << "=================================================" << endl;
    cout << "          CARGAR DETALLE DEL TURNO               " << endl;
    cout << "=================================================" << endl;
    cout << "0. Cancelar carga del detalle                    " << endl;
    cout << "-------------------------------------------------" << endl;

    cout << "\nSERVICIOS ACTIVOS DISPONIBLES:\n";

    while (serv.leerDisco(pos)) {
        if (serv.getEstado()) {
            cout << "ID: [" << serv.getIdServicio() << "] - "
                 << serv.getNombre()
                 << " - Tipo: " << serv.getTipo()
                 << " - Precio: $" << serv.getPrecio()
                 << endl;
            hayServicios = true;
        }
        pos++;
    }

    if (!hayServicios) {
        cout << "\n[ERROR] No hay servicios activos cargados.\n";
        return false;
    }

    do {
        cout << "\nIngrese ID del servicio: ";
        cin >> idServicio;

        if (idServicio == 0) {
            estado = false;
            return false;
        }

        if (!serv.buscarPorId(idServicio)) {
            cout << "[ERROR] No existe un servicio activo con ese ID.\n";
        }

    } while (!serv.buscarPorId(idServicio));

    pos = 0;

    while (serv.leerDisco(pos)) {
        if (serv.getIdServicio() == idServicio && serv.getEstado()) {
            precioMomento = serv.getPrecio();
            break;
        }
        pos++;
    }

    cout << "\nPROFESIONALES ACTIVOS DISPONIBLES:\n";

    pos = 0;

    while (prof.leerDisco(pos)) {
        if (prof.getEstado()) {
            cout << "ID: [" << prof.getIdProfesional() << "] - "
                 << prof.getApellido() << ", "
                 << prof.getNombre()
                 << " - Especialidad: " << prof.getEspecialidad()
                 << endl;
            hayProfesionales = true;
        }
        pos++;
    }

    if (!hayProfesionales) {
        cout << "\n[ERROR] No hay profesionales activos cargados.\n";
        return false;
    }

    do {
        cout << "\nIngrese ID del profesional: ";
        cin >> idProfesional;

        if (idProfesional == 0) {
            estado = false;
            return false;
        }

        if (!prof.buscarPorId(idProfesional)) {
            cout << "[ERROR] No existe un profesional activo con ese ID.\n";
        }

    } while (!prof.buscarPorId(idProfesional));

    do {
        cout << "Ingrese hora del servicio (0-23): ";
        cin >> hora;

        if (hora < 0 || hora > 23) {
            cout << "[ERROR] Hora invalida.\n";
        }

    } while (hora < 0 || hora > 23);

    do {
        cout << "Ingrese minuto del servicio (0, 15, 30 o 45): ";
        cin >> minuto;

        if (!(minuto == 0 || minuto == 15 || minuto == 30 || minuto == 45)) {
            cout << "[ERROR] Minuto invalido. Use 0, 15, 30 o 45.\n";
        }

    } while (!(minuto == 0 || minuto == 15 || minuto == 30 || minuto == 45));

    cin.ignore(1000, '\n');

    cout << "Ingrese observaciones: ";
    cin.getline(observaciones, 200);

    estado = true;
    return true;
}

//METODO MOSTRAR
void DetalleTurno::mostrar() {
    if (estado) {
        cout << "-----------------------------------" << endl;
        cout << "ID TURNO: " << idTurno << endl;
        cout << "ID SERVICIO: " << idServicio << endl;
        cout << "ID PROFESIONAL: " << idProfesional << endl;
        cout << "HORARIO: ";

        if (minuto < 10) {
            cout << hora << ":0" << minuto << endl;
        }
        else {
            cout << hora << ":" << minuto << endl;
        }

        cout << "PRECIO MOMENTO: $" << precioMomento << endl;
        cout << "OBSERVACIONES: " << observaciones << endl;
        cout << "-----------------------------------" << endl;
    }
}

//PERSISTENCIA
bool DetalleTurno::leerDisco(int pos) {
    FILE* p = fopen("detalleturnos.dat", "rb");

    if (p == NULL) {
        return false;
    }

    fseek(p, pos * sizeof(DetalleTurno), SEEK_SET);
    bool leyo = fread(this, sizeof(DetalleTurno), 1, p);

    fclose(p);
    return leyo;
}

bool DetalleTurno::escribirDisco() {
    FILE* p = fopen("detalleturnos.dat", "ab");

    if (p == NULL) {
        return false;
    }

    bool escribio = fwrite(this, sizeof(DetalleTurno), 1, p);

    fclose(p);
    return escribio;
}

        //FUNCIONES GLOBALES
//LISTAR DETALLES POR TURNO (en proceso)
void listarDetallesPorTurno(int idTurno) {
    DetalleTurno reg;
    int pos = 0;
    bool hayDetalles = false;

    cout << "=================================================" << endl;
    cout << "          DETALLES DEL TURNO ID: " << idTurno << endl;
    cout << "=================================================" << endl;

    while (reg.leerDisco(pos)) {
        if (reg.getEstado() == true && reg.getIdTurno() == idTurno) {
            reg.mostrar();
            hayDetalles = true;
        }
        pos++;
    }

    if (!hayDetalles) {
        cout << "No hay detalles cargados para este turno.\n";
    }
}
