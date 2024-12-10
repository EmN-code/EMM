#include <udp.h>
#include <net/sock/util.h>
#include <MQTTClient.h>
#include <net/gcoap.h>
#include <netinet/in.h>
#include <paho_mqtt.h>
#include <esp_system.h>
#include <arpa/inet.h>
#include <esp_wifi.h>
#include <sys/socket.h>
#include <net/sock/udp.h>
#include <mutex.h>
#include <net/emcute.h>
#include <thread.h>
#include <net/af.h>
#include <event/thread.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL 5
#define STACKSIZE 1024
#define MSG_BUFFER_SIZE 128

static char udp_sender_stack[STACKSIZE];
static int should_work = 1;  // example state

void *_udp_sender_thread(void *arg) {
    (void) arg;
    
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv6);

    sock_udp_t sock;
    ssize_t res = sock_udp_create(&sock, NULL, NULL, 0);
    if (res < 0) {
        printf("Error creating socket: %d\n", res);
        return NULL;
    }

    char message[] = "device state: ";
    char buffer[MSG_BUFFER_SIZE];

    while (1) {
        snprintf(buffer, sizeof(buffer), "%s%s", message, should_work ? "work" : "not work");
        res = sock_udp_send(&sock, buffer, strlen(buffer), &remote);
        if (res < 0) {
            printf("Error sending message: %d\n", res);
        } else {
            printf("Message sent: %s\n", buffer);
        }

        riot_thread_sleep(SEND_INTERVAL);
    }

    sock_udp_close(&sock);
    return NULL;
}

int main(void) {
    printf("UDP sender thread starting\n");
    thread_create(udp_sender_stack, sizeof(udp_sender_stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, _udp_sender_thread, NULL, "udp_sender_thread");

    return 0;
}