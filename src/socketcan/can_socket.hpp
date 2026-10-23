
#ifndef CAN_SOCKET_HPP
#define CAN_SOCKET_HPP

int open_can_socket(const char* interface, int can_id, bool filter_enabled);

#endif
