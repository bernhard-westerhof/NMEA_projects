#include <Wire.h>
#include <SFE_BMP180.h>
#include <stdlib.h>

SFE_BMP180 bmp180;

char pressureXDR[30] = "$WIXDR,P,1.01408,B,Barometer*";
char pressureMDA[35] = "$WIMDA,,,1.00401,B,,,,,,,,,,,,,,,*";
char temperatureXDR[25] = "$WIXDR,C,19.8,C,AirTemp*";
char temperatureMTA[16] = "$WIMTA,19.40,C*";
char pressure[8];
char temperature[6];

void setup() {
  Serial.begin(4800);
  bool success = bmp180.begin();
}

void loop() {

  char status;
  double T, P;
  bool success = false;

  status = bmp180.startTemperature();

  if (status != 0) {
    delay(1000);
    status = bmp180.getTemperature(T);

    if (status != 0) {
      status = bmp180.startPressure(3);

      if (status != 0) {
        delay(status);
        status = bmp180.getPressure(P, T);

        if (status != 0) {
          //Format pressure and temperature readings
          dtostrf(P / 1000, 1, 5, pressure);
          dtostrf(T, 2, 2, temperature);
          for (int idx = 0; idx < 7; idx++) {
            pressureXDR[9 + idx] = pressure[idx];
            pressureMDA[9 + idx] = pressure[idx];
          }
          for (int idx = 0; idx < 4; idx++) {
            temperatureXDR[9 + idx] = temperature[idx];
          }
          for (int idx = 0; idx < 5; idx++) {
            temperatureMTA[7 + idx] = temperature[idx];
          }

          sentencePrinter(pressureXDR,checksumCalculator(pressureXDR,28));
          delay(1000);
          sentencePrinter(pressureMDA,checksumCalculator(pressureMDA,33));
          delay(1000);
          sentencePrinter(temperatureXDR,checksumCalculator(temperatureXDR,23));
          delay(1000);
          sentencePrinter(temperatureMTA,checksumCalculator(temperatureMTA,14));
        }
      }
    }
  }
  delay(7000);
}

int checksumCalculator(char *sentence, int sentenceLength) {
  int checksum = 0;
  for ( int idx = 1; idx < sentenceLength; idx++) {
    checksum ^= sentence[idx];
  }
  return checksum;
}

int sentencePrinter(char *sentence, int checksum) {
  Serial.print(sentence);
  if (checksum > 15) {
    Serial.println(checksum, HEX);
  } else {
    Serial.print(0);
    Serial.println(checksum, HEX);
  }
  return 0;
}
