#include <time_units.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mutex.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <kernel_defines.h>
#include <esp_system.h>
#include <dht_params.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define COAP_METHOD COAP_METHOD_POST
#define COAP_MESSAGE "Hello, COAP Cloud"

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    // Convert the server IP address
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: invalid server IP address\n");
        return -1;
    }

    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;

    // Initialize the CoAP request
    ssize_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD, URI_PATH);
    if (len < 0) {
        printf("Error initializing CoAP request\n");
        return -1;
    }

    // Add payload to the CoAP packet
    size_t payload_len = strlen(COAP_MESSAGE);
    memcpy(pdu.payload, COAP_MESSAGE, payload_len);
    pdu.payload_len = payload_len;

    // Finalize and send the CoAP request
    if (coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE) < 0) {
        printf("Error finalizing CoAP options\n");
        return -1;
    }

    if (gcoap_req_send(buf, pdu.hdr->len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Error sending CoAP request\n");
        return -1;
    }

    printf("CoAP request sent successfully\n");
    return 0;
}