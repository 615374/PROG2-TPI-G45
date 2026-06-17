#pragma once

class Fecha {
    private:
        int dia;
        int mes;
        int anio;

    public:
        // Constructor
        Fecha();

        // Validaciones
        bool validar();
        bool esBisiesto();

        // Metodos principales
        bool cargar();
        void mostrar();
        void mostrarFormateada();

        // Setters
        void setDia(int d);
        void setMes(int m);
        void setAnio(int a);

        // Getters
        int getDia();
        int getMes();
        int getAnio();
};
