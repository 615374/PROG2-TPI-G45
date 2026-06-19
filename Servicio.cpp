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

// METODO CARGAR
bool Servicio::cargar() {
    cin.ignore(1000, '\n'); // Limpia buffer

    cout << "=================================================" << endl;
    cout << "          REGISTRAR NUEVO SERVICIO               " << endl;
    cout << "=================================================" << endl;
    cout << "0. Volver al Menu Principal / Cancelar alta      " << endl;
    cout << "-------------------------------------------------" << endl;

    bool valido;

    // Validacion de nombre
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
        // Control para que no contenga numeros
        for (int i = 0; nombre[i] != '\0'; i++) {
            if (nombre[i] >= '0' && nombre[i] <= '9') {
                valido = false;
            }
        }
        if (!valido && strlen(nombre) > 0) {
            cout << "[ERROR] El nombre del servicio no puede contener numeros.\n";
        }
    } while (!valido);

    // Validacion de tipo
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
        // Control de caracteres numericos
        for (int i = 0; tipo[i] != '\0'; i++) {
            if (tipo[i] >= '0' && tipo[i] <= '9') {
                valido = false;
            }
        }
        if (!valido && strlen(tipo) > 0) {
            cout << "[ERROR] El tipo de servicio no puede contener numeros.\n";
        }
    } while (!valido);

    // Validacion de precio
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
    cin.ignore(1000, '\n'); // Limpia buffer
    bool valido;

    // Validacion de nombre
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
        // Control para que no contenga numeros en el nombre
        for (int i = 0; nombre[i] != '\0'; i++) {
            if (nombre[i] >= '0' && nombre[i] <= '9') {
                valido = false;
            }
        }
        if (!valido && strlen(nombre) > 0) {
            cout << "[ERROR] El nombre del servicio no puede contener numeros.\n";
        }
    } while (!valido);

    // Validacion de tipo
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
        // Control para que no contenga numeros en el tipo
        for (int i = 0; tipo[i] != '\0'; i++) {
            if (tipo[i] >= '0' && tipo[i] <= '9') {
                valido = false;
            }
        }
        if (!valido && strlen(tipo) > 0) {
            cout << "[ERROR] El tipo de servicio no puede contener numeros.\n";
        }
    } while (!valido);

    // Validacion de precio
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
        //Porque no alcanza con que el registro exista fisicamente
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

// PERSISTENCIA
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

