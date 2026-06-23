# SISTEMA DE GESTION PARA CENTRO DE ESTETICA INTEGRAL 

Trabajo Práctico Integrador · **Programación II** · UTN FRGP

Un sistema de gestión desarrollado en **C++** orientado a la administración eficiente de agendas de turnos, historiales clínicos de tratamientos, control financiero de caja y liquidación automatizada de comisiones para profesionales de la estética.

---

## Integrantes
* **Gisela Grisel Lanzillotta**
* **Sol Dánae Lezcano**
  

---

## Características Principales y Arquitectura

El sistema implementa conceptos avanzados de la Programación Orientada a Objetos (POO) y persistencia de datos en archivos binarios, estructurado bajo las siguientes reglas de negocio:

* **Lógica de Cabecera y Detalle:** Se diseñó una arquitectura desacoplada utilizando las clases `Turno` (Cabecera) y `DetalleTurno` (Detalle). Esto permite agendar múltiples servicios y diferentes profesionales en una sola visita, garantizando la integridad de los precios cobrados frente a futuras actualizaciones de tarifas.
* **Seguimiento Clínico:** Registro de observaciones detalladas por tratamiento para documentar productos utilizados y sugerencias de sesiones futuras.
* **Gestión Financiera Avanzada:** Control de flujo de caja mediante un esquema de señas y saldos pendientes. El sistema calcula automáticamente las comisiones del staff según porcentajes variables configurados de manera independiente por profesional.
* **Mantenimiento y Resiliencia del Sistema:** Menú de configuración con utilidades para realizar Backups y restauración de archivos `.dat`, además de un módulo de exportación de reportes a formato `.csv` para su análisis externo.
* **Robustez en Interfaz de Consola:** Implementación de mecanismos de control de flujo mediante el vaciado del buffer de entrada (cin.ignore). Esto blinda al sistema contra ingresos de datos inválidos (como caracteres alfabéticos en menús numéricos), evitando bucles infinitos y garantizando la estabilidad de la aplicación durante la ejecución.

---

## Tecnologías y Herramientas
* **Lenguaje:** C++ (Estándar POO)
* **Persistencia:** Archivos binarios estructurados (`.dat`)
* **Entorno de desarrollo:** Code::Blocks 
* **Control de versiones:** GitHub

---

## Estructura de Clases (Diagrama de Clases)

El diseño del modelo de datos está compuesto por las siguientes entidades principales que interactúan mediante relaciones de agregación y composición:

* **`Cliente`:** Gestión de datos personales e historial de deudas acumuladas.
* **`Profesional`:** Administración de staff, especialidades y porcentaje de comisión variable.
* **`Servicio`:** Catálogo de precios actuales y tipos de tratamientos.
* **`ServicioXProfesional`:** Tabla intermedia para la asignación de especialidades por personal.
* **`Turno` (Cabecera):** Control de ID de turno, cliente, fecha de la cita, seña abonada y estado de asistencia.
* **`DetalleTurno` (Detalle):** Desglose del turno que almacena el servicio brindado, el profesional asignado, el horario exacto, el precio congelado al momento de la reserva y las observaciones clínicas.
* **`Fecha`:** Componente de validación de calendario laboral.

---

## Salidas del Sistema e Informes

El programa procesa la información en disco para generar los siguientes reportes gerenciales:
* **Listados:** Clientes ordenados por apellido, frecuencia de visitas o saldos pendientes; profesionales activos/inactivos o clasificados por volumen de servicios; catálogo y ranking de servicios más pedidos.
* **Consultas:** Búsqueda individual de deudas e historial por ID de cliente, y visualización de disponibilidad de agenda diaria por profesional o por calendario completo.
* **Informes de Gestión:** Reporte de recaudación total de caja (discriminando señas de saldos finales) y liquidaciones periódicas de comisiones del staff.

---


## Compilación y Ejecución

Para compilar y ejecutar el sistema en un entorno local, siga estos pasos:
1. Clone este repositorio o descargue los archivos fuente en su máquina.
2. Abra el archivo de proyecto `SistemaEstetica.cbp` utilizando el entorno **Code::Blocks**.
3. Asegúrese de realizar una compilación limpia seleccionando **Build -> Clean** y luego **Build -> Rebuild**.
4. Presione el botón de **Run** (o `F9`) para iniciar la consola ejecutable.
*Nota: Los archivos de base de datos `.dat` se generarán automáticamente en la raíz del proyecto tras la primera ejecución.*

---

## Estructura del Proyecto

Para facilitar la compilación directa y mantener la compatibilidad con el entorno Code::Blocks del equipo, todos los componentes del sistema se encuentran organizados de forma plana en la raíz del directorio principal:

* **Archivos de Código Fuente (`*.cpp`):** Contienen la lógica de negocio, flujos de menús y desarrollos de métodos de cada clase.
* **Archivos de Cabecera (`*.h`):** Definen las estructuras de las clases, prototipos de funciones globales y atributos.
* **Archivos de Base de Datos (`*.dat`):** Archivos binarios de persistencia donde se almacenan y modifican los registros del centro de estética (generados automáticamente al ejecutar el sistema).

---

## Posibles Mejoras Futuras
* **Implementación de Herencia en POO:** Reestructurar el modelo para abstraer las clases `Cliente` y `Profesional` bajo una clase base común denominada `Persona`, optimizando la reutilización de código para atributos compartidos (Nombre, Apellido, DNI, Teléfono).
* **Migración a Base de Datos Relacional:** Evolucionar la persistencia de archivos binarios `.dat` hacia un motor estructurado (como SQL) para facilitar consultas complejas y subconsultas mediante sentencias nativas.



