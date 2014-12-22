/** @file Main.c
 * SerialToTCP allows a RS232 serial port to communicate through the Internet.
 * @author Adrien RICCIARDI
 * @version 1.0 : 18/03/2013
 * @version 1.1 : 21/03/2013, added multiple connections handling.
 * @version 1.2 : 23/03/2013, Text Games System is resetted when a new client has connected.
 * @version 1.3 : 29/06/2013, fixed multiple bytes reception from internet bug.
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include "UART.h"

int main(int argc, char *argv[])
{
	int Socket_Server, Temp_Int = 1, Socket_Client;
	struct sockaddr_in Address_Server, Address_Client;
	unsigned char Byte, Buffer[256], i;
	socklen_t Size;
	fd_set Set_Socket_Client;
	struct timeval Select_Time = {0, 0};
	
	// Check parameters
	if (argc != 3)
	{
		printf("Usage : %s TCP_Port COM_Port\n", argv[0]);
		printf("This program will be the server for a client supporting the raw mode (like puTTY).\n");
		return -1;
	}
	
	// Initialize server
	// Create socket
	Socket_Server = socket(AF_INET, SOCK_STREAM, 0);
	if (Socket_Server == -1)
	{
		perror("[Error] can't create server socket ");
		return -1;
	}
	// Allow address to be reused instantly
	setsockopt(Socket_Server, SOL_SOCKET, SO_REUSEADDR, &Temp_Int, sizeof(Temp_Int));
	
	// Set server address
	Address_Server.sin_family = AF_INET;
	Address_Server.sin_port = htons(atoi(argv[1]));
	Address_Server.sin_addr.s_addr = INADDR_ANY;
	if (bind(Socket_Server, (const struct sockaddr *) &Address_Server, sizeof(Address_Server)) == -1)
	{
		perror("[Error] can't bind TCP port ");
		return -1;
	}
	
	printf("Opening COM port %s...\n", argv[2]);
	
	// Open COM port
	if (!UARTOpen(argv[2]))
	{
		perror("[Error] can't open COM port ");
		return -1;
	}
	
	while (1)
	{	
		printf("Server ready.\nWaiting for the client...\n");
		
		// Wait for the client
		if (listen(Socket_Server, 1) == -1)
		{
			perror("[Error] listen() failed ");
			return -1;
		}
		
		// Accept the client
		Size = sizeof(Address_Client);
		Socket_Client = accept(Socket_Server, (struct sockaddr *) &Address_Client, &Size);
		if (Socket_Client == -1)
		{
			perror("[Error] can't create client's socket ");
			return -1;
		}
		
		printf("Client connected.\n");
		printf("IP : %s\n", inet_ntoa(Address_Client.sin_addr));
		printf("Port : %d\n", ntohs(Address_Client.sin_port));
		
		// Reset the Text Games System by going to the main menu
		// Quit the current game if needed
		//UARTWriteByte(27); // Escape key
		// Start first game if we were in the menu yet
		//UARTWriteByte('1');
		// Return to the main menu
		//UARTWriteByte(27); // Escape key
				
		while (1)
		{
			// Send serial port data to the client if there is data available
			if (UARTIsByteAvailable(&Byte)) write(Socket_Client, &Byte, 1);
			
			// Check if the client sent something and if he is still connected
			FD_ZERO(&Set_Socket_Client);
			FD_SET(Socket_Client, &Set_Socket_Client);
			
			if (select(Socket_Client + 1, &Set_Socket_Client, NULL, NULL, &Select_Time) > 0)
			{
				Temp_Int = read(Socket_Client, Buffer, sizeof(Buffer));
				// The client sent nothing if he disconnected
				if (Temp_Int == 0) break;
				// Transmit data to board
				for (i = 0; i < Temp_Int; i++) UARTWriteByte(Buffer[i]);
			}
		}
		
		close(Socket_Client);
		printf("Client disconnected.\n\n");
	}
	
	//return 0;
}	
