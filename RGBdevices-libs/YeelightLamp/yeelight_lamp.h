/*
 * Управление лампой Mijia Yeelight bedside 2. Model: MJCTD01YL, CMIIT ID: 2016DP7551
 * Лампа должна быть предварительно настроена для работы в WIFI сети (SSID & password),
 * а также у лампы должен быть включен режим разработчика. . 
 */

#if !defined(yeelight_lamp_h)
#define yeelight_lamp_h

#include <Arduino.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>

enum class YeelightLampCommandStatus {
  csOK,
  csNotConnected,
  csTimeout,
  csPowerOff,
  csNoAnswer,
  csNotCompleted,
  csNone
};

class YeelightLamp {
    public:
        YeelightLamp();
    
	void start();
	bool present();
	void find();
	void setPrintDebug(bool on); // on - включает/выключает вывод отладочной информации в Serial

	// st = true - включает, st = false - выключает устройство
	bool setPower(bool st, unsigned int smooth_time);
	bool setRGB(byte r, byte g, byte b, unsigned int smooth_time);
	bool setBrightness(byte brightness, unsigned int smooth_time);
	bool setAnimation(String animation);

        void (*pfnCommandStatus)(YeelightLampCommandStatus status); // callback вызывается при отправке команды на лампу,  
        YeelightLampCommandStatus commandStatus = YeelightLampCommandStatus::csNone;
        const char* commandStatusToText(YeelightLampCommandStatus commandStatus);
        
    private:
        static const unsigned int incomingPacketLen = 1000;
	char incomingPacket[incomingPacketLen];

	bool bFound = false;
	char Id[20] = {'\0'};
	String IP = "";

	WiFiClient tcpClient;
	WiFiUDP Udp;
        bool printDebug = false;

	// searching yeelight_lamp
	unsigned long findyeelight_lamp_PacketTime = 0;

	void sendPacket(const char* addr, unsigned int port, const char* packet);
	bool connect();
	bool sendCommand(const char* cmd);
        void setCommandStatus(YeelightLampCommandStatus commandStatus);
    };

#endif