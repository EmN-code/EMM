#include "stdio.h"
#include "stdlib.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "paho_mqtt.h"

#define SERVER_ADDR   "47.102.103.1"
#define SERVER_PORT   1883
#define BUFFER_SIZE   128

static int publish_state(void) {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;
    int keepAliveInterval = 20;

    rc = MQTTClient_create(&client, SERVER_ADDR, "ESP32Client",
                           MQTTCLIENT_PERSISTENCE_NONE, NULL);

    if (rc != MQTTCLIENT_SUCCESS) {
        return rc;
    }

    conn_opts.keepAliveInterval = keepAliveInterval;
    conn_opts.cleansession = 1;

    rc = MQTTClient_connect(client, &conn_opts);

    if (rc != MQTTCLIENT_SUCCESS) {
        MQTTClient_destroy(&client);
        return rc;
    }

    char payload[] = "work";
    if (state == "not work") {
        strcpy(payload, "not work");
    }
    rc = MQTTClient_publish(client, "state/topic", strlen(payload), 
                            payload, 2, 0, NULL);

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);

    return rc;
}

int main(void) {
    while (1) {
        publish_state();
        usleep(5000000);  // Sleep for 5 seconds
    }
    return 0;
}