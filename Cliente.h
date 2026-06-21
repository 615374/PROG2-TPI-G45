#pragma once

class Cliente {
    private:
        int idCliente;
        char nombre[50];
        char apellido[50];
        char telefono[15];
        bool estado; // Para el borrado lógico

        // Método privado auxiliar para calcular el ID autoincremental
        int generarNuevoId();

    public:
        // Constructor
        Cliente();

        // Metodos principales del objeto
        bool cargar();
        void mostrar();
        bool modificar();

        //Método para validar Id
        bool buscarPorId(int id);

        // Metodo auxiliar para la relacion Turno
        bool mostrarNombrePorId(int id);

        // Setters (Encapsulamiento)
        void setIdCliente(int id);
        void setNombre(const char* n);
        void setApellido(const char* a);
        void setTelefono(const char* tel);
        void setEstado(bool e);

        //Getters (Encapsulamiento)
        int getIdCliente();
        const char* getNombre();
        const char* getApellido();
        const char* getTelefono();
        bool getEstado();

        // Métodos de persistencia binaria (.dat)
        bool leerDisco(int pos);
        bool escribirDisco();
};

