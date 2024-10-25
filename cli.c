#include <netinet/in.h> // Provides constants and structures needed for internet domain addresses.
#include <stdio.h>       // Standard input/output library for functions like printf, fgets, etc.
#include <stdlib.h>      // Standard library for functions like exit.
#include <string.h>      // String handling functions.
#include <sys/socket.h>  // Includes a number of definitions of structures needed for sockets.
#include <sys/types.h>   // Definitions of data types used in system calls.
#include <unistd.h>      // Provides access to the POSIX operating system API, for close.

#define PORT 9001  // Defines the port number for the connection.
#define MAX_COMMAND_LINE_LEN 1024  // Defines the maximum length for the input buffer.

// Function to get a command line from the user.
char* getCommandLine(char *command_line){
    do { 
        // Read input from stdin and store it in command_line. If there's an
        // error, exit immediately. fgets reads up to MAX_COMMAND_LINE_LEN-1 characters.
        if ((fgets(command_line, MAX_COMMAND_LINE_LEN, stdin) == NULL) && ferror(stdin)) {
            fprintf(stderr, "fgets error");
            exit(EXIT_FAILURE);
        }
    } while(command_line[0] == 0x0A);  // Repeat if just ENTER was pressed
    command_line[strlen(command_line) - 1] = '\0'; // Replace newline char with null char
    return command_line;
}

int main(int argc, char const* argv[]) 
{ 
    int sockID = socket(AF_INET, SOCK_STREAM, 0);  // Creates a socket for TCP connection.
    if (sockID < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in servAddr; // Structure containing an internet address.
    servAddr.sin_family = AF_INET; 
    servAddr.sin_port = htons(PORT); // Specifies the port number.
    servAddr.sin_addr.s_addr = INADDR_ANY; // Binds socket to all available interfaces.
  
    int connectStatus = connect(sockID, (struct sockaddr*)&servAddr, sizeof(servAddr)); // Connects to server.
    if (connectStatus == -1) { 
        perror("Connection failed");
        exit(EXIT_FAILURE);
    } 

    char buf[MAX_COMMAND_LINE_LEN];  // Buffer to store command line input.
    char responseData[MAX_COMMAND_LINE_LEN];  // Buffer to store response from the server.
    char *token, *cp;

    while(1) {
        printf("Enter Command (or menu): ");
        getCommandLine(buf); // Gets command line from user.

        // Sends command to the server ensuring all data is sent
        int total = 0, bytesleft = strlen(buf), n;
        while(total < bytesleft) {
            n = send(sockID, buf + total, bytesleft, 0);
            if (n == -1) { break; }
            total += n;
            bytesleft -= n;
        }

        cp = buf;
        token = strtok(cp, " ");  // Tokenizes the input to parse commands.

        if(strcmp(token, "exit") == 0) {
            close(sockID); // Close the socket
            exit(EXIT_SUCCESS); // Exit cleanly
        } else if(strcmp(token, "menu") == 0) {
            printf("COMMANDS:\n---------\n1. print\n2. get_length\n3. add_back <value>\n4. add_front <value>\n5. add_position <index> <value>\n6. remove_back\n7. remove_front\n8. remove_position <index>\n9. get <index>\n10. exit\n");
        }

        int numBytes = recv(sockID, responseData, sizeof(responseData), 0);
        if (numBytes < 0) {
            perror("Receive failed");
            close(sockID);
            exit(EXIT_FAILURE);
        }

        printf("\nSERVER RESPONSE: %s\n", responseData); 
        memset(buf, '\0', MAX_COMMAND_LINE_LEN); // Clears buffer for next input.
    } 

    return 0; 
}
