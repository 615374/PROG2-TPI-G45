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

// METODOS PRINCIPALES

bool Cliente::cargar() {
    cin.ignore(); // Limpia buffer

    cout << "=================================================" << endl;
    cout << "          REGISTRAR NUEVA CLIENTA                " << endl;
    cout << "=================================================" << endl;
    cout << "0. Volver al Menu Principal / Cancelar alta      " << endl;
    cout << "-------------------------------------------------" << endl;

    // Validacion de Nombre
    do {
        cout << "Ingrese Nombre: ";
        cin.getline(nombre, 50);
        if (strlen(nombre) == 0) {
            cout << "[ERROR] El nombre no puede quedar vacio.\n";
        }
    } while (strlen(nombre) == 0);

    // Si puso '0', marcamos estado en false
    if (strcmp(nombre, "0") == 0) {
        estado = false;
        return false;
    }

    // Validacion de Apellido
    do {
        cout << "Ingrese Apellido: ";
        cin.getline(apellido, 50);
        if (strlen(apellido) == 0) {
            cout << "[ERROR] El apellido no puede quedar vacio.\n";
        }
    } while (strlen(apellido) == 0);

    // Validacion de Telefono
    do {
        cout << "Ingrese Telefono: ";
        cin.getline(telefono, 15);
        if (strlen(telefono) == 0) {
            cout << "[ERROR] El telefono no puede quedar vacio.\n";
        } else if (strlen(telefono) < 6) {
            cout << "[ERROR] Ingrese un numero de telefono valido (minimo 6 digitos).\n";
        }
    } while (strlen(telefono) == 0 || strlen(telefono) < 6);

    idCliente = generarNuevoId();
    cout << "ID ASIGNADO AUTOMATICAMENTE: " << idCliente << endl;

    estado = true;
    return true; // Retorna true si se completó toda la carga correctamente
}

// BUSQUEDA POR ID
bool Cliente::buscarPorId(int id) {
    int pos = 0;
    while (leerDisco(pos)) {
        if (idCliente == id && estado == true) {
            return true;
        }
        pos++;
    }
    return false;
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

//BAJA LOGICA
bool darDeBajaCliente() {
    int idBuscado;
    Cliente reg;
    int pos = 0;
    bool encontrado = false;

    cout << "=================================================" << endl;
    cout << "        SELECCIONE UNA CLIENTA PARA DAR DE BAJA   " << endl;
    cout << "=================================================" << endl;
    cout << " 0. Volver al Menu Principal / Cancelar baja" << endl;
    cout << "-------------------------------------------------" << endl;

    // Recorremos el archivo completo y mostramos las activas
    while (reg.leerDisco(pos)) {
        if (reg.getEstado() == true) {
            cout << " ID: [" << reg.getIdCliente() << "] - "
                 << reg.getApellido() << ", " << reg.getNombre() << endl;
        }
        pos++;
    }
    cout << "=================================================" << endl;
    cout << "Ingrese el ID de la clienta a eliminar: ";
    cin >> idBuscado;

    // Si presiona 0, vuelve al menu
    if (idBuscado == 0) {
        return false;
    }

    // Reiniciamos la posicion para buscar y sobreescribir
    pos = 0;
    FILE* p = fopen("clientes.dat", "rb+");
    if (p == NULL) {
        cout << "\n[ERROR] No se pudo acceder al archivo de clientes.\n";
        return true;
    }

    while (fread(&reg, sizeof(Cliente), 1, p) == 1) {
        if (reg.getIdCliente() == idBuscado && reg.getEstado() == true) {
            encontrado = true;
            reg.setEstado(false);

            fseek(p, pos * sizeof(Cliente), SEEK_SET);
            fwrite(&reg, sizeof(Cliente), 1, p);

            cout << "\n[OK] Clienta dada de baja correctamente del sistema.\n";
            break;
        }
        pos++;
    }

    fclose(p);

    if (!encontrado) {
        cout << "\n[ERROR] No se encontro ninguna clienta activa con el ID: " << idBuscado << endl;
    }

    cin.ignore(1000, '\n');
    cout << "\nPresione ENTER para continuar...";
    cin.get();

    return true;
}
