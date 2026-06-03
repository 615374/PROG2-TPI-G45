#include <iostream>
#include <cstring>
#include <cstdio>
#include "Cliente.h"

using namespace std;

// CONSTRUCTOR
Cliente::Cliente() {
    idCliente = 0;
    strcpy(nombre, "");
    strcpy(apellido, "");
    strcpy(telefono, "");
    estado = false;
}

// METODO AUXILIAR
// Simula el autoincrement contando registros en el .dat
int Cliente::generarNuevoId() {
    FILE* p = fopen("clientes.dat", "rb");
    if (p == NULL) {
        return 1;
    }

    fseek(p, 0, SEEK_END);
    long bytesTotales = ftell(p);
    fclose(p);

    int cantidadRegistros = bytesTotales / sizeof(Cliente);

    return cantidadRegistros + 1;
}

// SETTERS
void Cliente::setIdCliente(int id) {
    idCliente = id;
}

void Cliente::setNombre(const char* n) {
    int longitud_maxima = 49;
    strncpy(nombre, n, longitud_maxima);
    nombre[longitud_maxima] = '\0';
}

void Cliente::setApellido(const char* a) {
    int longitud_maxima = 49;
    strncpy(apellido, a, longitud_maxima);
    apellido[longitud_maxima] = '\0';
}

void Cliente::setTelefono(const char* tel) {
    int longitud_maxima = 14;
    strncpy(telefono, tel, longitud_maxima);
    telefono[longitud_maxima] = '\0';
}

void Cliente::setEstado(bool e) {
    estado = e;
}

// GETTERS
int Cliente::getIdCliente() {
    return idCliente;
}

const char* Cliente::getNombre() {
    return nombre;
}

const char* Cliente::getApellido() {
    return apellido;
}

bool Cliente::getEstado() {
    return estado;
}

// METODOS PRINCIPALES DE LA CLASE
void Cliente::cargar() {
    // El sistema calcula y asigna el ID automáticamente
    idCliente = generarNuevoId();

    cout << "ID ASIGNADO AUTOMATICAMENTE: " << idCliente << endl;
    cin.ignore(); // Limpia el buffer del teclado

    cout << "Ingrese Nombre: ";
    cin.getline(nombre, 50);

    cout << "Ingrese Apellido: ";
    cin.getline(apellido, 50);

    cout << "Ingrese Telefono: ";
    cin.getline(telefono, 15);

    estado = true; // Nace activa en el sistema
}


void Cliente::mostrar() {
    if (estado) {
        cout << "------------------------------------------" << endl;
        cout << "ID CLIENTA: " << idCliente << endl;
        cout << "APELLIDO Y NOMBRE: " << apellido << ", " << nombre << endl;
        cout << "TELEFONO: " << telefono << endl;
        cout << "------------------------------------------" << endl;
    }
}

// PERSISTENCIA EN DISCO
bool Cliente::leerDisco(int pos) {
    FILE* p = fopen("clientes.dat", "rb");
    if (p == NULL) return false;

    fseek(p, pos * sizeof(Cliente), SEEK_SET);
    bool leyo = fread(this, sizeof(Cliente), 1, p);

    fclose(p);
    return leyo;
}

bool Cliente::escribirDisco() {
    FILE* p = fopen("clientes.dat", "ab");
    if (p == NULL) return false;

    bool escribio = fwrite(this, sizeof(Cliente), 1, p);

    fclose(p);
    return escribio;
}
