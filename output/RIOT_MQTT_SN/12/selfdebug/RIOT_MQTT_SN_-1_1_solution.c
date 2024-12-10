#include "thread.h"
#include "msg.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define EMCUTE_ID "riot"
#define EMCUTE_PORT (1883U)
#define MQTT_BROKER_IP "47.102.103.1"
#define NUMOFSUBS (16U)
#define TOPIC_MAXLEN (64U)

static emcute_sub_t subscriptions[NUMOFSUBS];
static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
}

static int publish_state(emcute_con_t *conn) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    t.name = "state";
    if (emcute_reg(conn, &t) != EMCUTE_OK) {
        return 1;
    }

    const char *state = "work or not work";
    if (emcute_pub(&t, state, strlen(state), flags) != EMCUTE_OK) {
        return 1;
    }

    return 0;
}

void *mqtt_thread(void *arg) {
    (void)arg;
    emcute_con_t conn;
    conn.sock = -1;

    while (1) {
        ipv6_addr_t addr;
        if (ipv6_addr_from_str(&addr, MQTT_BROKER_IP) == NULL) {
            puts("Error parsing broker IP");
            continue;
        }

        conn.id = EMCUTE_ID;
        conn.port = EMCUTE_PORT;
        if (emcute_con(&conn, &addr, sizeof(addr)) != EMCUTE_OK) {
            puts("Connection failed");
            continue;
        }

        printf("Successfully connected to [%s]:%u\n", MQTT_BROKER_IP, EMCUTE_PORT);

        while (1) {
            publish_state(&conn);
            xtimer_sleep(5);
        }

        emcute_discon(&conn);
    }
    return NULL;
}

int main(void) {
    puts("MQTT-SN example");

    msg_init_queue(queue, ARRAY_SIZE(queue));
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt");

    return 0;
}
