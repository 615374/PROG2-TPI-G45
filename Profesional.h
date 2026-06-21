#pragma once

class Profesional {
    private:
        int idProfesional;
        char nombre[50];
        char apellido[50];
        char especialidad[50];
        float porcentajeComision;
        bool estado; // Baja lógica

        // Metodo privado auxiliar para calcular el ID autoincremental
        int generarNuevoId();

    public:
        // Constructor
        Profesional();

        // Metodos principales del objeto
        bool cargar();
        void mostrar();
        bool modificar();

        // Metodo para validar existencia de ID
        bool buscarPorId(int id);

        // Metodo auxiliar para la relacion ServicioXProfesional
        bool mostrarNombrePorId(int id);

        // Setters (Encapsulamiento)
        void setIdProfesional(int id);
        void setNombre(const char* n);
        void setApellido(const char* a);
        void setEspecialidad(const char* e);
        void setComision(float c);
        void setEstado(bool est);

        // Getters (Encapsulamiento)
        int getIdProfesional();
        const char* getNombre();
        const char* getApellido();
        const char* getEspecialidad();
        float getComision();
        bool getEstado();

        // Métodos de persistencia binaria (.dat)
        bool leerDisco(int pos);
        bool escribirDisco();
};


