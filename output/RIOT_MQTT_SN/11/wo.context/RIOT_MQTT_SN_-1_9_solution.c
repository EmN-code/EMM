#include <xtimer.h>
#include <net/emcute.h>
#include <net/af.h>

#define EMCUTE_PORT     (1883U)
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)
#define PUBLISH_INTERVAL (5U * US_PER_SEC) // 5 seconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_topic_t my_topic;
static const char *topic_name = "state";

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;  // unused
    (void)data;  // unused
    (void)len;   // unused
}

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, "esp32-client");
    return NULL; // should never be reached
}

int main(void) {
    /* initialize our subscription buffers */
    memset(subscriptions, 0, (NUMOFSUB + 1) * sizeof(emcute_sub_t));

    /* Initialize the emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    /* setup the remote address and port */
    sock_udp_ep_t gw = {.family = AF_INET, .port = EMCUTE_PORT};
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    /* connect to MQTT broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to the broker\n");
        return 1;
    }

    /* register publish topic */
    my_topic.name = topic_name;
    if (emcute_reg(&my_topic) != EMCUTE_OK) {
        printf("error: unable to register topic\n");
        return 1;
    }

    /* Publish messages in a loop */
    while (1) {
        char *message = "work";
        if (emcute_pub(&my_topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("error: unable to publish data\n");
        }
        xtimer_sleep(PUBLISH_INTERVAL);
    }
    return 0;
}