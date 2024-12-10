#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <net/af.h>
#include <net/sock/udp.h>
#include <net/sock.h>
#include <net/netif.h>
#include <sys/socket.h>
#include "mqtt.h"
#include "thread.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define INTERVAL (5U * 1000000U)  // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];
static char client_id[] = "esp32_client";
static mqtt_client_t client;
static mqtt_connect_client_info_t client_info = {
    .client_id = client_id,
    .keep_alive = 60,
    .will_topic = NULL,
    .will_message = NULL,
    .will_qos = 0,
    .will_retain = 0,
    .user = NULL,
    .password = NULL,
    .clean_session = 1,
};

void *pub_thread(void *arg) {
    (void) arg;

    mqtt_pub_t pub;
    pub.topic = "device/state";
    pub.payload = "{"state":"work"}";
    pub.payload_len = strlen(pub.payload);
    pub.qos = 0;
    pub.retain = 0;

    while (1) {
        int res = mqtt_publish(&client, &pub);
        if (res != 0) {
            printf("mqtt_publish failed: %d\n", res);
        }
        xtimer_usleep(INTERVAL);
    }

    return NULL;
}

int main(void) {
    /* Start network connection */
    sock_udp_ep_t server = {
        .family = AF_INET,
        .port = SERVER_PORT,
    };
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);

    msg_init_queue(queue, 8);
    
    /* Connect the MQTT client */
    if (mqtt_connect(&client, &server, &client_info) != 0) {
        puts("Error: Unable to connect to MQTT broker\n");
        return 1;
    }
    
    /* Start publish thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, pub_thread, NULL, "pub_thread");

    /* Main thread does nothing, just sleep */
    while (1) {
        xtimer_usleep(INTERVAL);
    }

    return 0;
}