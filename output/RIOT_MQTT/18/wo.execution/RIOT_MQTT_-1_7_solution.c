#include <ztimer.h>
#include <stdatomic.h>
#include <stdio.h>
#include <msg.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <timex.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <evtimer_msg.h>
#include <mbox.h>
#include <stdbool.h>
#include <esp_wifi.h>
#include <event/thread.h>
#include <mma8x5x_params.h>
#include <dht_params.h>
#include <dht.h>
#include <time_units.h>
#include <paho_mqtt.h> // Ensure paho MQTT headers are usable if needed
#include <architecture.h>
#include <net/mqttsn.h>
#include <esp_system.h>
#include <mqtt_client.h> // Custom MQTT header
#include <net/gcoap.h>
#include <kernel_defines.h>
#include <net/emcute.h>
#include <stddef.h>
#include <mutex.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t msg_queue[8];

void *mqtt_thread(void *arg) {
    (void) arg;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Failed to create socket");
        return NULL;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Failed to connect to broker");
        close(sock);
        return NULL;
    }

    while (1) {
        const char *msg = "work";
        send(sock, msg, strlen(msg), 0);
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    close(sock);
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  0, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}
