/**
 * @file sensores.h
 * @author Eduardo José dos Santos (eduardo22santos@hotmail.com)
 * @brief Definições de classes, funções e métodos para obter, calcular e gerenciar as variáveis térmicas.
 * @version 2.0
 * @date 2023-07-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SENSORES_H
#define SENSORES_H

#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>


#define pin0 14
#define pin1 4
#define pin2 25
#define pin3 33
#define pin4 26
#define pin5 27
#define pin6 13

#define ldr1 35
#define ldr2 32



/**
 * @brief Led indicador dos sensores.
 * 
 */
#define LED_VERMELHO 2 

/**
 * @brief Objeto contendo os dados de cada sensor
 * 
 */
struct Dados
{
    // Umidade Relativa no sensor dht22
    float umidadeRelativa = 0;
    // Temperatura ambiente no sensor dht22
    float temperatura = 0;
    DHT_Unified * sensor;
    /**
     * @brief Construct a new Dados object
     * 
     * @param sensor 
     */
    Dados(DHT_Unified * sensor)
    {
        this->sensor = sensor;
    }
    /**
     * @brief Get the Umidade object
     * 
     * @return float 
     */
    float getUmidade();
    /**
     * @brief Get the Temperatura object
     * 
     * @return float 
     */
    float getTemperatura();
    /**
     * @brief Zera os dados de umidade e temperatura em cada sensor
     * 
     */
    void zeraVariaveis();
};

/**
 * @brief Zera os dados de umidade e temperatura em todos os sensores
 * 
 */
void zeraVariaveis();

/**
 * @brief Inicializa os Sensores e o led de status dos sensores.
 * 
 */
void sensoresBegin();

/**
 * @brief Atualiza as variáveis fazendo um somatório em cada sensor.
 * 
 * @attention Liga o led de status dos sensores sempre que inicia a leitura dos sensores, 
 * a apaga o led ao fim da leitura.
 */
void realizarLeitura();
/**
 * @brief Get the Umidade object
 * 
 * @attention Divide o valor por 10, assim calculando a média de 10 leituras.
 * 
 * @param i número do sensor
 * @return float 
 */
float getUmidade(size_t i);
/**
 * @brief Get the Temperatura object
 * 
 * @attention Divide o valor por 10, assim calculando a média de 10 leituras.
 * 
 * @param i número do sensor
 * @return float 
 */
float getTemperatura(size_t i);
/**
 * @brief Obtem uma String no formato json com os dados de todos os sensores
 * 
 * @attention inicia com ","
 * 
 * @return String 
 */
String getString();
float getLdr1();
float getLdr2();
#endif // SENSORES_H