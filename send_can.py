
import can

bus = can.Bus(interface="socketcan", channel="vcan0")

print("CAN Terminal")
print("Enter CAN ID and data")
print("Example: 123 11 22 33 44")
print("Type exit to stop\n")

try:
    while True:
        user_input = input("CAN> ")

        if user_input.lower() == "exit":
            break

        try:
            parts = user_input.split()

            can_id = int(parts[0], 16)
            data = [int(x, 16) for x in parts[1:]]

            if can_id > 0x7FF:
                print("Invalid standard CAN ID")
                continue

            if len(data) > 8:
                print("Maximum 8 data bytes")
                continue

            if any(x < 0 or x > 255 for x in data):
                print("Invalid data byte")
                continue

            message = can.Message(
                arbitration_id=can_id,
                data=data,
                is_extended_id=False
            )

            bus.send(message)

            print("Frame sent successfully\n")

        except (ValueError, IndexError):
            print("Invalid input. Example: 123 11 22 33\n")

except KeyboardInterrupt:
    print("\nStopping CAN sender")

finally:
    bus.shutdown()
