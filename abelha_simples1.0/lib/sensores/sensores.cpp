/**
 * @file sensores.cpp
 * @author Eduardo José dos Santos (eduardo22santos@hotmail.com)
 * @brief Definições de classes, funções e métodos para obter, calcular e gerenciar as variáveis térmicas.
 * @version 2.0
 * @date 2023-07-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <sensores.h>

DHT_Unified s0(pin0,DHT22);
DHT_Unified s1(pin1,DHT22);
DHT_Unified s2(pin2,DHT22);
DHT_Unified s3(pin3,DHT22);
DHT_Unified s4(pin4,DHT22);
DHT_Unified s5(pin5,DHT22);
DHT_Unified s6(pin6,DHT22);


Dados sensor0(&s0);
Dados sensor1(&s1);
Dados sensor2(&s2);
Dados sensor3(&s3);
Dados sensor4(&s4);
Dados sensor5(&s5);
Dados sensor6(&s6);

Dados sensores[] = {sensor0,sensor1,sensor2,sensor3,sensor4,
                        sensor5,sensor6};

float ldr_1 = 0;
float ldr_2 = 0;


void realizarLeitura()
{

    for (size_t i = 0; i < 7; i++)
    {
        sensors_event_t event;
        Serial.print("\nsensor: ");Serial.println(i);
        sensores[i].sensor->temperature().getEvent(&event);
        if (isnan(event.temperature)) {
            Serial.println(F("Error reading temperature!"));
        }
        else {
            Serial.print(F("Temperature: "));
            Serial.print(event.temperature);
            Serial.println(F("°C"));
            sensores[i].temperatura += event.temperature;
        }
        // Get humidity event and print its value.
        sensores[i].sensor->humidity().getEvent(&event);
        if (isnan(event.relative_humidity)) {
            Serial.println(F("Error reading humidity!"));
        }
        else {
            Serial.print(F("Humidity: "));
            Serial.print(event.relative_humidity);
            Serial.println(F("%"));
            sensores[i].umidadeRelativa += event.relative_humidity;
        }
    }
    float ld1 = analogRead(ldr1);
    float ld2 = analogRead(ldr2);
    ldr_1 += ld1;
    ldr_2 += ld2;
    Serial.println();
    Serial.print("Ldr1: ");Serial.println(ld1);
    Serial.print("Ldr2: ");Serial.println(ld2);
    Serial.println();
}

void sensoresBegin()
{
    for (size_t i = 0; i < 7; i++)
    {
        sensores[i].sensor->begin();
    }
}

float Dados::getTemperatura()
{
    return temperatura/10;
}
float Dados::getUmidade()
{
    return umidadeRelativa/10;
}
void Dados::zeraVariaveis()
{
    temperatura = 0;
    umidadeRelativa = 0;
}
void zeraVariaveis()
{
    for (size_t i = 0; i < 7; i++)
    {
        sensores[i].zeraVariaveis();
    }
}
String getString()
{
    String datalog;
    for (size_t i = 0; i < 7; i++)
    {
        datalog.operator+=(String(","+String(sensores[i].getUmidade())+","+String(sensores[i].getTemperatura())));
    }
    datalog.operator+=(String(","+String(getLdr1())+","+String(getLdr2())));
    return datalog;
}
float getUmidade(size_t i)
{
    return sensores[i].getUmidade();
}
float getTemperatura(size_t i)
{
    return sensores[i].getTemperatura();
}
float getLdr1()
{
    return ldr_1/10;
}
float getLdr2()
{
    return ldr_2/10;
}