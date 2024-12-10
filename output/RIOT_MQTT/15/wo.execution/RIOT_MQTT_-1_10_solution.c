#include <arpa/inet.h>
#include <evtimer_msg.h>
#include <timex.h>
#include <dht.h>
#include <stdatomic.h>
#include <sys/socket.h>
#include <msg.h>
#include <mbox.h>
#include <paho_mqtt.h>
#include <stdlib.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <mutex.h>
#include <unistd.h>
#include <time_units.h>
#include <thread.h>
#include <dht_params.h>
#include <xtimer.h>
#include <mma8x5x_regs.h>
#include <esp_wifi.h>
#include <mma8x5x.h>
#include <net/emcute.h>
#include <stdbool.h>
#include <esp_system.h>
#include <mma8x5x_params.h>
#include <event/thread.h>
#include <stdio.h>
#include <MQTTClient.h>
#include <ztimer.h>
#include <netinet/in.h>
#include <evtimer_mbox.h>
#include <stddef.h>
#include <net/sock/udp.h>

#define MQTT_BROKER_IP    "47.102.103.1"
#define MQTT_BROKER_PORT  1883
#define MQTT_CLIENT_ID    "esp32_mqtt_client"

#define TOPIC             "device/state"
#define PUBLISH_INTERVAL  5

static int sock;
static struct sockaddr_in broker_addr;

void setup_socket(void) {
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&broker_addr, 0, sizeof(broker_addr));
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        close(sock);
        exit(EXIT_FAILURE);
    }
}

void publish_state(const char *state) {
    char payload[50];
    snprintf(payload, sizeof(payload), "{\"state\": \"%s\"}", state);

    if (sendto(sock, payload, strlen(payload), 0, (const struct sockaddr *)&broker_addr, sizeof(broker_addr)) < 0) {
        perror("Failed to send message");
    }
}

int main(void){
    printf("ESP32 MQTT State Publisher\n");

    setup_socket();

    while (1) {
        publish_state("work");
        sleep(PUBLISH_INTERVAL);
    }

    close(sock);
    return 0;
}