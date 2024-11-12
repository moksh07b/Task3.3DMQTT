#include <WiFiNINA.h>            // Library for WiFi connectivity using the NINA module
#include <ArduinoMqttClient.h>   // Library for MQTT client functionality

// WiFi network credentials
char ssid[] = "realme";           // Your WiFi network SSID (name)
char pass[] = "Realme25";         // Your WiFi network password

const int Light = 6;               // Digital pin connected to the LED

WiFiClient wificlient;             // Create a WiFi client instance
MqttClient mqttClient(wificlient); // Create an MQTT client instance using the WiFi client

// MQTT Broker details
const char broker[] = "broker.emqx.io"; // MQTT broker address
int port = 1883;                        // MQTT broker port (1883 is the default for non-secure connections)

const char topic[] = "SIT210/waves";    // MQTT topic to subscribe to

void setup() {
  Serial.begin(9600);       // Initialize serial communication at 9600 baud rate

  pinMode(Light, OUTPUT);   // Set the LED pin as an output

  connectToWiFi();          // Establish connection to the WiFi network

  connectToMQTTBroker();    // Connect to the MQTT broker

  mqttClient.subscribe(topic); // Subscribe to the specified MQTT topic
  Serial.print("Subscribing to Topic: ");
  Serial.println(topic);
  Serial.println("Waiting for messages...");
}

void loop() {
  // Check if the MQTT client is connected; if not, attempt to reconnect
  if (!mqttClient.connected()) {
    connectToMQTTBroker(); // Re-establish MQTT connection if it's lost
  }

  mqttClient.poll(); // Process incoming MQTT messages and maintain the connection

  // Handle all available incoming MQTT messages
  while (mqttClient.available()) {
    Serial.print("Received a message with topic: ");
    Serial.println(mqttClient.messageTopic());

    String message = mqttClient.readString(); // Read the incoming message as a string
    Serial.print("Message: ");
    handleMessage(message); // Process the received message

    Serial.println();
  }
}

// Function to connect to the WiFi network
void connectToWiFi() {
  Serial.print("Attempting to connect to ");
  Serial.println(ssid);

  // Attempt to connect to WiFi until successful
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print("."); // Indicate connection attempt
    delay(1000);       // Wait for 1 second before retrying
  }

  Serial.println("\nYou're Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); // Print the assigned IP address
}

// Function to connect to the MQTT broker
void connectToMQTTBroker() {
  Serial.print("Attempting to connect to MQTT Broker: ");
  Serial.println(broker);

  // Attempt to connect to the MQTT broker until successful
  while (!mqttClient.connect(broker, port)) {
    Serial.print("."); // Indicate connection attempt
    delay(1000);       // Wait for 1 second before retrying
  }

  Serial.println("\nConnected to MQTT Broker");
}

// Function to handle the "Wave" command: print "Wave" and blink the LED 3 times
void Wave() {
  Serial.println("Wave");
  for (int i = 0; i < 3; i++) {
    digitalWrite(Light, HIGH); // Turn the LED on
    delay(500);                // Wait for 500 milliseconds
    digitalWrite(Light, LOW);  // Turn the LED off
    delay(500);                // Wait for 500 milliseconds
  }
}

/*
void Pat() { // Function to handle the "Pat" command: print "Pat" and blink the LED 5 times
  Serial.println("Pat");
  for (int i = 0; i < 5; i++) {
    digitalWrite(Light, HIGH); // Turn the LED on
    delay(1000);               // Wait for 1 second
    digitalWrite(Light, LOW);  // Turn the LED off
    delay(1000);               // Wait for 1 second
  }
}
*/

// Function to process incoming MQTT messages
void handleMessage(String message) {
  Serial.print("Received a message with topic: ");
  Serial.println(mqttClient.messageTopic());

  Serial.print("Message: ");
  Serial.println(message);

  // Determine the action based on the message content
  if (message == "Wave") {
    Wave(); // Call the Wave function
  }
  // Uncomment the following lines if you want to handle the "Pat" command
  /*
  else if (message == "Pat") {
    Pat(); // Call the Pat function
  }
  */
}
