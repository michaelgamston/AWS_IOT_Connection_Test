#include <Arduino.h>

//#include <cvzone.h>

#include <Confidential.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>

#define LED_WS2812B   0

#define LTE_RTS   32
#define LTE_TX    1
#define LTE_RX    3
#define LTE_CTS   35
#define LORA_WAKE 38

#define LORA_SS   18
#define LORA_CLK  5
#define LORA_MOSI 22
#define LORA_MISO 19
#define LORA_INT  23

#define AWS_IOT_PUBLISH_TOPIC "TEST_LAMP_1/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "TEST_LAMP_1/sub"
#define LAMP_GPIO_PIN  2  // 36
#define ERROR_GPIO_PIN 2  // Error indicator.

// Local Functions.

void messageHandler(char* topic, byte* payload, unsigned int length);
void indicate_error(void);

// Global Variables.

std::string wifi_ssid;
std::string wifi_password;

WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);

void connectAWS()
{
 Serial.println("Attempting to connect to Wi-Fi");
 Serial.println(wifi_ssid.c_str());
 Serial.println(wifi_password.c_str());

 WiFi.mode(WIFI_STA);
 // WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

 WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());
  
 while( WiFi.status() != WL_CONNECTED)
 {
  delay(500);
  Serial.print(".");
 }

 Serial.println("\nConnected to Wi-Fi");          // *** Debug ***
 
 // Configure WiFiClientSecure to use the AWS IoT device credentials
 
 Serial.println("Configuring WiFiClientSecure");  // *** Debug ***
 net.setCACert(AWS_CERT_CA);             // AWS certificate authority.
 net.setCertificate(AWS_CERT_CRT);       // AWS_CERT_CA Device certificate.
 net.setPrivateKey(AWS_CERT_PRIVATE);    // Device private key.

 // Connect to the MQTT broker on the AWS endpoint we defined earlier
 
 Serial.println("Setting client's endpoint");     // *** Debug ***
 client.setServer(AWS_IOT_ENDPOINT, 8883);

 // Create a message handler
 client.setCallback(messageHandler);

 Serial.println("Connecting to AWS IOT");         // *** Debug ***

 uint8_t client_connection_attempts = 0;

 while(!client.connect(THINGNAME)) 
 {
  Serial.print(".");
  delay(500);  // 100
  client_connection_attempts++;

  if (client_connection_attempts > 2)
   break;
 }
 
 if (!client.connected())
 {
  Serial.println("\nAWS IoT Timeout!");
  indicate_error();
  return;
 }

 // Subscribe to a topic
 Serial.println("Subscribing to topic");         // *** Debug ***
 client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

 Serial.println("AWS IoT Connected!");
}

void messageHandler(char* topic, byte* payload, unsigned int length)
{
 Serial.print("Incoming: ");
 Serial.println(topic);

 StaticJsonDocument<200> doc;
 deserializeJson(doc, payload);
 const char* message = doc["message"];
 Serial.println();

 for(int i=0; i<length; i++)
 {
  Serial.print((char)payload[i]); // Pring payload content
 }

 char led = (char)payload[62]; // Extracting the controlling command from the Payload to control LED from AWS
 Serial.print("Command: ");
 Serial.println(led);

 if(led == 0x32) // 49 is the ASCI value of 1
 {
  digitalWrite(LAMP_GPIO_PIN, HIGH);
  Serial.println("Lamp_State changed to HIGH");
 }  
 
 else if(led == 48) // 48 is the ASCI of 0
 {
  digitalWrite(LAMP_GPIO_PIN, LOW);
  Serial.println("Lamp_State changed to LOW");
 }

 Serial.println();
}

void indicate_error(void)
{
 for (uint8_t i = 0; i < 4; i++)
 {
  digitalWrite(ERROR_GPIO_PIN, HIGH);
  delay(500);
  digitalWrite(ERROR_GPIO_PIN, LOW);
  delay(500);
 }
}

void setup() 
{
 pinMode (LAMP_GPIO_PIN, OUTPUT);
 digitalWrite(LAMP_GPIO_PIN, LOW);
 pinMode (ERROR_GPIO_PIN, OUTPUT);
 digitalWrite(ERROR_GPIO_PIN, LOW);

 Serial.begin(115200);
 Serial.println("Calling connectAWS().");

 wifi_ssid     = WIFI_SSID;
 wifi_password = WIFI_PASSWORD;
  
 connectAWS();
}

void loop() 
{
 client.loop();
 delay(1000);
}
