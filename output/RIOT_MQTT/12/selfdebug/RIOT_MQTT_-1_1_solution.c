#include "thread.h"
#include "mqtt.h"
#include "net/sock/udp.h"
#include "xtimer.h"
#include "stdio.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "esp32/state"
#define BUFFER_SIZE 128

static mqtt_client_t client;
static char client_id[] = "riot_esp32_client";
static sock_udp_ep_t broker;

static char send_buf[BUFFER_SIZE];
static char recv_buf[BUFFER_SIZE];

void *publish_state(void *arg) {
    (void)arg; // Unused variable
    mqtt_connect(&client, &broker, true, NULL, NULL, 0, NULL, NULL);
    
    while (1) {
        const char *state_msg = (xtimer_now_usec() % 10 > 5) ? "not work" : "work";
        mqtt_publish(&client, MQTT_TOPIC, state_msg, strlen(state_msg), MQTT_QOS_0, false);
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    broker.family = AF_INET;
    broker.netif = SOCK_ADDR_ANY_NETIF;
    broker.port = MQTT_BROKER_PORT;
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.addr);

    mqtt_client_init(&client, &broker, client_id, send_buf, BUFFER_SIZE, recv_buf, BUFFER_SIZE);

    thread_create(publish_state, NULL, "publish_state_thread", THREAD_STACKSIZE_MAIN, THREAD_PRIORITY_MAIN, NULL);

    while (1) {
        xtimer_sleep(60); // Sleep to prevent exiting
    }

    return 0;
}