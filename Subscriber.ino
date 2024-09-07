#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>

char ssid[] = "realme";
char pass[] = "Realme25";

const int Light = 6;

WiFiClient wificlient;

MqttClient mqttClient(wificlient);

const char broker[] = "broker.emqx.io";
int port = 1883;

const char topic[] = "SIT210/waves";    //Topic in MQTT Client


void setup() {
  Serial.begin(9600);

  pinMode(Light, OUTPUT);

  connectToWiFi();

  connectToMQTTBroker();

  mqttClient.subscribe(topic);
  Serial.print("Subscribing to Topic: ");
  Serial.println(topic);
  Serial.println("Waiting for messages...");

}

void loop() {
  if(!mqttClient.connected()){
    connectToMQTTBroker();    //Reconnect the connection if broken
  }
  mqttClient.poll();  // Handle any incoming messages.

  
  while(mqttClient.available()){
    Serial.print("Recieved a message with topic: ");
    Serial.println(mqttClient.messageTopic());

    String message = mqttClient.readString();   
    Serial.print("Message: ");
    handleMessage(message);

    Serial.println();
  }

}

void connectToWiFi(){         // Code for Connecting to the WiFi
  Serial.print("Attempting to connect to ");
  Serial.println(ssid);

  while(WiFi.begin(ssid, pass) != WL_CONNECTED){
    Serial.print(".");
    delay(1000);
  }

  Serial.println("You're Connected");

}

void connectToMQTTBroker(){       //Code for connecting to the MQTT Broker
  Serial.print("Attempting to connect to ");
  Serial.println(broker);

  while(!mqttClient.connect(broker, port)){
    Serial.print(".");
    delay(1000);
  }
  Serial.println("You're Connected");
}

void Wave(){      // Code for Printing Wave and Blinking the LED 3 Times
  Serial.println("Wave");
  for(int i = 0; i < 3; i++){
    digitalWrite(Light, HIGH);
    delay(500);
    digitalWrite(Light, LOW);
    delay(500);
  }
}


// void Pat(){    // BLinking LED 5 Times and Printing Pat
//   Serial.println("Pat");
//   for(int i = 0; i < 5; i++){
//     digitalWrite(Light, HIGH);
//     delay(1000);
//     digitalWrite(Light, LOW);
//     delay(1000);
//   }
// }


void handleMessage(String message){   // When message is recieved it will print
  Serial.print("Received a message with topic: ");
  Serial.println(mqttClient.messageTopic());

  Serial.print("Message: ");
  Serial.println(message);

    Wave();
  // else if(message == "Pat"){
  //   Pat();
  // }
}
