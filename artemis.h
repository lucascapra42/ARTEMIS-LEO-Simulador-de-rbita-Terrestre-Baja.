#ifndef ARTEMIS_H
#define ARTEMIS_H

/* CONSTANTES FISICAS DE LA TIERRA Y ORION */

#define ARTEMIS_G 6.674e-11

#define ARTEMIS_MASA_TIERRA_KG 5.972e24

#define ARTEMIS_RADIO_TIERRA_M 6371000.0

#define ARTEMIS_ALTITUD_ORBITAL_M 192000.0

#define ARTEMIS_MASA_ORION_KG 33000.0

#define ARTEMIS_ACELERACION_LATERAL 0.50

#define ARTEMIS_DURACION_PULSO 10.0

/* CAPACIDAD DEL HISTORIAL */

#define ARTEMIS_MAX_EVENTOS 20

#define ARTEMIS_TAM_DESCRIPCION 120

/* PARAMETROS DE LA ORBITA */

typedef struct {
    double masa_tierra_kg;
    double radio_tierra_m;
    double altitud_orbital_m;
    double masa_orion_kg;
    double aceleracion_lateral;
    double duracion_pulso;

} ParametrosOrbita;

/* RESULTADOS DE LOS CALCULOS */

typedef struct {
    double radio_orbital_m;
    double fuerza_gravitatoria_n;
    double fuerza_centripeta_n;
    double velocidad_orbital_m_s;
    double periodo_orbital_s;
    double periodo_orbital_min;
    double aceleracion_centripeta_m_s2;
    double desviacion_lateral_m;
    double velocidad_lateral_final_m_s;

} ResultadosOrbita;

/* REGISTRO DE EVENTOS */

typedef struct {
    char descripcion[ARTEMIS_TAM_DESCRIPCION];
    double tiempo_s;

} EventoMision;

typedef struct {
    EventoMision eventos[ARTEMIS_MAX_EVENTOS];
    int cantidad_eventos;

} HistorialMision;

/* PROTOTIPOS DE FUNCIONES */

/* Inicializacion */
void inicializarParametrosPredeterminados(ParametrosOrbita *parametros);
void inicializarHistorial(HistorialMision *historial);

/* Calculos fisicos */
int calcularOrbita(const ParametrosOrbita *parametros,
                   ResultadosOrbita *resultados);

int calcularPropulsorLateral(const ParametrosOrbita *parametros,
                             ResultadosOrbita *resultados);

/* Registro de eventos */
int registrarEvento(HistorialMision *historial,
                    const char *descripcion,
                    double tiempo_s);

/* Presentacion de resultados */
void mostrarResultados(const ResultadosOrbita *resultados);
void mostrarHistorial(const HistorialMision *historial);


#endif
