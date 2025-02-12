
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {

  // 控制网络接口
  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  struct ifreq ifr;
  struct sockaddr_in *addr;

  strcpy(ifr.ifr_name, "wlp0s20f3");
  // 获取网络接口的ip地址
  if (ioctl(fd, SIOCGIFADDR, &ifr) < 0) {
    perror("ioctl SIOCGIFADDR");
    return 1;
  }

  addr = (struct sockaddr_in *)&ifr.ifr_addr;
  printf("IP address: %s\n", inet_ntoa(addr->sin_addr));

  // 获取接口子网掩码
  if (ioctl(fd, SIOCGIFNETMASK, &ifr) < 0) {
    perror("ioctl SIOCGIFNETMASK");
    return 1;
  }

  addr = (struct sockaddr_in *)&ifr.ifr_netmask;
  printf("Netmask: %s\n", inet_ntoa(addr->sin_addr));

  // 设置新的IP地址和子网掩码
  /* addr->sin_addr.s_addr = inet_addr("192.168.1.100");
  addr->sin_family = AF_INET;
  if (ioctl(fd, SIOCSIFADDR, &ifr) < 0) {
    perror("ioctl SIOCSIFADDR");
    return 1;
  }

  addr->sin_addr.s_addr = inet_addr("255.255.255.0");
  if (ioctl(fd, SIOCSIFNETMASK, &ifr) < 0) {
    perror("ioctl SIOCSIFNETMASK");
    return 1;
  } */
  close(fd);
  return 0;
}