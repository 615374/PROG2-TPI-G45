#include <iostream>
#include <cstring>
#include <cstdio>
#include "Profesional.h"

using namespace std;

// CONSTRUCTOR
Profesional::Profesional() {
    idProfesional = 0;
    strcpy(nombre, "");
    strcpy(especialidad, "");
    porcentajeComision = 0;
    estado = false;
}

// METODO AUXILIAR
int Profesional::generarNuevoId() {
    FILE* p = fopen("profesionales.dat", "rb");
    if (p == NULL) {
        return 1;
    }
    fseek(p, 0, SEEK_END);
    long bytesTotales = ftell(p);
    fclose(p);

    int cantidadRegistros = bytesTotales / sizeof(Profesional);

    return cantidadRegistros + 1;
}

// SETTERS
void Profesional::setIdProfesional(int id) {
    idProfesional = id;
}

void Profesional::setNombre(const char* n) {
    int longitudMaxima = 49;
    strncpy(nombre, n, longitudMaxima);
    nombre[longitudMaxima] = '\0';
}

void Profesional::setEspecialidad(const char* e) {
    int longitudMaxima = 49;
    strncpy(especialidad, e, longitudMaxima);
    especialidad[longitudMaxima] = '\0';
}

void Profesional::setComision(float c) {
    porcentajeComision = c;
}

void Profesional::setEstado(bool est) {
    estado = est;
}

// GETTERS
int Profesional::getIdProfesional() {
    return idProfesional;
}

const char* Profesional::getNombre() {
    return nombre;
}

const char* Profesional::getEspecialidad() {
    return especialidad;
}

float Profesional::getComision() {
    return porcentajeComision;
}

bool Profesional::getEstado() {
    return estado;
}

// METODOS PRINCIPALES
void Profesional::cargar() {
    idProfesional = generarNuevoId();
    cout << "ID ASIGNADO AUTOMATICAMENTE: "
         << idProfesional << endl;
    cin.ignore(1000, '\n');
    cout << "Ingrese Nombre: ";
    cin.getline(nombre, 50);
    cout << "Ingrese Especialidad: ";
    cin.getline(especialidad, 50);
    cout << "Ingrese porcentaje de comision: ";
    cin >> porcentajeComision;

    estado = true;
}

void Profesional::mostrar() {
    if (estado) {
        cout << "-----------------------------------" << endl;
        cout << "ID PROFESIONAL: "
             << idProfesional << endl;
        cout << "NOMBRE: "
             << nombre << endl;
        cout << "ESPECIALIDAD: "
             << especialidad << endl;
        cout << "COMISION: "
             << porcentajeComision << "%" << endl;
        cout << "-----------------------------------" << endl;
    }
}

//BUSQUEDA (EXISTE EL ID? TRUE O FALSE)
bool Profesional::buscarPorId(int id){
    int pos = 0;
    while(leerDisco(pos)){
        if(idProfesional == id){
            return true;
        }
        pos++;
    }
    return false;
}

// PERSISTENCIA
bool Profesional::leerDisco(int pos) {
    FILE* p = fopen("profesionales.dat", "rb");
    if (p == NULL) return false;

    fseek(p, pos * sizeof(Profesional), SEEK_SET);
    bool leyo = fread(this, sizeof(Profesional), 1, p);
    fclose(p);

    return leyo;
}

bool Profesional::escribirDisco() {
    FILE* p = fopen("profesionales.dat", "ab");
    if (p == NULL) return false;

    bool escribio = fwrite(this, sizeof(Profesional), 1, p);
    fclose(p);

    return escribio;
}
