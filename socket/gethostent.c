#include <apue.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>

int main() {
  struct hostent *host;

  while ((host = gethostent()) != NULL) {
    printf("Official name: %s\n", host->h_name);

    printf("Aliases: ");
    char **alias = host->h_aliases;
    if (alias) {
      while (*alias) {
        printf("%s ", *alias++);
      }
    } else {
      printf("None\n");
    }
    printf("\n");
    printf("Address type: %s\n ",
           (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");
    printf("Address length: %d\n ", host->h_length);

    printf("Address list: ");
    char **addr = host->h_addr_list;
    if (addr) {
      while (*addr) {
        printf("%s ", inet_ntoa(*(struct in_addr *)*addr++));
      }
    } else {
      printf("None");
    }
    printf("\n\n");
  }

  return (0);
}