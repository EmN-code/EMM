#include "msg.h"
#include "net/sock/tcp.h"
#include "xtimer.h"
#include "ztimer.h"
#include "stdio.h"
#include "MQTTClient.h"

// Define Timer structures if not already defined
struct Timer {
    ztimer_t timer;
};

// Define Network structure if not already defined
struct Network {
    sock_tcp_t sock; // TCP socket for network communication
    int (*mqttread)(Network*, unsigned char*, int, int);
    int (*mqttwrite)(Network*, unsigned char*, int, int);
};

void NetworkInit(Network *n) {
    // Initialize the network structure
    // Additional initialization code can be added here
}

int NetworkConnect(Network *n, char* address_ip, int port_number) {
    // Example function to connect to the MQTT broker
    // This should include actual network connection logic
    return 0; // Return 0 on success
}

void TimerInit(Timer *timer) {
    // Initialize the timer
    // Additional initialization code can be added here
}

void publish_state(MQTTClient *client) {
    MQTTMessage message;
    char *payload = (char *)"work";
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)payload;
    message.payloadlen = strlen(payload);
    MQTTPublish(client, "state", &message);
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[100], readbuf[100];
    int rc;
    Timer timer;

    NetworkInit(&network);
    rc = NetworkConnect(&network, "47.102.103.1", 1883);
    if (rc != 0) {
        printf("Failed to connect to network\n");
        return -1;
    }

    MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "ESP32Client";

    rc = MQTTConnect(&client, &data);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }
    
    // Publish initial state
    publish_state(&client);

    while (1) {
        // Wait for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
        // Publish state
        publish_state(&client);
    }

    return 0;
}