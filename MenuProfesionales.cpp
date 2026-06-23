#include <iostream>
#include <cstdlib>
#include <cstring>
#include "Profesional.h"
#include "MenuProfesionales.h"
#include "DetalleTurno.h"
#include "Turno.h"

using namespace std;


/// FUNCIONES GLOBALES DE MANTENIMIENTO
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
        cin.ignore(1000, '\n');
        cout << "\nOperacion cancelada. Volviendo al menu...\n";
        cout << "Presione ENTER para continuar...";
        cin.get();
        return;
    }
    pos = 0;
    FILE* p = fopen("profesionales.dat", "rb+");

    if (p == NULL) {
        cin.ignore(1000, '\n');
        cout << "\n[ERROR] No se pudo acceder al archivo.\n";
        cout << "Presione ENTER para continuar...";
        cin.get();
        return;
    }
    while (fread(&reg, sizeof(Profesional), 1, p) == 1) {
        if (reg.getIdProfesional() == idBuscado && reg.getEstado() == true) {
            encontrado = true;

            cout << "\nProfesional encontrado. Reingrese los datos:\n\n";
            cout << "ID PROFESIONAL: " << idBuscado << endl;

            if (!reg.modificar()) {
                cin.ignore(1000, '\n');
                cout << "\nModificacion cancelada.\n";
                cout << "Presione ENTER para continuar...";
                cin.get();
                fclose(p);
                return;
            }

            reg.setIdProfesional(idBuscado);
            reg.setEstado(true);

            fseek(p, pos * sizeof(Profesional), SEEK_SET);
            fwrite(&reg, sizeof(Profesional), 1, p);

            cin.ignore(1000, '\n');
            cout << "\n[OK] Profesional modificado correctamente.\n";
            cout << "Presione ENTER para continuar...";
            cin.get();
            break;
        }
        pos++;
    }
    fclose(p);
    if (!encontrado) {
        cin.ignore(1000, '\n');
        cout << "\n[ERROR] No se encontro ningun profesional activo con ese ID.\n";
        cout << "Presione ENTER para continuar...";
        cin.get();
    }
}

/// BAJA LOGICA
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

/// Algoritmo: Recorre el archivo mostrando unicamente el staff dado de baja
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

/// Algoritmo: Filtra y muestra los profesionales activos agrupados por especialidad usando memoria dinamica
void listarProfesionalesPorEspecialidad() {
    Profesional reg;
    int pos = 0;
    int totalActivos = 0;

    // Contamos cuantos profesionales activas hay en disco para dimensionar la memoria exacta
    while (reg.leerDisco(pos++)) {
        if (reg.getEstado()) {
            totalActivos++;
        }
    }
    if (totalActivos == 0) {
        cout << "\n[AVISO] No hay profesionales activos cargados en el sistema.\n";
        return;
    }

    // Asignacion Dinamica de memoria para almacenar el staff activo temporalmente
    Profesional* staffActivo = new Profesional[totalActivos];
    if (staffActivo == nullptr) {
        cout << "\n[ERROR] Fallo crítico de memoria dinámica.\n";
        return;
    }

    // Volcamos los registros del disco a nuestro espacio dinamico en memoria
    pos = 0;
    int indice = 0;
    while (reg.leerDisco(pos++)) {
        if (reg.getEstado()) {
            staffActivo[indice] = reg;
            indice++;
        }
    }
    // Extraemos las especialidades de nuestro array dinamico
    // Usamos memoria dinamica tambien para la lista de cadenas de texto
    char (*especialidades)[50] = new char[totalActivos][50];
    int cantidadEspecialidades = 0;

    for (int i = 0; i < totalActivos; i++) {
        bool repetida = false;
        for (int j = 0; j < cantidadEspecialidades; j++) {
            if (strcmp(especialidades[j], staffActivo[i].getEspecialidad()) == 0) {
                repetida = true;
                break;
            }
        }
        if (!repetida) {
            strcpy(especialidades[cantidadEspecialidades], staffActivo[i].getEspecialidad());
            cantidadEspecialidades++;
        }
    }
    system("cls");
    cout << "=================================================" << endl;
    cout << "          ESPECIALIDADES EN EL SISTEMA           " << endl;
    cout << "=================================================" << endl;
    for (int i = 0; i < cantidadEspecialidades; i++) {
        cout << " " << i + 1 << ". " << especialidades[i]       << endl;
    }
    cout << " 0. Volver al menu anterior"                       << endl;
    cout << "-------------------------------------------------" << endl;

    int opcionEspecialidad;
    do {
        cout << "Seleccione una especialidad: ";
        cin >> opcionEspecialidad;
        if (opcionEspecialidad < 0 || opcionEspecialidad > cantidadEspecialidades) {
            cout << "[ERROR] Opcion invalida. Reintente.\n";
        }
    } while (opcionEspecialidad < 0 || opcionEspecialidad > cantidadEspecialidades);

    if (opcionEspecialidad != 0) {
        system("cls");
        char* espSeleccionada = especialidades[opcionEspecialidad - 1];

        cout << "=================================================" << endl;
        cout << "   STAFF DISPONIBLE: " << espSeleccionada          << endl;
        cout << "=================================================" << endl;

        bool encontrado = false;
        for (int i = 0; i < totalActivos; i++) {
            if (strcmp(staffActivo[i].getEspecialidad(), espSeleccionada) == 0) {
                staffActivo[i].mostrar();
                encontrado = true;
            }
        }
        if (!encontrado) {
            cout << "No se encontraron profesionales para la seleccion.\n";
        }
        cout << "=================================================" << endl;
    }

    // Liberacion de memoria dinamica (Evita Memory Leaks)
    delete[] staffActivo;
    delete[] especialidades;
}


/// Algoritmo: Filtrara el listado ordenando por volumen de servicios brindados
void listarProfesionalesPorVolumenServicios() {
    Profesional profReg;
    int posP = 0;
    int totalProfesionalesActivos = 0;

    // Contamos cuantos profesionales activos hay en el sistema para dimensionar la memoria
    while (profReg.leerDisco(posP++)) {
        if (profReg.getEstado()) {
            totalProfesionalesActivos++;
        }
    }

    if (totalProfesionalesActivos == 0) {
        cout << "No hay profesionales activos registrados para analizar.\n\n";
        return;
    }

    // Creamos los arrays dinamicos en memoria
    Profesional* listaProfesionales = new Profesional[totalProfesionalesActivos];
    int* volumenServicios = new int[totalProfesionalesActivos];

    if (listaProfesionales == nullptr || volumenServicios == nullptr) {
        cout << "\n[ERROR] Fallo critico de asignacion de memoria dinamica.\n";
        return;
    }

    // Volcamos los profesionales a la memoria e inicialmente calculamos su volumen recorriendo los desgloses asistidos
    posP = 0;
    int idx = 0;
    while (profReg.leerDisco(posP++)) {
        if (profReg.getEstado()) {
            listaProfesionales[idx] = profReg;
            int contadorServicios = 0;

            DetalleTurno dtReg;
            int posDT = 0;
            while (dtReg.leerDisco(posDT++)) {
                // El detalle debe estar activo y pertenecer al profesional actual
                if (dtReg.getEstado() && dtReg.getIdProfesional() == profReg.getIdProfesional()) {
                    Turno tReg;
                    int posT = 0;
                    while (tReg.leerDisco(posT++)) {
                        // El turno padre debe estar activo y la clienta haber asistido de verdad
                        if (tReg.getEstado() && tReg.getAsistio() && tReg.getIdTurno() == dtReg.getIdTurno()) {
                            contadorServicios++;
                            break;
                        }
                    }
                }
            }
            volumenServicios[idx] = contadorServicios;
            idx++;
        }
    }

    // Algoritmo de ordenamiento de mayor a menor (Burbuja)
    for (int i = 0; i < totalProfesionalesActivos - 1; i++) {
        for (int j = 0; j < totalProfesionalesActivos - i - 1; j++) {
            if (volumenServicios[j] < volumenServicios[j + 1]) {
                // Intercambio de volumen
                int vAux = volumenServicios[j];
                volumenServicios[j] = volumenServicios[j + 1];
                volumenServicios[j + 1] = vAux;

                // Intercambio paralelo de objetos de Profesionales
                Profesional pAux = listaProfesionales[j];
                listaProfesionales[j] = listaProfesionales[j + 1];
                listaProfesionales[j + 1] = pAux;
            }
        }
    }

    // Renderizado del reporte por pantalla
    cout << "========================================================================" << endl;
    cout << "             RANKING DE PROFESIONALES POR VOLUMEN DE SERVICIOS          " << endl;
    cout << "========================================================================" << endl;
    cout << " PUESTO | ID  | PROFESIONAL                  | TOTAL SERVICIOS BRINDADOS" << endl;
    cout << "------------------------------------------------------------------------" << endl;

    for (int i = 0; i < totalProfesionalesActivos; i++) {
        char nombreCompleto[50];
        strcpy(nombreCompleto, listaProfesionales[i].getApellido());
        strcat(nombreCompleto, ", ");
        strcat(nombreCompleto, listaProfesionales[i].getNombre());

        if (strlen(nombreCompleto) > 26) {
            nombreCompleto[23] = '.'; nombreCompleto[24] = '.'; nombreCompleto[25] = '.'; nombreCompleto[26] = '\0';
        }

        cout << "   #" << (i + 1) << "   | "
             << (listaProfesionales[i].getIdProfesional() < 10 ? "0" : "") << listaProfesionales[i].getIdProfesional() << "  | "
             << nombreCompleto;

        for (int e = strlen(nombreCompleto); e < 28; e++) cout << " ";
        cout << " | " << volumenServicios[i] << " servicios" << endl;
    }
    cout << "========================================================================\n" << endl;

    // Liberacion obligatoria de memoria dinamica
    delete[] listaProfesionales;
    delete[] volumenServicios;
}


/// Algoritmo: Filtra la liquidacion de comisiones (menusal / semanal)
void liquidacionComisiones() {
    int mesBuscado, anioBuscado, opcionPeriodo;
    int diaInicio = 1, diaFin = 31; // Rangos por defecto para mes completo
    int semanaBuscada = 0;

    cout << "=================================================" << endl;
    cout << "        LIQUIDACION MENSUAL DE COMISIONES        " << endl;
    cout << "=================================================" << endl;
    cout << " 1. Liquidar Mes Completo" << endl;
    cout << " 2. Liquidar por Semana Especifica" << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "Seleccione modalidad: ";
    cin >> opcionPeriodo;

    if (opcionPeriodo != 1 && opcionPeriodo != 2) {
        cout << "\n[ERROR] Opcion invalida.\n\n";
        return;
    }

    cout << "\nIngrese mes a liquidar (1-12): ";
    cin >> mesBuscado;

    cout << "Ingrese anio a liquidar: ";
    cin >> anioBuscado;

    if (mesBuscado < 1 || mesBuscado > 12 || anioBuscado < 2025) {
        cout << "\n[ERROR] Periodo invalido.\n\n";
        return;
    }

    // Si es por semana, configuramos el rango de dias del mes
    if (opcionPeriodo == 2) {
        cout << "Ingrese numero de semana (1 a 4): ";
        cin >> semanaBuscada;

        if (semanaBuscada < 1 || semanaBuscada > 4) {
            cout << "\n[ERROR] Semana invalida.\n\n";
            return;
        }

        // Mapeo de los bloques de dias
        if (semanaBuscada == 1) { diaInicio = 1;  diaFin = 7;  }
        else if (semanaBuscada == 2) { diaInicio = 8;  diaFin = 14; }
        else if (semanaBuscada == 3) { diaInicio = 15; diaFin = 21; }
        else if (semanaBuscada == 4) { diaInicio = 22; diaFin = 31; } // Cubre hasta fin de mes
    }

    Profesional profesional;
    int posProfesional = 0;
    bool encontroLiquidaciones = false;

    system("cls");
    cout << "========================================================================" << endl;
    if (opcionPeriodo == 1) {
        cout << "        ORDEN DE LIQUIDACION MENSUAL EMITIDA EN PERIODO " << (mesBuscado < 10 ? "0" : "") << mesBuscado << "/" << anioBuscado << endl;
    } else {
        cout << "    ORDEN DE LIQUIDACION EMITIDA: SEMANA " << semanaBuscada << " DEL MES " << (mesBuscado < 10 ? "0" : "") << mesBuscado << "/" << anioBuscado << endl;
    }
    cout << "========================================================================" << endl;

    while (profesional.leerDisco(posProfesional)) {
        if (profesional.getEstado()) {
            float totalProducido = 0;
            int cantidadServicios = 0;

            DetalleTurno detalle;
            int posDetalle = 0;

            while (detalle.leerDisco(posDetalle)) {
                if (detalle.getEstado() && detalle.getIdProfesional() == profesional.getIdProfesional()) {
                    Turno turno;
                    int posTurno = 0;

                    while (turno.leerDisco(posTurno)) {
                        if (turno.getEstado() && turno.getAsistio() && turno.getIdTurno() == detalle.getIdTurno()) {
                            Fecha fechaTurno = turno.getFecha();

                            // Filtrado cruzado: anio + mes + rango de dias (semanal o mensual)
                            if (fechaTurno.getAnio() == anioBuscado &&
                                fechaTurno.getMes() == mesBuscado &&
                                fechaTurno.getDia() >= diaInicio &&
                                fechaTurno.getDia() <= diaFin) {

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

                cout << " PROFESIONAL:     " << profesional.getApellido() << ", " << profesional.getNombre()
                     << " [ID: " << profesional.getIdProfesional() << "]" << endl;
                cout << " SERV. BRINDADOS: " << cantidadServicios << endl;
                cout << " FACTURACION:     $" << totalProducido << endl;
                cout << " COMISION (%):    " << profesional.getComision() << "%" << endl;
                cout << " NETO A LIQUIDAR: $" << comision << endl;
                cout << "------------------------------------------------------------------------" << endl;

                encontroLiquidaciones = true;
            }
        }
        posProfesional++;
    }

    if (!encontroLiquidaciones) {
        cout << "\n No se registraron servicios asistidos para liquidar en este rango temporal.\n";
    }
    cout << "========================================================================\n" << endl;
}



/// SUB-MENU INTERNO: PANEL DE CONSULTAS Y FILTROS DE STAFF
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
            system("cls");
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
            system("cls");
            cout << "=================================================" << endl;
            cout << "      PROFESIONALES POR VOLUMEN DE SERVICIOS     " << endl;
            cout << "=================================================" << endl << endl;
            listarProfesionalesPorVolumenServicios();
            cout << "=================================================" << endl << endl;
        }
        else if (op == 4) {
            system("cls");
            cout << "=================================================" << endl;
            cout << "         LISTADO DE PROFESIONALES INACTIVOS      " << endl;
            cout << "=================================================" << endl;
            listarProfesionalesInactivos();
            cout << "=================================================" << endl << endl;
        }
        else if (op == 5) {
            system("cls");
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
            cout << "Presione ENTER para continuar...";
            cin.ignore(1000, '\n');
            cin.get();
            system("cls");
        }
    } while (op != 0);
  system("cls");
}

/// MENU PRINCIPAL: MODULO DE PROFESIONALES
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
