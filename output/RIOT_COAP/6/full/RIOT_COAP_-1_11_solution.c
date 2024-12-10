#include <netinet/in.h>  // For IP addresses
#include <net/gcoap.h>   // For CoAP functions
#include <net/sock/udp.h> // For UDP sockets
#include <arpa/inet.h>    // For IP address conversions
#include <stdlib.h>       // For general utilities
#include <stdbool.h>      // For boolean types
#include <stdio.h>        // For standard I/O functions
#include <sys/socket.h>   // For socket functions
#include <xtimer.h>       // For timer functions
#include <mutex.h>        // For mutual exclusion (mutex) utilities
#include <net/sock/util.h> // For socket utilities

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT (5683)
#define COAP_METHOD_POST 2  // Define the POST method
#define PAYLOAD "Hello, COAP Cloud"
#define URI_PATH "/coap/test"

int main(void) {
    sock_udp_ep_t remote = { .port = COAP_SERVER_PORT };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Convert the string representation of the IP address to a network address
    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: Malformed IP address\n");
        return EXIT_FAILURE;
    }

    // Initialize CoAP request
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Handling retries in network connection
    ssize_t result;
    while (true) {
        result = gcoap_req_send(buf, pdu.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (result < 0) {
            printf("Request failed, retrying in 1 second\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Request sent successfully\n");
            break;
        }
    }

    return EXIT_SUCCESS;
}