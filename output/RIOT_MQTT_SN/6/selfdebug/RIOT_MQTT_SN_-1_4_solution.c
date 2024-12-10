#include <thread.h>
#include <stdio.h>
#include <string.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define EMCUTE_PORT     (1883U)
#define EMCUTE_ID       "esp32_client"
#define STATE_TOPIC     "device/state"
#define STATE_WORK      "work"
#define STATE_NOT_WORK  "not work"
#define BROKER_IP       "47.102.103.1"

char stack[THREAD_STACKSIZE_MAIN];
msg_t msg_queue[8];

/* MQTT-SN topic declaration */
emcute_topic_t subscription;
emcute_topic_t publications[1];

static int pub_state(const char *state) {
    /* Prepare the topic */
    publications[0].name = STATE_TOPIC;
    publications[0].id = 0; // dynamic ID allocation desired

    /* Publish data */
    return emcute_pub(&publications[0], state, strlen(state), EMCUTE_QOS_0);
}

static void *publisher_thread(void *arg) {
    (void) arg;
    while (1) {
        pub_state(STATE_WORK);
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    /* Initialize the emcute stack */
    memset(stack, 0, sizeof(stack));
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_t));

    /* Initialize socket */
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    /* Connect to broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to broker");
        return 1;
    }

    /* Start publisher thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publisher_thread, NULL, "publisher");

    return 0;
}