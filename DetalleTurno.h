#pragma once
#include "Fecha.h"
#include "Turno.h"

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

        // Carga el detalle del turno con datos ya seleccionados desde el menu
        bool cargar(int idT, int idS, int idP, int h, int m, float precioBase, const char* obs);
        void mostrar();


        // Setters
        void setIdTurno(int id);
        void setIdServicio(int id);
        void setIdProfesional(int id);
        void setHora(int h);
        void setMinuto(int m);
        void setPrecioAlMomento(float p);
        void setObservaciones(const char* obs);
        void setEstado(bool e);

        //Getters
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

// Muestra todos los detalles asociados a un turno
void mostrarDetallesPorTurno(int idTurno);

// Calcula el total de un turno sumando los precios de sus detalles
float calcularTotalTurno(int idTurno);

// FUNCIONES AUXILIARES PARA VALIDAR Y MOSTRAR PROFESIONALES DISPONIBLES

        // Verifica si un profesional realiza un servicio determinado
        bool profesionalRealizaServicio(int idProfesional, int idServicio);

        // Lista los profesionales habilitados para realizar un servicio
        void listarProfesionalesParaServicio(int idServicio);


// FUNCIONES AUXILIARES PARA VALIDACIONES DE AGENDA Y DISPONIBILIDAD

    // Busca un turno activo por ID y devuelve sus datos completos
    bool obtenerTurnoPorId(int idTurnoBuscado, Turno &turnoEncontrado);

    // Compara si dos fechas tienen el mismo dia, mes y anio
    bool fechasIguales(Fecha f1, Fecha f2);

    // Verifica si un profesional ya esta ocupado en la fecha y horario del turno actual
    bool profesionalOcupadoEnFechaYHorario(int idProfesional, int hora, int minuto, int idTurnoActual);

    // Verifica si el mismo turno ya tiene un detalle en ese horario
    bool turnoYaTieneDetalleEnHorario(int idTurnoActual, int hora, int minuto);

    // Muestra horarios disponibles para un profesional en la fecha del turno
    void listarHorariosDisponibles(int idProfesional, int idTurnoActual);
