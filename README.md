# SISTEMA DE GESTION PARA CENTRO DE ESTETICA INTEGRAL 

Trabajo Práctico Integrador · **Programación II** · UTN FRGP

Un sistema de gestión desarrollado en **C++** orientado a la administración eficiente de agendas de turnos, historiales clínicos de tratamientos, control financiero de caja y liquidación automatizada de comisiones para profesionales de la estética[cite: 1].

---

## 👥 Integrantes
* **Gisela Grisel Lanzillotta**
* **Sol Dánae Lezcano**
  

---

## Características Principales y Arquitectura

El sistema implementa conceptos avanzados de la Programación Orientada a Objetos (POO) y persistencia de datos en archivos binarios, estructurado bajo las siguientes reglas de negocio[cite: 1]:

* **Lógica de Cabecera y Detalle:** Se diseñó una arquitectura desacoplada utilizando las clases `Turno` (Cabecera) y `DetalleTurno` (Detalle)[cite: 1]. Esto permite agendar múltiples servicios y diferentes profesionales en una sola visita, garantizando la integridad de los precios cobrados frente a futuras actualizaciones de tarifas[cite: 1].
* **Seguimiento Clínico:** Registro de observaciones detalladas por tratamiento para documentar productos utilizados y sugerencias de sesiones futuras[cite: 1].
* **Gestión Financiera Avanzada:** Control de flujo de caja mediante un esquema de señas y saldos pendientes[cite: 1]. El sistema calcula automáticamente las comisiones del staff según porcentajes variables configurados de manera independiente por profesional[cite: 1].
* **Mantenimiento y Resiliencia del Sistema:** Menú de configuración con utilidades para realizar Backups y restauración de archivos `.dat`, además de un módulo de exportación de reportes a formato `.csv` para su análisis externo[cite: 1].

---

## Tecnologías y Herramientas
* **Lenguaje:** C++ (Estándar POO)[cite: 1]
* **Persistencia:** Archivos binarios estructurados (`.dat`)[cite: 1]
* **Entorno de desarrollo:** Code::Blocks / Visual Studio Code
* **Control de versiones:** GitHub

---

## Estructura de Clases (Diagrama de Clases)

El diseño del modelo de datos está compuesto por las siguientes entidades principales que interactúan mediante relaciones de agregación y composición[cite: 1]:

* **`Cliente`:** Gestión de datos personales e historial de deudas acumuladas[cite: 1].
* **`Profesional`:** Administración de staff, especialidades y porcentaje de comisión variable[cite: 1].
* **`Servicio`:** Catálogo de precios actuales y tipos de tratamientos[cite: 1].
* **`ServicioXProfesional`:** Tabla intermedia para la asignación de especialidades por personal[cite: 1].
* **`Turno` (Cabecera):** Control de ID de turno, cliente, fecha de la cita, seña abonada y estado de asistencia[cite: 1].
* **`DetalleTurno` (Detalle):** Desglose del turno que almacena el servicio brindado, el profesional asignado, el horario exacto, el precio congelado al momento de la reserva y las observaciones clínicas[cite: 1].
* **`Fecha`:** Componente de validación de calendario laboral[cite: 1].

---

## Salidas del Sistema e Informes

El programa procesa la información en disco para generar los siguientes reportes gerenciales[cite: 1]:
* **Listados:** Clientes ordenados por apellido, frecuencia de visitas o saldos pendientes; profesionales activos/inactivos o clasificados por volumen de servicios; catálogo y ranking de servicios más pedidos[cite: 1].
* **Consultas:** Búsqueda individual de deudas e historial por ID de cliente, y visualización de disponibilidad de agenda diaria por profesional o por calendario completo[cite: 1].
* **Informes de Gestión:** Reporte de recaudación total de caja (discriminando señas de saldos finales) y liquidaciones periódicas de comisiones del staff[cite: 1].

---

## Estructura del Repositorio

```text
sistema-estetica/
├── main.cpp                 # Punto de entrada de la aplicación
├── include/                 # Archivos de cabecera (.h) para declaración de clases
├── src/                     # Implementación de métodos (.cpp)
├── data/                    # Archivos de datos binarios (.dat)
├── backups/                 # Copias de seguridad del sistema
└── README.md
