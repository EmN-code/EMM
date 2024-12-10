#include <zephyr/devicetree.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/posix/sys/socket.h> // replaced the missing POSIX socket include
#include <zephyr/stats/stats.h>
#include <zephyr/sys/printk.h>
#include <zephyr/tracing/tracing.h>

#define STACK_SIZE 1024
#define PRIORITY 7

/* MQTT related definitions */
#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state_topic"

struct k_thread my_thread_data;
K_THREAD_STACK_DEFINE(my_stack_area, STACK_SIZE);

void mqtt_thread(void *arg1, void *arg2, void *arg3) {
    int sock;
    struct sockaddr_in broker_addr;
    char buffer[100];
    const char *state_msg = "work or not work";

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&broker_addr, sizeof(broker_addr)) < 0) {
        printk("Failed to connect to broker\n");
        close(sock);
        return;
    }

    while (1) {
        snprintf(buffer, sizeof(buffer), "PUBLISH %s %s", MQTT_TOPIC, state_msg);
        send(sock, buffer, strlen(buffer), 0);
        k_sleep(K_SECONDS(5));
    }

    close(sock);
}

int main(void) {
    k_thread_create(&my_thread_data, my_stack_area, STACK_SIZE,
                    mqtt_thread, NULL, NULL, NULL,
                    PRIORITY, 0, K_NO_WAIT);
    return 0;
}