#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <time.h>
#define DEFAULT_BUFFLEN 512


struct tcp_hdr{
                unsigned short int src;
                unsigned short int des;
                unsigned int seq;
                unsigned int ack;
                unsigned short int hdr_flags;
                
                unsigned short int rec;
              
                unsigned short int ptr;
                unsigned int opt;
                unsigned short int cksum;
                char data[128];
              };
void create_log(char *recvBuff)
{
 struct tcp_hdr tcp_seg;
struct tcp_hdr *tcp_recv;

tcp_recv = (struct tcp_hdr *)&recvBuff;  
tcp_seg= *tcp_recv ;

FILE *f = fopen("server_out.txt", "a+");
if (f == NULL)
{
    printf("Error opening file!\n");
    exit(1);
}

  //fprintf(f,"Source port %d\n",  s_port); 
  //fprintf(f,"Dest port %d\n",  d_port);
  fprintf(f,"Sequence 0x%08X\n", tcp_seg.seq);
  fprintf(f,"Flags 0x%016X\n", tcp_seg.hdr_flags);
  fprintf(f,"Rec 0x%04X\n", tcp_seg.rec);
  fprintf(f,"Checksum calculated 0x%04X\n", tcp_seg.cksum);
  fprintf(f,"PTR 0x%04X\n", tcp_seg.ptr);
  fprintf(f,"Optional 0x%08X\n", tcp_seg.opt);
}
 
int checksum(unsigned short int chk_arr[])
{
  unsigned int i,sum=0, cksum;
for (i=0;i<10;i++) 	
{					              // Compute sum
 sum = sum + chk_arr[i];

}

  cksum = sum >> 16;              // Fold once
  sum = sum & 0x0000FFFF; 
  sum = cksum + sum;

  cksum = sum >> 16;             // Fold once more
  sum = sum & 0x0000FFFF;
  cksum = cksum + sum;

return 0xFFFF^cksum ;
}



int main(int argc , char *argv[])
{

struct tcp_hdr tcp_seg;

struct tcp_hdr *tcp_recv;
  unsigned short int cksum_arr[13],cksum_arr1[13];
  unsigned int i,sum=0, cksum;
unsigned short int cksum1,cksum2;
FILE *fp;
char *data_recv;
 
int sockfd = 0;
int n=0;
int ack;
int seq;

int connfd = 0;
int listenfd = 0;
struct sockaddr_in serv_addr;

char recvBuff[sizeof(tcp_seg)];
char recvBuff1[sizeof(tcp_seg)];

int portno;


 memset(recvBuff, 0, sizeof(tcp_seg));
/*----------
Connection with client
-------------*/

portno = atoi(argv[1]);

//create socket
 
listenfd = socket(AF_INET, SOCK_STREAM, 0);
bzero(&serv_addr,sizeof(serv_addr));

if (listenfd == -1)
    {
        printf("Could not create socket");
    }
puts("Socket created");

//Prepare the sockaddr_in structure

serv_addr.sin_family = AF_INET;
serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
serv_addr.sin_port = htons(portno);

//Bind
if((bind(listenfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)))<0)
{
//print the error message
        perror("bind failed. Error");
        return 1;
}
puts("bind done");

//Listen
listen(listenfd, 10);

//Accept and incoming connection
puts("Waiting for incoming connections...");

//accept connection from an incoming client
connfd = accept(listenfd, (struct sockaddr *)NULL,NULL);
if (connfd < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");

 //Receive a message from client
memset(recvBuff, 0, sizeof(tcp_seg));

n = recv(connfd,recvBuff, sizeof(recvBuff),0);
create_log(recvBuff);
tcp_recv = (struct tcp_hdr *)&recvBuff;  
tcp_seg= *tcp_recv ;



printf("---------receiving values from client first time-----\n\n");

int s_port=  tcp_seg.des;
int d_port=  tcp_seg.src;
 
printf("Source port %d\n",  s_port); // Printing all values
  printf("Dest port %d\n",  d_port);
  printf("Sequence 0x%08X\n", tcp_seg.seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);
  printf("Flags 0x%016X\n", tcp_seg.hdr_flags);
  printf("Rec 0x%04X\n", tcp_seg.rec);

printf("Cheksum received 0x%04X\n",tcp_seg.cksum);
cksum2 = tcp_seg.cksum;
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum = checksum(cksum_arr1);

  printf("Cheksum calculated 0x%04X\n",tcp_seg.cksum);
  printf("PTR 0x%04X\n", tcp_seg.ptr);
  printf("Optional 0x%08X\n", tcp_seg.opt);
  


tcp_seg.ack = tcp_seg.seq + 1;
tcp_seg.seq = 12;
tcp_seg.cksum = 0;
tcp_seg.hdr_flags = 000000000000010010;

/* Calculating checksum*/	

memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum = checksum(cksum_arr1);

/* populating the recvBuff*/
	 
memset(recvBuff, 0, sizeof(tcp_seg));
memcpy(recvBuff, &tcp_seg, sizeof(tcp_seg));


  printf("---------Values after implementing part 6b------- \n\n");

  printf("Source port %d\n",  s_port); // Printing all values
  printf("Dest port %d\n",  d_port);
  printf("Sequence 0x%08X\n", tcp_seg.seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);
  printf("Flags 0x%016X\n", tcp_seg.hdr_flags);
  printf("Rec 0x%04X\n", tcp_seg.rec);
  printf("Checksum 0x%04X\n",tcp_seg.cksum);
  printf("PTR 0x%04X\n", tcp_seg.ptr);
  printf("Optional 0x%08X\n", tcp_seg.opt);

/* Writing data into the socket*/

  write(connfd, recvBuff, sizeof(recvBuff));
 create_log(recvBuff);

/* Receiving data from the socket*/

memset(recvBuff, 0, sizeof(tcp_seg));
recv(connfd,recvBuff, sizeof(recvBuff),0);
create_log(recvBuff);

tcp_recv = (struct tcp_hdr *)&recvBuff;  
tcp_seg= *tcp_recv ;



  printf("-------Values after implementing part 6c received from client---- \n\n");

 printf("Source port %d\n",  s_port); // Printing all values
  printf("Dest port %d\n",  d_port);
  printf("Sequence 0x%08X\n", tcp_seg.seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);
  printf("Flags 0x%016X\n", tcp_seg.hdr_flags);
  printf("Rec 0x%04X\n", tcp_seg.rec);
  printf("Checksum received 0x%04X\n", tcp_seg.cksum);

cksum2 = tcp_seg.cksum;
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum = checksum(cksum_arr1);
printf("Cheksum calculated 0x%04X\n",tcp_seg.cksum);

  printf("PTR 0x%04X\n", tcp_seg.ptr);
  printf("Optional 0x%08X\n", tcp_seg.opt);
 
  

  printf("-------------------Receiving 128 bytes------------------ \n\n");

  memset(recvBuff, 0, sizeof(tcp_seg));
  recv(connfd,recvBuff, sizeof(recvBuff),0);
create_log(recvBuff);

  tcp_recv = (struct tcp_hdr *)&recvBuff;  
  tcp_seg= *tcp_recv ;
  ack = tcp_seg.ack;
  seq = 0 ; 
  
printf("Cheksum calculated 0x%04X\n",tcp_seg.cksum);
  printf("Sequence 0x%08X\n", tcp_seg.seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);
  printf("Ack in numbers %d\n", ack);
printf("Seq in numbers %d\n", seq);

printf("Cheksum received 0x%04X\n",tcp_seg.cksum);
cksum2 = tcp_seg.cksum;
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum = checksum(cksum_arr1);

  printf("Cheksum calculated 0x%04X\n",tcp_seg.cksum);  
  printf("DATA %s\n\n", tcp_seg.data);

printf("-------------------Sending ACK of 128------------------ \n\n");
ack = 128;
tcp_seg.ack = ack ;
tcp_seg.seq = seq +1;

seq = 0;

/* Calculating checksum*/
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum = checksum(cksum_arr1);

printf("Sequence 0x%08X\n", tcp_seg.seq);
printf("Ack 0x%08X\n", tcp_seg.ack);
printf("Ack in numbers %d\n", ack);
printf("Seq in numbers %d\n", seq);
printf("Checksum 0x%04X\n",tcp_seg.cksum);

/* Writing data into the socket*/

memset(recvBuff, 0, sizeof(tcp_seg));
memcpy(recvBuff, &tcp_seg, sizeof(tcp_seg));
write(connfd, recvBuff, sizeof(recvBuff));
create_log(recvBuff);

printf("-------------------Receiving 128 to 256 bytes------------------ \n\n");

/* Receiving data from the socket*/

  memset(recvBuff, 0, sizeof(tcp_seg));
  recv(connfd,recvBuff, sizeof(recvBuff),0);
create_log(recvBuff);

  tcp_recv = (struct tcp_hdr *)&recvBuff;  
  tcp_seg= *tcp_recv ;

  ack = tcp_seg.ack;
  seq = tcp_seg.seq;

  printf("Sequence 0x%08X\n", tcp_seg.seq);
  printf("Ack in numbers %d\n", ack);
  printf("Seq in numbers %d\n", seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);

/* Calculating checksum*/

printf("Cheksum received 0x%04X\n",tcp_seg.cksum);
cksum2 = tcp_seg.cksum;
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum = checksum(cksum_arr1);
printf("Cheksum calculated 0x%04X\n",tcp_seg.cksum);
    printf("DATA %s\n\n", tcp_seg.data);

printf("-------------------Sending ACK of 256------------------ \n\n");
ack =128 + tcp_seg.seq;
seq = tcp_seg.ack;
tcp_seg.seq = tcp_seg.ack;
tcp_seg.ack = ack ;

/* Calculating checksum*/
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum = checksum(cksum_arr1);

printf("Sequence 0x%08X\n", tcp_seg.seq);
printf("Ack 0x%08X\n", tcp_seg.ack);
printf("Ack in numbers %d\n", ack);
printf("Seq in numbers %d\n", seq);
printf("Checksum 0x%04X\n",tcp_seg.cksum);

memset(recvBuff, 0, sizeof(tcp_seg));
memcpy(recvBuff, &tcp_seg, sizeof(tcp_seg));
write(connfd, recvBuff, sizeof(recvBuff));
create_log(recvBuff);

printf("-------------------Receiving 384 bytes------------------ \n\n");
  memset(recvBuff, 0, sizeof(tcp_seg));
  recv(connfd,recvBuff, sizeof(recvBuff),0);
create_log(recvBuff);
  tcp_recv = (struct tcp_hdr *)&recvBuff;  


  tcp_seg= *tcp_recv ;
   ack = tcp_seg.ack;
   seq = tcp_seg.seq;
    printf("Sequence 0x%08X\n", tcp_seg.seq);
printf("Ack in numbers %d\n", ack);
printf("Seq in numbers %d\n", seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);
  printf("Cheksum received 0x%04X\n",tcp_seg.cksum);
cksum2 = tcp_seg.cksum;
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum = checksum(cksum_arr1);

  printf("Cheksum calculated 0x%04X\n",tcp_seg.cksum);
  printf("DATA %s\n\n", tcp_seg.data);

printf("-------------------Sending ACK of 384------------------ \n\n");
ack =128 + tcp_seg.seq;
seq = tcp_seg.ack;
tcp_seg.seq = tcp_seg.ack;
tcp_seg.ack = ack ;

/* Calculating checksum*/
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum = checksum(cksum_arr1);

printf("Sequence 0x%08X\n", tcp_seg.seq);
printf("Ack 0x%08X\n", tcp_seg.ack);
printf("Ack in numbers %d\n", ack);
printf("Seq in numbers %d\n", seq);
printf("Checksum 0x%04X\n",tcp_seg.cksum);

memset(recvBuff, 0, sizeof(tcp_seg));
memcpy(recvBuff, &tcp_seg, sizeof(tcp_seg));
write(connfd, recvBuff, sizeof(recvBuff));
create_log(recvBuff);
  
printf("-------------------Receiving 512 bytes------------------ \n\n");
  memset(recvBuff, 0, sizeof(tcp_seg));
  recv(connfd,recvBuff, sizeof(recvBuff),0);
create_log(recvBuff);

  tcp_recv = (struct tcp_hdr *)&recvBuff;  
  tcp_seg= *tcp_recv ;
  ack = tcp_seg.ack;
 
  seq = tcp_seg.seq;
  
 
  printf("Sequence 0x%08X\n", tcp_seg.seq);
printf("Ack in numbers %d\n", ack);
printf("Seq in numbers %d\n", seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);
 printf("Cheksum received 0x%04X\n",tcp_seg.cksum);
cksum2 = tcp_seg.cksum;
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum = checksum(cksum_arr1);

  printf("Cheksum calculated 0x%04X\n",tcp_seg.cksum);
  printf("DATA %s\n\n", tcp_seg.data);

printf("-------------------Sending ACK of 512------------------ \n\n");
ack =128 + tcp_seg.seq;
seq = tcp_seg.ack;
tcp_seg.seq = tcp_seg.ack;
tcp_seg.ack = ack ;

/* Calculating checksum*/
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum = checksum(cksum_arr1);

printf("Sequence 0x%08X\n", tcp_seg.seq);
printf("Ack 0x%08X\n", tcp_seg.ack);
printf("Ack in numbers %d\n", ack);
printf("Seq in numbers %d\n", seq);
printf("Checksum 0x%04X\n",tcp_seg.cksum);

memset(recvBuff, 0, sizeof(tcp_seg));
memcpy(recvBuff, &tcp_seg, sizeof(tcp_seg));
write(connfd, recvBuff, sizeof(recvBuff));
create_log(recvBuff);

  
printf("-------------------Receiving 640 bytes------------------ \n\n");
  memset(recvBuff, 0, sizeof(tcp_seg));
  recv(connfd,recvBuff, sizeof(recvBuff),0);
create_log(recvBuff);

  tcp_recv = (struct tcp_hdr *)&recvBuff;  
  tcp_seg= *tcp_recv ;
  ack = tcp_seg.ack;
 
  seq = tcp_seg.seq;
  
  printf("Sequence 0x%08X\n", tcp_seg.seq);
printf("Ack in numbers %d\n", ack);
printf("Seq in numbers %d\n", seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);
printf("Cheksum received 0x%04X\n",tcp_seg.cksum);
cksum2 = tcp_seg.cksum;
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum = checksum(cksum_arr1);

  printf("Cheksum calculated 0x%04X\n",tcp_seg.cksum);
  printf("DATA %s\n\n", tcp_seg.data);

printf("-------------------Sending ACK of 640------------------ \n\n");
ack =128 + tcp_seg.seq;
seq = tcp_seg.ack;
tcp_seg.seq = tcp_seg.ack;
tcp_seg.ack = ack ;

/* Calculating checksum*/
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum = checksum(cksum_arr1);

printf("Sequence 0x%08X\n", tcp_seg.seq);
printf("Ack 0x%08X\n", tcp_seg.ack);
printf("Ack in numbers %d\n", ack);
printf("Seq in numbers %d\n", seq);
printf("Checksum 0x%04X\n",tcp_seg.cksum);
memset(recvBuff, 0, sizeof(tcp_seg));
memcpy(recvBuff, &tcp_seg, sizeof(tcp_seg));
write(connfd, recvBuff, sizeof(recvBuff));
create_log(recvBuff);

printf("-------------------Receiving 768 bytes------------------ \n\n");
  memset(recvBuff, 0, sizeof(tcp_seg));
  recv(connfd,recvBuff, sizeof(recvBuff),0);
create_log(recvBuff);

  tcp_recv = (struct tcp_hdr *)&recvBuff;  
  tcp_seg= *tcp_recv ;
  
  ack = tcp_seg.ack;
 
  seq = tcp_seg.seq;
  
  printf("Sequence 0x%08X\n", tcp_seg.seq);
printf("Ack in numbers %d\n", ack);
printf("Seq in numbers %d\n", seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);
  printf("Cheksum received 0x%04X\n",tcp_seg.cksum);
cksum2 = tcp_seg.cksum;
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum = checksum(cksum_arr1);

  printf("Cheksum calculated 0x%04X\n",tcp_seg.cksum);
  printf("DATA %s\n\n", tcp_seg.data);

printf("-------------------Sending ACK of 768------------------ \n\n");
ack =128 + tcp_seg.seq;
seq = tcp_seg.ack;
tcp_seg.seq = tcp_seg.ack;
tcp_seg.ack = ack ;

/* Calculating checksum*/
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum = checksum(cksum_arr1);

printf("Sequence 0x%08X\n", tcp_seg.seq);
printf("Ack 0x%08X\n", tcp_seg.ack);
printf("Ack in numbers %d\n", ack);
printf("Seq in numbers %d\n", seq);
printf("Checksum 0x%04X\n",tcp_seg.cksum);
memset(recvBuff, 0, sizeof(tcp_seg));
memcpy(recvBuff, &tcp_seg, sizeof(tcp_seg));
write(connfd, recvBuff, sizeof(recvBuff));
create_log(recvBuff);

printf("-------------------Receiving 896 bytes------------------ \n\n");
  memset(recvBuff, 0, sizeof(tcp_seg));
  recv(connfd,recvBuff, sizeof(recvBuff),0);
create_log(recvBuff);

  tcp_recv = (struct tcp_hdr *)&recvBuff;  
  tcp_seg= *tcp_recv ;
  
  ack = tcp_seg.ack;
  
  seq = tcp_seg.seq;
  printf("Sequence 0x%08X\n", tcp_seg.seq);
printf("Ack in numbers %d\n", ack);
printf("Seq in numbers %d\n", seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);
  printf("Cheksum received 0x%04X\n",tcp_seg.cksum);
cksum2 = tcp_seg.cksum;
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum = checksum(cksum_arr1);

  printf("Cheksum calculated 0x%04X\n",tcp_seg.cksum);
  printf("DATA %s\n\n", tcp_seg.data);

printf("-------------------Sending ACK of 896------------------ \n\n");
ack =128 + tcp_seg.seq;
seq = tcp_seg.ack;
tcp_seg.seq = tcp_seg.ack;
tcp_seg.ack = ack ;

/* Calculating checksum*/
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum = checksum(cksum_arr1);

printf("Sequence 0x%08X\n", tcp_seg.seq);
printf("Ack 0x%08X\n", tcp_seg.ack);
printf("Ack in numbers %d\n", ack);
printf("Seq in numbers %d\n", seq);
printf("Checksum 0x%04X\n",tcp_seg.cksum);
memset(recvBuff, 0, sizeof(tcp_seg));
memcpy(recvBuff, &tcp_seg, sizeof(tcp_seg));
write(connfd, recvBuff, sizeof(recvBuff));
create_log(recvBuff);

printf("-------------------Receiving 1024 bytes------------------ \n\n");
  memset(recvBuff, 0, sizeof(tcp_seg));
  recv(connfd,recvBuff, sizeof(recvBuff),0);
create_log(recvBuff);

  tcp_recv = (struct tcp_hdr *)&recvBuff;  
  tcp_seg= *tcp_recv ;
  
  ack = tcp_seg.ack;
  
  seq = tcp_seg.seq;
  
  printf("Sequence 0x%08X\n", tcp_seg.seq);
printf("Ack in numbers %d\n", ack);
printf("Seq in numbers %d\n", seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);
  printf("Cheksum received 0x%04X\n",tcp_seg.cksum);
cksum2 = tcp_seg.cksum;
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum = checksum(cksum_arr1);

  printf("Cheksum calculated 0x%04X\n",tcp_seg.cksum);
  printf("DATA %s\n\n", tcp_seg.data);

printf("-------------------Sending ACK of 1024------------------ \n\n");
ack =128 + tcp_seg.seq;
seq = tcp_seg.ack;
tcp_seg.seq = tcp_seg.ack;
tcp_seg.ack = ack ;

/* Calculating checksum*/
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum = checksum(cksum_arr1);

printf("Sequence 0x%08X\n", tcp_seg.seq);
printf("Ack 0x%08X\n", tcp_seg.ack);
printf("Ack in numbers %d\n", ack);
printf("Seq in numbers %d\n", seq);
printf("Checksum 0x%04X\n",tcp_seg.cksum);
memset(recvBuff, 0, sizeof(tcp_seg));
memcpy(recvBuff, &tcp_seg, sizeof(tcp_seg));
write(connfd, recvBuff, sizeof(recvBuff));
create_log(recvBuff);


printf("Values after implementing part 8a received from client \n\n");
  memset(recvBuff, 0, sizeof(tcp_seg));
  recv(connfd,recvBuff, sizeof(recvBuff),0);
create_log(recvBuff);

  tcp_recv = (struct tcp_hdr *)&recvBuff;  
  tcp_seg= *tcp_recv ;  

  printf("Source port %d\n",  s_port); // Printing all values
  printf("Dest port %d\n",  d_port);
  printf("Sequence 0x%08X\n", tcp_seg.seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);
  printf("Flags 0x%016X\n", tcp_seg.hdr_flags);
  printf("Rec 0x%04X\n", tcp_seg.rec);
  printf("Cheksum received 0x%04X\n",tcp_seg.cksum);
cksum2 = tcp_seg.cksum;
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum = checksum(cksum_arr1);

  printf("Cheksum calculated 0x%04X\n",tcp_seg.cksum);
  printf("PTR 0x%04X\n", tcp_seg.ptr);
  printf("Optional 0x%08X\n", tcp_seg.opt);
 
  

printf("Values after implementing part 8b \n\n");

tcp_seg.ack = tcp_seg.seq + 1;
tcp_seg.seq = 12;
tcp_seg.hdr_flags = 000000000000010000;

 /* Calculating checksum*/
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum = checksum(cksum_arr1);

 memset(recvBuff, 0, sizeof(tcp_seg));
memcpy(recvBuff, &tcp_seg, sizeof(tcp_seg));
write(connfd, recvBuff, sizeof(recvBuff));
create_log(recvBuff);

printf("Source port %d\n",  s_port); // Printing all values
  printf("Dest port %d\n",  d_port);
  printf("Sequence 0x%08X\n", tcp_seg.seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);
  printf("Flags 0x%016X\n", tcp_seg.hdr_flags);
  printf("Rec 0x%04X\n", tcp_seg.rec);
  printf("Checksum 0x%04X\n",tcp_seg.cksum);
  printf("PTR 0x%04X\n", tcp_seg.ptr);
  printf("Optional 0x%08X\n", tcp_seg.opt);
 


printf("Values after implementing part 8c  \n\n");

tcp_seg.ack = tcp_seg.seq + 1;
 
tcp_seg.seq = 12;

tcp_seg.hdr_flags = 000000000000000001;

/* Calculating checksum*/
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum = checksum(cksum_arr1);

 memset(recvBuff, 0, sizeof(tcp_seg));
memcpy(recvBuff, &tcp_seg, sizeof(tcp_seg));
write(connfd, recvBuff, sizeof(recvBuff));
create_log(recvBuff);

printf("Source port %d\n",  s_port); // Printing all values
  printf("Dest port %d\n",  d_port);
  printf("Sequence 0x%08X\n", tcp_seg.seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);
  printf("Flags 0x%016X\n", tcp_seg.hdr_flags);
  printf("Rec 0x%04X\n", tcp_seg.rec);
  printf("Checksum 0x%04X\n", tcp_seg.cksum);
  printf("PTR 0x%04X\n", tcp_seg.ptr);
  printf("Optional 0x%08X\n", tcp_seg.opt);



  printf("Values after implementing part 8d received from client \n\n");
 memset(recvBuff, 0, sizeof(tcp_seg));
recv(connfd,recvBuff, sizeof(recvBuff),0);
tcp_recv = (struct tcp_hdr *)&recvBuff;  
create_log(recvBuff);

tcp_seg= *tcp_recv ;

  printf("Source port %d\n",  s_port); // Printing all values
  printf("Dest port %d\n",  d_port);
  printf("Sequence 0x%08X\n", tcp_seg.seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);
  printf("Flags 0x%016X\n", tcp_seg.hdr_flags);
  printf("Rec 0x%04X\n", tcp_seg.rec);
printf("Cheksum received 0x%04X\n",tcp_seg.cksum);
cksum2 = tcp_seg.cksum;
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum = checksum(cksum_arr1);

  printf("Cheksum calculated 0x%04X\n",tcp_seg.cksum);
  printf("PTR 0x%04X\n", tcp_seg.ptr);
  printf("Optional 0x%08X\n", tcp_seg.opt);
 


return 0;
}



