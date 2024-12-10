#include <string.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define PUBLISH_INTERVAL (5U)
#define EMCUTE_ID "mqtt_sn_client"
#define TOPIC_NAME "device/state"

static emcute_topic_t topic;
static char *state = "work";

static void on_conn(emcute_con_t *con, int status) {
    (void)con; // suppress unused parameter warning
    if (status == EMCUTE_OK) {
        printf("Successfully connected to broker.\n");
    } else {
        printf("Failed to connect to broker.\n");
    }
}

int main(void) {
    emcute_con_t con;
    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT};
    inet_pton(AF_INET, SERVER_ADDR, &gw.addr.ipv4);

    con.cb = on_conn;
    con.id = EMCUTE_ID;

    int res = emcute_con(&con, &gw);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect [%d]\n", res);
        return 1;
    }

    topic.name = TOPIC_NAME;
    topic.id = 1;  // Assuming topic ID as 1; this should be managed appropriately in actual implementations

    while (1) {
        res = emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Error: unable to publish [%d]\n", res);
        }
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    // This will never be reached
    return 0;
}