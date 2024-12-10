#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <mutex.h>
#include <thread.h>
#include <dht_params.h>
#include <net/emcute.h>
#include <esp_system.h>
#include <event/thread.h>
#include <MQTTClient.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_TOPIC "esp32/state"

static char stack[THREAD_STACKSIZE_DEFAULT];
char mqtt_state[20];

void *publish_thread(void *arg) {
    struct sockaddr_in broker_addr;
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("Socket creation failed\n");
        return NULL;
    }

    memset(&broker_addr, 0, sizeof(broker_addr));
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(BROKER_PORT);
    if (inet_pton(AF_INET, BROKER_IP, &broker_addr.sin_addr) <= 0) {
        printf("Invalid MQTT broker IP\n");
        close(sock);
        return NULL;
    }

    MQTTClient client;
    MQTTClient_create(&client, BROKER_IP, PUB_TOPIC, MQTTCLIENT_PERSISTENCE_NONE, NULL);

    int rc;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        return NULL;
    }

    while (1) {
        strcpy(mqtt_state, "work"); // Example: Just "work", toggle for "not work"
        MQTTClient_message pubmsg = MQTTClient_message_initializer;
        pubmsg.payload = mqtt_state;
        pubmsg.payloadlen = (int)strlen(mqtt_state);
        pubmsg.qos = 1;
        pubmsg.retained = 0;
        MQTTClient_deliveryToken token;

        rc = MQTTClient_publishMessage(client, PUB_TOPIC, &pubmsg, &token);
        if (rc != MQTTCLIENT_SUCCESS) {
            printf("Failed to publish message, return code %d\n", rc);
        }
        MQTTClient_waitForCompletion(client, token, 1000L);

        printf("Message '%s' published.\n", mqtt_state);

        sleep(5);
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    close(sock);
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  publish_thread, NULL, "publish thread");
    return 0;
}
