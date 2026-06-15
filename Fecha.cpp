#include <iostream>
#include "Fecha.h"

using namespace std;

// CONSTRUCTOR
Fecha::Fecha() {
    dia = 1;
    mes = 1;
    anio = 2026;
}

// SETTERS
void Fecha::setDia(int d) {
    dia = d;
}

void Fecha::setMes(int m) {
    mes = m;
}

void Fecha::setAnio(int a) {
    anio = a;
}

// GETTERS
int Fecha::getDia() {
    return dia;
}

int Fecha::getMes() {
    return mes;
}

int Fecha::getAnio() {
    return anio;
}

// VALIDACION GENERAL
bool Fecha::validar() {
    if (anio < 2025) return false;
    if (mes < 1 || mes > 12) return false;
    if (dia < 1 || dia > 31) return false;
    switch (mes) {
        case 4:
        case 6:
        case 9:
        case 11:
            if (dia > 30) return false;
            break;
        case 2:
            if (esBisiesto()) {
                if (dia > 29) return false;
            }
            else {
                if (dia > 28) return false;
            }
            break;
        default:
            if (dia > 31) return false;
    }
    return true;
}

// VALIDACION DE AÑO BISIESTO
bool Fecha::esBisiesto() {
    return (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
}

// METODOS PRINCIPALES
bool Fecha::cargar() {
    do {
        cout << "Ingrese dia (0 para cancelar): ";
        cin >> dia;
        if (dia == 0) {
            return false;
        }
        cout << "Ingrese mes: ";
        cin >> mes;

        cout << "Ingrese anio: ";
        cin >> anio;
        if (!validar()) {
            cout << "\nFecha invalida. Reingrese.\n\n";
        }
    } while (!validar());
    return true;
}

void Fecha::mostrar() {
    getFechaFormateada();
    cout << endl;
}

//MOSTRAMOS FECHA FORMATEADA
void Fecha::getFechaFormateada() {
    cout << dia << "/" << mes << "/" << anio;
}
