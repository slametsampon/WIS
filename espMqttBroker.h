/*
 * uMQTTBroker demo for Arduino (C++-style)
 * 
 * The program defines a custom broker class with callbacks, 
 * starts it, subscribes locally to anything, and publishs a topic every second.
 * Try to connect from a remote client and publish something - the console will show this as well.
 */

#ifndef espMqttBroker_h
#define espMqttBroker_h

#include <ESP8266WiFi.h>
#include "uMQTTBroker.h"

/*
 * Custom broker class with overwritten callback functions
 */
class EspMqttBroker : public uMQTTBroker
{
public:
    virtual bool onConnect(IPAddress addr, uint16_t client_count)
    {
        Serial.println(addr.toString() + " connected");
        return true;
    }

    virtual void onDisconnect(IPAddress addr, String client_id)
    {
        Serial.println(addr.toString() + " (" + client_id + ") disconnected");
    }

    virtual bool onAuth(String username, String password, String client_id)
    {
        Serial.println("Username/Password/ClientId: " + username + "/" + password + "/" + client_id);
        return true;
    }

    virtual void onData(String topic, const char *data, uint32_t length)
    {
        char data_str[length + 1];
        os_memcpy(data_str, data, length);
        data_str[length] = '\0';

        Serial.println("received topic '" + topic + "' with data '" + (String)data_str + "'");
        //printClients();
    }

    // Sample for the usage of the client info methods

    virtual void printClients()
    {
        for (int i = 0; i < getClientCount(); i++)
        {
            IPAddress addr;
            String client_id;

            getClientAddr(i, addr);
            getClientId(i, client_id);
            Serial.println("Client " + client_id + " on addr: " + addr.toString());
        }
    }
};
#endif