#pragma once

class Profesional{
    private:
        int idProfesional;
        char nombre[50];
        char apellido[50];
        char especialidad[50];
        float porcentajeComision;
        bool estado; // baja lógica

        // Metodo privado auxiliar para calcular el ID autoincremental
        int generarNuevoId();

    public:
        // Constructor
        Profesional();

        // Metodos principales
        void cargar();
        void mostrar();

        //Metodo para validar Id
        bool buscarPorId(int id);

        //Metodo que nos sirve para ServicioXProfesional
        bool mostrarNombrePorId(int id);

        // Setters
        void setIdProfesional(int id);
        void setNombre(const char* n);
        void setApellido(const char* n);
        void setEspecialidad(const char* e);
        void setComision(float c);
        void setEstado(bool e);

        //Getters
        int getIdProfesional();
        const char* getNombre();
        const char* getApellido();
        const char* getEspecialidad();
        float getComision();
        bool getEstado();

        // Metodos de persistencia binaria (.dat)
        bool leerDisco(int pos);
        bool escribirDisco();
};

void darDeBajaProfesional();

