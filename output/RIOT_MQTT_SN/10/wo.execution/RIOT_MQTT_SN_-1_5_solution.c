#include <net/sock/udp.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <event/thread.h>
#include <thread.h>
#include <ztimer.h>
#include <MQTTClient.h>
#include <mutex.h>
#include <dht_params.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <kernel_defines.h>
#include <net/gcoap.h>

#define EMCUTE_PORT      (1883U)
#define SERVER_IP        "47.102.103.1"
#define TOPIC_NAME       "device/status"
#define RECONNECT_DELAY  (5U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];
static emcute_topic_t topic;

static int connect_to_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) != 1) {
        return -1;
    }
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    return res;
}

static int publish_status(const char *status) {
    int res = emcute_pub(&topic, status, strlen(status), 0);
    return res;
}

static void mqttsn_thread(void *arg) {
    (void)arg;
    mutex_t lock;
    mutex_init(&lock);
    char *status = "work";

    while (1) {
        mutex_lock(&lock);
        
        if (connect_to_broker() != EMCUTE_OK) {
            printf("Failed to connect to broker\n");
            mutex_unlock(&lock);
            ztimer_sleep(ZTIMER_MSEC, RECONNECT_DELAY * 1000);
            continue;
        }
        
        if (emcute_reg(&topic) != EMCUTE_OK) {
            printf("Failed to register topic\n");
            emcute_discon();
            mutex_unlock(&lock);
            ztimer_sleep(ZTIMER_MSEC, RECONNECT_DELAY * 1000);
            continue;
        }

        printf("Publishing status: %s\n", status);
        if (publish_status(status) != EMCUTE_OK) {
            printf("Failed to publish status\n");
        }

        emcute_discon();
        mutex_unlock(&lock);
        ztimer_sleep(ZTIMER_MSEC, 5000);
        status = (strcmp(status, "work") == 0) ? "not work" : "work";
    }
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  mqttsn_thread, NULL, "mqtt-sn");
    emcute_run(EMCUTE_PORT, "riot_device");
    return 0;
}