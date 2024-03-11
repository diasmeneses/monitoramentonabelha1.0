/**
 * @file wifi_manager.h
 * @author Eduardo José dos Santos (eduardo22santos@hotmail.com)
 * @brief Classe e funções para gerenciar o funcionamento da conexão rede WiFi.
 * @version 2.0
 * @date 2023-07-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <configuracao.h> // Original do desenvolvedor
#include <WiFi.h>
#include <esp_wifi.h>
#include <WiFiClientSecure.h>
#include <esp_wpa2.h> //wpa2 library for connections to Enterprise networks
#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>

// Pino para o led de status da internet
#define internetPin 15

/**
 * @brief Objeto que armazena o status da conexão com a rede WiFi.
 * 
 */
struct WifiManager
{
    bool wifiStatus = false;
};

/**
 * @brief Inicializa a conexão com a rede WiFi utilizando as credenciais 
 * disponível no objeto de configuração na entrada.
 * 
 * @param config 
 */
void initWifi(Configuracao config);

/**
 * @brief Tenta fazer a reconexão com a rede WiFi, em caso de quedas.
 * 
 * @attention A função não fará nada quando chamada em intervalos menores que 5 segundos.
 * 
 */
void reconnectWifi();

/**
 * @brief Testa a conexão com a rede WiFi.
 * 
 * @attention Chama a função reconnectWifi() quando identifica que a conexão caiu.
 * @attention Atualiza o status da variável wifiSatus do Objeto WifiManager.
 * 
 */
void updateWifi();

/**
 * @brief Retorna o valor da variável wifiStatus do objeto WifiManager.
 * 
 * @return true 
 * @return false 
 */
bool getWifiStatus();

/**
 * @brief Chama a função interna que reinicia o sistema.
 * 
 * @param time 
 */
void reiniciarPlacaPorWifiTimercallback(TimerHandle_t time);

#endif // WIFI_MANAGER_H