#pragma once
#include "Fecha.h"

class Turno {
private:
    int idTurno;
    int idCliente;
    Fecha fecha;
    float sena;
    bool asistio;
    bool estado;

    int generarNuevoId();

public:
    // CONSTRUCTOR
    Turno();

    // METODOS PRINCIPALES
    bool cargar();
    void mostrar();

    // BÚSQUEDA
    bool buscarPorId(int id);

    // SETTERS
    void setIdTurno(int id);
    void setIdCliente(int id);
    void setFecha(Fecha f);
    void setSena(float s);
    void setAsistio(bool a);
    void setEstado(bool e);

    // GETTERS
    int getIdTurno();
    int getIdCliente();
    Fecha getFecha();
    float getSena();
    bool getAsistio();
    bool getEstado();

    // PERSISTENCIA/MECANICA
    bool leerDisco(int pos);
    bool escribirDisco();
};

// FUNCIONES GLOBALES
bool darDeBajaTurno();
