#include <xtimer.h>
#include <thread.h>
#include <net/emcute.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "device/state"
#define INTERVAL (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_MAIN];

static void *publish_thread(void *arg) {
    (void)arg;
    emcute_topic_t t;
    t.name = TOPIC;
    t.id = 0;

    char message[] = "work";
    while (1) {
        if (emcute_publish(&t, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        } else {
            printf("Message published: %s\n", message);
        }

        xtimer_sleep(INTERVAL / US_PER_SEC);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(stack, ARRAY_SIZE(stack));

    emcute_con_t conn;
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    conn.gw = gw;
    if (emcute_connect(&conn, false, NULL, 0, NULL, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to broker\n");
        return 1;
    }

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, publish_thread, NULL, "publish_thread");

    return 0;
}