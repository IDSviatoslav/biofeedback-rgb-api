/*
 * ���������� ������ Mijia Yeelight bedside 2. Model: MJCTD01YL, CMIIT ID: 2016DP7551
 * ����� ������ ���� �������������� ��������� ��� ������ � WIFI ���� (SSID & password),
 * � ����� � ����� ������ ���� ������� ����� ������������. . 
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
	void setPrintDebug(bool on); // on - ��������/��������� ����� ���������� ���������� � Serial

	// st = true - ��������, st = false - ��������� ����������
	bool setPower(bool st, unsigned int smooth_time);
	bool setRGB(byte r, byte g, byte b, unsigned int smooth_time);
	bool setBrightness(byte brightness, unsigned int smooth_time);
	bool setAnimation(String animation);

        void (*pfnCommandStatus)(YeelightLampCommandStatus status); // callback ���������� ��� �������� ������� �� �����,  
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