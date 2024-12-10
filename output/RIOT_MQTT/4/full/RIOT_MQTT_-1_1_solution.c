#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xtimer.h"
#include "thread.h"
#include "net/mqttsn_paho.h"
#include "net/network.h"

#define MQTT_BROKER "47.102.103.1"
#define MQTT_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static mqttsn_paho_t mqtt_client;
static char mqtt_buf[128];
static char mqtt_readbuf[128];

static int is_working = 0;

void publish_state(void)
{
    const char *state_msg = is_working ? "work" : "not work";
    printf("Publishing state: %s\n", state_msg);

    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    MQTTMessageInit(&message);
    message.payload = (void*)state_msg;
    message.payloadlen = strlen(state_msg);

    MQTTPublish(&mqtt_client, "state", &message);
    
    // Flip state for next publication
    is_working = !is_working;
}

void *pub_thread(void *arg)
{
    (void) arg;
    while (1) {
        xtimer_sleep(PUBLISH_INTERVAL);
        publish_state();
    }
    return NULL;
}

int main(void)
{
    printf("MQTT state publisher example\n");
    Network network;
    MQTTClientInit(&mqtt_client, &network, 1000, mqtt_buf, sizeof(mqtt_buf), mqtt_readbuf, sizeof(mqtt_readbuf));

    NetworkConnect(&network, MQTT_BROKER, MQTT_PORT);

    MQTTPush(&mqtt_client, &network, "", MQTT_BROKER, MQTT_PORT);

    thread_create(pub_thread, NULL, THREAD_PRIORITY_MAIN, "pub_thread");
    while (1) {
        thread_yield();  // Let other processes run
    }

    NetworkDisconnect(&network);

    return 0;
}