#!/bin/sh

Serial_Port=/dev/ttyS0

# Set the serial port at 19200 bauds, no parity, 8 data bits, 1 stop bit
echo "Configuring serial port..."
stty --file=$Serial_Port 19200

# Send the reboot code to the Text Games System
echo "Sending reboot code..."
echo -e "\xFE" > $Serial_Port
echo "Board should have rebooted."