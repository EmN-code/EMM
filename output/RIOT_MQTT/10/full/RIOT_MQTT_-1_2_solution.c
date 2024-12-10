#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "paho_mqtt.h"
#include "xtimer.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC "esp32/status"

#define MSG_INTERVAL (5U * US_PER_SEC) // Message interval of 5 seconds

void on_message_delivered(void *context, int result) {
    if (result == 0) {
        puts("Message delivered successfully.");
    } else {
        printf("Failed to deliver message, error code: %d\n", result);
    }
}

int main(void) {
    int rc;
    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    MQTTMessage message;
    unsigned char sendbuf[200], readbuf[200];
    
    MQTTClientInit(&client, NULL, 3000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    while (1) {
        rc = MQTTConnect(&client, &connectData);

        // Validate connection
        if (rc != 0) {
            printf("MQTT connect failed with error code %d. Retrying...\n", rc);
            xtimer_sleep(1);
            continue;
        } else {
            printf("Connected to MQTT broker at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
            break;
        }
    }

    message.qos = 1;
    message.retained = 0;
    message.payload = "work";
    message.payloadlen = strlen(message.payload);
    
    evtimer_msg_trigger_t trigger;
    evtimer_msg_t timer;
    evtimer_mbox_t mbox;

    evtimer_msg_init_queue(&mbox, 8);
    evtimer_msg_init(&timer, &mbox, "MQTT timer");
    evtimer_msg_set(&timer, EVTIMER_MSG_TYPE_SET, MSG_INTERVAL, &trigger);
    
    // Main loop for publishing messages every 5 seconds
    while (1) {
        evtimer_msg_wait(&mbox, NULL);
        rc = MQTTPublish(&client, TOPIC, &message);

        // Validate publish
        if (rc != 0) {
            printf("MQTT publish failed with error code %d\n", rc);
        } else {
            printf("Published message: %s\n", (char *)message.payload);
        }
    }
    
    return 0;
}
