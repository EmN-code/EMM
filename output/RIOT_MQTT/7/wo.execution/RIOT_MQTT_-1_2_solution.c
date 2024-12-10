#include <stdatomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include <paho_mqtt.h>
#include <time_units.h>
#include <event/thread.h>
#include <stdio.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL_S 5

static MQTTClient client;
static atomic_bool keep_running = true;

void publish_state(void) {
    MQTTMessage message;
    char* topic = "state_topic";
    char* payload;

    while (atomic_load(&keep_running)) {
        payload = "work";

        message.payload = (void*)payload;
        message.payloadlen = strlen(payload);
        message.qos = QOS0;
        message.retained = 0;

        int rc = MQTTPublish(&client, topic, &message);
        if (rc != 0) {
            printf("Publish failed with return code %d\n", rc);
        }

        sleep(PUBLISH_INTERVAL_S);
    }
}

int main() {
    unsigned char sendbuf[100], readbuf[100];
    Network network;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Network connect failed with return code %d\n", rc);
        return -1;
    }

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("MQTT connect failed with return code %d\n", rc);
        return -1;
    }

    puts("Connected to MQTT broker!");

    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  (thread_task_func_t)publish_state, NULL, "publish_state");

    while (atomic_load(&keep_running)) {
        rc = MQTTYield(&client, 100);
        if (rc != 0) {
            printf("MQTT yield failed with return code %d\n", rc);
        }
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}