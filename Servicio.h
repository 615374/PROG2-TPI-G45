#pragma once

class Servicio {
    private:
        int idServicio;
        char nombre[50];
        char tipo[30];
        float precioActual;
        bool estado;

        int generarNuevoId();

    public:
        Servicio();

        // Métodos principales
        bool cargar();
        void mostrar();
        bool modificar();

        // Métodos para validar / mostrar por ID
        bool buscarPorId(int id);
        bool mostrarNombrePorId(int id);

        // Setters
        void setIdServicio(int id);
        void setNombre(const char* n);
        void setTipo(const char* t);
        void setPrecio(float p);
        void setEstado(bool e);

        // Getters
        int getIdServicio();
        const char* getNombre();
        const char* getTipo();
        float getPrecio();
        bool getEstado();

        // Persistencia
        bool leerDisco(int pos);
        bool escribirDisco();
};

// FUNCIONES GLOBALES

// Mantenimiento
bool darDeBajaServicio();
void modificarServicio();

// Listados
void listarServiciosPorTipo();
void listarServiciosInactivos();
