
#include <net/emcute.h>
#include <netinet/in.h>
#include <net/ipv6/addr.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_BUF_SIZE 20

static char msg_buf[MSG_BUF_SIZE];
static emcute_topic_t topic;

static void _on_pub(void *arg, const emcute_topic_t *topic, void *data, size_t len)
{
    (void)arg;
    (void)topic;
    (void)data;
    (void)len;
}

int main(void)
{
    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_ADDR, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    while (1) {
        strncpy(msg_buf, "work", sizeof(msg_buf) - 1);
        msg_buf[sizeof(msg_buf) - 1] = '\0';
        if (emcute_pub(&topic, msg_buf, strlen(msg_buf), 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        }

        xtimer_sleep(5);

        strncpy(msg_buf, "not work", sizeof(msg_buf) - 1);
        msg_buf[sizeof(msg_buf) - 1] = '\0';
        if (emcute_pub(&topic, msg_buf, strlen(msg_buf), 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        }

        xtimer_sleep(5);
    }

    return 0;
}
