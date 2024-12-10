#include <stdio.h>
#include <string.h>
#include <net/emcute.h>
#include <net/conn.h>
#include <xtimer.h>
#include <msg.h>
#include <stdlib.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "esp32/state"
#define PUBLISH_INTERVAL (5U * US_PER_SEC)  // 5 seconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static void *mqtt_publish(void *arg) {
    (void)arg;
    conn_pubsub_t *conn = conn_pubsub_init(BROKER_IP, BROKER_PORT);

    if (conn == NULL) {
        printf("Failed to initialize MQTT connection\n");
        return NULL;
    }

    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        const char *msg = "work";
        if (conn_pubsub_publish(conn, TOPIC, msg, strlen(msg), 0) < 0) {
            printf("Error publishing message\n");
        }
        xtimer_periodic_wakeup(&last_wakeup, PUBLISH_INTERVAL);
    }

    conn_pubsub_release(conn);

    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));
    printf("Starting MQTT state publisher on ESP32\n");
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  mqtt_publish, NULL, "mqtt_pub_thread");

    return 0;
}