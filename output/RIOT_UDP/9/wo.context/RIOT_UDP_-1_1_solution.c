#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/af.h>
#include <net/gnrc/udp.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL 5
#define STATE_MSG "work"

static void *send_thread(void *arg) {
    (void)arg;  // Unused parameter
    
    sock_udp_ep_t server = {
        .family = AF_INET,
        .port = SERVER_PORT
    };
    
    if (ipv6_addr_from_str((ipv6_addr_t *)&server.addr.ipv6, SERVER_ADDR) == NULL) {
        puts("Error: unable to parse IPv6 address");
        return NULL;
    }


    while (1) {
        if (sock_udp_send(NULL, STATE_MSG, strlen(STATE_MSG), &server) < 0) {
            puts("Failed to send message");
        } else {
            puts("Message sent successfully");
        }
        
        xtimer_sleep(INTERVAL);
    }

    return NULL;
}

int main(void) {
    puts("UDP client started");

    static char thread_stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, send_thread, NULL, "send thread");

    return 0;
}