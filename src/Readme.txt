client:
client program name : client_tcp
To make the client run: 
gcc client_tcp.c
./a.out portno

description: client program has three functions (create_log) (checksum) and (main)
from (main) actual execution of the program starts. (create_log) function creates text file and write to it the various segments transmitted and received. (checksum) calculates the checksum.

Server:
Server program name : server_tcp
To make the server run: 
 gcc server_tcp.c
./a.out portno

Also the file to be transfered that is provided within the zip file should be placed wherever the client code is present and also the name of the file should not be changes(Datafile.txt) 

description: Server program also has three functions (create_log) (checksum) and (main)
from (main) actual execution of the program starts. (create_log) function creates text file and write to it the various segments transmitted and received. (checksum) calculates the checksum. server program keep on running until it's  stopped manually.once we stop the server it creates the client_out and server_out files.

Everytime when there is a change in the tcp segment it is printed and then sent to the other side. 
