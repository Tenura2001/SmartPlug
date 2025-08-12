/*
  ADE9153A Custom Chip Arduino Driver Code
*/

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include <SPI.h>
#include "ADE9153A.h"
#include "ADE9153AAPI.h"
#define SPI_SPEED 1000000     //SPI Speed
#define CS 15
#define STATE 2
#define LED 33
ADE9153AClass ade9153A;


struct PowerRegs powerVals;    //Metrology data can be accessed from these structures
struct RMSRegs rmsVals;  

void readandwrite(void);

unsigned long lastReport = 0;
const long reportInterval = 2000;  /// Every 2s
const long SwitchToggleInterval = 2500;
const long blinkInterval = 500;

// WiFi
const char *ssid = "Wokwi-GUEST"; // Enter your WiFi name
const char *password = "";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "broker.mqttdashboard.com";
const char *topic = "Oversight_PM_In";
const char *mqtt_username = "";
const char *mqtt_password = "";
const int mqtt_port = 1883;

char buffer[200];

unsigned long lastMsg = 0;
char  randNumber;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");


WiFiClient espClient;
PubSubClient client(espClient);

/////////////////////////////////////////////////////////

void setup() {
  
  Serial.begin(115200);
  //////// TODO 1. Intialize the PinModes for the CS, STATE and LED Pins, and set STATE to LOW
      pinMode(CS, OUTPUT);
      pinMode(STATE, INPUT);
      pinMode(LED, OUTPUT);
      digitalWrite(STATE, LOW);
  ///////

  //////// TODO 2. Initialize the ADE9153A chip and SPI communication and check whether the device has been properly connected. 
  bool commscheck = ade9153A.SPI_Init(SPI_SPEED,CS); //Initialize SPI

  if (!commscheck) {
    Serial.println("ADE9153A Shield not detected. Plug in Shield and reset the Arduino");
    while (!commscheck) {     //Hold until arduino is reset
      delay(1000);
    }
  }
  ///////

 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.println("Connecting to WiFi..");
 }
 Serial.println("Connected to the WiFi network");

  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
 timeClient.begin();
 timeClient.setTimeOffset(19800);
 client.setServer(mqtt_broker, mqtt_port);
 client.setCallback(callback);
 mqttReconnect();
 client.subscribe(topic);

}

void loop() {
  if (!client.connected()) {
    mqttReconnect();
  }
  client.loop();

  /////// TODO 3. Read the Power values from the ADE9153A every reportInterval (Non-Blocking)
  unsigned long currentReport = millis();
  if ((currentReport - lastReport) >= reportInterval){
    lastReport = currentReport;
    readandwrite();
  }
  ///////

}


void readandwrite()
{ 
  /////// TODO 4. Read and Print Power and RMS Register using ADE9153A Read Library
  ade9153A.ReadPowerRegs(&powerVals);    //Template to read Power registers from ADE9000 and store data in Arduino MCU
  ade9153A.ReadRMSRegs(&rmsVals);

  ///////
  
  Serial.print("RMS Current:\t");        
  Serial.print(rmsVals.CurrentRMSValue/1000); 
  Serial.println(" A");
  
  Serial.print("RMS Voltage:\t");        
  Serial.print(rmsVals.VoltageRMSValue/1000);
  Serial.println(" V");
  
  Serial.print("Active Power:\t");        
  Serial.print(powerVals.ActivePowerValue/1000);
  Serial.println(" W");
  
  Serial.print("Reactive Power:\t");        
  Serial.print(powerVals.FundReactivePowerValue/1000);
  Serial.println(" VAR");
  
  Serial.print("Apparent Power:\t");        
  Serial.print(powerVals.ApparentPowerValue/1000);
  Serial.println(" VA");
  

  Serial.println("");
  Serial.println("");

  timeClient.update();
  
  time_t epochTime = timeClient.getEpochTime();
  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime); 
 
  int currentYear = ptm->tm_year+1900;
  int currentMonth = ptm->tm_mon+1;
  int monthDay = ptm->tm_mday;
  String formattedTime = timeClient.getFormattedTime();
 
  char month_str[4];
  sprintf(month_str,"%02d",currentMonth);
  String currentDate = String(currentYear) + "-" + month_str + "-" + String(monthDay)+" "+ String(formattedTime);
 

  DynamicJsonDocument doc(200);
  doc["deviceId"] = "0001";
  doc["time_stamp"] = currentDate;
  doc["state"] = digitalRead(STATE);
  doc["energy"] = String(powerVals.ActivePowerValue/1000,2);
  doc["durationInSeconds"] = "1";
  doc["voltage"] = String(rmsVals.VoltageRMSValue/1000,2);
  doc["current"] = String(rmsVals.CurrentRMSValue/1000,2);
  doc["kva"] = String(powerVals.ApparentPowerValue/1000,2);
  doc["valid"] = 1;
  serializeJson(doc, buffer);
  
  client.publish("Oversight_PM_Out", buffer);
}


void callback(char *topic, byte *payload, unsigned int length) {
 Serial.print("Message and subscribe arrived in topic: ");
 Serial.println(topic);
 Serial.print("Message:");
 for (int i = 0; i < length; i++) {
     Serial.print((char) payload[i]);
 }
 if ((char) payload[0] == '1'){
   digitalWrite(STATE,HIGH);
   digitalWrite(LED,HIGH);
 } else {
   digitalWrite(STATE,LOW);
   digitalWrite(LED,LOW);
   
 }
 Serial.println();
 Serial.println("-----------------------");
}

void mqttReconnect() {
 while (!client.connected()) {
     String client_id = "esp32-client-moko";
     client_id += String(WiFi.macAddress());
     Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
       if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
         Serial.println("Public broker connected");
     } else {
         Serial.print("failed with state ");
         Serial.print(client.state());
         delay(2000);
     }
 }
}