#include <WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "define";
const char* password = "11223344";
const char* mqtt_server = "m9.wqtt.ru"; //ip или http адрес
int mqtt_port = 14720; //порт
const char* mqtt_login="u_V2ITJ2"; //логин
const char* mqtt_pass="rJJtMNKJ"; //пароль

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

 delay(10);
 // We start by connecting to a WiFi network
 Serial.println();
 Serial.print("Connecting to ");
 Serial.println(ssid);

 WiFi.mode(WIFI_STA);
 WiFi.begin(ssid, password);

 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 }

 randomSeed(micros());

 Serial.println("");
 Serial.println("WiFi connected");
 Serial.println("IP address: ");
 Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
 Serial.print("Message arrived [");
 Serial.print(topic);
 Serial.print("] ");
 for (int i = 0; i < length; i++) {
 Serial.print((char)payload[i]);
 }
 Serial.println();

 // Switch on the LED if an 1 was received as first character
 if ((char)payload[0] == '1') {
 digitalWrite(2, LOW); // Turn the LED on (Note that LOW is the voltage level
 // but actually the LED is on; this is because
 // it is active low on the ESP-01)
 } else {
 digitalWrite(2, HIGH); // Turn the LED off by making the voltage HIGH
 }

}

void reconnect() {
 // Loop until we're reconnected
 while (!client.connected()) {
 Serial.print("Attempting MQTT connection...");
 // Create a random client ID
 String clientId = "esp32";
 clientId += String(random(0xffff), HEX);
 // Attempt to connect
 if (client.connect(clientId.c_str(),mqtt_login,mqtt_pass)) {
 Serial.println("connected");
 // Once connected, publish an announcement...
 client.publish("outTopic", "hello world");
 // ... and resubscribe
 client.subscribe("power");
 } else {
 Serial.print("failed, rc=");
 Serial.print(client.state());
 Serial.println(" try again in 5 seconds");
 // Wait 5 seconds before retrying
 delay(5000);
 }
 }
}

void setup() {
 pinMode(2, OUTPUT); // Initialize the BUILTIN_LED pin as an output
 Serial.begin(115200);
 setup_wifi();
 client.setServer(mqtt_server, mqtt_port);
 client.setCallback(callback);
}

void loop() {

 if (!client.connected()) {
 reconnect();
 }
 client.loop();
}