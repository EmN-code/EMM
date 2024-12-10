#include <xtimer.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <arpa/inet.h>
#include <msg.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)
#define SEND_INTERVAL (5U * US_PER_SEC)

static const char* device_state = "work";
static msg_t msg_queue[8];
static kernel_pid_t main_thread_pid;

static void *udp_sender(void *arg) {
    (void)arg;
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);

    while (1) {
        printf("Sending device state: %s\n", device_state);
        ssize_t res = sock_udp_send(NULL, device_state, strlen(device_state), &server);
        if (res < 0) {
            printf("Error sending UDP packet: %d\n", (int)res);
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);
    main_thread_pid = thread_create((char *)stack, sizeof(stack), SENDER_PRIO, 0,
                                    udp_sender, NULL, "udp_sender");
    return 0;
}