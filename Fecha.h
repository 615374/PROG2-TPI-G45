#pragma once

class Fecha {
    private:
        int dia;
        int mes;
        int anio;

    public:
        Fecha();

        bool cargar();
        void mostrar();

        void getFechaFormateada();

        bool validar();
        bool esBisiesto();

        void setDia(int d);
        void setMes(int m);
        void setAnio(int a);

        int getDia();
        int getMes();
        int getAnio();

};
