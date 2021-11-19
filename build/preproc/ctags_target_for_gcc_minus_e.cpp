# 1 "c:\\DATA\\Projects\\IoT\\wis\\wis.ino"
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

 *  "mode" : 1, (Manual = 0, Auto = 1)

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
# 31 "c:\\DATA\\Projects\\IoT\\wis\\wis.ino"
// Import required libraries
# 33 "c:\\DATA\\Projects\\IoT\\wis\\wis.ino" 2
# 34 "c:\\DATA\\Projects\\IoT\\wis\\wis.ino" 2
# 35 "c:\\DATA\\Projects\\IoT\\wis\\wis.ino" 2
# 36 "c:\\DATA\\Projects\\IoT\\wis\\wis.ino" 2
# 37 "c:\\DATA\\Projects\\IoT\\wis\\wis.ino" 2
# 38 "c:\\DATA\\Projects\\IoT\\wis\\wis.ino" 2

ODeDu odedu("On Delay-Duration");

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

//funtions declaration
void urlController();
void setupWifi();

void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);

  setupWifi();

  urlController();

  // Start server
  server.begin();
}

void loop()
{
  odedu.execute(SAMPLING_TIME);
}

//functions detail
void setupWifi()
{
  // Connect to Wi-Fi
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());
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
