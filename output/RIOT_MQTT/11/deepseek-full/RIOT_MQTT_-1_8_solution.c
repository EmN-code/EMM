
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/af.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <paho_mqtt.h>
#include <mutex.h>
#include <timex.h>
#include <stdatomic.h>
#include <architecture.h>
#include <xtimer.h>
#include <event/thread.h>
#include <net/gcoap.h>
#include <esp_wifi.h>
#include <MQTTClient.h>
#include <esp_system.h>
#include <net/emcute.h>
#include <msg.h>
#include <thread.h>
#include <net/sock/util.h>
#include <mbox.h>
#include <stddef.h>
#include <sys/socket.h>
#include <ztimer.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 1
#define KEEP_ALIVE 20
#define MAX_RETRIES 5

static MQTTClient client;
static Network network;
static unsigned char sendbuf[2048];
static unsigned char readbuf[2048];

int mqtt_connect(void) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = KEEP_ALIVE;
    options.cleansession = 1;
    options.MQTTVersion = 3;

    int rc = MQTTConnect(&client, &options);
    if (rc != 0) {
        printf("MQTT connect failed\n");
        return -1;
    }
    return 0;
}

int mqtt_publish(const char *topic, const char *payload) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)payload;
    message.payloadlen = strlen(payload);

    int rc = MQTTPublish(&client, topic, &message);
    if (rc != 0) {
        printf("MQTT publish failed\n");
        return -1;
    }
    return 0;
}

int network_connect(void) {
    int rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    if (rc != 0) {
        printf("Network connect failed\n");
        return -1;
    }
    return 0;
}

int main(void) {
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    int retry_count = 0;
    while (retry_count < MAX_RETRIES) {
        if (network_connect() == 0) {
            break;
        }
        printf("Network connection failed, retrying...\n");
        xtimer_sleep(1);
        retry_count++;
    }
    if (retry_count == MAX_RETRIES) {
        printf("Failed to connect to network after multiple attempts\n");
        return -1;
    }

    retry_count = 0;
    while (retry_count < MAX_RETRIES) {
        if (mqtt_connect() == 0) {
            break;
        }
        printf("MQTT connection failed, retrying...\n");
        xtimer_sleep(1);
        retry_count++;
    }
    if (retry_count == MAX_RETRIES) {
        printf("Failed to connect to MQTT broker after multiple attempts\n");
        return -1;
    }

    while (1) {
        if (mqtt_publish(TOPIC, PAYLOAD) != 0) {
            printf("Failed to publish message\n");
        }
        xtimer_sleep(5);
    }

    return 0;
}
