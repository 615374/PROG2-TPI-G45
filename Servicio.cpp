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
    cout << "ID ASIGNADO AUTOMATICAMENTE: "
         << idServicio << endl;
    cin.ignore(1000, '\n');
    cout << "Ingrese Nombre del Servicio: ";
    cin.getline(nombre, 50);
    cout << "Ingrese Tipo: ";
    cin.getline(tipo, 30);
    //aca le sumo una validacion para que no sea negativo
    do{
    cout << "Ingrese Precio Actual: ";
    cin >> precioActual;

    if(precioActual<=0){
        cout << "Precio invalido.\n";
    }
    }while(precioActual<=0);
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
