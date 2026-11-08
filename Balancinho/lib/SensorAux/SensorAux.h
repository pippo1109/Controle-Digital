#include <Arduino.h>

// ===============================================================
// Sensor de distancia VL53L1X
// ===============================================================
// centerPosition: mm correspondente ao centro da viga
// timeout: ms
// timingBudget: us
// continuousPeriod: ms (>= timingBudget)
// Retorna false se o sensor nao inicializar.
bool distanceBegin(int centerPosition, uint16_t timeout,
                   uint8_t roiWidth, uint8_t roiHeight, uint8_t roiCenter,
                   uint32_t timingBudget, uint32_t continuousPeriod);

// Retorna true se havia medida nova; nesse caso preenche os parametros:
//   distance    : mm
//   position    : mm, relativo ao centro
//   valid       : range_status == RangeValid
//   timestampUs : micros() no instante da leitura
bool distanceUpdate(uint16_t& distance, int& position, bool& valid,
                    uint32_t& timestampUs);