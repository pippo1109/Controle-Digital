#include <Arduino.h>
#include <AtuadorAux.h>
#include <SensorAux.h>
#include <stdlib.h>

static const int CENTRO_MM = 220;   // leitura do ToF com a bola no centro (confirmar)

String linha;

void processarComando(String comando) {
  comando.trim();
  if (comando.length() == 0) {
    return;
  }

  char* fim;
  const float angulo = strtof(comando.c_str(), &fim);
  if (*fim == '\0' && isfinite(angulo)) {
    servoWrite(angulo);
  }
}

void setup() {
  Serial.begin(115200);
  servoBegin(D2, 0, 50, 14, 1500.0f, 10.0f, 25.0f);

  if (!distanceBegin(CENTRO_MM, 500, 16, 16, 199, 20000, 20)) {
    Serial.println("erro_sensor:1");
  }
}

void loop() {
  // Angulo digitado no terminal
  while (Serial.available()) {
    char c = (char)Serial.read();

    if (c == '\n' || c == '\r') {
      if (linha.length() > 0) {
        processarComando(linha);
        linha = "";
      }
    } else if (linha.length() < 24) {
      linha += c;
    }
  }

  // Leitura do sensor de distancia
    uint16_t distancia;
    int posicao;
    bool valido;
    uint32_t timestampUs;
    distancia, posicao, valido, timestampUs = distanceUpdate(distancia, posicao, valido, timestampUs);
    Serial.println(posicao);

}