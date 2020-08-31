// TND - justy copied hre. "controller" sends the data, so it will be used for the remote control.
// ESP32 because we need 2 analog inputs for the joystick
#include "WiFi.h"
#include "esp_now.h"


#define MY_NAME         "TANK_REMOTE_CONTROL"
#define MY_ROLE         ESP_NOW_ROLE_CONTROLLER         // set the role of this device: CONTROLLER, SLAVE, COMBO
#define RECEIVER_ROLE   ESP_NOW_ROLE_SLAVE              // set the role of the receiver
#define WIFI_CHANNEL    1



const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = 34; // analog pin connected to X output
const int Y_pin = 35; // analog pin connected to Y output

uint8_t tank_rcv_Addr[] = {0xEC, 0xFA, 0xBC, 0xC3, 0x01, 0xA0};   // please update this with the MAC address of the receiver

struct __attribute__((packed)) dataPacket {
  int tank_command; // commands: 0-stop,1-move
  int X_value;
  int Y_value;
};

void transmissionComplete(uint8_t *receiver_mac, uint8_t transmissionStatus) {
  if(transmissionStatus == 0) {
    Serial.println(" ... Data sent successfully");
  } else {
    Serial.print(" .. Error code: ");
    Serial.println(transmissionStatus);
  }
}


void setup() {
  Serial.begin(9600);     // initialize serial por
  Serial.println();
  Serial.print("Initializing...");
  Serial.print(MY_NAME);
  Serial.print("    My MAC address is: ");
  Serial.println(WiFi.macAddress());

  WiFi.mode(WIFI_STA);
  
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, tank_rcv_Addr, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
         
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
Serial.println("Initialized.");
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);
  int X_Value = analogRead(X_pin);
  int Y_Value = analogRead(Y_pin);
  
  int map_X = map(X_Value, 0, 1023, -512, 512);
  int map_Y = map(Y_Value, 0, 1023, -512, 512);


  dataPacket packet;
  
  packet.X_value = map_X;
  packet.Y_value = map_Y;

  Serial.print("Sending: ");
  Serial.print(map_X);
  Serial.print(" /  ");
  Serial.println(map_Y);

  esp_now_send(tank_rcv_Addr, (uint8_t *) &packet, sizeof(packet));
  digitalWrite(LED_BUILTIN, LOW);
  delay(400);
}
