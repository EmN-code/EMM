
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/af.h>
#include <net/mqttsn.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_DUP 0
#define MQTT_WILL 0
#define MQTT_CS 0
#define MQTT_TIT_MASK 0x03
#define MQTT_TIT_SHORT 0x02
#define MQTT_TIT_PREDEF 0x01
#define MQTT_TIT_NORMAL 0x00
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define MQTT_THREAD_NAME "mqtt_thread"
#define MQTT_THREAD_INTERVAL (5 * 1000 * 1000U) // 5 seconds

static void *mqtt_thread(void *arg)
{
    (void)arg;

    sock_udp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    uint8_t addr_buf[sizeof(struct in_addr)];

    if (inet_pton(AF_INET, MQTT_BROKER_IP, addr_buf) <= 0) {
        puts("Error: invalid address");
        return NULL;
    }

    memcpy(&remote.addr.ipv4, addr_buf, sizeof(struct in_addr));

    while (1) {
        // Publish message
        printf("Publishing message: %s\n", MQTT_MESSAGE);

        // TODO: Implement MQTT publish logic here

        xtimer_usleep(MQTT_THREAD_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    puts("MQTT Publisher Example");

    thread_create(NULL, MQTT_THREAD_STACKSIZE, MQTT_THREAD_PRIORITY, 0,
                  mqtt_thread, NULL, MQTT_THREAD_NAME);

    return 0;
}
