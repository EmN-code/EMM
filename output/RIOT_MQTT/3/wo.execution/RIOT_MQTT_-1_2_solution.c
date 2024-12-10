#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdatomic.h>
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "time_units.h"
#include "timex.h"
#include "ztimer.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "periph/rtc.h"
#include "dht.h"
#include "dht_params.h"
#include "paho_mqtt.h"  // Ensure this is the last included header related to MQTT.

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL (5 * 1000U)  // 5 seconds expressed in milliseconds

static MQTTClient client;
static Network network;
static unsigned char sendbuf[256], readbuf[256];
static char* topic = "device/state";
static char* message = "not work";

void publish_state(void)
{
    MQTTMessage msg;
    msg.qos = QOS0;
    msg.retained = 0;
    msg.payload = (void*)message;
    msg.payloadlen = strlen(message);

    if (MQTTPublish(&client, topic, &msg) != SUCCESS) {
        printf("Failed to publish message\n");
    }
    else {
        printf("Message published successfully\n");
    }
}

int main(void)
{
    sock_udp_ep_t remote = {
        .family = AF_INET,
        .port = BROKER_PORT,
    };

    // Convert IP into the binary format
    ipv4_addr_from_str((ipv4_addr_t *)&remote.addr, BROKER_IP);
    
    // Initialize network
    network.mqttread = Network_read;
    network.mqttwrite = Network_write;

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to the broker
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "esp32-device";

    if ((MQTTConnect(&client, &data)) != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    // Loop to publish the state every 5 seconds
    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_MSEC, PUB_INTERVAL);
    }

    return 0;
}

static int Network_read(Network* network, unsigned char* buffer, int len, int timeout_ms) {
    return sock_udp_recv(&network->sock, buffer, len, timeout_ms, NULL);
}

static int Network_write(Network* network, unsigned char* buffer, int len, int timeout_ms) {
    return sock_udp_send(&network->sock, buffer, len, NULL);
}