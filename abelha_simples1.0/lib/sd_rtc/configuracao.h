/**
 * @file configuracao.h
 * @author Eduardo José dos Santos (eduardo22santos@hotmail.com)
 * @brief Possui definições de classes responsáveis por gerenciar as 
 * variáveis de configuração de funcionamento do sistema.
 * @version 2.0
 * @date 2023-07-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef CONFIGURACAO_H
#define CONFIGURACAO_H
#include <Arduino.h>

/**
 * @brief enumerador contendo as palavras chave das variáveis de configuração
 * 
 */
enum config_
{
    /**
     * @brief SSid do wifi
     * 
     */
    wifiSsid,
    /**
     * @brief Senha do wifi
     * 
     */
    wifiSenha,
    /**
     * @brief eduroan login
     * 
     */
    eduroanLogin,
    /**
     * @brief eduroan senha
     * 
     */
    eduroanSenha,
    /**
     * @brief Porta do servidor mqtt
     * 
     */
    mqttPort,
    /**
     * @brief endereço do servidor mqtt
     * 
     */
    mqttHostname,
    /**
     * @brief Senha de acesso do servidor mqtt
     * 
     */
    mqttSenha,
    /**
     * @brief Usuario de acesso do servidor mqtt
     * 
     */
    mqttUser,
    /**
     * @brief Nome do tópico publish mqtt
     * 
     */
    mqttTopico,
    /**
     * @brief O status do servidor, ativo ou desconectado
     * 
     */
    mqttStatus,
    /**
     * @brief Endereço do servidor NTP, responsável por sincronizar o horario
     * 
     */
    servidorNtp,
    /**
     * @brief Define o fuso horário do Relógio
     * 
     */
    timeZone,
    /**
     * @brief Define o intervalo em que o aparelho irá enviar os dados online
     * 
     */
    intervaloOnline,
    /**
     * @brief Define o intervalo em que o aparelho irá salvar os dados no cartão de memória
     * 
     */
    intervaloSalvar,
    /**
     * @brief Indica o status da conexão de internet, conectado ou desconectado
     * 
     */
    internetStatus,
    /**
     * @brief Indica se a conexão wifi é do tipo enterprise
     * 
     */
    eduroamStatus,
    /**
     * @brief Endereço de um servidor para testar conexão de internet
     * 
     */
    hostTest
};

/**
 * @brief Struct de configuração, armazena as infomações de configuração do sistema como senhas e endereços.
 * 
 */
struct Configuracao{
	/**
	 * @brief SSid do wifi
	 * 
	 */
	char wifiSsid[32];
	/**
	 * @brief Senha do wifi
	 * 
	 */
	char wifiSenha[32];
	/**
	 * @brief eduroan login
	 * 
	 */
	char eduroanLogin[32];
	/**
	 * @brief eduroan senha
	 * 
	 */
	char eduroanSenha[32];
	/**
	 * @brief Porta do servidor mqtt
	 * 
	 */
	int mqttPort;
	/**
	 * @brief endereço do servidor mqtt
	 * 
	 */
	char mqttHostname[32];
	/**
	 * @brief Senha de acesso do servidor mqtt
	 * 
	 */
	char mqttSenha[32];
	/**
	 * @brief Usuario de acesso do servidor mqtt
	 * 
	 */
	char mqttUser[32];
	/**
	 * @brief Nome do tópico publish mqtt
	 * 
	 */
	char mqttTopico[32];
	/**
	 * @brief O status do servidor, ativo ou desconectado
	 * 
	 */
	bool mqttStatus;
	/**
	 * @brief Endereço do servidor NTP, responsável por sincronizar o horario
	 * 
	 */
	char servidorNtp[32];
	/**
	 * @brief Define o fuso horário do Relógio
	 * 
	 */
	int timeZone;
	/**
	 * @brief Define o intervalo em que o aparelho irá enviar os dados online.
	 * 
	 * @attention O tempo é medido em minutos.
	 * 
	 */
	float intervaloOnline;
	/**
	 * @brief Define o intervalo em que o aparelho irá salvar os dados no cartão de memória.
	 * 
	 * @attention O tempo é medido em minutos.
	 * 
	 */
	float intervaloSalvar;
	/**
	 * @brief Indica o status da conexão de internet, conectado ou desconectado.
	 * 
	 */
	bool internetStatus;
	/**
	 * @brief Indica se a conexão wifi é do tipo enterprise.
	 * 
	 */
	bool eduroamStatus;
	/**
	 * @brief Endereço de um servidor para testar conexão de internet, o padrão é usar o google.com.
	 * 
	 */
	char hostTest[32];
	/**
	 * @brief True para ativar o ssd.
	 * 
	 */
	bool ssd;
	/**
	 * @brief True para ativar o rtc.
	 * 
	 */
	bool rtc;
};

#endif // CONFIGURACAO_H