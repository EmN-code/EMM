#include <ztimer.h>
#include <stdio.h>
#include <string.h>
#include <mqtt_client.h>
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <net/netif.h>
#include <net/sock/udp.h>
#include <net/network_layer.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define BUFFER_SIZE 128
#define PUBLISH_INTERVAL (5U * US_PER_SEC) // 5 seconds in microseconds

static char buffer[BUFFER_SIZE];
static sock_udp_ep_t server;
static MQTTClient client;
static Network network;

void mqtt_publish_task(void) {
    MQTTMessage message;
    int rc;

    // Initialize the address
    server.family = AF_INET6;
    ipv6_addr_from_str((ipv6_addr_t *)&server.addr.ipv6, SERVER_ADDR);
    server.port = SERVER_PORT;

    while (1) {
        // Prepare the message
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void *)"work";
        message.payloadlen = strlen("work");

        // Publish the message
        rc = MQTTPublish(&client, "state/status", &message);
        if (rc != SUCCESS) {
            printf("Error publishing message: %d\n", rc);
        } else {
            printf("Message published successfully\n");
        }

        // Wait for the next interval
        ztimer_sleep(ZTIMER_SEC, 5);
    }
}

int main(void) {
    unsigned char sendbuf[BUFFER_SIZE];
    unsigned char readbuf[BUFFER_SIZE];
    int rc;

    // Initialize the network structure
    NetworkInit(&network);

    // Initialize the MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, BUFFER_SIZE, readbuf, BUFFER_SIZE);

    // Connect to the server
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "ESP32Client";
    rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        return -1;
    }

    printf("Connected to MQTT broker\n");

    // Start the publish task
    mqtt_publish_task();

    return 0;
}