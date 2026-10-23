#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <cstring>

int main()
{
    int sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    if (sock < 0)
    {
        perror("socket");
        return 1;
    }

    struct ifreq ifr {};

    std::strncpy(ifr.ifr_name, "vcan0", IFNAMSIZ - 1);

    if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0)
    {
        perror("ioctl");
        close(sock);
        return 1;
    }

    struct sockaddr_can addr {};

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        close(sock);
        return 1;
    }

    struct can_frame frame {};

    frame.can_id = 0x123;
    frame.can_dlc = 4;

    frame.data[0] = 0x11;
    frame.data[1] = 0x22;
    frame.data[2] = 0x33;
    frame.data[3] = 0x44;

    if (write(sock, &frame, sizeof(frame)) < 0)
    {
        perror("write");
        close(sock);
        return 1;
    }

    std::cout << "Test CAN frame sent\n";

    close(sock);

    return 0;
}
