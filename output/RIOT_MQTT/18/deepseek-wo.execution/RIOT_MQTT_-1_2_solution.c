
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <thread.h>
#include <ztimer.h>
#include <MQTTClient.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static Network network;
static MQTTClient mqttClient;

static void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = PAYLOAD_LEN;

    MQTTPublish(&mqttClient, TOPIC, &message);
}

static void mqtt_task(void) {
    int rc;
    char address[16];
    strcpy(address, BROKER_IP);

    NetworkInit(&network);
    rc = NetworkConnect(&network, address, BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to network\n");
        return;
    }

    MQTTClientInit(&mqttClient, &network, 1000, NULL, 0, NULL, 0);

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "riot_client";
    data.keepAliveInterval = 20;
    data.cleansession = 1;

    rc = MQTTConnect(&mqttClient, &data);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }

    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_USEC, INTERVAL);
    }
}

int main(void) {
    thread_create(NULL, 1024, THREAD_PRIORITY_MAIN - 1, 0, (thread_task_func_t)mqtt_task, NULL, "mqtt_task");
    return 0;
}
