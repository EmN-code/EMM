
#include <event/thread.h>
#include <net/emcute.h>
#include <net/sock.h>
#include <thread.h>
#include <net/sock/util.h>
#include <net/ipv6/addr.h>
#include <netinet/in.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MQTT_SERVER_IP "47.102.103.1"
#define MQTT_SERVER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work or not work"
#define SEND_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t server_ep;

void send_message(void) {
    char message[] = MQTT_MESSAGE;
    ssize_t res = sock_udp_send(&sock, message, sizeof(message) - 1, &server_ep);
    if (res < 0) {
        puts("Failed to send message");
    } else {
        puts("Message sent");
    }
}

void *mqtt_thread(void *arg) {
    (void)arg;

    while (1) {
        send_message();
        xtimer_usleep(SEND_INTERVAL);
    }

    return NULL;
}

int main(void) {
    // Initialize the server endpoint
    server_ep.family = AF_INET;
    server_ep.port = MQTT_SERVER_PORT;
    inet_pton(AF_INET, MQTT_SERVER_IP, &server_ep.addr.ipv4);

    // Create the UDP socket
    sock_udp_ep_t local_ep = SOCK_IPV4_EP_ANY;
    local_ep.port = 0; // Auto-assign port
    if (sock_udp_create(&sock, &local_ep, &server_ep, 0) < 0) {
        puts("Failed to create UDP socket");
        return 1;
    }

    // Start the MQTT thread
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1, 0, mqtt_thread, NULL, "mqtt");

    return 0;
}
