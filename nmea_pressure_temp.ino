#include <Wire.h>
#include <SFE_BMP180.h>
#include <stdlib.h>

SFE_BMP180 bmp180;

char presSentence[30] = "$WIXDR,P,1.01408,B,Barometer*";
char presSentence2[35] = "$WIMDA,,,1.00401,B,,,,,,,,,,,,,,,*";
char tempSentence[25] = "$WIXDR,C,19.8,C,AirTemp*";
char tempSentence2[16] = "$WIMTA,19.40,C*";
char chksm[3];
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
          /*Serial.print("Pressure: ");
          Serial.print(String(P/1000,4));
          Serial.println(" bar");*/
          /*presSentence[10] = String(P/1000,4);*/
          dtostrf(P/1000,1,5,pressure);
          dtostrf(T,2,2,temperature);
          for(int idx = 0; idx < 7; idx++){
            presSentence[9+idx] = pressure[idx];
            presSentence2[9+idx] = pressure[idx];
          }
          for(int idx = 0; idx < 4; idx++){
            tempSentence[9+idx] = temperature[idx];
          }
          for(int idx = 0; idx < 5; idx++){
            tempSentence2[7+idx] = temperature[idx];
          }

          //Calculate checksums
          int chksm = 0;
          for( int idx = 1; idx < 28; idx++){
            chksm ^= presSentence[idx];
          }
          Serial.print(presSentence);
          if(chksm > 15){
          Serial.println(chksm,HEX);
          }else{
            Serial.print(0);
            Serial.println(chksm,HEX);
          }

          chksm = 0;
          for( int idx = 1; idx < 23; idx++){
            chksm ^= tempSentence[idx];
          }

          Serial.print(tempSentence);
          if(chksm > 15){
          Serial.println(chksm,HEX);
          }else{
            Serial.print(0);
            Serial.println(chksm,HEX);
          }

          chksm = 0;
          for( int idx = 1; idx < 33; idx++){
            chksm ^= presSentence2[idx];
          }

          Serial.print(presSentence2);

          if(chksm > 15){
          Serial.println(chksm,HEX);
          }else{
            Serial.print(0);
            Serial.println(chksm,HEX);
          }

          chksm = 0;
          for( int idx = 1; idx < 14; idx++){
            chksm ^= tempSentence2[idx];
          }

          Serial.print(tempSentence2);
          if(chksm > 15){
          Serial.println(chksm,HEX);
          }else{
            Serial.print(0);
            Serial.println(chksm,HEX);
          }
        }
      }
    }
  }
  delay(10000);
  //Serial.println( F("$IIVHW,,T,,M,06.11,N,11.31,K*51") );
}
