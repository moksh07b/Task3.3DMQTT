#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>

// WiFi credentials
char ssid[] = "Moksh";
char pass[] = "1234567890";

// Pin configurations
const int trigPin = 9;
const int echoPin = 10;
const int Light = 6;

// Variables for distance measurement
float duration, distance;
float speedofSnd = 0.0343;  // Speed of sound in cm/us

WiFiClient wificlient;
String lstMsg = "";

// MQTT client setup
MqttClient mqttClient(wificlient);
const char broker[] = "broker.emqx.io";
int port = 1883;
const char topic[] = "SIT210/waves";    // Topic to publish to

// Timer variables
const long interval = 1000;
long prevMillis = 0;

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);  // Set trigPin as output
  pinMode(echoPin, INPUT);   // Set echoPin as input

  connectToWiFi();           // Connect to WiFi
  connectToMQTTBroker();     // Connect to MQTT Broker
}

void loop() {
  // Reconnect to MQTT Broker if connection is lost
  if(!mqttClient.connected()){
    connectToMQTTBroker();
  }
  mqttClient.poll();  // Handle any incoming messages

  long crntMillis = millis();

  // Send measurement data at intervals
  if(crntMillis - prevMillis >= interval){
    prevMillis = crntMillis;

    distance = readMeasurement();  // Get distance measurement

    Serial.print("Distance : ");
    Serial.println(distance);

    SendMessageBasedOnDistance();  // Send message based on distance
  }
}

// Function to connect to WiFi
void connectToWiFi(){   
  Serial.print("Attempting to connect to ");
  Serial.println(ssid);

  while(WiFi.begin(ssid, pass) != WL_CONNECTED){
    Serial.print(".");
    delay(1000);
  }

  Serial.println("You're Connected");
}

// Function to connect to the MQTT Broker
void connectToMQTTBroker(){   
  Serial.print("Attempting to connect to ");
  Serial.println(broker);

  while(!mqttClient.connect(broker, port)){
    Serial.print(".");
    delay(1000);
  }
  Serial.println("You're Connected");
}

// Function to measure distance using ultrasonic sensor
float readMeasurement(){    
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 25000); // Read pulse duration

  return (duration * speedofSnd) / 2; // Calculate and return distance
}

// Function to send messages based on distance
void SendMessageBasedOnDistance(){
  if(distance > 0 && distance < 15){ // If distance is less than 15cm, send a message
    mqttClient.beginMessage(topic);
    mqttClient.print("Hi, I'm Moksh");
    mqttClient.endMessage();
    lstMsg = "Wave";
    Serial.println("Message Sent!");
    delay(5000); // Wait 5 seconds before next measurement
  }

  // if(distance > 15 && distance < 50){  // If distance is between 15cm and 50cm, send "Pat"
  //   mqttClient.beginMessage(topic);
  //   mqttClient.print("Pat");
  //   mqttClient.endMessage();
  //   lstMsg = "Pat";
  //   delay(5000);
  // }
}
