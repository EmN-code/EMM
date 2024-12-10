#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "esp_event.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define MQTT_CLIENT_STACK_SIZE THREAD_STACKSIZE_MAIN
#define MQTT_PUBLISH_INTERVAL_SEC 5

static char mqtt_client_stack[MQTT_CLIENT_STACK_SIZE];

#define QOS         1
#define TIMEOUT     10000L

static MQTTClient client;
static Network network;
static MQTTMessage message;
static unsigned char sendbuf[100], readbuf[100];

void publish_state(void) {
    const char *topic = "esp32/state";
    static char payload[50];
    snprintf(payload, sizeof(payload), "device is %s", "working");

    message.qos = QOS;
    message.retained = 0;
    message.payload = (void *)payload;
    message.payloadlen = strlen(payload);

    if(MQTTPublish(&client, topic, &message) != SUCCESS) {
        printf("Error publishing MQTT message\n");
    }
    else {
        printf("Published: %s\n", payload);
    }
}

static void *mqtt_client_thread(void *arg){
    (void)arg;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != SUCCESS) {
        printf("Failed to connect to network\n");
        return NULL;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "RIOT_ESP32_Client";

    if ((rc = MQTTConnect(&client, &connectData)) != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return NULL;
    }

    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_MSEC, MQTT_PUBLISH_INTERVAL_SEC * MS_PER_SEC);
    }

    return NULL;
}

int main(void) {
    puts("MQTT Example starting");

    thread_create(mqtt_client_stack, sizeof(mqtt_client_stack),
                  MQTT_CLIENT_THREAD_PRIORITY, 0,
                  mqtt_client_thread, NULL, "mqtt_client_thread");

    return 0;
}
