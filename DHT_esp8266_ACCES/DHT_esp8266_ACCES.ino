
// no olvides suscribirte !!!!!!!!
  // https://www.youtube.com/channel/UCKD9PvMAW0nYi681AZbSZSQ 
// Librerias conexion Wifi - cliente ESp8266
/* 
 *  Nota:
 *  1. instalcion arduino 1.8.15
2. instalar gestor de PLACA
https://github.com/esp8266/Arduino
Colocar en el IDE en preferencias y en gestor de URL adicionaeles de tarjeta
https://arduino.esp8266.com/stable/package_esp8266com_index.json

3 instalar desde gestor de librerias
liberaria Adafruit Esp8266 
Version 1.1.0
 *  
 */



#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiMulti.h>

#include <WiFiClient.h>

// defino credenciales red
const char* ssid ="DROGUERIAHAMBURGO.DC_2_GETB";
const char* password ="L.DIANA1512";

// Variables para lectura del DHT 11
float t;
float h;
float f;
float hif;
float hic;

#include "DHT.h"

#define DHTPIN 14     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client; 

void setup() {
  Serial.begin(115200);
  Serial.println(F("DHT 11 prueba de conexión con el servidor"));
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Conectando...");
  while (WiFi.status()!= WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conexión OK!");
  Serial.print("IP Local: ");
  Serial.println(WiFi.localIP());
  

 
}

void loop() {

  LecturaTH();
  // funcion  de envio por POST
  EnvioDatos();

}


// funcion de lectura de temperatura y humedad
void LecturaTH(){

  h = dht.readHumidity();
  t = dht.readTemperature();
  f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  hic = dht.computeHeatIndex(t, h, false);

  /*

  Serial.print(F("Humidity: "));
  Serial.print(h);
 
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
 
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));  */

}

// rutina de envio de datos por POST
void EnvioDatos(){
  if (WiFi.status() == WL_CONNECTED){ 
     HTTPClient http;  // creo el objeto http
     String datos_a_enviar = "temperatura=" + String(t) + "&humedad=" + String(h);  

     http.begin(client,"http://Dominio.000webhostapp.com/EspPost.php");
     http.addHeader("Content-Type", "application/x-www-form-urlencoded"); // defino texto plano..

     int codigo_respuesta = http.POST(datos_a_enviar);

     if (codigo_respuesta>0){
      Serial.println("Código HTTP: "+ String(codigo_respuesta));
        if (codigo_respuesta == 200){
          String cuerpo_respuesta = http.getString();
          Serial.println("El servidor respondió: ");
          Serial.println(cuerpo_respuesta);
        }
     } else {
        Serial.print("Error enviado POST, código: ");
        Serial.println(codigo_respuesta);
     }

       http.end();  // libero recursos
       
  } else {
     Serial.println("Error en la conexion WIFI");
  }
  delay(60000); //espera 60s
}
