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
