#pragma once

class ServicioXProfesional {
    private:
        int idProfesional;
        int idServicio;
        bool estado;

    public:
        // Constructor
        ServicioXProfesional();

        // Métodos principales
        void cargar();
        void mostrar();

        // Setters
        void setIdProfesional(int idP);
        void setIdServicio(int idS);
        void setEstado(bool e);

        // Getters
        int getIdProfesional();
        int getIdServicio();
        bool getEstado();

        // Persistencia
        bool leerDisco(int pos);
        bool escribirDisco();
};
