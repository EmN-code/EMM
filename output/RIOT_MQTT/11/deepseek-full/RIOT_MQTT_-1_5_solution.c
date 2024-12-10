
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <net/sock.h>
#include <net/af.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <mutex.h>
#include <timex.h>
#include <stdatomic.h>
#include <net/sock/udp.h>
#include <architecture.h>
#include <xtimer.h>
#include <time_units.h>
#include <event/thread.h>
#include <stdbool.h>
#include <msg.h>
#include <thread.h>
#include <mbox.h>
#include <ztimer.h>
#include <MQTTClient.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 5000

static MQTTClient client;
static Network network;
static unsigned char sendbuf[2048];
static unsigned char readbuf[2048];

void delay(unsigned int milliseconds) {
    ztimer_sleep(ZTIMER_MSEC, milliseconds);
}

int connect_to_broker(const char *broker_ip, int broker_port) {
    NetworkInit(&network);
    int rc = NetworkConnect(&network, broker_ip, broker_port);
    if (rc != 0) {
        printf("Failed to connect to network\n");
        return -1;
    }

    MQTTClientInit(&client, &network, TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";

    rc = MQTTConnect(&client, &options);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    printf("Connected to MQTT broker\n");
    return 0;
}

int publish_message(const char *topic, const char *payload) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.payload = (void *)payload;
    message.payloadlen = strlen(payload);

    int rc = MQTTPublish(&client, topic, &message);
    if (rc != 0) {
        printf("Failed to publish message\n");
        return -1;
    }

    printf("Message published: %s\n", payload);
    return 0;
}

int main(void) {
    if (connect_to_broker(BROKER_IP, BROKER_PORT) != 0) {
        return -1;
    }

    while (1) {
        if (publish_message(TOPIC, PAYLOAD) != 0) {
            break;
        }
        delay(5000);
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}
