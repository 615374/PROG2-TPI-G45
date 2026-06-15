#pragma once

class DetalleTurno {
private:
    int idTurno;
    int idServicio;
    int idProfesional;
    int hora;
    int minuto;
    float precioMomento;
    char observaciones[200];
    bool estado;

public:
    //CONSTRUCTOR
    DetalleTurno();

    //METODOS PRINCIPALES
    bool cargar(int idTurnoRecibido);
    void mostrar();

    //METODO PARA EVITAR SOBRETURNOS
    //bool existeSolapamiento(int idProf, int h, int m);

    //SETTERS
    void setIdTurno(int id);
    void setIdServicio(int id);
    void setIdProfesional(int id);
    void setHora(int h);
    void setMinuto(int m);
    void setPrecioMomento(float p);
    void setObservaciones(const char* obs);
    void setEstado(bool e);

    //GETTERS
    int getIdTurno();
    int getIdServicio();
    int getIdProfesional();
    int getHora();
    int getMinuto();
    float getPrecioMomento();
    const char* getObservaciones();
    bool getEstado();

    //PERSISTENCIA
    bool leerDisco(int pos);
    bool escribirDisco();
};

//FUNCIONES GLOBALES
void listarDetallesPorTurno(int idTurno);
