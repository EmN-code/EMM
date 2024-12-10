#include "net/sock/util.h"
#include "mutex.h"
#include "thread.h"
#include "kernel_defines.h"
#include "MQTTClient.h"
#include "esp_system.h"
#include "net/sock/udp.h"
#include "arpa/inet.h"
#include "net/emcute.h"
#include "event/thread.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/gcoap.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define STACKSIZE (THREAD_STACKSIZE_MAIN)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define SLEEP_DURATION (5U)

static char stack[STACKSIZE];
static kernel_pid_t main_thread;

static void *thread_handler(void *arg) {
    (void)arg;
    
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);
    
    while (1) {
        if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
            continue;  // Skip if connection fails
        }

        emcute_topic_t t;
        t.name = MQTT_TOPIC;
        if (emcute_reg(&t) != EMCUTE_OK) {
            emcute_discon();
            continue;  // Skip if registration fails
        }

        const char* pub_data = "work";  // or "not work"
        if (emcute_pub(&t, pub_data, strlen(pub_data), 0) != EMCUTE_OK) {
            emcute_discon();
        }

        emcute_discon();
        usleep(SLEEP_DURATION * 1000000);
    }
    return NULL;
}

int main(void) {
    main_thread = thread_create(stack, STACKSIZE, PRIORITY, 0, thread_handler, NULL, "main");
    thread_wakeup(main_thread);
    return 0;
}
