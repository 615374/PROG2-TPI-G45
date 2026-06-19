#pragma once
#include "Fecha.h"

class Turno {
    private:
        int idTurno;
        int idCliente;
        Fecha fecha;
        float sena;
        bool asistio;
        bool estado; // Baja logica

        // Metodo privado auxiliar para el autoincremental
        int generarNuevoId();

    public:
        // Constructor
        Turno();

        // Metodos principales del objeto
        bool cargar();
        void mostrar();

        // Metodos de búsqueda interna
        bool buscarPorId(int id);

        // Setters
        void setIdTurno(int id);
        void setIdCliente(int id);
        void setFecha(Fecha f);
        void setSena(float s);
        void setAsistio(bool a);
        void setEstado(bool e);

        // Getters
        int getIdTurno();
        int getIdCliente();
        Fecha getFecha();
        float getSena();
        bool getAsistio();
        bool getEstado();

        // Metodos de persistencia binaria (.dat)
        bool leerDisco(int pos);
        bool escribirDisco();
};

// DECLARACIONES DE FUNCIONES GLOBALES

// Mantenimiento
bool darDeBajaTurno();

// Asistencia para el Menú (UX/UI)
int buscarClienteParaTurno(const char* apellidoBuscado);
