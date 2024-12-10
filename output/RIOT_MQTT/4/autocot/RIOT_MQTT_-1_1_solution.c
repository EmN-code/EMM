#include <ztimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP      "47.102.103.1"
#define MQTT_BROKER_PORT    1883
#define MQTT_TOPIC          "device/status"
#define PUBLISH_INTERVAL    (5U * ZTIMER_USEC)
#define DEVICE_NAME         "device1"

static MQTTClient client;
static Network network;
static unsigned char sendbuf[80], readbuf[80];
static const char *state_msg[] = {"work", "not work"};

void mqtt_publish_state(void) {
    static uint8_t state = 0;
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)state_msg[state];
    message.payloadlen = strlen(state_msg[state]);

    if (MQTTPublish(&client, MQTT_TOPIC, &message) != SUCCESS) {
        printf("Failed to publish state\n");
    }
    else {
        printf("State '%s' published\n", state_msg[state]);
        state = !state; // Toggle state
    }
}

int main(void) {
    int rc;
    ztimer_clock_t *clock = ZTIMER_MSEC;
    ztimer_now_t last_wakeup = ztimer_now(clock);
    
    printf("Setting up MQTT client\n");
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    printf("Connecting to MQTT broker\n");
    if ((rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT)) != SUCCESS) {
        printf("Failed to connect to network: %d\n", rc);
        return rc;
    }

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = DEVICE_NAME;

    if ((rc = MQTTConnect(&client, &data)) != SUCCESS) {
        printf("Failed to connect to MQTT broker: %d\n", rc);
        return rc;
    }

    printf("Connected to MQTT broker
");
    while (1) {
        mqtt_publish_state();
        ztimer_periodic_wakeup(clock, &last_wakeup, PUBLISH_INTERVAL);
    }

    return 0;
}