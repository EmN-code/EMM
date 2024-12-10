#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdatomic.h>
#include <sched.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <msg.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <arpa/inet.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

#define SERVER_IP    "47.102.103.1"
#define SERVER_PORT  5683
#define URI_PATH     "/coap/test"
#define PAYLOAD      "Hello, COAP Cloud"

static void _my_coap_req_handler(uint8_t *data){
    printf("Response received: %s\n", (char *)data);
}

int main(void) {
    uint8_t buf[128];
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    ssize_t bytes_sent;
    
    // Convert IP address
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: invalid IP address\n");
        return -1;
    }

    // Setup the CoAP Message (this is a hypothetical replacement for the COAP function)
    uint8_t *pdu = buf; // Treat buf as the pdu storage
    snprintf((char *)pdu, sizeof(buf), "POST %s %s", URI_PATH, PAYLOAD);
    
    // Send UDP Packet
    printf("Sending request to %s\n", SERVER_IP);
    bytes_sent = sock_udp_send(NULL, pdu, strlen((char *)pdu), &remote);

    if (bytes_sent <= 0) {
        printf("Error: failed to send UDP packet\n");
        return -1;
    }

    // In a real-world scenario, you might listen for responses here and call _my_coap_req_handler
    //_my_coap_req_handler((uint8_t *)"Simulated response");

    return 0;
}