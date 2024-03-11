/**
 * @file ssd_rtc.cpp
 * @author Eduardo José dos Santos (eduardo22santos@hotmail.com)
 * @brief Definições de funções para gerenciar os módulos de rtc e cartão de memôria, 
 * além de gerenciar memória para salvar as variáveis de configuração.
 * @version 2.0
 * @date 2023-07-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <ssd_rtc.h>

Preferences preferences;
RTC_DS3231 rtc;

bool rtcBegin()
{
    if (rtc.begin())
    {
        rtc.disable32K();
        rtc.clearAlarm(1);
        rtc.clearAlarm(2);
        rtc.writeSqwPinMode(DS3231_OFF);
        rtc.disableAlarm(2);
        return true;
    }else
    {
        return false;
    }
}

bool verificarRtc()
{
    if (rtc.lostPower())
    {
        return false;
    }else if (!rtc.now().isValid())
    {
        return false;
    }else if (rtc.now().year() == 2000)
    {
        return false;
    }else
    {
        return true;
    }
}

void salvarConfiguracao(Configuracao & config)
{
    preferences.begin("my-app", false);
    preferences.clear();
    preferences.putString(String(wifiSsid).c_str(),config.wifiSsid);
    preferences.putString(String(wifiSenha).c_str(),config.wifiSenha);
    preferences.putString(String(eduroanLogin).c_str(),config.eduroanLogin);
    preferences.putString(String(eduroanSenha).c_str(),config.eduroanSenha);
    preferences.putString(String(mqttHostname).c_str(),config.mqttHostname);
    preferences.putString(String(mqttSenha).c_str(),config.mqttSenha);
    preferences.putString(String(mqttUser).c_str(),config.mqttUser);
    preferences.putString(String(mqttTopico).c_str(),config.mqttTopico);
    preferences.putString(String(servidorNtp).c_str(),config.servidorNtp);
    preferences.putString(String(hostTest).c_str(),config.hostTest);
    preferences.putInt(String(mqttPort).c_str(),config.mqttPort);
    preferences.putInt(String(timeZone).c_str(),config.timeZone);
    preferences.putInt(String(intervaloOnline).c_str(),config.intervaloOnline);
    preferences.putInt(String(intervaloSalvar).c_str(),config.intervaloSalvar);
    preferences.putBool(String(mqttStatus).c_str(),config.mqttStatus);
    preferences.putBool(String(internetStatus).c_str(),config.internetStatus);
    preferences.putBool(String(eduroamStatus).c_str(),config.eduroamStatus);
    preferences.end();
    esp_restart();
}

Configuracao carregarConfiguracao()
{
    Configuracao config;

    if(preferences.begin("my-app",true))
    {
        strlcpy(config.wifiSsid, preferences.getString(String(wifiSsid).c_str()).c_str(), sizeof(config.wifiSsid));
        strlcpy(config.wifiSenha, preferences.getString(String(wifiSenha).c_str()).c_str(), sizeof(config.wifiSenha));
        strlcpy(config.eduroanLogin, preferences.getString(String(eduroanLogin).c_str()).c_str(), sizeof(config.eduroanLogin));
        strlcpy(config.eduroanSenha, preferences.getString(String(eduroanSenha).c_str()).c_str(), sizeof(config.eduroanSenha));
        strlcpy(config.mqttHostname, preferences.getString(String(mqttHostname).c_str()).c_str(), sizeof(config.mqttHostname));
        strlcpy(config.mqttSenha, preferences.getString(String(mqttSenha).c_str()).c_str(), sizeof(config.mqttSenha));
        strlcpy(config.mqttUser, preferences.getString(String(mqttUser).c_str()).c_str(), sizeof(config.mqttUser));
        strlcpy(config.mqttTopico, preferences.getString(String(mqttTopico).c_str()).c_str(), sizeof(config.mqttTopico));
        strlcpy(config.servidorNtp, preferences.getString(String(servidorNtp).c_str()).c_str(), sizeof(config.servidorNtp));
        strlcpy(config.hostTest, preferences.getString(String(hostTest).c_str()).c_str(), sizeof(config.hostTest));
        config.mqttPort = preferences.getInt(String(mqttPort).c_str());
        config.timeZone = preferences.getInt(String(timeZone).c_str());
        config.intervaloOnline = preferences.getInt(String(intervaloOnline).c_str());
        config.intervaloSalvar = preferences.getInt(String(intervaloSalvar).c_str());
        config.mqttStatus = preferences.getBool(String(mqttStatus).c_str());
        config.internetStatus = preferences.getBool(String(internetStatus).c_str());
        config.eduroamStatus = preferences.getBool(String(eduroamStatus).c_str());
        preferences.end();        
        return config;
    }else
    {

        preferences.begin("my-app", false);
        preferences.clear();
        preferences.putString(String(wifiSsid).c_str(),String("eduroam"));
        preferences.putString(String(wifiSenha).c_str(),String("Edu.gtx66"));
        preferences.putString(String(eduroanLogin).c_str(),"eduardo22santos@ufs.br");
        preferences.putString(String(eduroanSenha).c_str(),"Edu.gtx66");
        preferences.putString(String(mqttHostname).c_str(),"hostMqtt");
        preferences.putString(String(mqttSenha).c_str(),"senhaMqtt");
        preferences.putString(String(mqttUser).c_str(),"usuarioMqtt");
        preferences.putString(String(mqttTopico).c_str(),"topicoMqtt");
        preferences.putString(String(servidorNtp).c_str(),String("a.st1.ntp.br"));
        preferences.putString(String(hostTest).c_str(),String("https://ntp.br/"));
        preferences.putInt(String(mqttPort).c_str(),1883);
        preferences.putInt(String(timeZone).c_str(),-3);
        preferences.putInt(String(intervaloOnline).c_str(),5);
        preferences.putInt(String(intervaloSalvar).c_str(),1);
        preferences.putBool(String(mqttStatus).c_str(),false);
        preferences.putBool(String(internetStatus).c_str(),true);
        preferences.putBool(String(eduroamStatus).c_str(),true);
        preferences.end();

        return carregarConfiguracao();
    }
    
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    File file = fs.open(path, FILE_APPEND);
    file.println(message);
    file.close();
}

bool loadConfiguration(const char * filename,Configuracao & config) {
    Configuracao configuracao = carregarConfiguracao();
    
    File file = SD.open(filename, "r");
    StaticJsonDocument<1536> doc;

    DeserializationError error = deserializeJson(doc, file);
    if (error)
    {
        Serial.println("Erro ao ler o arquivo de configuração no cartão SD...");
        delay(60000);
        SD.remove(filename);
        file.close();
        return false;
    }else
    {
        strlcpy(config.mqttHostname, doc["mqtt_host"] | configuracao.mqttHostname, sizeof(config.mqttHostname));
        config.mqttPort = doc["mqtt_porta"] | configuracao.mqttPort;
        strlcpy(config.wifiSsid, doc["wifi_ssid"] | configuracao.wifiSsid, sizeof(config.wifiSsid));
        strlcpy(config.wifiSenha, doc["wifi_senha"] | configuracao.wifiSenha, sizeof(config.wifiSenha));
        strlcpy(config.eduroanLogin, doc["eduroam_login"] | configuracao.eduroanLogin, sizeof(config.eduroanLogin));
        strlcpy(config.eduroanSenha, doc["eduroam_senha"] | configuracao.eduroanSenha, sizeof(config.eduroanSenha));
        strlcpy(config.mqttSenha, doc["mqtt_senha"] | configuracao.mqttSenha, sizeof(config.mqttSenha));
        strlcpy(config.mqttUser, doc["mqtt_usuario"] | configuracao.mqttUser, sizeof(config.mqttUser));
        config.timeZone = doc["fuso_horario"] | configuracao.timeZone;
        config.intervaloOnline = doc["intervalo_internet"] | configuracao.intervaloOnline;
        config.intervaloSalvar = doc["intervalo_sd"] | configuracao.intervaloSalvar;
        config.internetStatus = doc["wifi_status"] | configuracao.internetStatus;
        config.eduroamStatus = doc["eduroam_status"] | configuracao.eduroamStatus;
        config.mqttStatus = doc["plataforma_status"] | configuracao.mqttStatus;
        strlcpy(config.mqttTopico, doc["mqtt_topico"] | configuracao.mqttTopico, sizeof(config.mqttTopico));
        strlcpy(config.servidorNtp, doc["ntp_host"] | configuracao.servidorNtp, sizeof(config.servidorNtp));
        strlcpy(config.hostTest, doc["host_teste_conexao"] | configuracao.hostTest, sizeof(config.hostTest));
        SD.remove(filename);
        salvarConfiguracao(config);
        return true;
    }
}

DateTime returnHorario()
{
    DateTime tempoAtual = rtc.now();
    return tempoAtual;
}

void atualizarRtc(Configuracao & config)
{ 
    unsigned long tempo = millis();
    while (!getWifiStatus())
    {
        if ((millis() - tempo) >= 30000)
        {
            break;
        }
        updateWifi();
    }
    
    if (getWifiStatus())
    {
        HTTPClient http;
        http.begin(config.hostTest); 
        int httpCode = http.GET();
        if(httpCode == HTTP_CODE_OK)//ok, há internet!
        {
            http.end();
            
            WiFiUDP ntpUDP;
            
            NTPClient timeClient(ntpUDP, config.servidorNtp, config.timeZone * 3600, 60000); //pode-se alterar o servidor ntp
            //Iniciando a atualização do relógio rtc
            timeClient.setTimeOffset(config.timeZone*3600);
            timeClient.begin();
            vTaskDelay(1000/ portTICK_PERIOD_MS);
            timeClient.update();
            rtc.adjust(DateTime(timeClient.getEpochTime()));
            if (!verificarRtc())
            {
                ESP.restart();
            }
            
        }else //Abortando a atualização do relógio
        {
            http.end();
        }
    }    
}