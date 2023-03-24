
#include "all_system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_MOSQUITTO_H
#include <mosquitto.h>
#endif

#define MAX_BROKERS 10

struct Broker {
    struct mosquitto* client;
    char* address;
    int port;
};

void on_message(struct mosquitto *client, void *userdata, const struct mosquitto_message *message)
{
    printf("Message received on topic %s: %s\n", message->topic, (char*)message->payload);
}

int main()
{
    struct Broker brokers[MAX_BROKERS];
    int numBrokers = 2; // Change this to the number of brokers you want to connect to

    // Define the brokers you want to connect to
    brokers[0].address = "broker1.example.com";
    brokers[0].port = 1883;

    brokers[1].address = "broker2.example.com";
    brokers[1].port = 1883;

    // Create a mosquitto object for each broker and connect to them
    for (int i = 0; i < numBrokers; i++) 
    {
        brokers[i].client = mosquitto_new(NULL, true, NULL);
        mosquitto_connect(brokers[i].client, brokers[i].address, brokers[i].port, 60);
    }

    // Subscribe to topics on each broker
    for (int i = 0; i < numBrokers; i++) 
    {
        mosquitto_subscribe(brokers[i].client, NULL, "topic", 0);
    }

    // Register the on_message callback function for each broker
    for (int i = 0; i < numBrokers; i++) 
    {
        mosquitto_message_callback_set(brokers[i].client, on_message);
    }

    // Start the mosquitto loop to receive messages
    mosquitto_loop_forever(brokers[0].client, -1, 1);

    // Disconnect from each broker and clean up
    for (int i = 0; i < numBrokers; i++) 
    {
        mosquitto_disconnect(brokers[i].client);
        mosquitto_destroy(brokers[i].client);
    }

    mosquitto_lib_cleanup();

    return 0;
}
