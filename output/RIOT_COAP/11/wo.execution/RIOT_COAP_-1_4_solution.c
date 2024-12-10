#include <unistd.h>
#include <time_units.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <esp_system.h>  // For ESP32 specific system functionality
#include <arpa/inet.h>
#include <netinet/in.h>  // For inet_pton
#include <sys/socket.h>  // For AF_INET
#include <mutex.h>
#include <net/sock/util.h>  // Utilities for socket programming
#include <net/sock/udp.h>  // UDP sockets
#include <net/gcoap.h>  // For gCoAP functionalities
#include <dht_params.h>  // Placeholder, assuming this is required for other purposes
#include <mma8x5x_params.h>  // Placeholder, assuming this is required for other purposes
#include <mma8x5x_regs.h>  // Placeholder, assuming this is required for other purposes
#include <mma8x5x.h>  // Placeholder, assuming this is required for other purposes
#include <net/emcute.h>  // Placeholder, assuming this is for other functionalities

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"
#define EMQX_SERVER_PORT 5683

static void _resp_handler(unsigned req_state, coap_pkt_t *pdu, 
                          sock_udp_ep_t *remote){
    /* Handle the server response here */
    if (req_state == GCOAP_MEMO_TIMEOUT) {
        printf("GCOAP_MEMO_TIMEOUT\n");
    } else {
        printf("Response received\n");
    }
}

int main(void) {
    // CoAP initialization buffer and packet
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);

    // Payload to send
    const char *payload = "Hello, COAP Cloud";
    size_t len = strlen(payload);
    memcpy(pdu.payload, payload, len);
    pdu.payload_len = len;

    // Remote server endpoint definition
    sock_udp_ep_t remote;
    memset(&remote, 0, sizeof(remote));
    remote family = AF_INET;
    remote.port = EMQX_SERVER_PORT;

    // Convert and set IP address
    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result == 0) {
        printf("Invalid IP address\n");
        return -1;
    }

    // Send CoAP request
    ssize_t res = gcoap_req_send(&buf[0], pdu.hdr_len + len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res < 0) {
        printf("Failed to send request\n");
        return -1;
    }

    printf("CoAP request sent\n");
    return 0;
}