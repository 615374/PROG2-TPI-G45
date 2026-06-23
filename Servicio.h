#pragma once

class Servicio {
    private:
        int idServicio;
        char nombre[50];
        char tipo[30];
        float precioActual;
        bool estado; // Para el borrado lógico

        // Método privado auxiliar para calcular el ID autoincremental
        int generarNuevoId();

        public:
        // Constructor
        Servicio();

        // Metodos principales del objeto
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

