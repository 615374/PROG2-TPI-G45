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
void Servicio::cargar() {
    idServicio = generarNuevoId();
    cout << "ID SERVICIO ASIGNADO AUTOMATICAMENTE: " << idServicio << endl;
    cin.ignore(); // Tu marca registrada: limpia el buffer antes de las cadenas

    // Validacin de Nombre de Servicio
    do {
        cout << "Ingrese Nombre del Servicio (Ej: Perfilado, Esculpidas): ";
        cin.getline(nombre, 50);
        if (strlen(nombre) == 0) {
            cout << "[ERROR] El nombre del servicio no puede quedar vacio.\n";
        }
    } while (strlen(nombre) == 0);

    // Validacion de Tipo
    do {
        cout << "Ingrese Tipo de Servicio (Ej: Facial, Corporal, Unias): ";
        cin.getline(tipo, 30);
        if (strlen(tipo) == 0) {
            cout << "[ERROR] El tipo de servicio no puede quedar vacio.\n";
        }
    } while (strlen(tipo) == 0);

    // Validacion de Precio (Mayor a cero)
    do {
        cout << "Ingrese Precio Actual: $";
        cin >> precioActual;
        if (precioActual <= 0) {
            cout << "[ERROR] El precio debe ser un valor mayor a cero.\n";
        }
    } while (precioActual <= 0);

    estado = true;
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

//BAJA LOGICA SERVICIO
void darDeBajaServicio() {
    int idBuscado;
    Servicio reg;
    int pos = 0;
    bool encontrado = false;

    cout << "=================================================" << endl;
    cout << "       SELECCIONE UN SERVICIO PARA DAR DE BAJA   " << endl;
    cout << "=================================================" << endl;

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

    pos = 0;
    FILE* p = fopen("servicios.dat", "rb+");
    if (p == NULL) {
        cout << "\n[ERROR] No se pudo acceder al archivo.\n";
        return;
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
}
