
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <emcute.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define CLIENT_ID "esp32_client"
#define MSG_SIZE 128

static char msg_buf[MSG_SIZE];
static sock_udp_ep_t gw = { .port = BROKER_PORT, .family = AF_INET };

void publish_state(void) {
    static int state = 0;
    const char *states[] = { "not work", "work" };

    snprintf(msg_buf, sizeof(msg_buf), "%s", states[state]);
    state = !state;

    emcute_topic_t t;
    t.name = TOPIC;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID");
        return;
    }

    if (emcute_pub(&t, msg_buf, strlen(msg_buf), 0) != EMCUTE_OK) {
        puts("error: unable to publish message");
    } else {
        printf("Published: %s\n", msg_buf);
    }
}

int main(void) {
    puts("MQTT-SN example application");

    /* parse IP address */
    if (ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, BROKER_IP) == NULL) {
        puts("error: unable to parse IP address");
        return 1;
    }

    /* initialize emcute stack */
    emcute_run(BROKER_PORT, CLIENT_ID);

    /* connect to MQTT-SN broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to broker");
        return 1;
    }
    puts("Connected to MQTT-SN broker");

    /* publish state every 5 seconds */
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
