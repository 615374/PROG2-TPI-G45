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

        bool cargar();
        void mostrar();
        bool modificar();

        //Método para validar Id
        bool buscarPorId(int id);

        //Método que despues nos sirve ServicioXProfesional
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

//DECLARACIONES DE FUNCIONES GLOBALES

// Mantenimiento
void modificarServicio();
bool darDeBajaServicio();

// Listados
void listarServiciosPorTipo();
void listarServiciosInactivos();
// Lista servicios ordenados por cantidad de veces solicitados
void rankingServiciosMasPedidos();
