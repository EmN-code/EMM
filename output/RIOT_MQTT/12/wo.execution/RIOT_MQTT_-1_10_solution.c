#include <arpa/inet.h>
#include <architecture.h>
#include <dht.h>
#include <dht_params.h>
#include <stdlib.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <kernel_defines.h>
#include <mbox.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <msg.h>
#include <mutex.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <paho_mqtt.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stdio.h>
#include <sys/socket.h>
#include <thread.h>
#include <time_units.h>
#include <timex.h>
#include <unistd.h>
#include <xtimer.h>
#include <ztimer.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/status"
#define MQTT_MSG "{"state":"work or not work"}"
#define INTERVAL (5U * US_PER_SEC)

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100];
static unsigned char readbuf[100];

int main(void) {
    NetworkInit(&network);
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "esp32_client";
    MQTTConnect(&client, &options);

    MQTTMessage message;
    message.qos = 1;
    message.retained = 0;
    message.payload = MQTT_MSG;
    message.payloadlen = strlen(MQTT_MSG);

    while (1) {
        MQTTPublish(&client, MQTT_TOPIC, &message);
        xtimer_sleep(5);
    }
    return 0;
}