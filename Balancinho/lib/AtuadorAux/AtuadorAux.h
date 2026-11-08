#include <Arduino.h>

// ===============================================================
// Atuador Servo: recebe angulo (graus de servo) e satura em +/-limitDegrees
// ===============================================================
// centerPulse: us com a viga na horizontal
// microsecondsPerDegree: us por grau de servo
// limitDegrees: saturacao de seguranca (+/-)
void servoBegin(int pin, int channel, int frequency, int resolution,
                float centerPulse, float microsecondsPerDegree,
                float limitDegrees);
// Aplica o angulo (saturado) e devolve o valor aplicado.
float servoWrite(float degrees);
float servoPulse();