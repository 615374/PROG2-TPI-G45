#pragma once

class DetalleTurno {
    private:
        int idTurno;
        int idServicio;
        int idProfesional;
        int hora;
        int minuto;
        float precioAlMomento;
        char observaciones[200];
        bool estado;

    public:
        DetalleTurno();

        bool cargar(int idT, int idS, float precioBase, int idP);
        void mostrar();

        // Getters y Setters
        void setIdTurno(int id);
        void setIdServicio(int id);
        void setIdProfesional(int id);
        void setHora(int h);
        void setMinuto(int m);
        void setPrecioAlMomento(float p);
        void setObservaciones(const char* obs);
        void setEstado(bool e);

        int getIdTurno();
        int getIdServicio();
        int getIdProfesional();
        int getHora();
        int getMinuto();
        float getPrecioAlMomento();
        const char* getObservaciones();
        bool getEstado();

        // Persistencia
        bool leerDisco(int pos);
        bool escribirDisco();
};


