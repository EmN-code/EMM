// This program sends a CoAP POST request to a CoAP server from an ESP32 using RIOT OS.

#include "xtimer.h"    // Provides utilities needed for timer functionalities
#include "stdbool.h"  // For using boolean data type
#include "net/sock/udp.h" // For UDP socket programming
#include "net/gcoap.h" // For CoAP functionalities
#include "string.h"    // For handling string operations
#include "stdio.h"     // For printf and other standard I/O operations
#include "arpa/inet.h" // For IP address parsing

#define COAP_PORT 5683
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"

static ssize_t _send_message(const char *server_ip, uint16_t port, const char *path, const char *data) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;

    // Prepare the remote server endpoint
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = port;
    
    if (inet_pton(AF_INET, server_ip, &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse server IP");
        return -1;
    }

    // Prepare CoAP request
    ssize_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, path);
    if (len < 0) {
        puts("Error: unable to initialize CoAP request");
        return -1;
    }

    // Append data to the request
    memcpy(pdu.payload, data, strlen(data));
    len += strlen(data);

    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON); // Set as a confirmable message

    // Send the CoAP request
    ssize_t res = gcoap_req_send(buf, len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Error sending CoAP request\n");
        return res;
    }

    puts("CoAP message sent");
    return res;
}

int main(void) {
    puts("RIOT CoAP client example");

    // Wait before sending the request to ensure system readiness
    xtimer_sleep(5);

    const char *message = "Hello, COAP Cloud";
    _send_message(EMQX_SERVER_IP, COAP_PORT, EMQX_URI_PATH, message);

    return 0;
}