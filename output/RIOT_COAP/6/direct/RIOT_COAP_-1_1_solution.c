#include <string.h>
#include <stdio.h>
#include <net/gnrc/netif.h>
#include <net/af.h>
#include <net/coap.h>
#include <msg.h>
#include <kernel.h>

#define COAP_INBUF_SIZE      (128U)
#define COAP_OUTBUF_SIZE     (128U)
#define COAP_SERVER_ADDR     "47.102.103.1"
#define COAP_SERVER_PORT     5683
#define COAP_URI_PATH        "/coap/test"
#define COAP_PAYLOAD         "Hello, COAP Cloud"
#define SENDER_PRIO          (THREAD_PRIORITY_MAIN - 1)
#define SENDER_STACKSIZE     (THREAD_STACKSIZE_MAIN)

static gnrc_netif_t *get_interface(void) {
    return gnrc_netif_iter(NULL);
}

static void send_coap_request(void) {
    gnrc_netif_t *netif = get_interface();
    if (!netif) {
        printf("[Error] No available network interface found.\n");
        return;
    }

    sock_udp_ep_t remote = { .family = AF_INET6, .port = COAP_SERVER_PORT };
    inet_pton(AF_INET6, COAP_SERVER_ADDR, &remote.addr.ipv6);

    coap_pkt_t pdu;
    uint8_t buf[COAP_INBUF_SIZE];

    coap_build_hdr(&pdu, buf, COAP_OUTBUF_SIZE, COAP_POST, COAP_TYPE_CON, coap_get_id(), NULL);
    coap_hdr_set_uri_path(&pdu, COAP_URI_PATH);
    coap_payload(&pdu, (uint8_t *)COAP_PAYLOAD, strlen(COAP_PAYLOAD));

    printf("Sending CoAP POST request... \n");
    ssize_t res = coap_send(gnrc_netapi_driver_pid(GNRC_NETTYPE_UDP), &remote, &pdu);

    if (res < 0) {
        printf("Failed to send CoAP request\n");
    } else {
        printf("CoAP POST request sent successfully\n");
    }
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    thread_create(sender_stack, SENDER_STACKSIZE, SENDER_PRIO, 0, send_coap_request, NULL, "coap_sender");
    return 0;
}