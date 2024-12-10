#include <ztimer.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mqtt.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "device/state"
#define MESSAGE "work"
#define SLEEP_INTERVAL (5U * ZTIMER_SEC)

static emcute_con_t broker;

void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic;
    (void)data;
    (void)len;
}

int main(void)
{
    /* Initialize the network interface, in this case we assume the network is already set up */
    puts("MQTT-SN example: connect and publish messages
");

    /* Initialize connection structure */
    broker.port = BROKER_PORT;
    ipv6_addr_from_str((ipv6_addr_t *)&broker.addr.ipv6, BROKER_IP);

    /* Connect to Broker */
    if (emcute_con(&broker, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Could not connect to broker.");
        return 1;
    }

    /* Define the topic registration */
    emcute_topic_t t;
    t.name = TOPIC;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("Could not register topic.");
        return 1;
    }

    while (1) {
        /* Publish state to the broker */
        if (emcute_pub(&t, MESSAGE, strlen(MESSAGE), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Could not publish message.");
        }
        else {
            printf("Published: %s\n", MESSAGE);
        }

        /* Wait before publishing next message */
        ztimer_sleep(ZTIMER_MSEC, SLEEP_INTERVAL);
    }

    return 0;
}