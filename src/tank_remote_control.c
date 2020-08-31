// TND - justy copied hre. "controller" sends the data, so it will be used for the remote control.
// ESP32 because we need 2 analog inputs for the joystick
#include "esp_now.h"


#define MY_NAME         "REMOTE_CONTROL"
#define MY_ROLE         ESP_NOW_ROLE_REMMOTE_CONTROLE         // set the role of this device: CONTROLLER, SLAVE, COMBO
#define RECEIVER_ROLE   ESP_NOW_ROLE_TANK              // set the role of the receiver
#define WIFI_CHANNEL    1



const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = 36; // analog pin connected to X output
const int Y_pin = 39; // analog pin connected to Y output

uint8_t receiverAddress[] = {0xEC, 0xFA, 0xBC, 0xC3, 0x01, 0xA0};   // please update this with the MAC address of the receiver

struct __attribute__((packed)) dataPacket {
  int tank_command; // commands: 0-stop,1-move
  int command_X_value;
  int command_Y_value;
};

void transmissionComplete(uint8_t *receiver_mac, uint8_t transmissionStatus) {
  if(transmissionStatus == 0) {
    Serial.println(" ... Data sent successfully");
  } else {
    Serial.print(" .. Error code: ");
    Serial.println(transmissionStatus);
  }
}

int measure_dist(int _trig,int _echo)
{
  long duration;
  int l_distance;
  digitalWrite(_trig, LOW);   // Makes trigPin low
  delayMicroseconds(2);       // 2 micro second delay 

  digitalWrite(_trig, HIGH);  // tigPin high
  delayMicroseconds(10);      // trigPin high for 10 micro seconds
  digitalWrite(_trig, LOW);   // trigPin low

  duration = pulseIn(_echo, HIGH);   //Read echo pin, time in microseconds
  l_distance= duration*0.034/2;        //Calculating actual/real distance
  return l_distance;
}

void setup() {
  Serial.begin(9600);     // initialize serial por
  Serial.println();
  Serial.print("Initializing...");
  Serial.print(MY_NAME);
  Serial.print("    My MAC address is: ");
  Serial.println(WiFi.macAddress());

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();        // we do not want to connect to a WiFi network

  if(esp_now_init() != 0) {
    Serial.println("ESP-NOW initialization failed");
    return;
  }

  esp_now_set_self_role(MY_ROLE);   
  esp_now_register_send_cb(transmissionComplete);   // this function will get called once all data is sent
  esp_now_add_peer(receiverAddress, RECEIVER_ROLE, WIFI_CHANNEL, NULL, 0);

  Serial.println("Initialized.");
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  distance=measure_dist(trigP,echoP);
  dataPacket packet;
  
  packet.sensor_name = sensor_name;
  packet.sensor_dist = distance;

  Serial.print("Sending ");
  Serial.print(distance);
  esp_now_send(receiverAddress, (uint8_t *) &packet, sizeof(packet));
  digitalWrite(LED_BUILTIN, LOW);
  delay(400);
}
