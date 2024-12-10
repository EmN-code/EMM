#include "ztimer.h"
#include "xtimer.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stdio.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "mutex.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "arpa/inet.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "netinet/in.h"
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "net/emcute.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

void initialize_mqtt(MQTTClient* client, Network* network, unsigned char* sendbuf, 
                      unsigned char* readbuf, size_t buf_size) {
    while (1) {
        int rc = NetworkConnect(network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed\n");
            xtimer_sleep(1);
            continue;
        }
        else {
            printf("Network connected successfully\n");
            break;
        }
    }
    MQTTClientInit(client, network, 1000, sendbuf, buf_size, readbuf, buf_size);
}

void publish_message(MQTTClient* client) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = false;
    message.payload = (void*)"work";
    message.payloadlen = strlen("work");

    if (MQTTPublish(client, "state", &message) != 0) {
        printf("Failed to publish message\n");
    }
    else {
        printf("Message published successfully\n");
    }
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[128];
    unsigned char readbuf[128];

    initialize_mqtt(&client, &network, sendbuf, readbuf, sizeof(sendbuf));

    while (1) {
        publish_message(&client);
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }

    return 0;
}