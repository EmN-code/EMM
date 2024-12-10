#include <assert.h>
#include <stdbool.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <arpa/inet.h>
#include <dht.h>
#include <dht_params.h>
#include <msg.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread.h>
#include <time_units.h>
#include <timex.h>
#include <xtimer.h>
#include <ztimer.h>

static char stack[THREAD_STACKSIZE_DEFAULT];

int main(void) {
    /* Initialize network */
    Network network;
    NetworkInit(&network);

    /* Connect to the network */
    int rc = NetworkConnect(&network, "47.102.103.1", 1883);
    if (rc != 0) {
        printf("Network connection failed with code: %d\n", rc);
        return -1;
    }

    /* Initialize MQTT client */
    MQTTClient client;
    unsigned char sendbuf[100], readbuf[100];
    MQTTClientInit(&client, &network, 5000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    /* Connect to MQTT broker */
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("MQTT connect failed with code: %d\n", rc);
        return -1;
    }

    /* Publishing state every five seconds */
    const char *topic = "device/state";
    MQTTMessage message;
    message.qos = 1;
    message.retained = 0;
    message.dup = 0;

    while (1) {
        message.payload = "work";
        message.payloadlen = strlen("work");
        rc = MQTTPublish(&client, topic, &message);
        if (rc != 0) {
            printf("MQTT publish failed with code: %d\n", rc);
            continue;
        }

        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}