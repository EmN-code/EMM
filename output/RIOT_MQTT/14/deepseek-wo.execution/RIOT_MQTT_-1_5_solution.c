
#include <net/ipv6/addr.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <event/thread.h>
#include <time_units.h>
#include <esp_system.h>
#include <xtimer.h>
#include <thread.h>
#include <mutex.h>
#include <mma8x5x_params.h>
#include <dht_params.h>
#include <stdio.h>
#include <ztimer.h>
#include <esp_wifi.h>
#include <stdbool.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <net/sock.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <mbox.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <MQTTClient.h>  // Ensure this is the last header file to be included

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4

static Network network;
static MQTTClient client;

static int mqtt_read(Network *n, unsigned char *buffer, int len, int timeout) {
    return recv(n->sock, buffer, len, 0);
}

static int mqtt_write(Network *n, unsigned char *buffer, int len, int timeout) {
    return send(n->sock, buffer, len, 0);
}

int main(void) {
    struct sockaddr_in server_addr;
    int sock;

    // Initialize the network structure
    network.mqttread = mqtt_read;
    network.mqttwrite = mqtt_write;

    // Create a socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        puts("Error creating socket");
        return 1;
    }

    // Set up the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        puts("Error connecting to server");
        close(sock);
        return 1;
    }

    // Assign the socket to the network structure
    network.sock = sock;

    // Initialize the MQTT client
    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

    // Connect to the MQTT broker
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "riot_client";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&client, &options) != 0) {
        puts("Error connecting to MQTT broker");
        close(sock);
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        MQTTMessage message;
        message.qos = 0;
        message.retained = 0;
        message.payload = (void *)PAYLOAD;
        message.payloadlen = PAYLOAD_LEN;

        if (MQTTPublish(&client, TOPIC, &message) != 0) {
            puts("Error publishing message");
        }

        xtimer_sleep(5);
    }

    // Disconnect from the MQTT broker
    MQTTDisconnect(&client);

    // Close the socket
    close(sock);

    return 0;
}
