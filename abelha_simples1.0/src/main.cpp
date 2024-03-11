#include <Arduino.h>    // Include the Arduino library

#include <DHT.h>        // Include the DHT library for temperature and humidity sensors
#include <DHT_U.h>      // Include the DHT utility library
#include <RTClib.h>   // Include the RTC3231 library for real-time clock
#include <SD.h>         // Include the SD library for SD card
#include <Adafruit_I2CDevice.h>
#include <SPI.h>
#include <Wire.h>
// Define which pins are used for communication
#define DHTPIN_1 2
#define DHTPIN_2 4
#define DHTPIN_3 13
#define DHTPIN_4 15
#define DHTPIN_5 16
#define DHTPIN_6 17
#define DHTPIN_7 18
#define DHTTYPE DHT22
#define SDCS 5

RTC_DS3231 rtc;
char daysOfWeek[7][12] = {
  "Domingo",
  "Segunda",
  "Terca",
  "Quarta",
  "Quinta",
  "Sexta",
  "Sabado"
};

DHT dhtSensors[7] = {
  DHT(DHTPIN_1, DHTTYPE),
  DHT(DHTPIN_2, DHTTYPE),
  DHT(DHTPIN_3, DHTTYPE),
  DHT(DHTPIN_4, DHTTYPE),
  DHT(DHTPIN_5, DHTTYPE),
  DHT(DHTPIN_6, DHTTYPE),
  DHT(DHTPIN_7, DHTTYPE)
};

File dataFile;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  rtc.begin();
  rtc.adjust(DateTime((__DATE__), (__TIME__)));
  if (!rtc.begin()) {
    Serial.println("Erro ao inicializar o RTC!");
    return;
  }

  if (!SD.begin(SDCS)) {
    Serial.println("Erro ao inicializar o cartão SD!");
    return;
  }

  // Open the data file for writing
  dataFile = SD.open("/data.csv", FILE_APPEND);
  
  if (!dataFile) {
    Serial.println("Erro ao criar o arquivo data.csv!");
    return;
  }
  

// Check if the file is empty (first run)
  if (dataFile.size() == 0) {
    // Write header row only on the first run
    dataFile.print("Date,Time,");
    for (int i = 0; i < 7; i++) {
      dataFile.print("DHT");
      dataFile.print(i + 1);
      dataFile.print("_Temp,DHT");
      dataFile.print(i + 1);
      dataFile.print("_Humid,");
    }
    dataFile.println();
  }
}

void loop() {
  DateTime now = rtc.now();

  // Read sensor data
  for (int i = 0; i < 7; i++) {
    float temp = dhtSensors[i].readTemperature();
    float humid = dhtSensors[i].readHumidity();

    if (isnan(temp) || isnan(humid)) {
      Serial.print("Falha na leitura do sensor DHT");
      Serial.print(i + 1);
      Serial.println("!");

      //Escreve mesmo com a falha
      dataFile.print(now.year());
      dataFile.print("-");
      dataFile.print(now.month());
      dataFile.print("-");;
      dataFile.print(now.day());
      dataFile.print(",");
      dataFile.print(now.hour());
      dataFile.print(":");
      dataFile.print(now.minute());
      dataFile.print(":");
      dataFile.print(now.second());
      dataFile.print(",");
      dataFile.print(i+1);
      dataFile.print(",N/A,N/A,");
      continue;
      
    }

    // Write sensor data to file
    dataFile.print(now.year());
    dataFile.print("-");
    dataFile.print(now.month());
    dataFile.print("-");
    dataFile.print(now.day());
    dataFile.print(",");
    dataFile.print(now.hour());
    dataFile.print(":");
    dataFile.print(now.minute());
    dataFile.print(":");
    dataFile.print(now.second());
    dataFile.print(",");
    dataFile.print(temp);
    dataFile.print(",");
    dataFile.print(humid);
    dataFile.print(",");
  }
  dataFile.println();

  delay(6000);
}

void closeFile() {
  // Close the data file
  dataFile.close();
}

