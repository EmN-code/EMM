#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <string.h>
#include <net/ipv6/addr.h>
#include <net/emcute.h>

#define EMCUTE_PORT     (1883U)
#define MSG_TYPE_STRING (1)

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static const shell_command_t shell_commands[] = {
    { NULL, NULL, NULL }
};

static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic;
    char *payload = (char *)data;
    printf("Received publication: %s\n", payload);
}

int main(void)
{
    msg_init_queue(queue, sizeof(queue) / sizeof(msg_t));
    puts("MQTT-SN application
");

    /* initialize our topic */
    topic.name = "state";
    topic.id = 0;

    /* configure the MQTT-SN interface */
    emcute_sub_t subscription;
    subscription.topic = topic;
    subscription.cb = on_pub;
    subscription.arg = NULL;

    /* network configuration */
    sock_udp_ep_t gw = { .port = EMCUTE_PORT };
    char *gw_ip = "47.102.103.1";
    ipv6_addr_t gw_addr;

    if (ipv6_addr_from_str(&gw_addr, gw_ip) == NULL) {
        puts("Error: unable to parse IPv6 address\n");
        return 1;
    }
    memcpy(&gw.addr.ipv6, &gw_addr, sizeof(ipv6_addr_t));

    /* connect to gateway */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway\n");
        return 1;
    }

    /* publish state periodically */
    char *message = "work";
    while (1) {
        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish message\n");
        }
        xtimer_sleep(5);
    }
    return 0;
}