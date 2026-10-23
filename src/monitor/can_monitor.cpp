
#include "can_monitor.hpp"

#include <iostream>
#include <unistd.h>
#include <linux/can.h>

#include <map>
#include <chrono>
#include <iomanip>

void monitor_can(int sock)
{
    struct can_frame frame {};

    unsigned long total_frames = 0;
    unsigned long total_bytes = 0;

    std::map<unsigned int, unsigned long> id_count;

    auto start_time = std::chrono::steady_clock::now();
    auto last_update = start_time;

    std::cout << "\n========== CAN TRAFFIC OBSERVER ==========\n";
    std::cout << "Monitoring CAN frames...\n\n";

    while (true)
    {
        int n = read(sock, &frame, sizeof(frame));

        if (n != sizeof(frame))
        {
            continue;
        }

        // Count received frames
        total_frames++;

        // Count received data bytes
        total_bytes += frame.can_dlc;

        // Count frames for each CAN ID
        unsigned int can_id = frame.can_id & CAN_SFF_MASK;

        id_count[can_id]++;

        // Display received frame
        std::cout << "[FRAME] ID: 0x"
                  << std::hex << can_id
                  << std::dec
                  << " | DLC: "
                  << static_cast<int>(frame.can_dlc)
                  << " | DATA: ";

        for (int i = 0; i < frame.can_dlc; i++)
        {
            std::cout << std::hex
                      << std::setw(2)
                      << std::setfill('0')
                      << static_cast<int>(frame.data[i])
                      << " ";
        }

        std::cout << std::dec << "\n";

        // Calculate elapsed time
        auto current_time = std::chrono::steady_clock::now();

        auto elapsed =
            std::chrono::duration<double>(
                current_time - start_time).count();

        auto update_elapsed =
            std::chrono::duration<double>(
                current_time - last_update).count();

        // Update statistics every second
        if (update_elapsed >= 1.0)
        {
            double fps = total_frames / elapsed;

            std::cout << "\n========== CAN STATISTICS ==========\n";

            std::cout << "Total Frames : "
                      << total_frames << "\n";

            std::cout << "Total Bytes  : "
                      << total_bytes << "\n";

            std::cout << "Unique IDs   : "
                      << id_count.size() << "\n";

            std::cout << "Frames/sec   : "
                      << std::fixed
                      << std::setprecision(2)
                      << fps << "\n";

            std::cout << "\nFrames per CAN ID:\n";

            for (const auto& entry : id_count)
            {
                std::cout << "ID 0x"
                          << std::hex << entry.first
                          << std::dec
                          << " : "
                          << entry.second
                          << " frames\n";
            }

            std::cout << "==================================\n\n";

            last_update = current_time;
        }
    }
}
