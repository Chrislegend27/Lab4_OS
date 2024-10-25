#include <netinet/in.h> //structure for storing address information 
#include <stdio.h> 
#include <string.h>
#include <stdlib.h> 
#include <sys/socket.h> //for socket APIs 
#include <sys/types.h> 
#include "list.h"

#define PORT 9001
#define ACK "ACK"
  
int main(int argc, char const* argv[]) 
{ 
  
	  int n, val, idx;
    // create server socket similar to what was done in 
    // client program 
    int servSockD = socket(AF_INET, SOCK_STREAM, 0); 
  
    // string store data to recv/send to/from client 
    char buf[1024];
		char sbuf[1024];
    char* token;

    // define server address 
    struct sockaddr_in servAddr; 
  
	  // list
		list_t *mylist;


    servAddr.sin_family = AF_INET; 
    servAddr.sin_port = htons(PORT); 
    servAddr.sin_addr.s_addr = INADDR_ANY; 
  
    // bind socket to the specified IP and port 
    bind(servSockD, (struct sockaddr*)&servAddr, 
         sizeof(servAddr)); 
  
    // listen for connections 
    listen(servSockD, 1); 
  
    // integer to hold client socket. 
    int clientSocket = accept(servSockD, NULL, NULL); 

    mylist = list_alloc();  // create the list

    while(1){
        // recvs messages from client socket 
        n = recv(clientSocket, buf, sizeof(buf), 0);
				buf[n] = '\0';

        if(n > 0){
            token = strtok(buf, " ");

            //Exit
						if(strcmp(token,"exit") == 0){
							  list_free(mylist);
							  exit(1);
						}
            //Get_length
						else if(strcmp(token,"get_length") == 0){
								// Make a Call
								val = list_length(mylist);
								sprintf(sbuf,"%s%d", "Length = ", val);
						}
            //Add front
						else if(strcmp(token,"add_front") == 0){
							  token = strtok(NULL, " ");  // get next token (value)
								val = atoi(token);
								// Make a Call
								sprintf(sbuf,"%s%d", ACK, val);
								list_add_to_front(mylist,val);
						}
            //Remove position
						else if(strcmp(token,"remove_position") == 0){
							  token = strtok(NULL, " ");
								idx = atoi(token);
								// Make a Call
								val = list_remove_at_index(mylist,idx);
								sprintf(sbuf,"%s%d", ACK, val);
						}
            //Print
						else if(strcmp(token,"print") == 0){
							  sprintf(sbuf,"%s", listToString(mylist));
						}
						// ADD THE OTHER OPERATIONS



            //add_back
            else if(strcmp(token,"add_back") == 0){
							  token = strtok(NULL, " ");  // get next token (value)
								val = atoi(token);
								// Make a Call
								sprintf(sbuf,"%s%d", ACK, val);
								list_add_to_back(mylist,val);
						}

            // Add_position
            else if (strcmp(token, "add_position") == 0) {
              // Get the value and position from the input
              token = strtok(NULL, " ");
              int position = atoi(token);  // Convert the value to integer

              token = strtok(NULL, " ");
              int value = atoi(token);  // Convert the position to integer

              // Call the function to add the element at the specified position
              list_add_at_index(mylist, value, position);

              // Send response to the client
              sprintf(sbuf, "ACK: Inserted %d at position %d", value, position);
            }



            //Remove front
            else if (strcmp(token, "remove_front") == 0) {
                // Call the function to remove the last element and capture its value
                val = list_remove_from_front(mylist);

                // Check if the list was empty
                if (val == -1) {
                    sprintf(sbuf, "List is empty, nothing to remove");
                } else {
                    sprintf(sbuf, "%s%d", ACK, val);  // Send ACK with removed value
                }
            }
            
            //Remove back
            else if (strcmp(token, "remove_back") == 0) {
                // Call the function to remove the last element and capture its value
                val = list_remove_from_back(mylist);

                // Check if the list was empty
                if (val == -1) {
                    sprintf(sbuf, "List is empty, nothing to remove");
                } else {
                    sprintf(sbuf, "%s%d", ACK, val);  // Send ACK with removed value
                }
            }


            // Get index of element
            else if (strcmp(token, "get") == 0) {
                // Get the value from the input
                token = strtok(NULL, " ");
                int value = atoi(token);  // Convert the value to integer

                // Call the function to get the index of the element
                int index = list_get_index_of(mylist, value);

                // Check if the element is in the list
                if (index == -1) {
                    sprintf(sbuf, "Element %d not found in the list", value);
                } else {
                    sprintf(sbuf, "ACK: Index of %d is %d", value, index);
                }
            }






                // send's messages to client socket 
            send(clientSocket, sbuf, sizeof(sbuf), 0);
        }
				memset(buf, '\0', 1024);
    }
  
    return 0; 
}
