#include <Arduino.h>
#include <Wire.h>
#include <VL53L1X.h>

VL53L1X sensor;

const int CENTRO_TRILHO = 220;

// ==========================
// SERVO
// ==========================
const int SERVO_PIN = D2;

const int PWM_CHANNEL = 0;
const int PWM_FREQ = 50;
const int PWM_RESOLUTION = 14;

// Centro calibrado
const int SERVO_ZERO = 1493;

// Aproximadamente 11 us por grau
const int US_POR_GRAU = 11;

// Limites +/-15 graus
const int SERVO_POS_15 = SERVO_ZERO + (15 * US_POR_GRAU);
const int SERVO_NEG_15 = SERVO_ZERO - (15 * US_POR_GRAU);

// Pulso atual do servo
int pulsoAtual = SERVO_ZERO;

// Destino atual
int destinoServo = SERVO_POS_15;

// Estado da sequência:
// 0 = indo para +15
// 1 = indo para 0
// 2 = indo para -15
// 3 = indo para 0
int estadoServo = 0;

// Velocidade do movimento
const unsigned long INTERVALO_PASSO = 12; // ms por grau
unsigned long ultimoPasso = 0;


// ==========================
// PWM SERVO
// ==========================
void servoPulse(int us) {

  uint32_t duty = ((uint64_t)us * 16383) / 20000;

  ledcWrite(PWM_CHANNEL, duty);
}


// ==========================
// SETUP
// ==========================
void setup() {

  Serial.begin(115200);
  delay(1000);

  // --------------------------
  // SERVO
  // --------------------------
  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(SERVO_PIN, PWM_CHANNEL);

  servoPulse(SERVO_ZERO);


  // --------------------------
  // SENSOR
  // --------------------------
  Wire.begin();

  sensor.setTimeout(500);

  if (!sensor.init()) {

    Serial.println("Sensor nao encontrado!");

    while (1) {
      delay(100);
    }
  }

  sensor.setDistanceMode(VL53L1X::Short);

  sensor.setROISize(4, 4);

  sensor.setROICenter(60);

  sensor.setMeasurementTimingBudget(50000);

  sensor.startContinuous(50);
}


// ==========================
// LOOP
// ==========================
void loop() {

  unsigned long agora = millis();


  // ==================================================
  // SERVO
  // ==================================================
  if (agora - ultimoPasso >= INTERVALO_PASSO) {

    ultimoPasso = agora;

    // Move 1 grau por vez
    if (pulsoAtual < destinoServo) {

      pulsoAtual += US_POR_GRAU;

      if (pulsoAtual > destinoServo)
        pulsoAtual = destinoServo;
    }

    else if (pulsoAtual > destinoServo) {

      pulsoAtual -= US_POR_GRAU;

      if (pulsoAtual < destinoServo)
        pulsoAtual = destinoServo;
    }

    servoPulse(pulsoAtual);


    // Quando chegou no destino,
    // escolhe o próximo
    if (pulsoAtual == destinoServo) {

      estadoServo++;

      if (estadoServo > 3)
        estadoServo = 0;

      if (estadoServo == 0)
        destinoServo = SERVO_POS_15;

      else if (estadoServo == 1)
        destinoServo = SERVO_ZERO;

      else if (estadoServo == 2)
        destinoServo = SERVO_NEG_15;

      else if (estadoServo == 3)
        destinoServo = SERVO_ZERO;
    }
  }


  // ==================================================
  // SENSOR - NÃO BLOQUEANTE
  // ==================================================
  if (sensor.dataReady()) {

    uint16_t distancia = sensor.read(false);

    int posicao = (int)distancia - CENTRO_TRILHO;

    Serial.print("Distancia:");
    Serial.print(distancia);

    Serial.print("\tPosicao:");
    Serial.print(posicao);

    Serial.print("\tPulso:");
    Serial.println(pulsoAtual);
  }
}