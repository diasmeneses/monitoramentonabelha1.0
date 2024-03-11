/**
 * @file ssd_rtc.h
 * @author Eduardo José dos Santos (eduardo22santos@hotmail.com)
 * @brief Definições de funções para gerenciar os módulos de rtc e cartão de memôria, 
 * além de gerenciar memória para salvar as variáveis de configuração.
 * @version 2.0
 * @date 2023-07-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SSD_RTC_H
#define SSD_RTC_H

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <RTClib.h>
#include <Preferences.h>
#include <NTPClient.h>
#include <HTTPClient.h>
#include <configuracao.h> //original do desenvolvedor
#include <wifi_manager.h> //original do desenvolvedor

/**
 * @brief Verifica o horario do RTC.
 * 
 * @return true se estiver tudo OK.
 * @return false se peder o horario.
 */
bool verificarRtc();

/**
 * @brief Salva as variáveis de configuração na memoria interna.
 * 
 * @attention Reinicia a placa sempre que é chamada, após salvar as informações
 * na memória ROM.
 * 
 * @param config é um objeto de configuração contendo ás variáveis para serem salvas
 * na memória ROM da placa, utilizando a biblioteca PREFERENCES da ARDUINO CORE FOR ESP32.
 */
void salvarConfiguracao(Configuracao & config);

/**
 * @brief Carrega as variáveis de confirução na memoria ram a partir da memória ROM.
 * 
 * @attention Caso não haja configuração salva, no caso da primeira inicialização, 
 * irá salvar as configurações padrões de fábrica e irá retorná-lo.
 * 
 * @return Configuracao .
 */
Configuracao carregarConfiguracao();

/**
 * @brief Adiciona uma linha no arquivo contendo a informações que serão salvas no arquivo.
 * 
 * @param fs.
 * @param path é o diretório do arquivo.
 * @param message é a informação a ser adicionada no arquivo.
 */
void appendFile(fs::FS &fs, const char * path, const char * message);

/**
 * @brief  Carrega o arquivo de configuração salvo na memória SD para a memória interna
 * que armazena as informações de configurações em variáveis locais para uso do sistema.
 * 
 * @attention Remove o arquivo de configuração no cartão de memória após a leitura.
 * @attention Caso houver erro na leitura do arquivo, irá fazer um delay de um minuto 
 * e apagar o arquivo de configurção em seguida.
 * @attention Invoca a função salvarConfiguracao(), o que fará a placa reiniciar.
 * 
 * @param filename é nome do arquivo de configuração no formato json
 * @param config é o ponteiro que aponta para o local da memória que será atualizado 
 * com as novas informações.
 * 
 * @return true se ler o arquivo corretamente e salvar as configurçãos.
 * @return false se houver erro na leitura do arquivo json.
 */
bool loadConfiguration(const char * filename,Configuracao & config);

/**
 * @brief Retorna o Horario do rtc no momento em que
 * essa função é chamada.
 * 
 * @return DateTime .
 */
DateTime returnHorario();

/**
 * @brief Atualiza o horário no módulo rtc ds3231 utilizando o ntp.
 * 
 * @attention Necessário fazer a conexão com internet antes de chamar a função.
 * @attention Fára uma checagem de conexão fazendo um get http no em host especificado 
 * no objeto confi na variável hostTest.
 * @attention Se a função verificarRtc() identificar problemas após a atualização, 
 * a placa será reiniciada.
 * @attention Se não houver conexão com WiFi, a função irá tentar restabelecer a conexão, 
 * e caso demore mais de 30 segundos, a função será encerrada sem retorno e sem atualização de horário.
 * 
 * @param config Um objeto contendo as informações de configuração do sistema.
 */
void atualizarRtc(Configuracao & config);

/**
 * @brief Inicializa o módulo rtc ds3231.
 * 
 * @return true se for identificado e inicializar com sucesso.
 * @return false se houver falha ou não for identificado.
 */
bool rtcBegin();

#endif // SSD_RTC_H