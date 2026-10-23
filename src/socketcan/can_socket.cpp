
#include "can_socket.hpp"

#include <iostream>
#include <cstring>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include <linux/can.h>
#include <linux/can/raw.h>

int open_can_socket(const char* interface, int can_id, bool filter_enabled)
{
    int sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    if (sock < 0)
    {
        std::cerr << "Error creating CAN socket\n";
        return -1;
    }

    struct ifreq ifr {};

    strncpy(ifr.ifr_name, interface, IFNAMSIZ - 1);

    if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0)
    {
        std::cerr << "Error finding CAN interface\n";
        close(sock);
        return -1;
    }

    struct sockaddr_can addr {};

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        std::cerr << "Error binding CAN socket\n";
        close(sock);
        return -1;
    }

    if (filter_enabled)
    {
        struct can_filter filter {};

        filter.can_id = can_id;
        filter.can_mask = CAN_SFF_MASK;

        if (setsockopt(sock, SOL_CAN_RAW, CAN_RAW_FILTER,
                       &filter, sizeof(filter)) < 0)
        {
            std::cerr << "Error setting CAN filter\n";
            close(sock);
            return -1;
        }

        std::cout << "Filtering CAN ID: 0x"
                  << std::hex << can_id
                  << std::dec << "\n";
    }
    else
    {
        std::cout << "Monitoring all CAN IDs\n";
    }

    return sock;
}
