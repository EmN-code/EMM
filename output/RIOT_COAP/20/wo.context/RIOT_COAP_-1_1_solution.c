#include <net/gcoap.h>
#include <net/gnrc/udp.h>
#include <net/sock/udp.h>

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = {.family = AF_INET6, .port = CONFIG_GCOAP_PORT};

    // Set IP address
    remote.netif = 6; // Typically the network interface for ESP32 must be set correctly
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, "47.102.103.1");

    // Initialize CoAP request PDU
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");

    // Set payload
    size_t payload_offset = gcoap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    char payload[] = "Hello, COAP Cloud";
    memcpy(&buf[payload_offset], payload, sizeof(payload));
    pdu.payload_len = sizeof(payload);

    // Send CoAP request
    ssize_t res = gcoap_req_send(buf, pdu.payload_len + payload_offset, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res < 0) {
        printf("Error sending CoAP request\n");
    }
    else {
        printf("CoAP request sent successfully\n");
    }

    return 0;
}