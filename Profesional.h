<<<<<<< Updated upstream
#pragma once

class Profesional{
    private:
        int idProfesional;
        char nombre[50];
        char especialidad[50];
        float porcentajeComision;
        bool estado; // baja lógica

        // Método privado auxiliar para calcular el ID autoincremental
        int generarNuevoId();

    public:
        // Constructor
        Profesional();

        // Métodos principales
        void cargar();
        void mostrar();

        //Método para validar Id
        bool buscarPorId(int id);

        //Metodo que nos sirve para ServicioXProfesional
        bool mostrarNombrePorId(int id);

        // Setters
        void setIdProfesional(int id);
        void setNombre(const char* n);
        void setEspecialidad(const char* e);
        void setComision(float c);
        void setEstado(bool e);

        //Getters
        int getIdProfesional();
        const char* getNombre();
        const char* getEspecialidad();
        float getComision();
        bool getEstado();

        // Métodos de persistencia binaria (.dat)
        bool leerDisco(int pos);
        bool escribirDisco();
};
=======
#pragma once

class Profesional{
    private:
        int idProfesional;
        char nombre[50];
        char apellido[50];
        char especialidad[50];
        float porcentajeComision;
        bool estado; // baja lÃ³gica

        // MÃ©todo privado auxiliar para calcular el ID autoincremental
        int generarNuevoId();

    public:
        // Constructor
        Profesional();

        // MÃ©todos principales
        void cargar();
        void mostrar();

        //MÃ©todo para validar Id
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

        // MÃ©todos de persistencia binaria (.dat)
        bool leerDisco(int pos);
        bool escribirDisco();
};

void darDeBajaProfesional();
>>>>>>> Stashed changes
