
#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>

#include "socketcan/can_socket.hpp"
#include "monitor/can_monitor.hpp"

int main()
{
    const char* interface = "vcan0";

    int can_id = 0;
    bool filter_enabled = false;

    std::string choice;

    std::cout << "\n========== CAN OBSERVER ==========\n";
    std::cout << "1. Monitor all CAN IDs\n";
    std::cout << "2. Monitor specific CAN ID\n";
    std::cout << "Select option: ";

    std::cin >> choice;

    if (choice == "2")
    {
        std::string input;

        std::cout << "Enter CAN ID (hex): ";
        std::cin >> input;

        char* end = nullptr;

        can_id = std::strtol(input.c_str(), &end, 16);

        if (end == input.c_str() || *end != '\0' ||
            can_id < 0 || can_id > 0x7FF)
        {
            std::cerr << "Invalid standard CAN ID\n";
            return 1;
        }

        filter_enabled = true;
    }
    else if (choice != "1")
    {
        std::cerr << "Invalid option\n";
        return 1;
    }

    int sock = open_can_socket(interface, can_id, filter_enabled);

    if (sock < 0)
    {
        return 1;
    }

    std::cout << "CAN socket opened successfully\n";

    monitor_can(sock);

    close(sock);

    return 0;
}
