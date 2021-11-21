/*********
 * WIS - Wireless Irrigation System:
 * 
 * Operation Mode :
 * Auto - (No Delay)
 * Manual - cyclic (On Delay, On Duration)
 * Manual - continuous (On DElay = 0, On Duration)
 * Manual - Off/Idle (On Duration = 0)
 * 
 * data model :
 * config = {
 *  "idNode" : 9,
 *  "mode" : 1, (Stop = 0, Manual = 1, Auto = 2)
 *  "cyclic" : 0, (One Shoot = 0, Cyclic = 1)
 *  "onDelay" : xxxx,
 *  "onDuration" : yyyy
 * }
 * status = {
 *  "status" : 0, (Idle =0, 
 *                Wait = 1, 
 *                Active = 2,
 *                Manual-One = 3,
 *                Manual-Cyc = 4,
 *                Manual-Con = 5,
 *  )
 *  "onDelay" : xxxx,
 *  "onDuration" : yyyy
 * }
*********/

// Import required libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "espMqttBroker.h"
#include "wis_html.h"
#include "oDeDu.h"
#include "wis.h"

ODeDu odedu("On Delay-Duration");
EspMqttBroker wisMqtt; //mqtt

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

//funtions declaration
void urlController();
void startWiFiClient();
void startWiFiAP();

void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);

  odedu.init(IRRIGATION_VALVE);

  // Start WiFi
  if (WiFiAP)
    startWiFiAP();
  else
    startWiFiClient();

  urlController();

  // Start server
  server.begin();
}

void loop()
{
  odedu.execute(SAMPLING_TIME);
}

//functions detail
void startWiFiClient()
{
  Serial.println("Connecting to " + (String)SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: " + WiFi.localIP().toString());
}

void startWiFiAP()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP(SSID, PASSWORD);
  Serial.println("AP started");
  Serial.println("IP address: " + WiFi.softAPIP().toString());
}

void urlController()
{
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", index_html); });

  // route to config
  server.on("/getConfig", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              String configData = odedu.getConfig();
              request->send(200, "application/json", configData);
            });

  // route to status
  server.on("/getStatus", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              String statusData = odedu.getStatus();
              request->send(200, "application/json", statusData);
            });

  // route to config
  server.on("/config", HTTP_POST, [](AsyncWebServerRequest *request)
            {
              String argData;
              config configData;

              if (request->hasArg("mode"))
              {
                argData = request->arg("mode");
                Serial.print("Operation mode is: ");
                Serial.println(argData);

                if (argData == "modeA")
                  configData.mode = 1;
                else
                  configData.mode = 0;
              }

              if (request->hasArg("cyclic"))
              {
                argData = request->arg("cyclic");
                Serial.print("Cyclic is: ");
                Serial.println(argData);

                if (argData == "cyclic")
                  configData.cyclic = 1;
                else
                  configData.cyclic = 0;
              }

              if (request->hasArg("onDelay"))
              {
                argData = request->arg("onDelay");
                Serial.print("On Delay is: ");
                Serial.println(argData);

                unsigned long onDelay = (unsigned long)argData.toInt();
                configData.onDelay = onDelay;
              }

              if (request->hasArg("onDuration"))
              {
                argData = request->arg("onDuration");
                Serial.print("On Duration is: ");
                Serial.println(argData);

                unsigned long onDuration = (unsigned long)argData.toInt();
                configData.onDuration = onDuration;
              }

              //update config data

              odedu.setConfig(configData);
            });
}
