#include <iostream>
#include <cstdlib>
#include <cstring>
#include "MenuProfesionales.h"
#include "Profesional.h"
#include "DetalleTurno.h"
#include "Turno.h"

using namespace std;


// FUNCIONES GLOBALES

// Algoritmo: Filtra y muestra los profesionales activos agrupados por especialidad
void listarProfesionalesPorEspecialidad() {
    char especialidades[100][50];
    int cantidadEspecialidades = 0;
    int opcionEspecialidad;
    Profesional reg;
    int pos = 0;
    bool repetida;
    bool encontrado = false;

    // Armamos la lista de especialidades unicas en memoria
    while (reg.leerDisco(pos)) {
        if (reg.getEstado()) {
            repetida = false;
            for (int i = 0; i < cantidadEspecialidades; i++) {
                if (strcmp(especialidades[i], reg.getEspecialidad()) == 0) {
                    repetida = true;
                }
            }
            if (!repetida) {
                strcpy(especialidades[cantidadEspecialidades], reg.getEspecialidad());
                cantidadEspecialidades++;
            }
        }
        pos++;
    }

    if (cantidadEspecialidades == 0) {
        cout << "[AVISO] No hay especialidades disponibles porque no hay profesionales activos.\n\n";
        return;
    }
    cout << "ESPECIALIDADES DISPONIBLES EN SISTEMA:" << endl;
    cout << "-------------------------------------------------" << endl;
    for (int i = 0; i < cantidadEspecialidades; i++) {
        cout << i + 1 << ". " << especialidades[i] << endl;
    }
    cout << "0. Volver al menu anterior" << endl;
    cout << "-------------------------------------------------" << endl;

    do {
        cout << "Seleccione una especialidad: ";
        cin >> opcionEspecialidad;
        if (opcionEspecialidad < 0 || opcionEspecialidad > cantidadEspecialidades) {
            cout << "[ERROR] Opcion invalida. Reintente.\n";
        }
    } while (opcionEspecialidad < 0 || opcionEspecialidad > cantidadEspecialidades);

    cin.ignore(1000, '\n');

    if (opcionEspecialidad == 0) {
        cout << "\nOperacion cancelada. Volviendo al menu...\n";
        return;
    }

    system("cls");

    cout << "=================================================" << endl;
    cout << "          PANEL DE CONSULTAS Y REPORTES          " << endl;
    cout << "=================================================" << endl;
    cout << "1. Ver Staff de Profesionales Activas" << endl;
    cout << "2. Listar por Especialidad" << endl;
    cout << "3. Listar Ordenadas por Volumen de Servicios" << endl;
    cout << "4. Ver Staff Profesionales Inactivas" << endl;
    cout << "5. Liquidacion de Comisiones" << endl;
    cout << "0. Volver al Menu de Gestion" << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "Opcion seleccionada: 2" << endl << endl;

    pos = 0;
    cout << "=================================================" << endl;
    cout << "  RESULTADOS PARA: " << especialidades[opcionEspecialidad - 1] << endl;
    cout << "=================================================" << endl;

    while (reg.leerDisco(pos)) {
        if (reg.getEstado() && strcmp(reg.getEspecialidad(), especialidades[opcionEspecialidad - 1]) == 0) {
            reg.mostrar();
            encontrado = true;
        }
        pos++;
    }
    if (!encontrado) {
        cout << "No se encontraron profesionales activos con esa especialidad.\n";
    }
}

// Algoritmo: Recorre el archivo mostrando unicamente el staff dado de baja
void listarProfesionalesInactivos() {
    Profesional reg;
    int pos = 0;
    bool hayInactivos = false;
    while (reg.leerDisco(pos)) {
        if (reg.getEstado() == false) { // Si el estado es false (inactiva)
            // Tiramos los cout directos para que no se silencie por el if de mostrar()
            cout << "-----------------------------------" << endl;
            cout << "ID PROFESIONAL: " << reg.getIdProfesional() << endl;
            cout << "NOMBRE: " << reg.getNombre() << endl;
            cout << "APELLIDO: " << reg.getApellido() << endl;
            cout << "ESPECIALIDAD: " << reg.getEspecialidad() << endl;
            cout << "COMISION: " << reg.getComision() << "%" << endl;
            cout << "-----------------------------------" << endl;

            hayInactivos = true;
        }
        pos++;
    }

    if (!hayInactivos) {
        cout << "No hay profesionales inactivos para mostrar.\n";
    }
    cout << "=================================================" << endl << endl;
}

// Algoritmo: Lista profesionales ordenados por volumen de servicios brindados
void listarProfesionalesPorVolumenServicios() {
    Profesional profesional;
    DetalleTurno detalle;
    int posProfesional = 0;
    int cantidadProfesionales = 0;

    while (profesional.leerDisco(posProfesional)) {
        if (profesional.getEstado()) {
            cantidadProfesionales++;
        }
        posProfesional++;
    }

    if (cantidadProfesionales == 0) {
        cout << "No hay profesionales activos registrados.\n\n";
        return;
    }

    Profesional* vectorProfesionales = new Profesional[cantidadProfesionales];
    int* vectorCantidadServicios = new int[cantidadProfesionales];

    if (vectorProfesionales == NULL || vectorCantidadServicios == NULL) {
        cout << "[ERROR] Memoria insuficiente para generar el listado.\n\n";
        delete[] vectorProfesionales;
        delete[] vectorCantidadServicios;
        return;
    }

    posProfesional = 0;
    int indice = 0;

    while (profesional.leerDisco(posProfesional)) {
        if (profesional.getEstado()) {
            vectorProfesionales[indice] = profesional;

            int cantidadServicios = 0;
            int posDetalle = 0;

            while (detalle.leerDisco(posDetalle)) {
                if (detalle.getEstado() &&
                    detalle.getIdProfesional() == profesional.getIdProfesional()) {
                    cantidadServicios++;
                }
                posDetalle++;
            }

            vectorCantidadServicios[indice] = cantidadServicios;
            indice++;
        }
        posProfesional++;
    }

    for (int i = 0; i < cantidadProfesionales - 1; i++) {
        for (int j = 0; j < cantidadProfesionales - i - 1; j++) {
            if (vectorCantidadServicios[j] < vectorCantidadServicios[j + 1]) {
                int auxCantidad = vectorCantidadServicios[j];
                vectorCantidadServicios[j] = vectorCantidadServicios[j + 1];
                vectorCantidadServicios[j + 1] = auxCantidad;

                Profesional auxProfesional = vectorProfesionales[j];
                vectorProfesionales[j] = vectorProfesionales[j + 1];
                vectorProfesionales[j + 1] = auxProfesional;
            }
        }
    }

    bool hayServicios = false;

    for (int i = 0; i < cantidadProfesionales; i++) {
        if (vectorCantidadServicios[i] > 0) {
            cout << "-----------------------------------" << endl;
            cout << "PROFESIONAL: " << vectorProfesionales[i].getApellido()
                 << ", " << vectorProfesionales[i].getNombre() << endl;
            cout << "ID PROFESIONAL: " << vectorProfesionales[i].getIdProfesional() << endl;
            cout << "SERVICIOS REALIZADOS: " << vectorCantidadServicios[i] << endl;
            cout << "-----------------------------------" << endl;

            hayServicios = true;
        }
    }
    if (!hayServicios) {
        cout << "No hay profesionales con servicios registrados.\n\n";
    }
    delete[] vectorProfesionales;
    delete[] vectorCantidadServicios;
}

// Algoritmo: Filtra la liquidacion cruzando comisiones del mes con asistencias reales
void liquidacionComisiones() {
    int mesBuscado;
    int anioBuscado;

    cout << "Ingrese mes a liquidar (1-12): ";
    cin >> mesBuscado;

    cout << "Ingrese anio a liquidar: ";
    cin >> anioBuscado;

    if (mesBuscado < 1 || mesBuscado > 12) {
        cout << "[ERROR] Mes invalido.\n\n";
        return;
    }

    Profesional profesional;
    DetalleTurno detalle;
    Turno turno;
    int posProfesional = 0;
    bool encontroLiquidaciones = false;

    while (profesional.leerDisco(posProfesional)) {
        if (profesional.getEstado()) {
            float totalProducido = 0;
            int cantidadServicios = 0;
            int posDetalle = 0;

            while (detalle.leerDisco(posDetalle)) {
                if (detalle.getEstado() &&
                    detalle.getIdProfesional() == profesional.getIdProfesional()) {

                    int posTurno = 0;

                    while (turno.leerDisco(posTurno)) {
                        if (turno.getEstado() &&
                            turno.getAsistio() == true && // Valida que la clienta fue al centro
                            turno.getIdTurno() == detalle.getIdTurno()) {

                            Fecha fechaTurno = turno.getFecha();

                            if (fechaTurno.getMes() == mesBuscado &&
                                fechaTurno.getAnio() == anioBuscado) {
                                totalProducido += detalle.getPrecioAlMomento();
                                cantidadServicios++;
                            }
                            break;
                        }
                        posTurno++;
                    }
                }
                posDetalle++;
            }
            if (cantidadServicios > 0) {
                float comision = totalProducido * profesional.getComision() / 100;

                cout << "-----------------------------------" << endl;
                cout << "PROFESIONAL: " << profesional.getApellido()
                     << ", " << profesional.getNombre() << endl;
                cout << "ID PROFESIONAL: " << profesional.getIdProfesional() << endl;
                cout << "MES LIQUIDADO: " << mesBuscado << "/" << anioBuscado << endl;
                cout << "SERVICIOS ASISTIDOS: " << cantidadServicios << endl;
                cout << "TOTAL PRODUCIDO: $" << totalProducido << endl;
                cout << "PORCENTAJE COMISION: " << profesional.getComision() << "%" << endl;
                cout << "COMISION A PAGAR: $" << comision << endl;
                cout << "-----------------------------------" << endl;

                encontroLiquidaciones = true;
            }
        }
        posProfesional++;
    }
    if (!encontroLiquidaciones) {
        cout << "No hay servicios asistidos para liquidar en ese mes.\n\n";
    }
}

// FUNCIONES GLOBALES DE MANTENIMIENTO
void modificarProfesional() {
    int idBuscado;
    Profesional reg;
    int pos = 0;
    bool encontrado = false;

    cout << "=================================================" << endl;
    cout << "          SELECCIONE PROFESIONAL A MODIFICAR     " << endl;
    cout << "=================================================" << endl;
    cout << "0. Volver al Menu Principal / Cancelar Modificacion" << endl;
    cout << "-------------------------------------------------" << endl;

    while (reg.leerDisco(pos)) {
        if (reg.getEstado() == true) {
            cout << "ID: [" << reg.getIdProfesional() << "] - "
                 << reg.getApellido() << ", "
                 << reg.getNombre()
                 << " - Especialidad: " << reg.getEspecialidad()
                 << " - Comision: " << reg.getComision() << "%"
                 << endl;
        }
        pos++;
    }
    cout << "=================================================" << endl;
    cout << "Ingrese el ID del profesional a modificar: ";
    cin >> idBuscado;

    if (idBuscado == 0) {
        cout << "\nOperacion cancelada. Volviendo al menu...\n";
        return;
    }
    pos = 0;
    FILE* p = fopen("profesionales.dat", "rb+");

    if (p == NULL) {
        cout << "\n[ERROR] No se pudo acceder al archivo.\n";
        return;
    }
    while (fread(&reg, sizeof(Profesional), 1, p) == 1) {
        if (reg.getIdProfesional() == idBuscado && reg.getEstado() == true) {
            encontrado = true;

            cout << "\nProfesional encontrado. Reingrese los datos:\n\n";
            cout << "ID PROFESIONAL: " << idBuscado << endl;

            if (!reg.modificar()) {
                cout << "\nModificacion cancelada.\n";
                fclose(p);
                return;
            }

            reg.setIdProfesional(idBuscado);
            reg.setEstado(true);

            fseek(p, pos * sizeof(Profesional), SEEK_SET);
            fwrite(&reg, sizeof(Profesional), 1, p);

            cout << "\n[OK] Profesional modificado correctamente.\n";
            cin.ignore(1000, '\n');
            break;
        }
        pos++;
    }
    fclose(p);
    if (!encontrado) {
        cout << "\n[ERROR] No se encontro ningun profesional activo con ese ID.\n";
    }
}

//BAJA LOGICA
bool darDeBajaProfesional() {
    int idBuscado;
    Profesional reg;
    int pos = 0;
    bool encontrado = false;

    cout << "=================================================" << endl;
    cout << "      SELECCIONE UN PROFESIONAL PARA DAR DE BAJA " << endl;
    cout << "=================================================" << endl;
    cout << " 0. Volver al Menu Principal / Cancelar Baja     " << endl;
    cout << "-------------------------------------------------" << endl;

    while (reg.leerDisco(pos)) {
        if (reg.getEstado() == true) {
            cout << "ID: [" << reg.getIdProfesional() << "] - "
                 << reg.getApellido() << ", "
                 << reg.getNombre() << endl;
        }
        pos++;
    }
    cout << "=================================================" << endl;
    cout << "Ingrese el ID del profesional a dar de baja: ";
    cin >> idBuscado;

    if (idBuscado == 0) {
        return false;
    }

    pos = 0;
    FILE* p = fopen("profesionales.dat", "rb+");
    if (p == NULL) {
        cout << "\n[ERROR] No se pudo acceder al archivo.\n";
        return true;
    }
    while (fread(&reg, sizeof(Profesional), 1, p) == 1) {
        if (reg.getIdProfesional() == idBuscado && reg.getEstado() == true) {
            encontrado = true;
            reg.setEstado(false);
            fseek(p, pos * sizeof(Profesional), SEEK_SET);
            fwrite(&reg, sizeof(Profesional), 1, p);
            cout << "\n[OK] Profesional dado de baja correctamente.\n";
            break;
        }
        pos++;
    }
    fclose(p);
    if (!encontrado) {
        cout << "\n[ERROR] No se encontro ningun profesional activo con ese ID.\n";
    }

    cin.ignore(1000, '\n');
    cout << "\nPresione ENTER para continuar...";
    cin.get();

    return true;
}

// SUB-MENU INTERNO: PANEL DE CONSULTAS Y FILTROS DE STAFF
void menuConsultasProfesionales() {
    int op = 1;
    Profesional aux;
    int pos;

    do {
        cout << "=================================================" << endl;
        cout << "          PANEL DE CONSULTAS Y REPORTES          " << endl;
        cout << "=================================================" << endl;
        cout << "1. Ver Staff de Profesionales Activas" << endl;
        cout << "2. Listar por Especialidad" << endl;
        cout << "3. Listar por Volumen de Servicios" << endl;
        cout << "4. Ver Staff Profesionales Inactivas" << endl;
        cout << "5. Liquidacion de Comisiones" << endl;
        cout << "0. Volver al Menu de Gestion" << endl;
        cout << "-------------------------------------------------" << endl;
        cout << "Seleccione una opcion/filtro: ";
        cin >> op;
        cout << endl;

        system("cls");

        cout << "=================================================" << endl;
        cout << "          PANEL DE CONSULTAS Y REPORTES          " << endl;
        cout << "=================================================" << endl;
        cout << "1. Ver Staff de Profesionales Activas" << endl;
        cout << "2. Listar por Especialidad" << endl;
        cout << "3. Listar por Volumen de Servicios" << endl;
        cout << "4. Ver Staff Profesionales Inactivas" << endl;
        cout << "5. Liquidacion de Comisiones" << endl;
        cout << "0. Volver al Menu de Gestion" << endl;
        cout << "-------------------------------------------------" << endl;
        cout << "Opcion seleccionada: " << op << endl << endl;

        if (op == 1) {
            cout << "=================================================" << endl;
            cout << "            STAFF DE PROFESIONALES ACTIVAS       " << endl;
            cout << "=================================================" << endl;
            pos = 0;
            bool hayActivos = false;
            while (aux.leerDisco(pos)) {
                if (aux.getEstado()) {
                    aux.mostrar();
                    hayActivos = true;
                }
                pos++;
            }
            if (pos == 0) {
                cout << "Archivo vacio.\n\n";
            } else if (!hayActivos) {
                cout << "No hay profesionales activos registrados.\n\n";
            }
            cout << "=================================================" << endl << endl;
        }
        else if (op == 2) {
            listarProfesionalesPorEspecialidad();
        }
        else if (op == 3) {
            cout << "=================================================" << endl;
            cout << "      PROFESIONALES POR VOLUMEN DE SERVICIOS     " << endl;
            cout << "=================================================" << endl << endl;
            listarProfesionalesPorVolumenServicios();
            cout << "=================================================" << endl << endl;
        }
        else if (op == 4) {
            cout << "=================================================" << endl;
            cout << "         LISTADO DE PROFESIONALES INACTIVOS      " << endl;
            cout << "=================================================" << endl;
            listarProfesionalesInactivos();
            cout << "=================================================" << endl << endl;
        }
        else if (op == 5) {
            cout << "=================================================" << endl;
            cout << "               LIQUIDACION DE COMISIONES         " << endl;
            cout << "=================================================" << endl << endl;
            liquidacionComisiones();
            cout << "=================================================" << endl << endl;
        }
        else if (op != 0) {
            cout << "[ERROR] Opcion incorrecta. Reintente.\n\n";
        }

        if (op != 0) {
            cout << "Presione ENTER para refrescar o cambiar de filtro...";
            cin.ignore(1000, '\n');
            cin.get();
            system("cls");
        }

    } while (op != 0);
}

// MENU PRINCIPAL: MODULO DE PROFESIONALES
void menuProfesionales() {
    int op;
    Profesional aux;

    do {
        cout << "=================================================" << endl;
        cout << "          MODULO: GESTION DE PROFESIONALES       " << endl;
        cout << "=================================================" << endl;
        cout << "1. Registrar Profesional" << endl;
        cout << "2. Listado de Profesionales y Consultas" << endl;
        cout << "3. Modificar Datos de Profesional" << endl;
        cout << "4. Dar de Baja Profesional" << endl;
        cout << "0. Volver al Menu Principal" << endl;
        cout << "-------------------------------------------------" << endl;
        cout << "Seleccione una opcion: ";
        cin >> op;
        cout << endl;

        if (op == 1) {
            system("cls");
            if (aux.cargar() == true) {
                if (aux.escribirDisco()) {
                    cout << "\n[OK] Profesional guardado con exito.\n\n";
                } else {
                    cout << "\n[ERROR] No se pudo escribir el archivo.\n\n";
                }
                cin.ignore(1000, '\n');
                cout << "Presione ENTER para continuar...";
                cin.get();
            }
            system("cls");
        }
        else if (op == 2) {
            system("cls");
            menuConsultasProfesionales();
            system("cls");
        }
        else if (op == 3) {
            system("cls");
            modificarProfesional();
            system("cls");
        }
        else if (op == 4) {
            system("cls");
            darDeBajaProfesional();
            system("cls");
        }
    } while (op != 0);

    system("cls");
}
