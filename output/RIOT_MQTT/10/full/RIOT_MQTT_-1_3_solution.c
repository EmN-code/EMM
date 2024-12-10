#include "arpa/inet.h"
#include "event/thread.h"
#include "ztimer.h"
#include "net/sock/util.h"
#include "xtimer.h"
#include "paho_mqtt.h"
#include "evtimer_mbox.h"
#include "mbox.h"
#include "net/sock/udp.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "net/gcoap.h"
#include "timex.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID "ESP32Client"
#define TOPIC "device/state"
#define MESSAGE_INTERVAL 5

void handle_error(int return_code, const char *message) {
    if (return_code != 0) {
        printf("%s failed with code %d\n", message, return_code);
        while (1) {
            printf("Retrying...\n");
            xtimer_sleep(1);
            return;
        }
    }
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    MQTTMessage message;
    int rc;

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc == 0) {
            printf("Connected to network successfully\n");
            break;
        } else {
            printf("Network connect failed. Retrying...\n");
            xtimer_sleep(1);
            continue;
        }
    }

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    while (1) {
        rc = MQTTConnect(&client, &connectData);
        if (rc == 0) {
            printf("Connected to MQTT broker successfully\n");
            break;
        } else {
            printf("MQTT connect failed. Retrying...\n");
            xtimer_sleep(1);
            continue;
        }
    }

    char *state = "work";
    while (1) {
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void *)state;
        message.payloadlen = strlen(state);

        rc = MQTTPublish(&client, TOPIC, &message);
        if (rc == 0) {
            printf("Message published successfully\n");
        } else {
            printf("Publish failed. Retrying in 5 seconds...\n");
        }

        xtimer_sleep(MESSAGE_INTERVAL);
        
        // Toggle state
        state = (strcmp(state, "work") == 0) ? "not work" : "work";
    }

    return 0;
}