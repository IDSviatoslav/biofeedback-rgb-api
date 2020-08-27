/*
 * Управление лампой Mijia Yeelight bedside 2. Model: MJCTD01YL, CMIIT ID: 2016DP7551
 * Лампа должна быть предварительно настроена для работы в WIFI сети (SSID & password),
 * а также у лампы должен быть включен режим разработчика.
 */

#include <yeelight_lamp.h>

extern "C" {
#include<user_interface.h>
}

const unsigned int searchUdpPort = 1982;
const unsigned int commandTCPPort = 55443;

const char* searchAddr       = "239.255.255.250";
const char* searchPacket     = "M-SEARCH * HTTP/1.1\r\nHOST: 239.255.255.250:1982\r\nMAN: \"ssdp:discover\"\r\nST: wifi_bulb";
const char* responseOK       = "HTTP/1.1 200 OK";
const char* responseLocation = "Location: yeelight:";
const char* responseId       = "id: 0x";
const char* responseModel    = "model: bslamp1";


YeelightLamp::YeelightLamp() {
}

void YeelightLamp::start() {
  Udp.begin(searchUdpPort);
}

void YeelightLamp::setPrintDebug(bool on) { // on - включает/выключает вывод отладочной информации в Serial
  printDebug = on;
}

bool YeelightLamp::present() {
  return bFound;
}

void YeelightLamp::find() {
  unsigned long curr_t = millis();
  if ((findyeelight_lamp_PacketTime == 0) || (curr_t - findyeelight_lamp_PacketTime > 3000)) {
    sendPacket(searchAddr, searchUdpPort, searchPacket);
    findyeelight_lamp_PacketTime = curr_t;
  }
      
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    if (printDebug) {
      Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    }

    int len = Udp.read(incomingPacket, incomingPacketLen - 1);
    if (len > 0) {
      findyeelight_lamp_PacketTime = 0;
      incomingPacket[len] = 0;
      if (printDebug) {
        Serial.printf("UDP packet contents:\n %s\n", incomingPacket);
      }

      if ((strstr(incomingPacket, responseOK) != NULL) and (strstr(incomingPacket, responseModel) != NULL)) {
        char* id = strstr(incomingPacket, responseId);
       
        if (id != NULL) {
          id += 4;
          char* id2 = id;
          int n = 0;
          while ((*id2 != '\0') && (*id2 != '\r')) id2++;
          *id2 = '\0';
            
          strncpy(Id, id, 20);
          IP = Udp.remoteIP().toString();

          if (printDebug) {
            Serial.printf("Packet OK. lamp id = %s, lamp IP = %s\n", Id, IP.c_str());
          }
          bFound = true;
        }
      }
    }
  }  
}


void YeelightLamp::sendPacket(const char* addr, unsigned int port, const char* packet) {
  Udp.beginPacket(addr, port);
  Udp.write(packet);
  Udp.endPacket();
}


bool YeelightLamp::connect() {
  if (tcpClient.connected()) {
    if (printDebug) {
      Serial.println("yeelight lamp connect() - already connected...");
    }
    return true;
  }
  
  // Use WiFiClient class to create TCP connections
  if (!tcpClient.connect(IP, commandTCPPort)) {
    if (printDebug) {
      Serial.println("yeelight lamp connect() - TCP connection failed...");
    }
    return false;
  } 

  if (printDebug) {
    Serial.println("yeelight lamp connect() - TCP connection OK...");
  }
  return true;
}


void YeelightLamp::setCommandStatus(YeelightLampCommandStatus commandStatus) {
 this->commandStatus = commandStatus;
 if (pfnCommandStatus) {
   (*pfnCommandStatus)(commandStatus);
 }
}

const char* YeelightLamp::commandStatusToText(YeelightLampCommandStatus commandStatus) {
  static const char* strCmdStatus[8] = {
    "csOK",
    "csNotConnected",
    "csTimeout",
    "csPowerOff",
    "csNoAnswer",
    "csNotCompleted",
    "csNone"
  };

  switch (commandStatus) {
    case YeelightLampCommandStatus::csOK:
      return strCmdStatus[0];
      break;
     
    case YeelightLampCommandStatus::csNotConnected:
      return strCmdStatus[1];
      break;

    case YeelightLampCommandStatus::csTimeout:
      return strCmdStatus[2];
      break;
      
    case YeelightLampCommandStatus::csPowerOff:
      return strCmdStatus[3];
      break;
      
    case YeelightLampCommandStatus::csNoAnswer:
      return strCmdStatus[4];
      break;
      
    case YeelightLampCommandStatus::csNotCompleted:
      return strCmdStatus[5];
      break;
      
    case YeelightLampCommandStatus::csNone:
      return strCmdStatus[6];
      break;
      
    default:
      return strCmdStatus[6];
      break; 
  }
}

bool YeelightLamp::sendCommand(const char* cmd) {
  if (!connect()) { 
    setCommandStatus(YeelightLampCommandStatus::csNotConnected);
    return false;
    }

  if (printDebug) {
    Serial.printf("Send command: %s", cmd);
  }
  unsigned long t = millis();
  tcpClient.print(String(cmd));
  while (tcpClient.available() == 0) { // wait data for 5 sec 
    if (millis() - t > 5000) {
      if (printDebug) {
        Serial.println("TCP Client Timeout !");
      }
      tcpClient.stop();
      setCommandStatus(YeelightLampCommandStatus::csTimeout); // disconnected
      return false;
    }
  }

  if (printDebug) {
    Serial.printf("Send command time: %u\n", millis() - t);
  }
  t = millis();
  // Read all the lines of the reply from server and print them to Serial
  setCommandStatus(YeelightLampCommandStatus::csNoAnswer);
  while(tcpClient.available()){
    String line = tcpClient.readStringUntil('\n');
    if (line.indexOf("\"ok\"") != -1) {
      setCommandStatus(YeelightLampCommandStatus::csOK);
    } else {
      if (line.indexOf("\"power\":\"off") != -1) {
        setCommandStatus(YeelightLampCommandStatus::csPowerOff);
      } else {
        if (line.indexOf("\"method\"") == -1) { // notification
          setCommandStatus(YeelightLampCommandStatus::csNotCompleted); // лампа не выполнила команду
        }
      }
    }
    if (printDebug) {
      Serial.println("Lamp response: " + line);
    }
  }  
  
  if (printDebug) {
    Serial.printf("Send command response time: %u\n", millis() - t);
  }
  
  return true;
}


bool YeelightLamp::setRGB(byte r, byte g, byte b, unsigned int smooth_time) {
  unsigned long rgb = 65536*r + 256*g + b;
  char cmd[250];
 // {"id":1,"method":"set_rgb","params":[255, "smooth", 500]}
  sprintf(cmd, "{ \"id\": %s, \"method\": \"set_rgb\", \"params\":[%lu, \"smooth\", %u]}\r\n", Id, rgb, smooth_time);
  return sendCommand(cmd);
}

bool YeelightLamp::setBrightness(byte brightness, unsigned int smooth_time) {
  char cmd[250];
 // {"id":1,"method":"set_rgb","params":[255, "smooth", 500]}
  sprintf(cmd, "{ \"id\": %s, \"method\": \"set_bright\", \"params\":[%d, \"smooth\", %u]}\r\n", Id, brightness, smooth_time);
  return sendCommand(cmd);
}

// st = true - включает, st = false - выключает устройство
bool YeelightLamp::setPower(bool st, unsigned int smooth_time) {
  String s;
  if (st) { s = "on"; } else { s = "off"; } 
  
  char cmd[250];
  sprintf(cmd, "{ \"id\": %s, \"method\": \"set_power\", \"params\":[\"%s\", \"smooth\", %u]}\r\n", Id, s.c_str(), smooth_time);
  return sendCommand(cmd);
}


bool YeelightLamp::setAnimation(String animation) {
  char cmd[250];
  sprintf(cmd, "{ \"id\": %s, \"method\": \"start_cf\", \"params\":[%s]}\r\n", Id, animation.c_str());
  return sendCommand(cmd);
}
