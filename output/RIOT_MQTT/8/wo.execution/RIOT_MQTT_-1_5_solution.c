#include <unistd.h>  // Provides usleep function
#include <stdio.h>    // Provides perror function for error handling
#include <stdbool.h>  // Provides boolean type support in C
#include <stddef.h>   // Defines several useful types and macros
#include <stdlib.h>   // Provides support for dynamic memory allocation, random numbers etc.
#include <arpa/inet.h>  // Provides definitions for internet operations
#include <evtimer_msg.h> // Event Timer Message Handling
#include <timex.h>    // Timer extensions
#include <sys/socket.h> // Main sockets header
#include <msg.h>      // Used for message passing interfaces
#include <mbox.h>     // Used for mailbox messaging services
#include <architecture.h>  // Architecture specific definitions
#include <paho_mqtt.h>  // Paho MQTT packet definitions
#include <kernel_defines.h> // Kernel related definitions
#include <net/gcoap.h>  // GCOAP (Generic CoAP library)
#include <net/sock/util.h>  // Socket utility functions
#include <mutex.h>     // Mutex locks for thread synchronization
#include <time_units.h> // Time unit conversion utilities
#include <thread.h>    // Provides thread functionality
#include <xtimer.h>    // Used for timer functions
#include <net/emcute.h>  // Provides emcute configurations
#include <esp_system.h>  // ESP32's system related constants and declarations
#include <event/thread.h>  // For event thread handling
#include <MQTTClient.h>  // Used for MQTT client functionality
#include <ztimer.h>   // Realtime Timer support in RTOS
#include <netinet/in.h> // Provides internet protocol family
#include <evtimer_mbox.h> // Event Timer Mailbox handling
#include <net/sock/udp.h> // User Datagram Protocol (UDP) support

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUBLISH_TOPIC "state"

void *mqtt_thread(void *arg) {
    (void)arg;

    MQTTClient client;
    Network network;
    unsigned char sendbuf[80], readbuf[80];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    MQTTMessage message;
    char *payload = "work";

    // Initialize the network
    NetworkInit(&network);

    // Connect to the MQTT broker
    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
        perror("NetworkConnect");
        return NULL;
    }

    // Initialize as MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "ESP32_client";
    if (MQTTConnect(&client, &connectData) != 0) {
        perror("MQTTConnect");
        return NULL;
    }

    // Set up message properties
    message.qos = 1;
    message.retained = false;
    message.payload = (void *)payload;
    message.payloadlen = strlen(payload);

    while (1) {
        // Publish the message
        if (MQTTPublish(&client, MQTT_PUBLISH_TOPIC, &message) != 0) {
            perror("MQTTPublish");
        }
        // Sleep for 5 seconds
        usleep(5000000);
    }

    return NULL;
}

int main(void) {
    char mqtt_thread_stack[THREAD_STACKSIZE_DEFAULT];

    thread_create(mqtt_thread_stack, sizeof(mqtt_thread_stack), THREAD_PRIORITY_MAIN - 1,
                  0, mqtt_thread, NULL, "mqtt_thread");

    return 0;
}