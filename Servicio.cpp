#include <iostream>
#include <cstring>
#include <cstdio>
#include "Servicio.h"

using namespace std;

// CONSTRUCTOR
Servicio::Servicio() {
    idServicio = 0;
    strcpy(nombre, "");
    strcpy(tipo, "");
    precioActual = 0;

    estado = false;
}

// METODO AUXILIAR
int Servicio::generarNuevoId() {
    FILE* p = fopen("servicios.dat", "rb");
    if (p == NULL) {
        return 1;
    }

    fseek(p, 0, SEEK_END);
    long bytesTotales = ftell(p);
    fclose(p);

    int cantidadRegistros = bytesTotales / sizeof(Servicio);

    return cantidadRegistros + 1;
}

// SETTERS
void Servicio::setIdServicio(int id) {
    idServicio = id;
}

void Servicio::setNombre(const char* n) {
    int longitudMaxima = 49;
    strncpy(nombre, n, longitudMaxima);
    nombre[longitudMaxima] = '\0';
}

void Servicio::setTipo(const char* t) {
    int longitudMaxima = 29;
    strncpy(tipo, t, longitudMaxima);
    tipo[longitudMaxima] = '\0';
}

void Servicio::setPrecio(float p) {
    precioActual = p;
}

void Servicio::setEstado(bool e) {
    estado = e;
}

// GETTERS
int Servicio::getIdServicio() {
    return idServicio;
}

const char* Servicio::getNombre() {
    return nombre;
}

const char* Servicio::getTipo() {
    return tipo;
}

float Servicio::getPrecio() {
    return precioActual;
}

bool Servicio::getEstado() {
    return estado;
}

// METODOS PRINCIPALES
bool Servicio::cargar() {
    cin.ignore(1000, '\n');

    cout << "=================================================" << endl;
    cout << "          REGISTRAR NUEVO SERVICIO               " << endl;
    cout << "=================================================" << endl;
    cout << "0. Volver al Menu Principal / Cancelar alta      " << endl;
    cout << "-------------------------------------------------" << endl;

    bool valido;

    // VALIDACION DE NOMBRE
    do {
        valido = true;

        cout << "Ingrese Nombre del Servicio: ";
        cin.getline(nombre, 50);

        if (strcmp(nombre, "0") == 0) {
            estado = false;
            return false;
        }
        if (strlen(nombre) == 0) {
            cout << "[ERROR] El nombre del servicio no puede quedar vacio.\n";
            valido = false;
        }
    } while (!valido);

    // VALIDACION DE TIPO
    do {
        valido = true;

        cout << "Ingrese Tipo de Servicio: ";
        cin.getline(tipo, 30);
        if (strcmp(tipo, "0") == 0) {
            estado = false;
            return false;
        }
        if (strlen(tipo) == 0) {
            cout << "[ERROR] El tipo de servicio no puede quedar vacio.\n";
            valido = false;
        }
        for (int i = 0; tipo[i] != '\0'; i++) {
            if (tipo[i] >= '0' && tipo[i] <= '9') {
                valido = false;
            }
        }
        if (!valido && strlen(tipo) > 0) {
            cout << "[ERROR] El tipo de servicio no puede contener numeros.\n";
        }

    } while (!valido);

    // VALIDACION DE PRECIO
    do {
        cout << "Ingrese Precio Actual: ";
        cin >> precioActual;
        if (precioActual <= 0) {
            cout << "[ERROR] El precio debe ser mayor a cero.\n";
        }

    } while (precioActual <= 0);
    idServicio = generarNuevoId();
    cout << "ID SERVICIO ASIGNADO AUTOMATICAMENTE: " << idServicio << endl;

    estado = true;
    return true;
}

void Servicio::mostrar() {
    if (estado) {
        cout << "-----------------------------------" << endl;
        cout << "ID SERVICIO: "
             << idServicio << endl;
        cout << "NOMBRE: "
             << nombre << endl;
        cout << "TIPO: "
             << tipo << endl;
        cout << "PRECIO: $"
             << precioActual << endl;
        cout << "-----------------------------------" << endl;
    }
}

//METODO MODIFICAR
bool Servicio::modificar() {
    cin.ignore(1000, '\n');

    bool valido;

    // VALIDACION DE NOMBRE EN MODIFICAR
    do {
        valido = true;

        cout << "Ingrese Nombre del Servicio: ";
        cin.getline(nombre, 50);

        if (strcmp(nombre, "0") == 0) {
            return false;
        }
        if (strlen(nombre) == 0) {
            cout << "[ERROR] El nombre del servicio no puede quedar vacio.\n";
            valido = false;
        }
    } while (!valido);

    // VALIDACION DE TIPO EN MODIFICAR
    do {
        valido = true;

        cout << "Ingrese Tipo de Servicio: ";
        cin.getline(tipo, 30);

        if (strcmp(tipo, "0") == 0) {
            return false;
        }
        if (strlen(tipo) == 0) {
            cout << "[ERROR] El tipo de servicio no puede quedar vacio.\n";
            valido = false;
        }
        for (int i = 0; tipo[i] != '\0'; i++) {
            if (tipo[i] >= '0' && tipo[i] <= '9') {
                valido = false;
            }
        }
        if (!valido && strlen(tipo) > 0) {
            cout << "[ERROR] El tipo de servicio no puede contener numeros.\n";
        }

    } while (!valido);

    // VALIDACION DE PRECIO EN MODIFICAR
    do {
        cout << "Ingrese Precio Actual: ";
        cin >> precioActual;

        if (precioActual <= 0) {
            cout << "[ERROR] El precio debe ser mayor a cero.\n";
        }
    } while (precioActual <= 0);

    return true;
}

//BUSQUEDA (EXISTE EL ID? TRUE O FALSE)
bool Servicio::buscarPorId(int id){
    int pos = 0;
    while(leerDisco(pos)){
        if(idServicio == id && estado == true){
        //Porque no alcanza con que el registro exista físicamente
        //en el archivo, También debe estar activo para
        //poder utilizarse dentro del sistema
            return true;
        }
        pos++;
    }
    return false;
}

//Metodo para listar luego la relacion profesional-servicio
bool Servicio::mostrarNombrePorId(int id) {
    int pos = 0;

    while (leerDisco(pos)) {
        if (idServicio == id && estado == true) {
            cout << "[" << idServicio << "] " << nombre;
            return true;
        }
        pos++;
    }
    return false;
}

// PERSISTENCIA/MATENIMIENTO
bool Servicio::leerDisco(int pos) {
    FILE* p = fopen("servicios.dat", "rb");
    if (p == NULL) return false;

    fseek(p, pos * sizeof(Servicio), SEEK_SET);
    bool leyo = fread(this, sizeof(Servicio), 1, p);
    fclose(p);

    return leyo;
}

bool Servicio::escribirDisco() {
    FILE* p = fopen("servicios.dat", "ab");
    if (p == NULL) return false;

    bool escribio = fwrite(this, sizeof(Servicio), 1, p);
    fclose(p);
    return escribio;
}

      //FUNCIONES DE LISTADO
//LISTAR SERVICIOS POR TIPO
void listarServiciosPorTipo() {
    char tipos[100][30];
    int cantidadTipos = 0;
    int opcionTipo;

    Servicio reg;
    int pos = 0;
    bool repetido;
    bool encontrado = false;

    cout << "=================================================" << endl;
    cout << "             LISTADO DE SERVICIOS POR TIPO       " << endl;
    cout << "=================================================" << endl;

    while (reg.leerDisco(pos)) {
        if (reg.getEstado()) {
            repetido = false;

            for (int i = 0; i < cantidadTipos; i++) {
                if (strcmp(tipos[i], reg.getTipo()) == 0) {
                    repetido = true;
                }
            }
            if (!repetido) {
                strcpy(tipos[cantidadTipos], reg.getTipo());
                cantidadTipos++;
            }
        }
        pos++;
    }
    if (cantidadTipos == 0) {
        cout << "No hay tipos disponibles porque no hay servicios activos.\n";
        return;
    }
    cout << "\nTIPOS DISPONIBLES:" << endl;

    for (int i = 0; i < cantidadTipos; i++) {
        cout << i + 1 << ". " << tipos[i] << endl;
    }
    cout << "0. Volver al menu" << endl;
    cout << "-------------------------------------------------" << endl;

    do {
        cout << "Seleccione un tipo: ";
        cin >> opcionTipo;

        if (opcionTipo < 0 || opcionTipo > cantidadTipos) {
            cout << "[ERROR] Opcion invalida.\n";
        }

    } while (opcionTipo < 0 || opcionTipo > cantidadTipos);

    cin.ignore(1000, '\n');

    if (opcionTipo == 0) {
        cout << "\nVolviendo al menu...\n";
        return;
    }
    pos = 0;

    cout << endl;
    cout << "=== SERVICIOS DEL TIPO: " << tipos[opcionTipo - 1] << " ===" << endl;

    while (reg.leerDisco(pos)) {
        if (reg.getEstado() == true &&
            strcmp(reg.getTipo(), tipos[opcionTipo - 1]) == 0) {

            reg.mostrar();
            encontrado = true;
        }
        pos++;
    }
    if (!encontrado) {
        cout << "No se encontraron servicios activos con ese tipo.\n";
    }
}

//LISTAR SERVICIOS INACTIVOS
void listarServiciosInactivos() {
    Servicio reg;
    int pos = 0;
    bool hayInactivos = false;

    cout << "=================================================" << endl;
    cout << "             LISTADO DE SERVICIOS INACTIVOS      " << endl;
    cout << "=================================================" << endl;

    while (reg.leerDisco(pos)) {
        if (reg.getEstado() == false) {
            cout << "-----------------------------------" << endl;
            cout << "ID SERVICIO: " << reg.getIdServicio() << endl;
            cout << "NOMBRE: " << reg.getNombre() << endl;
            cout << "TIPO: " << reg.getTipo() << endl;
            cout << "PRECIO: $" << reg.getPrecio() << endl;
            cout << "-----------------------------------" << endl;

            hayInactivos = true;
        }
        pos++;
    }
    if (!hayInactivos) {
        cout << "No hay servicios inactivos para mostrar." << endl;
    }
}

//FUNCION MODIFICAR SERVICIO
void modificarServicio() {
    int idBuscado;
    Servicio reg;
    int pos = 0;
    bool encontrado = false;

    cout << "=================================================" << endl;
    cout << "             SELECCIONE SERVICIO A MODIFICAR     " << endl;
    cout << "=================================================" << endl;
    cout << "0. Volver al Menu Principal / Cancelar Modificacion" << endl;
    cout << "-------------------------------------------------" << endl;

    while (reg.leerDisco(pos)) {
        if (reg.getEstado() == true) {
            cout << "ID: [" << reg.getIdServicio() << "] - "
                 << reg.getNombre()
                 << " - Tipo: " << reg.getTipo()
                 << " - Precio: $" << reg.getPrecio()
                 << endl;
        }
        pos++;
    }
    cout << "=================================================" << endl;
    cout << "Ingrese el ID del servicio a modificar: ";
    cin >> idBuscado;

    if (idBuscado == 0) {
        cout << "\nOperacion cancelada. Volviendo al menu...\n";
        return;
    }
    pos = 0;
    FILE* p = fopen("servicios.dat", "rb+");

    if (p == NULL) {
        cout << "\n[ERROR] No se pudo acceder al archivo.\n";
        return;
    }
    while (fread(&reg, sizeof(Servicio), 1, p) == 1) {
        if (reg.getIdServicio() == idBuscado && reg.getEstado() == true) {
            encontrado = true;

            cout << "\nServicio encontrado. Reingrese los datos:\n\n";
            cout << "ID SERVICIO: " << idBuscado << endl;

            if (!reg.modificar()) {
                cout << "\nModificacion cancelada.\n";
                fclose(p);
                return;
            }
            reg.setIdServicio(idBuscado);
            reg.setEstado(true);

            fseek(p, pos * sizeof(Servicio), SEEK_SET);
            fwrite(&reg, sizeof(Servicio), 1, p);
            cout << "\n[OK] Servicio modificado correctamente.\n";
            cin.ignore(1000, '\n');
            break;
        }
        pos++;
    }
    fclose(p);
    if (!encontrado) {
        cout << "\n[ERROR] No se encontro ningun servicio activo con ese ID.\n";
    }
}

//FUNCION DAR DE BAJA
bool darDeBajaServicio() {
    int idBuscado;
    Servicio reg;
    int pos = 0;
    bool encontrado = false;

    cout << "=================================================" << endl;
    cout << "       SELECCIONE UN SERVICIO PARA DAR DE BAJA   " << endl;
    cout << "=================================================" << endl;
    cout << "0. Volver al Menu Principal / Cancelar Baja      " << endl;
    cout << "-------------------------------------------------" << endl;

    while (reg.leerDisco(pos)) {
        if (reg.getEstado() == true) {
            cout << "ID: [" << reg.getIdServicio() << "] - "
                 << reg.getNombre()
                 << " ($" << reg.getPrecio() << ")"
                 << endl;
        }
        pos++;
    }

    cout << "=================================================" << endl;
    cout << "Ingrese el ID del servicio a dar de baja: ";
    cin >> idBuscado;

    if (idBuscado == 0) {
        cout << "\nOperacion cancelada. Volviendo al menu...\n";
        return false;
    }
    pos = 0;
    FILE* p = fopen("servicios.dat", "rb+");

    if (p == NULL) {
        cout << "\n[ERROR] No se pudo acceder al archivo.\n";
        return true;
    }
    while (fread(&reg, sizeof(Servicio), 1, p) == 1) {
        if (reg.getIdServicio() == idBuscado &&
            reg.getEstado() == true) {

            encontrado = true;
            reg.setEstado(false);

            fseek(p, pos * sizeof(Servicio), SEEK_SET);
            fwrite(&reg, sizeof(Servicio), 1, p);

            cout << "\n[OK] Servicio dado de baja correctamente.\n";
            break;
        }
        pos++;
    }
    fclose(p);
    if (!encontrado) {
        cout << "\n[ERROR] No se encontro ningun servicio activo con ese ID.\n";
    }

    return true;
}
