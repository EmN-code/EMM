#include "net/sock/udp.h"
#include "net/sock/tcp.h"
#include "net/sock/dtls.h"
#include "net/sock/util.h"
#include "xtimer.h"
#include "MQTTClient.h"
#include <stdio.h>
#include <string.h>

#define SOCK_ADDR_STRLEN                        (IPV6_ADDR_MAX_STR_LEN + 6)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static void mqtt_event_handler(void *arg, const emcute_topic_t *topic,
                               void *data, size_t len)
{
    (void)arg;
    (void)topic;
    (void)data;
    (void)len;
}

int main(void)
{
    /* initialize our thread message queue */
    msg_init_queue(queue, ARRAY_SIZE(queue));

    /* setup MQTT client configuration */
    MQTTClient client;
    Network network;
    unsigned char sendbuf[100], readbuf[100];
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    /* connect to MQTT broker */
    char server_ip[] = "47.102.103.1";
    int rc = NetworkConnect(&network, server_ip, 1883);
    if (rc != SUCCESS) {
        printf("Error: Could not connect to broker\n");
        return 1;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("Error: Could not connect to MQTT broker\n");
        return 1;
    }

    while (1) {
        /* publish message every 5 seconds */
        char *topic = "esp32/state";
        char *msg = "work";
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.dup = 0;
        message.payload = (void*)msg;
        message.payloadlen = strlen(msg);

        rc = MQTTPublish(&client, topic, &message);
        if (rc != SUCCESS) {
            printf("Error: Could not publish message\n");
        }
        else {
            printf("Message published: %s\n", msg);
        }

        xtimer_sleep(5);
    }

    return 0;
}