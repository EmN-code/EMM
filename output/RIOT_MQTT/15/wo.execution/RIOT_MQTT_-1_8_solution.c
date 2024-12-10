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

#define REMOTE_IP "47.102.103.1"
#define REMOTE_PORT 1883
#define MQTT_TOPIC "device/status"
#define MQTT_MSG "work or not work"
#define PUBLISH_INTERVAL (5U * 1000U * 1000U) // 5 seconds in microseconds

static int socket_fd;
static struct sockaddr_in broker_addr;

void setup_socket(void) {
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0) {
        printf("Error: Unable to create socket\n");
        return;
    }

    memset(&broker_addr, 0, sizeof(broker_addr));
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(REMOTE_PORT);
    inet_pton(AF_INET, REMOTE_IP, &broker_addr.sin_addr);
}

void mqtt_publish(const char *topic, const char *message) {
    char packet[256];
    int packet_len = snprintf(packet, sizeof(packet),
                              "PUBLISH %s %s", topic, message);

    sendto(socket_fd, packet, packet_len, 0, 
           (struct sockaddr*)&broker_addr, sizeof(broker_addr));
}

int main(void) {
    setup_socket();

    while (1) {
        mqtt_publish(MQTT_TOPIC, MQTT_MSG);
        usleep(PUBLISH_INTERVAL);
    }

    return 0;
}