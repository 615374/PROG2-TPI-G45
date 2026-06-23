#include <iostream>
#include <cstdlib>
#include "MenuServiciosXProfesional.h"
#include "ServicioXProfesional.h"
#include "Profesional.h"
#include "Servicio.h"

using namespace std;

void menuServicioXProfesional() {
    int op;
    ServicioXProfesional aux;

    do {
        cout << "=================================================" << endl;
        cout << "     MODULO: SERVICIOS POR PROFESIONAL           " << endl;
        cout << "=================================================" << endl;
        cout << "1. Registrar relacion Profesional-Servicio"        << endl;
        cout << "2. Listar servicios por profesional"               << endl;
        cout << "0. Volver al Menu Principal"                       << endl;
        cout << "-------------------------------------------------" << endl;
        cout << "Seleccione una opcion: ";
        cin >> op;
        cout << endl;

        if (op == 1) {
            system("cls");
            if (aux.cargar() == true) {
                if (aux.escribirDisco()) {
                    cout << "\nRelacion guardada correctamente.\n\n";
                } else {
                    cout << "\n[ERROR] No se pudo guardar la relacion.\n\n";
                }
                cin.ignore(1000, '\n');
                cout << "Presione ENTER para continuar...";
                cin.get();
            }
            system("cls");
        }
        else if (op == 2) {
            system("cls");
            Profesional prof;
            Servicio serv;
            ServicioXProfesional rel;

            int posProf = 0;
            int posRel;
            bool tieneServicios;
            bool hayRelaciones = false;

            cout << "=================================================" << endl;
            cout << "        SERVICIOS ASIGNADOS POR PROFESIONAL      " << endl;
            cout << "=================================================" << endl << endl;

            while (prof.leerDisco(posProf)) {
                if (prof.getEstado()) {
                    tieneServicios = false;
                    posRel = 0;

                    // Array de control para no repetir tratamientos de esta profesional
                    int IDsServiciosMostrados[100] = {0};
                    int cantServiciosMostrados = 0;

                    while (rel.leerDisco(posRel)) {
                        if (rel.getEstado() && rel.getIdProfesional() == prof.getIdProfesional()) {

                            // Validamos si ya listamos este tratamiento para esta misma profesional
                            bool servicioYaMostrado = false;
                            for (int k = 0; k < cantServiciosMostrados; k++) {
                                if (IDsServiciosMostrados[k] == rel.getIdServicio()) {
                                    servicioYaMostrado = true;
                                    break;
                                }
                            }

                            // Si no se mostro todavia, lo imprimimos en el panel
                            if (!servicioYaMostrado) {
                                if (!tieneServicios) {
                                    cout << "PROFESIONAL: ";
                                    prof.mostrarNombrePorId(prof.getIdProfesional());
                                    cout << endl;
                                    cout << "SERVICIOS:" << endl;
                                    tieneServicios = true;
                                    hayRelaciones = true;
                                }
                                cout << "   - ";
                                serv.mostrarNombrePorId(rel.getIdServicio());
                                cout << endl;

                                // Registramos el ID en el historial para bloquear duplicados
                                if (cantServiciosMostrados < 100) {
                                    IDsServiciosMostrados[cantServiciosMostrados] = rel.getIdServicio();
                                    cantServiciosMostrados++;
                                }
                            }
                        }
                        posRel++;
                    }
                    if (tieneServicios) {
                        cout << "-------------------------------------------------" << endl;
                    }
                }
                posProf++;
            }
            if (!hayRelaciones) {
                cout << "No hay relaciones cargadas.\n\n";
            }
            cin.ignore(1000, '\n');
            cout << "\nPresione ENTER para continuar...";
            cin.get();
            system("cls");
        }
        else if (op != 0) {
            cout << "[ERROR] Opcion incorrecta. Reintente.\n\n";
            cin.ignore(1000, '\n');
            cout << "Presione ENTER para continuar...";
            cin.get();
            system("cls");
        }
    } while (op != 0);
    system("cls");
}
