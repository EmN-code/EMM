#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "thread.h"
#include "timer.h"
#include "time_units.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "stddef.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "mutex.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "esp_system.h"
#include "architecture.h"
#include "arpa/inet.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883

static char topic[] = "device/state";
static char mqtt_stack[THREAD_STACKSIZE_DEFAULT];

void *publisher_thread(void *arg) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = mqtt_broker_port
    };

    inet_pton(AF_INET, mqtt_broker_ip, &gw.addr.ipv4);

    // Connect to broker
    while(1) {
        int rc = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (rc != EMCUTE_OK) {
            printf("Failed to connect to broker. Retrying in 1 second.\n");
            xtimer_sleep(1);
        } else {
            printf("Connected to broker.\n");
            break;
        }
    }
    
    t.name = topic;
    t.id = 0;

    while (1) {
        // Publish state
        const char* message = "work";
        rc = emcute_pub(&t, message, strlen(message), flags);
        if (rc == EMCUTE_OK) {
            printf("Published state to topic: %s\n", topic);
        } else {
            printf("Failed to publish state.\n");
        }
        
        // Wait 5 seconds before publishing again
        xtimer_sleep(5);
    }
    
    return NULL;
}

int main(void) {
    puts("MQTT-SN Publisher Example\n");

    thread_create(mqtt_stack, sizeof(mqtt_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publisher_thread, NULL, "pub_thread");

    return 0;
}