#include <ztimer.h>
#include <xtimer.h>
#include <time_units.h>
#include <thread.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <msg.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <arpa/inet.h>

#define UDP_SERVER_ADDR "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define BUF_SIZE 128
#define SEND_INTERVAL (5U * US_PER_SEC)  // 5 seconds interval

void *udp_sender_thread(void *arg) {
    (void)arg;
    
    sock_udp_ep_t remote = { .family = AF_INET, .port = UDP_SERVER_PORT };
    inet_pton(AF_INET, UDP_SERVER_ADDR, &remote.addr.ipv4);
    
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    char *message = "work";
    size_t msg_len = strlen(message);
    ssize_t res;

    while (1) {
        res = sock_udp_send(&sock, message, msg_len, &remote);
        if (res < 0) {
            printf("Error sending message\n");
        } else {
            printf("Message sent: %s\n", message);
        }
        ztimer_sleep(ZTIMER_USEC, SEND_INTERVAL);
    }
    return NULL;
}

int main(void) {
    char udp_sender_stack[THREAD_STACKSIZE_DEFAULT];

    thread_create(udp_sender_stack, sizeof(udp_sender_stack),
                  THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST,
                  udp_sender_thread, NULL, "udp_sender");
    
    msg_t main_msg_queue[8];
    msg_init_queue(main_msg_queue, 8);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Process message if needed
    }

    return 0;
}
