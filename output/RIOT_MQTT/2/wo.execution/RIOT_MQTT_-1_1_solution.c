#include "xtimer.h"
#include "thread.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "net/emcute.h"

#define MQTT_BROKER_IP   "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * 1000U * 1000U)  // in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static char topic[] = "state";
static char message[] = "work";

typedef struct {
    char* topic;
    char* message;
} mqtt_settings_t;

static mqtt_settings_t mqtt_settings = {
    topic,
    message
};

static void *mqtt_thread(void *arg) {
    (void)arg;

    emcute_sock_t sock;
    sock.ipv4 = (ipv4_addr_t) {0};

    ipv4_addr_from_str(&sock.ipv4, MQTT_BROKER_IP);
    sock.port = MQTT_BROKER_PORT;

    if (emcute_con(&sock) != EMCUTE_OK) {
        puts("MQTT connection failed");
        return NULL;
    }

    while (1) {
        if (emcute_pub(mqtt_settings.topic, mqtt_settings.message, strlen(mqtt_settings.message), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Failed to publish");
        } else {
            printf("Published: %s\n", mqtt_settings.message);
        }

        xtimer_usleep(PUBLISH_INTERVAL);
    }

    // will never reach here
    emcute_discon();
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, mqtt_thread, NULL, "mqtt_thread");
    
    /* The main thread can perform other tasks if needed, or just wait forever */
    thread_sleep();
    return 0;
}