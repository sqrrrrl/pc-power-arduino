#include <ArduinoHttpClient.h>
#include <Arduino_JSON.h>
#include "WiFiS3.h"
#include "environment.h"

const int POWER_CONTROL_PIN = 3;
const int RESET_CONTROL_PIN = 9;
const uint8_t LED = A4; // LED+ Analog pin
const int PRESS_TIME = 400;
const int HARD_SHUTDOWN_PRESS_TIME = 5000;
const int RETRY_TIMEOUT = 1000;
const int ANALOG_PC_ON = 600;
const int PC_STATUS_ON = 1;
const int PC_STATUS_OFF = 0;

const char ssid[] = SECRET_SSID;
const char pass[] = SECRET_PASS;
const char serverAddress[] = API_ADDRESS;
const int serverPort = API_PORT;
const String deviceId = DEVICE_ID;
const String deviceSecret = DEVICE_SECRET;

int status = WL_IDLE_STATUS;
int lastPcStatus = PC_STATUS_OFF;
int retries = 0;
WiFiClient wifi;
WebSocketClient ws = WebSocketClient(wifi, serverAddress, serverPort);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(RETRY_TIMEOUT);
  }

  Serial.println("Connected!");
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
  delay(retries * RETRY_TIMEOUT);
  Serial.println("Attempting to connect to the server");
  if(ws.begin("/devices/gateway?device_id=" + deviceId + "&secret=" + deviceSecret) == 0){
    retries = 0;
    Serial.println("Connected!");
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
      Serial.print("Received a message: ");
      Serial.println(ws.readString());
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

int getPcStatus(){
  if(analogRead(LED) >= ANALOG_PC_ON){
    return PC_STATUS_ON;
  }
  return PC_STATUS_OFF;
}
