#include <iostream>
#include <cstdlib>
#include "main.h"
#include "MenuClientes.h"
#include "MenuProfesionales.h"
#include "MenuServicios.h"
#include "MenuServiciosXProfesional.h"
#include "MenuTurnos.h"
#include "MenuConfiguracion.h"

using namespace std;

int main(){
    int opcion;
    do {
        cout << "=================================================" << endl;
        cout << "   SISTEMA DE GESTION: CENTRO DE ESTETICA        " << endl;
        cout << "=================================================" << endl;
        cout << "1) Modulo: Gestion de Clientas" << endl;
        cout << "2) Modulo: Gestion de Profesionales (Staff)" << endl;
        cout << "3) Modulo: Catalogo de Servicios y Precios" << endl;
        cout << "4) Modulo: Agenda de Turnos y Detalles" << endl;
        cout << "5) Modulo: Servicio por Profesional" << endl;
        cout << "6) Modulo: Configuracion del Sistema" << endl;
        cout << "0) Cerrar Sistema" << endl;
        cout << "-------------------------------------------------" << endl;
        cout << "Seleccione una opcion de las anteriores: ";
        cin >> opcion;
        cout << endl;

        system("cls"); // Limpieza de pantalla al seleccionar

        switch (opcion) {
            case 1:
                menuClientes();
                break;
            case 2:
                menuProfesionales();
                break;
            case 3:
                menuServicios();
                break;
            case 4:
                menuTurnos();
                break;
            case 5:
                menuServicioXProfesional();
                break;
            case 6:
                menuConfiguracion();
                break;
            case 0:
                cout << "Saliendo de la aplicacion de gestion..." << endl;
                break;
            default:
                cout << "Opcion incorrecta. Reintente." << endl;
        }
    } while (opcion != 0);
}



