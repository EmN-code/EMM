#include "xtimer.h"
#include "thread.h"
#include "msg.h"
#include "emcute.h"

#define EMCUTE_QOS          0
#define EMCUTE_PORT         1883
#define EMCUTE_ID           "device123"
#define EMCUTE_STACK_SIZE   (THREAD_STACKSIZE_MAIN)
#define PUB_INTERVAL        (5U * US_PER_SEC)

static char stack[EMCUTE_STACK_SIZE];
static msg_t queue[8];

static const emcute_topic_t topic = {
    .name = "device/state",
    .id = 0
};

static emcute_cb_t on_pub = NULL;

static void *emcute_thread(void *arg) {
    (void)arg;
    int res;
    emcute_con_t conn;
    conn.port = EMCUTE_PORT;
    conn.address.family = AF_INET;
    inet_pton(AF_INET, "47.102.103.1", &conn.address.ipv4);

    res = emcute_con(&conn, EMCUTE_ID);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect [%d]\n", res);
        return NULL;
    }

    while (1) {
        const char *state = "work";  // default state
        emcute_pub(EMCUTE_QOS, &topic, state, strlen(state), on_pub);
        xtimer_sleep(PUB_INTERVAL);
    }

    emcute_discon();
    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute thread");
    return 0;
}