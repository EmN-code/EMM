#include <ztimer.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <msg.h>
#include <mutex.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <xtimer.h>
#include <MQTTClient.h>
#include <dht_params.h>
#include <dht.h>
#include <arpa/inet.h>
#include <time_units.h>
#include <thread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <timex.h>
#include <netinet/in.h>

#define MAIN_QUEUE_SIZE     (8)
#define EMCUTE_ID           "esp32-device"
#define EMCUTE_PORT         (1883)
#define SERVER_IP           "47.102.103.1"

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static emcute_topic_t _my_topic;

static char stack[THREAD_STACKSIZE_MAIN];

void *mqtt_thread(void *arg) {
    (void)arg;
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4);

    int rc;

    while (1) {  
        rc = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (rc != EMCUTE_OK) {
            puts("Connection to MQTT-SN gateway failed. Retrying...");
            ztimer_sleep(ZTIMER_SEC, 1);
            continue;
        }
        puts("Connected to MQTT-SN gateway");
        break;
    }

    _my_topic.name = "esp32/status";

    while (1) {
        rc = emcute_reg(&_my_topic);
        if (rc == EMCUTE_OK) {
            printf("Registered topic '%s'
", _my_topic.name);
            break;
        }
        else {
            puts("Unable to register topic. Retrying...");
            ztimer_sleep(ZTIMER_SEC, 1);
        }
    }

    while (1) {
        const char *status = "work";

        if (emcute_pub(&_my_topic, status, strlen(status), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data
");
        } else {
            printf("Published: %s \n", status);
        }
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");

    return 0;
}