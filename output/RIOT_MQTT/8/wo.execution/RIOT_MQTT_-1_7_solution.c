#include <timex.h>
#include <stdlib.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <MQTTClient.h>
#include <stdio.h>
#include <ztimer.h>

#define BROKER_IP     "47.102.103.1"
#define BROKER_PORT   1883
#define PUBLISH_TOPIC "device/state"
#define PUBLISH_INTERVAL (5U * US_PER_SEC)  // Publish every 5 seconds

static ztimer_t timer;

static void mqtt_publish_task(void)
{
    MQTTClient client;
    Network network;
    unsigned char sendbuf[80], readbuf[80];
    int rc;

    NetworkInit(&network);
    NetworkConnect(&network, BROKER_IP, BROKER_PORT);

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "riot-esp32-client";

    if ((rc = MQTTConnect(&client, &options)) != 0) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
        return;
    }

    MQTTMessage message;
    message.qos = 1;
    message.retained = 0;
    message.payload = (void*)"work or not work";
    message.payloadlen = strlen((const char*)message.payload);

    while (1) {
        MQTTPublish(&client, PUBLISH_TOPIC, &message);
        printf("Message published.\n");
        ztimer_sleep(ZTIMER_MSEC, PUBLISH_INTERVAL);
    }
}

int main(void)
{
    puts("MQTT publisher example\n");

    /* create a task to run MQTT publish task */
    if (thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, 0, mqtt_publish_task, NULL, "mqtt_publisher") <= 0) {
        printf("Error: unable to create mqtt publisher thread\n");
    }
    return 0;
}