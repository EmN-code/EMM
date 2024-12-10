#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/ipv6/addr.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <mutex.h>
#include <thread.h>
#include <dht_params.h>
#include <net/emcute.h>
#include <esp_system.h>
#include <event/thread.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_KEEP_ALIVE 60
#define MQTT_PUBLISH_TOPIC "iot/status"
#define MQTT_CLIENT_ID "esp32_mqtt_client"
#define PUBLISH_INTERVAL (5U) // Publish every 5 seconds

static MQTTClient client;

static char stack[THREAD_STACKSIZE_MAIN];

static void *publish_thread(void *arg) {
    (void)arg;
    int rc;
    char *message;

    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.keepAliveInterval = MQTT_KEEP_ALIVE;
    conn_opts.cleansession = 1;

    rc = MQTTClient_connect(client, &conn_opts);

    if (rc != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        return NULL;
    }

    while (1) {
        message = (rand() % 2) ? "work" : "not work";
        MQTTClient_message pubmsg = MQTTClient_message_initializer;
        pubmsg.payload = message;
        pubmsg.payloadlen = (int)strlen(message);
        pubmsg.qos = 1;
        pubmsg.retained = 0;
        MQTTClient_deliveryToken token;

        rc = MQTTClient_publishMessage(client, MQTT_PUBLISH_TOPIC, &pubmsg, &token);

        if (rc != MQTTCLIENT_SUCCESS) {
            printf("Failed to publish message, return code %d\n", rc);
        } else {
            printf("Message with delivery token %d delivered\n", token);
        }

        sleep(PUBLISH_INTERVAL);
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);

    return NULL;
}

int main(void) {
    MQTTClient_create(&client, MQTT_BROKER_IP, MQTT_CLIENT_ID,
                      MQTTCLIENT_PERSISTENCE_NONE, NULL);

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publish_thread, NULL, "publish_thread");

    return 0;
}