/*                      OpenWheatherMap_ESP32                   *
 *                      =====================                   *
 *                                                              *
 * Code to query the OpenWheatherMap service which              * 
 * provides weather forecast in JSON format                     *
 *                                                              *
 * Author: Giacomo Mancin                                       *
 *                                                              */

#include <Arduino.h>
#include "WiFi.h"
#include "ArduinoJson.h"
#include "HTTPClient.h"
#include <TimeLib.h>
#define MY_WIFI1_SSID ""
#define MY_WIFI1_PASS ""

unsigned long t1;
WiFiServer server(80);


void setup(){
  Serial.begin(9600);
  delay(1000);
  Serial.println("WiFi Client");
  WiFi.begin(MY_WIFI1_SSID, MY_WIFI1_PASS);
  while ((WiFi.status() != WL_CONNECTED)){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnect");

  Serial.print("IP: "); Serial.println(WiFi.localIP());     
  Serial.print("Subnet mask: "); Serial.println(WiFi.subnetMask()); 
  
  t1 = millis();
  // Start the server
  server.begin();
}

void loop(){
  if((millis() - t1) > 10000){
    if(WiFi.status() == WL_CONNECTED){
        // Web service call via WiFiClient and HTTPClient
        WiFiClient client;    
        HTTPClient http;  
        
        // OpenWeatherMap api address with city id
        String addr = "";
        
        
        http.begin(client, addr);           
        int code = http.GET();              
        String json = "{}";                 

        Serial.print("HTTP Response code: ");
        Serial.println(code);

        if(code > 0){
            json = http.getString();                // http server response
            Serial.println(json);
            DynamicJsonDocument doc(1024);          // doc json 1024 byte
            deserializeJson(doc, json);             // Decoding
            JsonObject obj = doc.as<JsonObject>();  

            // Assign JSON values to variables
            float lon = doc["coord"]["lon"];
            float lat = doc["coord"]["lat"];
            int weather_id = doc["weather"][0]["id"];
            const char* weather_main = doc["weather"][0]["main"];
            const char* weather_description = doc["weather"][0]["description"];
            const char* weather_icon = doc["weather"][0]["icon"];
            const char* base = doc["base"];
            float temp = doc["main"]["temp"];
            float feels_like = doc["main"]["feels_like"];
            float temp_min = doc["main"]["temp_min"];
            float temp_max = doc["main"]["temp_max"];
            int pressure = doc["main"]["pressure"];
            int humidity = doc["main"]["humidity"];
            int visibility = doc["visibility"];
            float wind_speed = doc["wind"]["speed"];
            int wind_deg = doc["wind"]["deg"];
            float wind_gust = doc["wind"]["gust"];
            int clouds_all = doc["clouds"]["all"];
            long dt = doc["dt"];
            int sys_type = doc["sys"]["type"];
            int sys_id = doc["sys"]["id"];
            const char* sys_country = doc["sys"]["country"];
            long timezone = doc["timezone"];
            long id = doc["id"];
            const char* name = doc["name"];
            int cod = doc["cod"];

            time_t now_sunrise = doc["sys"]["sunrise"];
            // Italian time zone, CET
            now_sunrise += 2 * 3600;
            struct tm* tLocalInfo_sunrise = localtime(&now_sunrise);
            char sunrise[35];
            strftime(sunrise, sizeof(sunrise), "%A %d/%m/%Y - %T", tLocalInfo_sunrise);

            time_t now_sunset = doc["sys"]["sunset"];
            // Italian time zone (CET)
            now_sunset += 2 * 3600;
            struct tm* tLocalInfo_sunset = localtime(&now_sunset);
            char sunset[35];
            strftime(sunset, sizeof(sunset), "%A %d/%m/%Y - %T", tLocalInfo_sunset);

            Serial.print("Response Code: ");
            Serial.println(cod);

            WiFiClient client = server.available();
            if(client){ 
                Serial.println("new req"); 
                String line = "";  
                while(client.connected()){ 
                    if(client.available()){
                        char c = client.read();
                        //Serial.print(c);
                        if(c == '\n'){
                            if(line.length() == 0){ 
                                client.println("HTTP/1.1 200 OK");
                                client.println("Content-type:text/html");
                                client.println("");
                                client.println("<html>");
                                client.println("<head><title>ESP32</title>");
                                // Refresh every 2sec
                                client.println("<meta http-equiv=\"refresh\" content='2;url=http://");
                                // Server address within refresh tag
                                client.print(WiFi.localIP());
                                client.print("/'>");
                                client.println("</head>");                             
                                client.println("<body>");
                                client.println("<h1>ESP32 Weather</h1>");
                                client.println("<h2>City name: ");
                                client.println(name);
                                client.println("</h2>");   
                                client.println("<p>Longitude: ");
                                client.println(lon); 
                                client.println("</p>");
                                client.println("<p>Latitude: ");
                                client.println(lat);
                                client.println("</p>");
                                client.println("<p>weather_main: ");
                                client.println(weather_main);
                                client.println("</p>");    
                                client.println("<p>weather_description: ");
                                client.println(weather_description);
                                client.println("</p>");           
                                client.println("<p>Temperature: ");
                                client.print(temp); client.println(" C");
                                client.println("</p>");
                                client.println("<p>Temperature feels like: ");
                                client.print(feels_like); client.println(" C");
                                client.println("</p>");   
                                client.println("<p>Temperature min: ");
                                client.print(temp_min); client.println(" C");
                                client.println("</p>");  
                                client.println("<p>Temperature max: ");
                                client.print(temp_max); client.println(" C");
                                client.println("</p>"); 
                                client.println("<p>Pressure: ");
                                client.print(pressure); client.println(" hPa");
                                client.println("</p>");
                                client.println("<p>Humidity: ");
                                client.print(humidity); client.println("%");
                                client.println("</p>");
                                client.println("<p>Visibility: ");
                                client.print(visibility); client.println("m");
                                client.println("</p>"); 
                                client.println("<p>Wind Speed: ");
                                client.print(wind_speed); client.println("m/s");
                                client.println("</p>");
                                client.println("<p>Wind direction in deg: ");
                                client.print(wind_deg); 
                                client.println("</p>");
                                client.println("<p>Wind gust in deg: ");
                                client.print(wind_gust); 
                                client.println("</p>");
                                client.println("<p>Sunrise: ");
                                client.println(sunrise);
                                client.println("</p>");   
                                client.println("<p>Sunset: ");
                                client.println(sunset);
                                client.println("</p>");    
                                                                                                                                                                                                                                      
                                client.println("</body>");
                                client.println("</html>");
                                break;
                            }
                            line = "";
                        }
                        else if(c != '\r'){
                            line += c;
                        }
                    }
                }
          }
          else{
          Serial.print("HTTP error: ");
          Serial.println(code);
          }
          http.end();

      }
      t1 = millis();
    }
  }
}