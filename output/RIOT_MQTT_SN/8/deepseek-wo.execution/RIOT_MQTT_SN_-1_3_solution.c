
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MESSAGE "work or not work"

int main(void)
{
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMQX_SERVER_PORT };
    emcute_topic_t topic;
    char topic_name[] = TOPIC_NAME;
    char message[] = MESSAGE;

    // Initialize emcute
    emcute_init();

    // Convert IP address string to byte representation
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error: invalid address");
        return 1;
    }

    // Connect to the MQTT-SN gateway
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: could not connect to MQTT-SN gateway");
        return 1;
    }

    // Register the topic
    topic.name = topic_name;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: could not register topic");
        return 1;
    }

    // Publish the message every 5 seconds
    while (1) {
        if (emcute_pub(&topic, message, sizeof(message) - 1, 0) != EMCUTE_OK) {
            puts("Error: could not publish message");
        } else {
            puts("Message published");
        }

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}
