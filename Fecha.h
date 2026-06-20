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

        // Metodo auxiliar para avanzar fechas
        void sumarDias(int cantidad);
         //(Suma dias a la fecha actual contemplando cambio de mes y año)

        // Setters
        void setDia(int d);
        void setMes(int m);
        void setAnio(int a);

        // Getters
        int getDia();
        int getMes();
        int getAnio();
};
