#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "msg.h"
#include "net/mqtt-sn.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define STATE_TOPIC "state"
#define CLIENT_ID "esp32_device"
#define MSG_QUEUE_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];

static void *mqtt_thread(void *arg) {
    (void)arg;
    
    mqtt_sn_t mqtt_client;
    sock_udp_ep_t mqtt_client_addr = { .family = AF_INET,
                                        .addr = { .ipv4 = { SERVER_IP } },
                                        .port = SERVER_PORT };

    mqtt_sn_init(&mqtt_client, &mqtt_client_addr, CLIENT_ID, sizeof(CLIENT_ID));

    while (1) {
        mqtt_sn_connect(&mqtt_client);
        
        mqtt_sn_publish(&mqtt_client, STATE_TOPIC, "work", strlen("work"), MQTT_SN_QOS_0);
        
        xtimer_sleep(5);

        mqtt_sn_disconnect(&mqtt_client);

        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    printf("MQTT-SN client on ESP32 to publish work state\n");
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    thread_create(mqtt_thread_stack, sizeof(mqtt_thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");

    return 0;
}