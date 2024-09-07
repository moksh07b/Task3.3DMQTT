
#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>

char ssid[] = "Moksh";
char pass[] = "1234567890";

const int trigPin = 9;
const int echoPin = 10;

const int Light = 6;

float duration, distance;
float speedofSnd = 0.0343;

WiFiClient wificlient;

String lstMsg = "";

MqttClient mqttClient(wificlient);

const char broker[] = "broker.emqx.io";
int port = 1883;

const char topic[] = "SIT210/waves";    //Topic to publish too..

const long interval = 1000;
long prevMillis = 0;


void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  connectToWiFi();  // Connect to WiFi

  connectToMQTTBroker();  //Connect to MQTT Broker
}

void loop() {
  if(!mqttClient.connected()){
    connectToMQTTBroker();  // Reconnect connection if broken
  }
  mqttClient.poll();  // Handle any incoming messages.

  long crntMillis = millis();

  if(crntMillis - prevMillis >= interval){
    prevMillis = crntMillis;

    distance = readMeasurement(); // Reading Measurement

    Serial.print("Distance : ");
    Serial.println(distance);

    SendMessageBasedOnDistance();
  }

}

void connectToWiFi(){   // Connect to WiFi
  Serial.print("Attempting to connect to ");
  Serial.println(ssid);

  while(WiFi.begin(ssid, pass) != WL_CONNECTED){
    Serial.print(".");
    delay(1000);
  }

  Serial.println("You're Connected");

}

void connectToMQTTBroker(){   // Connect To MQTT Broker
  Serial.print("Attempting to connect to ");
  Serial.println(broker);

  while(!mqttClient.connect(broker, port)){
    Serial.print(".");
    delay(1000);
  }
  Serial.println("You're Connected");
}

float readMeasurement(){    // Measuring the distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 25000);

  return (duration * speedofSnd) / 2; // Returning the distance
}

void SendMessageBasedOnDistance(){
  if(distance > 0 && distance < 15){ // If distance is less than 15cm then it will send it to the Client
    mqttClient.beginMessage(topic);
    mqttClient.print("Hi, I'm Moksh");
    mqttClient.endMessage();
    lstMsg = "Wave";
    Serial.println("Message Sent!");
    delay(5000);
  }

  // if(distance > 15 && distance < 50){  // Additional code for pat if greater than 15cm
  //   mqttClient.beginMessage(topic);
  //   mqttClient.print("Pat");
  //   mqttClient.endMessage();
  //   lstMsg = "Pat";
  //   delay(5000);
  // }
}





