#include <stddef.h> 
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <thread.h>
#include <unistd.h>
#include <xtimer.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <mutex.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <time_units.h>
#include <event/thread.h>
#include <kernel_defines.h>
#include <architecture.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <net/emcute.h>
#include <MQTTClient.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define MQTT_TOPIC "device/state"
#define BUFFER_SIZE 128

int mqtt_publish_state(MQTTClient *client) {
    MQTTMessage message;
    const char *state = (rand() % 2) ? "work" : "not work";
    message.qos = 1;
    message.retained = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);
    return MQTTPublish(client, MQTT_TOPIC, &message);
}

int main(void) {
    unsigned char sendbuf[BUFFER_SIZE], readbuf[BUFFER_SIZE];
    Network network;
    MQTTClient client;
    int rc;

    NetworkConnect(&network, SERVER_ADDR, SERVER_PORT);
    MQTTClientInit(&client, &network, 10000, sendbuf, BUFFER_SIZE, readbuf, BUFFER_SIZE);

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.keepAliveInterval = 20;
    data.cleansession = 1;
    data.clientID.cstring = "RIOT_Client";

    if ((rc = MQTTConnect(&client, &data)) != 0) {
        printf("Error connecting to MQTT broker, return code %d\n", rc);
        exit(1);
    }

    while (1) {
        mqtt_publish_state(&client);
        xtimer_sleep(5);
    }

    NetworkDisconnect(&network);
    return 0;
}