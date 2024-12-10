#include <xtimer.h>  // Provides time related functions such as sleep
#include <string.h>   // For string operations
#include <stdio.h>    // Standard I/O operations
#include <kernel.h>   // Core kernel functionality
#include <emcute.h>   // emcute library for MQTT-SN

#define MQTT_BROKER_ADDRESS "47.102.103.1"
#define MQTT_BROKER_PORT    1883
#define PUBLISH_INTERVAL    (5U * US_PER_SEC)  // every 5 seconds

#define TOPIC "device/state"

static emcute_topic_t my_topic;

// Function to setup and publish data
static void *mqtt_publisher_thread(void *arg) {
    (void)arg;

    while (1) {
        int res = emcute_pub(&my_topic, "work", strlen("work"), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Error in publishing data: %d\n", res);
        }
        xtimer_sleep(PUBLISH_INTERVAL);
    }
    return NULL;
}

int main(void) {
    // Initialize the network and emcute
    static char stack[THREAD_STACKSIZE_DEFAULT];
    static const shell_command_t shell_commands[] = { NULL };
    (void) shell_commands;

    puts("MQTT-SN publisher with emcute");

    // Initialize emcute
    kernel_pid_t pid = thread_create(stack, sizeof(stack),
                                     EMCUTE_PRIO, THREAD_CREATE_STACKTEST,
                                     mqtt_publisher_thread, NULL, "mqtt_publisher");

    if (pid <= KERNEL_PID_UNDEF) {
        puts("Error: unable to create thread");
        return 1;
    }

    // setup connection to the broker
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, MQTT_BROKER_ADDRESS);

    while (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        xtimer_sleep(1);
    }

    // setup topic
    my_topic.name = TOPIC;

    if (emcute_reg(&my_topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    return 0;
}