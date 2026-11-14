#include "artemis.h"
#include <math.h>
#include <stddef.h>



int calcularOrbita(const ParametrosOrbita *parametros,
                   ResultadosOrbita *resultados)
{
    double radio_orbital;
    double velocidad;
    double periodo;
    double aceleracion;
    double fuerza_gravitatoria;
    double fuerza_centripeta;

    /* Validar punteros */
    if (parametros == NULL || resultados == NULL) {
        return 0;
    }

    /* Validar parametros fisicos */
    if (!isfinite(parametros->masa_tierra_kg) ||
        !isfinite(parametros->radio_tierra_m) ||
        !isfinite(parametros->altitud_orbital_m) ||
        !isfinite(parametros->masa_orion_kg) ||
        parametros->masa_tierra_kg <= 0 ||
        parametros->radio_tierra_m <= 0 ||
        parametros->altitud_orbital_m <= 0 ||
        parametros->masa_orion_kg <= 0) {
        return 0;
    }

    /* Calcular radio orbital total */
    radio_orbital = parametros->radio_tierra_m +
                    parametros->altitud_orbital_m;

    if (!isfinite(radio_orbital) || radio_orbital <= 0) {
        return 0;
    }

    /* Calcular velocidad orbital */
    velocidad = sqrt((ARTEMIS_G *
                      parametros->masa_tierra_kg) /
                      radio_orbital);

    /* Calcular fuerza gravitatoria */
    fuerza_gravitatoria =
        (ARTEMIS_G * parametros->masa_tierra_kg *
         parametros->masa_orion_kg) /
        (radio_orbital * radio_orbital);

    /* Calcular aceleracion centripeta */
    aceleracion = (velocidad * velocidad) / radio_orbital;

    /* Calcular fuerza centripeta */
    fuerza_centripeta = parametros->masa_orion_kg *
                        aceleracion;

    /* Calcular periodo orbital */
    periodo = (2.0 * acos(-1.0) * radio_orbital) /
              velocidad;

    /* Comprobar resultados */
    if (!isfinite(velocidad) ||
        !isfinite(fuerza_gravitatoria) ||
        !isfinite(fuerza_centripeta) ||
        !isfinite(aceleracion) ||
        !isfinite(periodo)) {
        return 0;
    }

    /* Guardar resultados */
    resultados->radio_orbital_m = radio_orbital;
    resultados->velocidad_orbital_m_s = velocidad;
    resultados->fuerza_gravitatoria_n = fuerza_gravitatoria;
    resultados->fuerza_centripeta_n = fuerza_centripeta;
    resultados->aceleracion_centripeta_m_s2 = aceleracion;
    resultados->periodo_orbital_s = periodo;
    resultados->periodo_orbital_min = periodo / 60.0;

    return 1;
}
/* CALCULO DEL PROPULSOR LATERAL */

int calcularPropulsorLateral(const ParametrosOrbita *parametros,
                             ResultadosOrbita *resultados)
{
    double aceleracion;
    double duracion;
    double velocidad_final;
    double desviacion;

    /* Validar punteros */
    if (parametros == NULL || resultados == NULL) {
        return 0;
    }

    aceleracion = parametros->aceleracion_lateral;
    duracion = parametros->duracion_pulso;

    /* Validar datos */
    if (!isfinite(aceleracion) ||
        !isfinite(duracion) ||
        aceleracion < 0.0 ||
        duracion < 0.0) {
        return 0;
    }

    /* Calcular velocidad lateral final */
    velocidad_final = aceleracion * duracion;

    /* Calcular desviacion lateral */
    desviacion = 0.5 * aceleracion * duracion * duracion;

    /* Comprobar resultados */
    if (!isfinite(velocidad_final) ||
        !isfinite(desviacion)) {
        return 0;
    }

    /* Guardar resultados */
    resultados->velocidad_lateral_final_m_s = velocidad_final;
    resultados->desviacion_lateral_m = desviacion;

    return 1;
}
void inicializarParametrosPredeterminados(ParametrosOrbita *parametros)
{
    if (parametros == NULL) {
        return;
    }

    parametros->masa_tierra_kg = ARTEMIS_MASA_TIERRA_KG;
    parametros->radio_tierra_m = ARTEMIS_RADIO_TIERRA_M;
    parametros->altitud_orbital_m = ARTEMIS_ALTITUD_ORBITAL_M;
    parametros->masa_orion_kg = ARTEMIS_MASA_ORION_KG;

    parametros->aceleracion_lateral = ARTEMIS_ACELERACION_LATERAL;
    parametros->duracion_pulso = ARTEMIS_DURACION_PULSO;
}

/* INICIALIZAR HISTORIAL */

void inicializarHistorial(HistorialMision *historial)
{
    int i;

    if (historial == NULL) {
        return;
    }

    historial->cantidad_eventos = 0;

    for (i = 0; i < ARTEMIS_MAX_EVENTOS; i++) {
        historial->eventos[i].descripcion[0] = '\0';
        historial->eventos[i].tiempo_s = 0.0;
    }
}


/* REGISTRAR EVENTOS DE LA MISION */

int registrarEvento(HistorialMision *historial,
                    const char *descripcion,
                    double tiempo_s)
{
    int indice;

    if (historial == NULL || descripcion == NULL) {
        return 0;
    }

    if (historial->cantidad_eventos < 0 ||
        historial->cantidad_eventos >= ARTEMIS_MAX_EVENTOS) {
        return 0;
    }

    if (descripcion[0] == '\0' ||
        !isfinite(tiempo_s) || tiempo_s < 0.0) {
        return 0;
    }

    indice = historial->cantidad_eventos;

    snprintf(historial->eventos[indice].descripcion,
             ARTEMIS_TAM_DESCRIPCION,
             "%s", descripcion);

    historial->eventos[indice].tiempo_s = tiempo_s;

    historial->cantidad_eventos++;

    return 1;
}

/* MOSTRAR RESULTADOS ORBITALES */

void mostrarResultados(const ResultadosOrbita *resultados)
{
    if (resultados == NULL) {
        printf("Error: no hay resultados disponibles.\n");
        return;
    }

    printf("\n====================================\n");
    printf("       RESULTADOS DE ORION\n");
    printf("====================================\n");

    printf("Radio orbital: %.2f m\n",
           resultados->radio_orbital_m);

    printf("Velocidad orbital: %.2f m/s\n",
           resultados->velocidad_orbital_m_s);

    printf("Fuerza gravitatoria: %.2f N\n",
           resultados->fuerza_gravitatoria_n);

    printf("Fuerza centripeta: %.2f N\n",
           resultados->fuerza_centripeta_n);

    printf("Periodo orbital: %.2f segundos\n",
           resultados->periodo_orbital_s);

    printf("Periodo orbital: %.2f minutos\n",
           resultados->periodo_orbital_min);

    printf("Aceleracion centripeta: %.2f m/s2\n",
           resultados->aceleracion_centripeta_m_s2);

    printf("Desviacion lateral: %.2f m\n",
           resultados->desviacion_lateral_m);

    printf("Velocidad lateral final: %.2f m/s\n",
           resultados->velocidad_lateral_final_m_s);

    printf("====================================\n");
}


/* MOSTRAR HISTORIAL DE LA MISION */

void mostrarHistorial(const HistorialMision *historial)
{
    int i;

    if (historial == NULL) {
        printf("Error: historial no disponible.\n");
        return;
    }

    printf("\n====================================\n");
    printf("       HISTORIAL DE MISION\n");
    printf("====================================\n");

    if (historial->cantidad_eventos == 0) {
        printf("No hay eventos registrados.\n");
        return;
    }

    for (i = 0; i < historial->cantidad_eventos; i++) {

        printf("\nEvento %d\n", i + 1);

        printf("Descripcion: %s\n",
               historial->eventos[i].descripcion);

        printf("Tiempo: %.2f segundos\n",
               historial->eventos[i].tiempo_s);
    }

    printf("\n====================================\n");
}