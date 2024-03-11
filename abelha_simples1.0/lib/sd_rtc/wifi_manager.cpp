/**
 * @file wifi_manager.cpp
 * @author Eduardo José dos Santos (eduardo22santos@hotmail.com)
 * @brief Classe e funções para gerenciar o funcionamento da conexão rede WiFi.
 * @version 2.0
 * @date 2023-07-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <wifi_manager.h>


/**
 * @brief variável para criação do timer que irá reiniciar a placa.
 * @attention Quando detectado falha na conexão com a internet, 
 * será iniciado um timer, 5 minutos, onde ao fim da contagem 
 * irá chamar a função a qual irá reiniciar a placa.
 * @attention Caso a internet volte antes da contagem, o contador 
 * deverá ser interrompido.
 */
TaskHandle_t resetBoardByWifi;


void initWifi(Configuracao config)
{
    Serial.println();
    Serial.println("Iniciando wifi...");
    Serial.println();
    //Atribuimos ao canal 0 a frequencia de 1000Hz com resolucao de 10bits.
    WiFi.disconnect(true);  //disconnect form wifi to set new wifi connection
    WiFi.mode(WIFI_STA); //init wifi mode
    if(config.eduroamStatus)
    {
        esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)config.eduroanLogin, strlen(config.eduroanLogin)); //provide identity
        esp_wifi_sta_wpa2_ent_set_username((uint8_t *)config.eduroanLogin, strlen(config.eduroanLogin)); //provide username --> identity and username is same
        esp_wifi_sta_wpa2_ent_set_password((uint8_t *)config.eduroanSenha, strlen(config.eduroanSenha)); //provide password
        esp_wifi_sta_wpa2_ent_enable();
        WiFi.begin(config.wifiSsid);
        delay(5000);
    }else
    {
        WiFi.begin(config.wifiSsid, config.wifiSenha);
        delay(5000);
    }
    resetBoardByWifi = xTimerCreate("reiniciar a placa devido wifi",pdMS_TO_TICKS(300000),pdFALSE,0,reiniciarPlacaPorWifiTimercallback);
    updateWifi();
}
void reconnectWifi()
{    
        Serial.println();
        Serial.println("tentando reconectar o wifi...");
        Serial.println();
        WiFi.reconnect();
        delay(10000);
}
void updateWifi()
{
    if (!WiFi.isConnected())
    {
        if (xTimerIsTimerActive(resetBoardByWifi) == pdFALSE)
        {
            Serial.println();
            Serial.println("Iniciando timer do wifi...");
            Serial.println();
            xTimerStart(resetBoardByWifi,pdMS_TO_TICKS(300000));
        }
        reconnectWifi();
    }else
    {
        if (xTimerIsTimerActive(resetBoardByWifi) != pdFALSE)
        {
            Serial.println();
            Serial.println("Parando o timer do wifi...");
            Serial.println();
            xTimerStop(resetBoardByWifi, 0);
        }
    }
}
void reiniciarPlacaPorWifiTimercallback(TimerHandle_t time)
{
    esp_restart();
}
bool getWifiStatus()
{
    return WiFi.isConnected();
}