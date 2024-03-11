#include <Arduino.h>    // Include the Arduino library
#include <DHT.h>        // Include the DHT library for temperature and humidity sensors
#include <DHT_U.h>      // Include the DHT utility library
#include <rtclib.h>   // Include the relogio3231 library for real-time clock
#include <SD.h>         // Include the SD library for SD card
#include <Adafruit_I2CDevice.h>
#include <SPI.h>
#include <Wire.h>
//bibliotecas originais
#include <sensores.h>
#include <ssd_rtc.h>

//PINAGEM DEFINIDA NO ARQUIVO SENSORES.H
    // Define which pins are used for communication
    //#define DHTPIN_1 2
    //#define DHTPIN_2 4
    //#define DHTPIN_3 13
    //#define DHTPIN_4 15
    //#define DHTPIN_5 16
    //#define DHTPIN_6 17
    //#define DHTPIN_7 18
    //#define DHTTYPE DHT22
    //#define SDCS 5

RTC_DS3231 relogio;
char daysOfWeek[7][12] = {
  "Domingo",
  "Segunda",
  "Terca",
  "Quarta",
  "Quinta",
  "Sexta",
  "Sabado"
};

unsigned long intervalo_6s = 0;
unsigned long intervalo_60s = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  relogio.begin();
  relogio.adjust(DateTime((__DATE__), (__TIME__)));
  if (!relogio.begin()) {
    Serial.println("Erro ao inicializar o relogio!");
    return;
  }

  //Inicia a conexão com o sd, tentando tres vezes
  uint8_t cont;
    do
    {
      if (SD.begin())
      {
        // Verifica se existe o arquivo datalog no cartao de memoria, criando-o caso contrario
        if (!SD.exists("/datalog.csv")) // verifica ou cria o arquivo datalog.txt
        {
          File file = SD.open("/datalog.csv", FILE_WRITE);
          file.close();
          appendFile(SD, "/datalog.csv",
                     "data,hora,s0u,s0t,s1u,s1t,s2u,s2t,s3u,s3t,s4u,s4t,s5u,s5t,s6u,s6t,ldr1,ldr2");
        }
      } else
      {
        cont++;
      }
    } while (cont < 3);

  //Inicializando sensores
  sensoresBegin();
}

void loop() {
  //Estrutura de super loop
  unsigned long tempoAtual = millis();
  uint8_t controle = 0; //variável para manter a certeza de 10 leituras por minuto

  //Tarefa 1, faz 10 leituras em 1 minuto,
  //realizando o somatório dos dados
  if ((tempoAtual - intervalo_6s) >= 6000 && controle < 10)
  {
    realizarLeitura();
    intervalo_6s = tempoAtual;
    controle++;
  }
  

  //Tarefa 2, salva os dados no cartão sd a cada 1 minuto
  if ((tempoAtual - intervalo_60s) >= 60000 && controle >= 10)
  {
    //Inicia a conexão com o sd, tentando tres vezes
    uint8_t testesSd = 0;
    do
    {
      if(SD.begin())
      {
        break;
      }else
      {
        testesSd++;
      }
      
    } while (testesSd <= 3);
        
    DateTime now = relogio.now();
    char horarioArquivo[9] = "hh:mm:ss";
    char dataArquivo[11] = "DD/MM/YYYY";

    //"data,hora,s0u,s0t,s1u,s1t,s2u,s2t,s3u,s3t,s4u,s4t,s5u,s5t,s6u,s6t,s7u,s7t,s8u,s8t,s9u,s9t,s10u,s10t,s11u,s11t"
    String datalog = String(now.toString(dataArquivo)) + "," + String(now.toString(horarioArquivo)) + getString();
    // Salva no arquivo datalog
    Serial.println(datalog);

    //Salvando no Cartão SD, arquivo datalog
    appendFile(SD, "/datalog.csv", datalog.c_str());
    SD.end();
    // Reinicia o somatório dos dados coletados dos sensores.
    zeraVariaveis();
    intervalo_60s = tempoAtual;
    controle = 0;
  }

}


