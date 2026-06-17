#include <iostream>
#include <cstring>
#include <cstdio>
#include "Profesional.h"

using namespace std;

// CONSTRUCTOR
Profesional::Profesional() {
    idProfesional = 0;
    strcpy(nombre, "");
    strcpy(apellido, "");
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

void Profesional::setApellido(const char* a) {
    int longitudMaxima = 49;
    strncpy(apellido, a, longitudMaxima);
    apellido[longitudMaxima] = '\0';
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

const char* Profesional::getApellido() {
    return apellido;
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

// METODOS PRINCIPALES DE LA CLASE

// METODO CARGAR
bool Profesional::cargar() {
    cin.ignore(1000, '\n'); // Limpieza preventiva del buffer

    cout << "=================================================" << endl;
    cout << "           REGISTRAR NUEVA PROFESIONAL           " << endl;
    cout << "=================================================" << endl;
    cout << "0. Volver al Menu Principal / Cancelar alta      " << endl;
    cout << "-------------------------------------------------" << endl;

    bool valido;

    // Validacion de nombre
    do {
        valido = true;
        cout << "Ingrese Nombre: ";
        cin.getline(nombre, 50);

        // Escape inmediato si cancela
        if (strcmp(nombre, "0") == 0) {
            estado = false;
            return false;
        }
        if (strlen(nombre) == 0) {
            cout << "[ERROR] El nombre no puede quedar vacio.\n";
            valido = false;
        }
        // Control de caracteres numericos
        for (int i = 0; nombre[i] != '\0'; i++) {
            if (nombre[i] >= '0' && nombre[i] <= '9') {
                valido = false;
            }
        }
        if (!valido && strlen(nombre) > 0) {
            cout << "[ERROR] El nombre no puede contener numeros.\n";
        }
    } while (!valido);


    // Validacion de apellido
    do {
        valido = true;
        cout << "Ingrese Apellido: ";
        cin.getline(apellido, 50);

        // Escape inmediato si cancela
        if (strcmp(apellido, "0") == 0) {
            estado = false;
            return false;
        }
        if (strlen(apellido) == 0) {
            cout << "[ERROR] El apellido no puede quedar vacio.\n";
            valido = false;
        }
        // Control de caracteres numericos
        for (int i = 0; apellido[i] != '\0'; i++) {
            if (apellido[i] >= '0' && apellido[i] <= '9') {
                valido = false;
            }
        }
        if (!valido && strlen(apellido) > 0) {
            cout << "[ERROR] El apellido no puede contener numeros.\n";
        }
    } while (!valido);


    // Validacion de especialidad
    do {
        valido = true;
        cout << "Ingrese Especialidad: ";
        cin.getline(especialidad, 50);

        // Escape inmediato si cancela
        if (strcmp(especialidad, "0") == 0) {
            estado = false;
            return false;
        }
        if (strlen(especialidad) == 0) {
            cout << "[ERROR] La especialidad no puede quedar vacia.\n";
            valido = false;
        }
        // Control de caracteres numericos
        for (int i = 0; especialidad[i] != '\0'; i++) {
            if (especialidad[i] >= '0' && especialidad[i] <= '9') {
                valido = false;
            }
        }
        if (!valido && strlen(especialidad) > 0) {
            cout << "[ERROR] La especialidad no puede contener numeros.\n";
        }
    } while (!valido);


    // Validacion de porcentaje de comision
    do {
        cout << "Ingrese Porcentaje de Comision (0-100): ";
        cin >> porcentajeComision;
        if (porcentajeComision < 0 || porcentajeComision > 100) {
            cout << "[ERROR] El porcentaje debe estar entre 0 y 100.\n";
        }
    } while (porcentajeComision < 0 || porcentajeComision > 100);

    idProfesional = generarNuevoId();
    cout << "ID PROFESIONAL ASIGNADO AUTOMATICAMENTE: " << idProfesional << endl;

    estado = true;
    return true;
}

// METODO MOSTRAR
void Profesional::mostrar() {
    if (estado) {
        cout << "-----------------------------------" << endl;
        cout << "ID PROFESIONAL: " << idProfesional << endl;
        cout << "NOMBRE: " << nombre << endl;
        cout << "APELLIDO: " << apellido << endl;
        cout << "ESPECIALIDAD: " << especialidad << endl;
        cout << "COMISION: " << porcentajeComision << "%" << endl;
        cout << "-----------------------------------" << endl;
    }
}

// METODO MODIFICAR
bool Profesional::modificar() {
    cin.ignore(1000, '\n');
    bool valido;

    // Validacion de nombre
    do {
        valido = true;
        cout << "Ingrese Nombre: ";
        cin.getline(nombre, 50);

        if (strcmp(nombre, "0") == 0) {
            return false;
        }
        if (strlen(nombre) == 0) {
            cout << "[ERROR] El nombre no puede quedar vacio.\n";
            valido = false;
        }
        for (int i = 0; nombre[i] != '\0'; i++) {
            if (nombre[i] >= '0' && nombre[i] <= '9') {
                valido = false;
            }
        }
        if (!valido && strlen(nombre) > 0) {
            cout << "[ERROR] El nombre no puede contener numeros.\n";
        }
    } while (!valido);

    // Validacion de apellido
    do {
        valido = true;
        cout << "Ingrese Apellido: ";
        cin.getline(apellido, 50);

        if (strcmp(apellido, "0") == 0) {
            return false;
        }
        if (strlen(apellido) == 0) {
            cout << "[ERROR] El apellido no puede quedar vacio.\n";
            valido = false;
        }
        for (int i = 0; apellido[i] != '\0'; i++) {
            if (apellido[i] >= '0' && apellido[i] <= '9') {
                valido = false;
            }
        }
        if (!valido && strlen(apellido) > 0) {
            cout << "[ERROR] El apellido no puede contener numeros.\n";
        }
    } while (!valido);

    // Validacion de especialidad
    do {
        valido = true;
        cout << "Ingrese Especialidad: ";
        cin.getline(especialidad, 50);

        if (strcmp(especialidad, "0") == 0) {
            return false;
        }
        if (strlen(especialidad) == 0) {
            cout << "[ERROR] La especialidad no puede quedar vacia.\n";
            valido = false;
        }
        for (int i = 0; especialidad[i] != '\0'; i++) {
            if (especialidad[i] >= '0' && especialidad[i] <= '9') {
                valido = false;
            }
        }
        if (!valido && strlen(especialidad) > 0) {
            cout << "[ERROR] La especialidad no puede contener numeros.\n";
        }
    } while (!valido);

    // Validacion de porcentaje de comision
    do {
        cout << "Ingrese Porcentaje de Comision (0-100): ";
        cin >> porcentajeComision;
        if (porcentajeComision < 0 || porcentajeComision > 100) {
            cout << "[ERROR] El porcentaje debe estar entre 0 y 100.\n";
        }
    } while (porcentajeComision < 0 || porcentajeComision > 100);

    return true;
}

// BUSQUEDA POR ID
bool Profesional::buscarPorId(int id){
    int pos = 0;
    while(leerDisco(pos)){
        if(idProfesional == id && estado == true){
            return true;
        }
        pos++;
    }
    return false;
}

// MOSTRAR NOMBRE Y APELLIDO POR ID
bool Profesional::mostrarNombrePorId(int id) {
    int pos = 0;
    while (leerDisco(pos)) {
        if (idProfesional == id && estado == true) {
            cout << apellido << ", " << nombre << " (ID: " << idProfesional << ")";
            return true;
        }
        pos++;
    }
    return false;
}

// PERSISTENCIA EN DISCO
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


