#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/emcute.h>

#define SERVER_ADDR           "47.102.103.1"
#define SERVER_PORT           1883
#define CLIENT_ID             "esp32_client"
#define TOPIC_NAME            "esp32/status"
#define MESSAGE_WORK          "work"
#define MESSAGE_NOT_WORK      "not work"
#define PUBLISH_INTERVAL_SEC  5

static char stack[THREAD_STACKSIZE_MAIN];

static int publish_message(const char *message) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    printf("Publishing message: %s\n", message);

    t.name = TOPIC_NAME;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("Failed to register topic");
        return 1;
    }

    if (emcute_pub(&t, message, strlen(message), flags) != EMCUTE_OK) {
        puts("Failed to publish message");
        return 1;
    }

    return 0;
}

static void *mqtt_thread(void *arg) {
    (void)arg;
    int state = 0;

    while (1) {
        if (publish_message(state ? MESSAGE_WORK : MESSAGE_NOT_WORK) != 0) {
            puts("Error publishing message");
        }
        state = !state;
        xtimer_sleep(PUBLISH_INTERVAL_SEC);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt");
    return 0;
}