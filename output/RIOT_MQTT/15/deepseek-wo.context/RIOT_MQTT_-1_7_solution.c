
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>
#include <net/mqtt_client.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS QOS_0
#define MQTT_RETAIN 0
#define MQTT_DUP 0
#define MQTT_ID 1
#define MQTT_PAYLOADLEN sizeof(MQTT_MESSAGE) - 1
#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define MQTT_THREAD_PRIO (THREAD_PRIORITY_MAIN - 1)

static char mqtt_stack[MQTT_THREAD_STACKSIZE];
static MQTTClient mqtt_client;
static Network mqtt_network;
static unsigned char sendbuf[100];
static unsigned char readbuf[100];

void mqtt_publish(void) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = MQTT_DUP;
    message.id = MQTT_ID;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = MQTT_PAYLOADLEN;

    MQTTPublish(&mqtt_client, MQTT_TOPIC, &message);
}

void mqtt_thread(void) {
    MQTTClientInit(&mqtt_client, &mqtt_network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    int result = NetworkConnect(&mqtt_network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (result != 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.clientID.cstring = "riot_mqtt_client";

    result = MQTTConnect(&mqtt_client, &options);
    if (result != 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }

    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }
}

int main(void) {
    thread_create(mqtt_stack, sizeof(mqtt_stack), MQTT_THREAD_PRIO, 0, (thread_task_func_t)mqtt_thread, NULL, "mqtt_thread");

    return 0;
}
