#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* SSID = "SSID";
const char* PASS = "senha";

const char* enderecoBroker = "test.mosquitto.org";
const char* topico = "10495285/ledCasa";



WiFiClient espClient;
PubSubClient client(espClient);


void callback(char* topic, byte* payload, unsigned int length) {
  
  Serial.print("Mensagem recebida [");
  Serial.print(topic);
  Serial.print("]:");
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
   }
   
  Serial.println();

  //Se a mensagem recebida for "1", o LED será ligado
  if ((char)payload[0] == '1') {
    digitalWrite(D5, LOW);   

  //Caso contrário, se a mensagem recebida for "0", o led será desligado.
  } 
  else if ((char)payload[0] == '0') {
    digitalWrite(D5, HIGH);  // Turn the LED off by making the voltage HIGH
   }

}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando se conectar ao Broker MQTT - ");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      
      Serial.println("Conectado!");

      client.subscribe(topico);
    } 
    
    else {     
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(D5, OUTPUT);  
  digitalWrite(D5, HIGH);
  
  Serial.begin(9600);
  
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(SSID,PASS);
  while (WiFi.status() != WL_CONNECTED)
   {
    delay(300);
    Serial.print(".");
   }
  Serial.print("Endereço IP obtido: ");
  Serial.println(WiFi.localIP());

  //Define qual é o servidor broker, bem como sua porta
  client.setServer(enderecoBroker, 1883);

  //Define qual é a função a ser executada quando uma mensagem é executada
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
