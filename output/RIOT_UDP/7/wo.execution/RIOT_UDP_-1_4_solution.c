#include <mutex.h>
#include <MQTTClient.h>
#include <event/thread.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <net/gcoap.h>
#include <dht_params.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/af.h>
#include <esp_system.h>
#include <thread.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define BUF_SIZE 64
#define SEND_INTERVAL 5

static char buf[BUF_SIZE];
static sock_udp_ep_t server;

static void udp_send_thread(void *arg) {
    (void)arg;
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);
    char *state = "work";
    while (1) {
        ssize_t res = sock_udp_send(&sock, state, strlen(state), &server);
        if (res < 0) {
            printf("Error sending data: %d\n", res);
        }
        else {
            printf("Sent state: %s\n", state);
        }
        state = (strcmp(state, "work") == 0) ? "not work" : "work";
        thread_sleep_for(SEND_INTERVAL * 1000000);
    }
}

int main(void) {
    server.family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);
    server.port = SERVER_PORT;

    static char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, udp_send_thread, NULL, "udp_send");
    
    return 0;
}