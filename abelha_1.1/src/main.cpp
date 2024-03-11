#include <Arduino.h>

#include <DHT.h>        // Include the DHT library for temperature and humidity sensors
#include <DHT_U.h>      // Include the DHT utility library
#include <RTClib.h>   // Include the RTC3231 library for real-time clock
#include <SD.h>         // Include the SD library for SD card
#include <Adafruit_I2CDevice.h>
#include <SPI.h>
#include <Wire.h>

#define DHT_COUNT 7
#define DHT_PINS {2, 4, 13, 15, 16, 17, 18} // Exemplo de pinos, ajuste conforme sua configuração

#define DHT_TYPE DHT22


DHT dhts[DHT_COUNT];
RTC_DS3231 rtc;
File dataFile;

void setup() {
  Serial.begin(9600);
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("Falha ao inicializar o cartão SD!");
    return;
  }

  if (!rtc.begin()) {
    Serial.println("Falha ao inicializar o RTC!");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC perdeu energia, ajustando a hora!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  dataFile = SD.open("data.csv", FILE_WRITE);
  if (!dataFile) {
    Serial.println("Erro ao abrir o arquivo!");
    return;
  }

  dataFile.println("Data,Hora,Sensor,Temperatura Umidade");
  dataFile.close();

  for (int i = 0; i < DHT_COUNT; i++) {
    dhts[i] = DHT(DHT_PINS[i], DHT_TYPE);
    dhts[i].begin();
  }
}

void loop() {
  DateTime now = rtc.now();

  for (int i = 0; i < DHT_COUNT; i++) {
    float temperature = dhts[i].readTemperature();
    float humidity = dhts[i].readHumidity();

    dataFile = SD.open("data.csv", FILE_WRITE);
    if (dataFile) {
      dataFile.print(now.year(), DEC);
      dataFile.print("/");
      dataFile.print(now.month(), DEC);
      dataFile.print("/");
      dataFile.print(now.day(), DEC);
      dataFile.print(",");
      dataFile.print(now.hour(), DEC);
      dataFile.print(":");
      dataFile.print(now.minute(), DEC);
      dataFile.print(":");
      dataFile.print(now.second(), DEC);
      dataFile.print(",");
      dataFile.print("Sensor ");
      dataFile.print(i+1);
      dataFile.print(",");
      dataFile.print(temperature);
      dataFile.print(",");
      dataFile.println(humidity);
      dataFile.close();
    } else {
      Serial.println("Erro ao abrir o arquivo!");
    }

    delay(100); // Pequeno atraso entre cada leitura de sensor
  }

  delay(60000); // Espera 1 minuto antes de ler novamente
}
