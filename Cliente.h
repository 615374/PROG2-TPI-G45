#ifndef CLIENTE_H
#define CLIENTE_H

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

        // Métodos de Carga y Muestra por consola
        void cargar();
        void mostrar();

        //Método para validar Id
        bool buscarPorId(int id);

        // Setters y Getters (Encapsulamiento)
        void setIdCliente(int id);
        void setNombre(const char* n);
        void setApellido(const char* a);
        void setTelefono(const char* tel);
        void setEstado(bool e);

        int getIdCliente();
        const char* getNombre();
        const char* getApellido();
        bool getEstado();

        // Métodos de persistencia binaria (.dat)
        bool leerDisco(int pos);
        bool escribirDisco();
};

#endif // CLIENTE_H
