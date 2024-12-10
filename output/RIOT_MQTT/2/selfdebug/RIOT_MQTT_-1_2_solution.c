#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static MQTTClient client;

/* Network initialization function */
static void network_init(Network *n) {
    n->mqttread = NULL; /* Assign the specified read function */
    n->mqttwrite = NULL; /* Assign the specified write function */
    sock_tcp_create(&n->sock, NULL, NULL, 0);
}

/* MQTT publish task */
static void *mqtt_pub_task(void *arg) {
    (void)arg;

    char *topic = "device/state";
    char *payload = "work";
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.id = 0;

    Network network;
    network_init(&network);
    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_riot";

    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    MQTTConnect(&client, &connectData);

    while (1) {
        message.payload = payload;
        message.payloadlen = strlen(payload);

        int rc = MQTTPublish(&client, topic, &message);
        if (rc != SUCCESS) {
            printf("MQTT Publish failed with code %d\n", rc);
        }
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }
    return NULL;
}

int main(void) {
    /* Start the MQTT Publisher thread */
    thread_create(NULL, THREAD_STACKSIZE_MAIN, THREAD_PRIORITY_MAIN - 1, 0, mqtt_pub_task, NULL, "mqtt_pub");
    /* Keep the main process active */
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}