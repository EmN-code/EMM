#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>

#define COAP_MAX_BUF_LEN 128

void main(void) {
   struct sockaddr_in server_addr;
   server_addr.sin_family = AF_INET;
   server_addr.sin_port = htons(5683);
   inet_pton(AF_INET, "47.102.103.1", &server_addr.sin_addr);

   uint8_t coap_buf[COAP_MAX_BUF_LEN];
   struct coap_packet request;
   uint8_t token[COAP_TOKEN_MAX_LEN];
   uint8_t *payload = (uint8_t *)"Hello, COAP Cloud";
   size_t payload_len = strlen("Hello, COAP Cloud");

   int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
   if (sock < 0) {
      printk("Failed to create socket\n");
      return;
   }

   int result = coap_packet_init(&request, coap_buf, sizeof(coap_buf), 1, COAP_TYPE_CON, 8, token, COAP_TOKEN_MAX_LEN, COAP_METHOD_POST, coap_next_id());
   if (result < 0) {
      printk("Failed to initialize CoAP packet\n");
      close(sock);
      return;
   }

   result = coap_packet_append_option(&request, COAP_OPTION_URI_PATH, "coap/test", strlen("coap/test"));
   if (result < 0) {
      printk("Failed to append CoAP option\n");
      close(sock);
      return;
   }

   result = coap_packet_append_payload_marker(&request);
   if (result < 0) {
      printk("Failed to append payload marker\n");
      close(sock);
      return;
   }

   result = coap_packet_append_payload(&request, payload, payload_len);
   if (result < 0) {
      printk("Failed to append payload\n");
      close(sock);
      return;
   }

   result = sendto(sock, coap_buf, request.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
   if (result < 0) {
      printk("Failed to send CoAP request\n");
      close(sock);
      return;
   }

   printk("CoAP request sent successfully\n");

   close(sock);
}