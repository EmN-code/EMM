#include "mma8x5x.h"
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "sys/socket.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "net/gcoap.h"

int main(void) {
    /* Define and initialize the CoAP PDU buffer and endpoint */
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t server_endpoint = { .family = AF_INET, .port = 5683 };
    
    /* Convert the IP address to binary form */
    inet_pton(AF_INET, "47.102.103.1", &server_endpoint.addr);

    /* Initialize the CoAP request */
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, "/coap/test");

    /* Prepare the payload */
    const char* payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    pdu.payload_len = strlen(payload);
    
    /* Send the CoAP request over UDP */
    ssize_t bytes_sent = gcoap_req_send(buf, sizeof(buf), &server_endpoint, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
    
    /* Check if the send was successful */
    if (bytes_sent <= 0) {
        printf("Failed to send CoAP request: %d\n", bytes_sent);
    } else {
        printf("CoAP request sent: %zd bytes\n", bytes_sent);
    }
    
    return 0;
}