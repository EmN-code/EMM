#include <string.h>  /* for strlen */
#include <stdlib.h>  /* for malloc, free */
#include <stdio.h>   /* for printf */
#include <net/sock/udp.h> /* Networking functionalities */
#include <esp_event.h>
#include <esp_wifi.h>
#include <timer.h>   /* Ensure Timer is defined */
#include <MQTTClient.h>  /* MQTT client functionalities */

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"

// Timer related struct
struct Timer {
    uint32_t end_time;  /* The end time for the timer */
};

void TimerInit(struct Timer* timer) {
    /* Initialize the timer structure */
    timer->end_time = 0;
}

void TimerCountdown(struct Timer* timer, unsigned int ms) {
    /* Assuming we have a function that returns the current time in ms */
    timer->end_time = (unsigned int) time_ms() + ms;
}

int TimerIsExpired(struct Timer* timer) {
    /* Check if the current time has passed the end time */
    return (time_ms() >= timer->end_time);
}

int main(void) {
    /* Initialize network */
    Network network;
    NetworkInit(&network);
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    /* MQTT client setup */
    unsigned char sendbuf[100], readbuf[100];
    MQTTClient client;
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    MQTTConnect(&client, &connectData);

    /* Publishing state every 5 seconds */
    struct Timer timer;
    TimerInit(&timer);
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = "work";
    message.payloadlen = strlen(message.payload);

    printf("Connected and publishing...");
    while (1) {
        if (MQTTIsConnected(&client)) {
            MQTTPublish(&client, MQTT_TOPIC, &message);
            printf("Published: %s\n", (char*)message.payload);
            message.payload = (strcmp(message.payload, "work") == 0) ? "not work" : "work";
            TimerCountdown(&timer, 5000); 
            while (!TimerIsExpired(&timer)) {}
        } else {
            printf("Disconnected. Reconnecting...\n");
            MQTTConnect(&client, &connectData);
        }
    }
    return 0;
}