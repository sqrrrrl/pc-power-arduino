#include <ArduinoHttpClient.h>
#include <Arduino_JSON.h>
#include "WiFiS3.h"
#include "environment.h"

const int POWER_CONTROL_PIN = 3; // POWER SW+
const int RESET_CONTROL_PIN = 5; // RESET SW+
const uint8_t LED = A4; // LED+ Analog pin
const int ARDUINO_RESET_PIN = 11;
const int PRESS_TIME = 400;
const int HARD_SHUTDOWN_PRESS_TIME = 5000;
const int RETRY_TIMEOUT = 1000;
const int WIFI_LOGIN_CHECK_INTERVAL = 100;
const int WIFI_TIMEOUT = 100000;
const int ANALOG_PC_ON = 600;
const int PC_STATUS_ON = 1;
const int PC_STATUS_OFF = 0;
const int OP_POWER_SWITCH = 1;
const int OP_RESET_SWITCH = 2;
const int OP_HARD_SHUTDOWN = 3;

const char ssid[] = SECRET_SSID;
const char pass[] = SECRET_PASS;
const char serverAddress[] = API_ADDRESS;
const int serverPort = API_PORT;
const String deviceId = DEVICE_ID;
const String deviceSecret = DEVICE_SECRET;

int retries = 0;
WiFiSSLClient wifi;
WebSocketClient ws = WebSocketClient(wifi, serverAddress, serverPort);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  int connectTime = 0;
  Serial.print("Attempting to connect to ");
  Serial.print(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    connectTime += WIFI_LOGIN_CHECK_INTERVAL;
    if(connectTime >= WIFI_TIMEOUT){
      resetArduino();
    }
    Serial.print(".");
    delay(WIFI_LOGIN_CHECK_INTERVAL);
  }

  Serial.println("\nConnected!");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void resetControlPins(){
  pinMode(POWER_CONTROL_PIN, INPUT);
  pinMode(RESET_CONTROL_PIN, INPUT);
}

void loop() {
  int lastPcStatus = PC_STATUS_OFF;
  delay(retries * RETRY_TIMEOUT);
  Serial.println("Attempting to connect to the server");
  int status = ws.begin("/devices/gateway?device_id=" + deviceId + "&secret=" + deviceSecret);
  if(status == 0){
    retries = 0;
    Serial.println("Connected!");
  }else if(status == -1){
    resetArduino();
  }else{
    Serial.print("Failed to connect with status ");
    Serial.println(status);
  }

  while (ws.connected()) {
    int pcStatus = getPcStatus();
    if(lastPcStatus != pcStatus){
      lastPcStatus = pcStatus;
      ws.beginMessage(TYPE_TEXT);
      JSONVar message;
      message["status"] = pcStatus;
      ws.print(JSON.stringify(message));
      ws.endMessage();
    }

    int messageSize = ws.parseMessage();

    if (messageSize > 0) {
      JSONVar message = JSON.parse(ws.readString());
      switch((int)message["op"]){
        case OP_POWER_SWITCH:
          pressPowerSwitch();
          break;
        case OP_RESET_SWITCH:
          pressResetSwitch();
          break;
        case OP_HARD_SHUTDOWN:
          hardShutdown();
          break;
      }
    }
    resetControlPins();
    delay(1000);
  }
  retries += 1;
}

void pressPowerSwitch() {
  pinMode(POWER_CONTROL_PIN, OUTPUT);
  digitalWrite(POWER_CONTROL_PIN, LOW);
  delay(PRESS_TIME);
}

void pressResetSwitch(){
  pinMode(RESET_CONTROL_PIN, OUTPUT);
  digitalWrite(RESET_CONTROL_PIN, LOW);
  delay(PRESS_TIME);
}

void hardShutdown(){
  pinMode(POWER_CONTROL_PIN, OUTPUT);
  digitalWrite(POWER_CONTROL_PIN, LOW);
  delay(HARD_SHUTDOWN_PRESS_TIME);
}

void resetArduino(){
  WiFi.disconnect();
  delay(10000);
  pinMode(ARDUINO_RESET_PIN, OUTPUT);
  digitalWrite(ARDUINO_RESET_PIN, LOW);
  while(true);
}

int getPcStatus(){
  if(analogRead(LED) >= ANALOG_PC_ON){
    return PC_STATUS_ON;
  }
  return PC_STATUS_OFF;
}
