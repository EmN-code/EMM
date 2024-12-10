#include <sys/socket.h>
#include <thread.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <event/thread.h>

#define MQTT_BROKER_IP      "47.102.103.1"
#define MQTT_BROKER_PORT    1883
#define MQTT_TOPIC          "device/status"
#define PUB_INTERVAL_SEC    5

static char topic_buf[32];
static sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_BROKER_PORT };

void *mqtt_pub_thread(void *arg) {
    char pub_data[] = "work";
    emcute_topic_t t;
    t.name = MQTT_TOPIC;

    while (1) {
        int retval = emcute_pub(&t, pub_data, sizeof(pub_data), EMCUTE_QOS_0);
        if (retval != EMCUTE_OK) {
            puts("Publish failed, retrying...");
        } else {
            puts("Data published successfully");
        }
        sleep(PUB_INTERVAL_SEC);
    }
    return NULL;
}

int main(void) {
    struct addrinfo hints = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_DGRAM
    };
    struct addrinfo *res;
    int rc;

    rc = getaddrinfo(MQTT_BROKER_IP, NULL, &hints, &res);
    if (rc != 0) {
        printf("Error resolving server address: %s\n", gai_strerror(rc));
        return 1;
    }
    memcpy(&gw.addr, &((struct sockaddr_in *)res->ai_addr)->sin_addr, sizeof(gw.addr.ipv4));
    freeaddrinfo(res);

    while (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Connection failed, retrying...");
        sleep(1);
    }
    puts("Connected successfully!");

    t.priority = THREAD_PRIORITY_MAIN - 1;
    t = thread_create(NULL, THREAD_STACKSIZE_MAIN, THREAD_PRIORITY_MAIN - 1, 0, mqtt_pub_thread, NULL, "mqtt_pub_thread");
    if(t == KERNEL_PID_UNDEF) {
        puts("Error creating the MQTT publish thread.");
        return 1;
    }

    return 0;
}