#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>



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

FILE *f = fopen("client_out.txt", "a+");
if (f == NULL)
{
    printf("Error opening file!\n");
    exit(1);
}

 // fprintf(f,"Source port %d\n",  s_port); 
 // fprintf(f,"Dest port %d\n",  d_port);
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
{		
 					              // Compute sum
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


int main(int argc, char *argv[]) {
 struct tcp_hdr tcp_seg;
  int ack;
int seq;
char c;
   int sockfd, portno,s_port, n1,n2;
   struct sockaddr_in serv_addr;
   struct hostent *server;
   char buffer[sizeof(tcp_seg)];
char buffer1[sizeof(tcp_seg)];
struct tcp_hdr *tcp_recv;
struct tcp_hdr *tcp_recv1;
int init_seq = 78;
char data[128];

FILE *fp;

  unsigned short int cksum_arr[13];
  unsigned short int cksum_arr1[13];
  unsigned int i,sum=0;

unsigned short int cksum1,cksum2;
 	
   
  
  
  /* XOR the sum for checksum */
   
   if (argc < 2) {
      fprintf(stderr,"usage %s hostname port\n", argv[0]);
      exit(0);
   }
	portno = atoi(argv[1]);

/* Create a socket point */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }
	 bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = inet_addr("129.120.151.94");
   serv_addr.sin_port = htons(portno);


  
/* Now connect to the server */
   if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR connecting");
      exit(1);
   }
         /*Finding the port number of the client*/
         int sa_len;
         /* We must put the length in a variable.              */
         sa_len = sizeof(serv_addr);
         if (getsockname(sockfd  , (struct sockaddr *)&serv_addr, &sa_len) == -1)
         perror("getsockname");
         else
         printf("port number %d\n", ntohs(serv_addr.sin_port));
         s_port = ntohs(serv_addr.sin_port);

  tcp_seg.src = s_port;
  tcp_seg.des = portno;
  tcp_seg.seq = init_seq;
  tcp_seg.ack = 0;
  tcp_seg.hdr_flags = 0000000000000010;			
  tcp_seg.rec = 0;
  tcp_seg.ptr = 0;
  tcp_seg.opt = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
  tcp_seg.cksum = checksum(cksum_arr1);
 


int d_port=  tcp_seg.des;
 
printf("---------Values after implementing part 6a----------- \n\n");

printf("Source port %d\n",  s_port); // Printing all values
  printf("Dest port %d\n",  d_port);
  printf("Sequence 0x%08X\n", tcp_seg.seq);
  printf("Flags 0x%016X\n", tcp_seg.hdr_flags);
  printf("Rec 0x%04X\n", tcp_seg.rec);
  printf("Checksum calculated 0x%04X\n", tcp_seg.cksum);
  printf("PTR 0x%04X\n", tcp_seg.ptr);
  printf("Optional 0x%08X\n", tcp_seg.opt);
 
  
memset(buffer1, 0, sizeof(tcp_seg));
memcpy(buffer1, &tcp_seg, sizeof(tcp_seg));
create_log(buffer1);
/* Send message to the server */

   n1 = write(sockfd, buffer1, sizeof(buffer1));
    
   if (n1 < 0) {
      perror("ERROR writing to socket");
      exit(1);
   }


memset(buffer1, 0, sizeof(tcp_seg));
recv(sockfd,buffer1, sizeof(buffer1),0);
create_log(buffer1);
 

 /* Now read server response */
tcp_recv = (struct tcp_hdr *)&buffer1;  
tcp_seg= *tcp_recv ;


printf("---------Values received from server after part 6b -----------\n\n");



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
tcp_seg.cksum =  checksum(cksum_arr1);

  printf("Checksum calculated 0x%04X\n",tcp_seg.cksum);
  printf("PTR 0x%04X\n", tcp_seg.ptr);
  printf("Optional 0x%08X\n", tcp_seg.opt);
  

tcp_seg.ack = tcp_seg.seq + 1;  
tcp_seg.seq = init_seq + 1;
tcp_seg.cksum = 0;
tcp_seg.hdr_flags = 000000000000010000;	

memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum =checksum(cksum_arr1);
memset(buffer1, 0, sizeof(tcp_seg));
memcpy(buffer1, &tcp_seg, sizeof(tcp_seg));

printf("----Values after implementing part 6c -------\n\n");

printf("Source port %d\n",  s_port); // Printing all values
  printf("Dest port %d\n",  d_port);
  printf("Sequence 0x%08X\n", tcp_seg.seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);
  printf("Flags 0x%016X\n", tcp_seg.hdr_flags);
  printf("Rec 0x%04X\n", tcp_seg.rec);
  printf("Checksum 0x%04X\n",tcp_seg.cksum);
  printf("PTR 0x%04X\n", tcp_seg.ptr);
  printf("Optional 0x%08X\n", tcp_seg.opt);


/* Send message to the server */
   n1 = write(sockfd, buffer1, sizeof(buffer1));
    create_log(buffer1);
   if (n1 < 0) {
      perror("ERROR writing to socket");
      exit(1);
   }



printf("-------------sending to server first 128 bytes------------- \n\n");
fp = fopen("Datafile.txt" , "a+");

for(i = 0; i < 127 && (c = getc(fp)) != EOF; ++i)
{        tcp_seg.data[i] = c;
 
}
fclose(fp);


tcp_seg.seq = 0 ; 
tcp_seg.ack = 0;

ack = 0;
/* Calculating checksum  */

tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum = checksum(cksum_arr1);

  printf("Sequence 0x%08X\n", tcp_seg.seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);
  printf("Ack in numbers %d\n", ack);
  printf("Seq in numbers %d\n", seq);
  printf("Checksum 0x%04X\n", tcp_seg.cksum);
  printf("DATA %s\n\n", tcp_seg.data);
  memset(buffer1, 0, sizeof(tcp_seg));
  memcpy(buffer1, &tcp_seg, sizeof(tcp_seg));
  n1 = write(sockfd, buffer1, sizeof(buffer1));
create_log(buffer1);
printf("-------------Receiving from server ACK 128------------------ \n\n");

memset(buffer1, 0, sizeof(tcp_seg));
recv(sockfd,buffer1, sizeof(buffer1),0);
create_log(buffer1);
/* Now read server response */

tcp_recv = (struct tcp_hdr *)&buffer1;  
tcp_seg= *tcp_recv ;

seq = tcp_seg.seq;
ack = 0;

memset(buffer1, 0, sizeof(tcp_seg));
memcpy(buffer1, &tcp_seg, sizeof(tcp_seg));
 ack = tcp_seg.ack;
  printf("Sequence 0x%08X\n", tcp_seg.seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);
   printf("Ack in numbers %d\n", ack);
printf("Seq in numbers %d\n", seq);
 
printf("Cheksum received 0x%04X\n",tcp_seg.cksum);
cksum2 = tcp_seg.cksum;
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum =  checksum(cksum_arr1);

  printf("Checksum calculated 0x%04X\n",tcp_seg.cksum);
  

 printf("---------Sending 128 to 256 and ack 1---------------  \n\n");

fp = fopen("Datafile.txt" , "a+");
fseek( fp, ack + 1, SEEK_SET );

for(i = 0; i < 127 && (c = getc(fp)) != EOF; ++i)
tcp_seg.data[i] = c;

fclose(fp);
ack = tcp_seg.seq + 1; 
tcp_seg.seq = tcp_seg.ack ;
seq = tcp_seg.seq ; 
tcp_seg.ack = ack;


/* Calculating checksum  */
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum = checksum(cksum_arr1); 

  printf("Sequence 0x%08X\n", tcp_seg.seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);
  printf("Ack in numbers %d\n", ack);
printf("Seq in numbers %d\n", seq);
  printf("Checksum 0x%04X\n", tcp_seg.cksum);
  printf("DATA %s\n\n", tcp_seg.data);
  memset(buffer1, 0, sizeof(tcp_seg));
  memcpy(buffer1, &tcp_seg, sizeof(tcp_seg));
  n1 = write(sockfd, buffer1, sizeof(buffer1));
create_log(buffer1);


printf("-------------Receiving from server ACK 256------------------ \n\n");

memset(buffer1, 0, sizeof(tcp_seg));
recv(sockfd,buffer1, sizeof(buffer1),0);
create_log(buffer1);
/* Now read server response */

tcp_recv = (struct tcp_hdr *)&buffer1;  
tcp_seg= *tcp_recv ;

memset(buffer1, 0, sizeof(tcp_seg));
memcpy(buffer1, &tcp_seg, sizeof(tcp_seg));


  ack = tcp_seg.ack;
  seq = tcp_seg.seq;
  printf("Sequence 0x%08X\n", tcp_seg.seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);
  printf("Ack in numbers %d\n", ack);
printf("Seq in numbers %d\n", seq);
  

printf("Cheksum received 0x%04X\n",tcp_seg.cksum);
cksum2 = tcp_seg.cksum;
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum =  checksum(cksum_arr1);

  printf("Checksum calculated 0x%04X\n",tcp_seg.cksum);
  

 printf("---------Sending 256 to 384---------------  \n\n");

fp = fopen("Datafile.txt" , "a+");
fseek( fp, ack + 1, SEEK_SET );

for(i = 0; i < 127 && (c = getc(fp)) != EOF; ++i)
tcp_seg.data[i] = c;

fclose(fp);
ack = tcp_seg.seq + 1; 
tcp_seg.seq = tcp_seg.ack ;
seq = tcp_seg.seq ; 
tcp_seg.ack = ack;

  /* Calculating checksum  */
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum = checksum(cksum_arr1);  
 
  printf("Sequence 0x%08X\n", tcp_seg.seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);
  printf("Ack in numbers %d\n", ack);
printf("Seq in numbers %d\n", seq);
  printf("Checksum 0x%04X\n", tcp_seg.cksum);
  printf("DATA %s\n\n", tcp_seg.data);

  memset(buffer1, 0, sizeof(tcp_seg));
  memcpy(buffer1, &tcp_seg, sizeof(tcp_seg));
  n1 = write(sockfd, buffer1, sizeof(buffer1));
create_log(buffer1);

printf("-------------Receiving from server ACK 384------------------ \n\n");

memset(buffer1, 0, sizeof(tcp_seg));
recv(sockfd,buffer1, sizeof(buffer1),0);
create_log(buffer1);
/* Now read server response */

tcp_recv = (struct tcp_hdr *)&buffer1;  
tcp_seg= *tcp_recv ;

memset(buffer1, 0, sizeof(tcp_seg));
memcpy(buffer1, &tcp_seg, sizeof(tcp_seg));


  ack = tcp_seg.ack;
  seq = tcp_seg.seq;
  printf("Sequence 0x%08X\n", tcp_seg.seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);
  printf("Ack in numbers %d\n", ack);
printf("Seq in numbers %d\n", seq);

  printf("Cheksum received 0x%04X\n",tcp_seg.cksum);
cksum2 = tcp_seg.cksum;
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum =  checksum(cksum_arr1);

  printf("Checksum calculated 0x%04X\n",tcp_seg.cksum);

 printf("---------Sending 384 to 512---------------  \n\n");

fp = fopen("Datafile.txt" , "a+");
fseek( fp, ack + 1, SEEK_SET );

for(i = 0; i < 127 && (c = getc(fp)) != EOF; ++i)
tcp_seg.data[i] = c;

fclose(fp);
ack = tcp_seg.seq + 1; 
tcp_seg.seq = tcp_seg.ack ;
seq = tcp_seg.seq ; 
tcp_seg.ack = ack;
  
  /* Calculating checksum  */
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum = checksum(cksum_arr1);

  
  printf("Sequence 0x%08X\n", tcp_seg.seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);
  printf("Ack in numbers %d\n", ack);
printf("Seq in numbers %d\n", seq);
  printf("Checksum 0x%04X\n", tcp_seg.cksum);
  printf("DATA %s\n\n", tcp_seg.data);

  memset(buffer1, 0, sizeof(tcp_seg));
  memcpy(buffer1, &tcp_seg, sizeof(tcp_seg));
  n1 = write(sockfd, buffer1, sizeof(buffer1));
create_log(buffer1);

printf("-------------Receiving from server ACK 512------------------ \n\n");

memset(buffer1, 0, sizeof(tcp_seg));
recv(sockfd,buffer1, sizeof(buffer1),0);
create_log(buffer1);
/* Now read server response */

tcp_recv = (struct tcp_hdr *)&buffer1;  
tcp_seg= *tcp_recv ;


memset(buffer1, 0, sizeof(tcp_seg));
memcpy(buffer1, &tcp_seg, sizeof(tcp_seg));


  ack = tcp_seg.ack;
  seq = tcp_seg.seq;
  printf("Sequence 0x%08X\n", tcp_seg.seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);
  printf("Ack in numbers %d\n", ack);
printf("Seq in numbers %d\n", seq);

   printf("Cheksum received 0x%04X\n",tcp_seg.cksum);
cksum2 = tcp_seg.cksum;
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum =  checksum(cksum_arr1);

  printf("Checksum calculated 0x%04X\n",tcp_seg.cksum);

 printf("---------Sending 512 to 640---------------  \n\n");

fp = fopen("Datafile.txt" , "a+");
fseek( fp, ack + 1, SEEK_SET );

for(i = 0; i < 127 && (c = getc(fp)) != EOF; ++i)
tcp_seg.data[i] = c;

fclose(fp);
ack = tcp_seg.seq + 1; 
tcp_seg.seq = tcp_seg.ack ;
seq = tcp_seg.seq ; 
tcp_seg.ack = ack;

  /* Calculating checksum  */
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum = checksum(cksum_arr1); 

  printf("Sequence 0x%08X\n", tcp_seg.seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);
  printf("Ack in numbers %d\n", ack);
printf("Seq in numbers %d\n", seq);
  printf("Checksum 0x%04X\n", tcp_seg.cksum);
  printf("DATA %s\n\n", tcp_seg.data);
  memset(buffer1, 0, sizeof(tcp_seg));
  memcpy(buffer1, &tcp_seg, sizeof(tcp_seg));
  n1 = write(sockfd, buffer1, sizeof(buffer1));
create_log(buffer1);

printf("-------------Receiving from server ACK 640------------------ \n\n");

memset(buffer1, 0, sizeof(tcp_seg));
recv(sockfd,buffer1, sizeof(buffer1),0);
create_log(buffer1);
/* Now read server response */

tcp_recv = (struct tcp_hdr *)&buffer1;  
tcp_seg= *tcp_recv ;

memset(buffer1, 0, sizeof(tcp_seg));
memcpy(buffer1, &tcp_seg, sizeof(tcp_seg));


  ack = tcp_seg.ack;
  seq = tcp_seg.seq;
  printf("Sequence 0x%08X\n", tcp_seg.seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);
  printf("Ack in numbers %d\n", ack);
printf("Seq in numbers %d\n", seq);

   printf("Cheksum received 0x%04X\n",tcp_seg.cksum);
cksum2 = tcp_seg.cksum;
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum =  checksum(cksum_arr1);

  printf("Checksum calculated 0x%04X\n",tcp_seg.cksum);
  

printf("---------Sending 640 to 768---------------  \n\n");

fp = fopen("Datafile.txt" , "a+");
fseek( fp, ack + 1, SEEK_SET );

for(i = 0; i < 127 && (c = getc(fp)) != EOF; ++i)
tcp_seg.data[i] = c;

fclose(fp);
ack = tcp_seg.seq + 1; 
tcp_seg.seq = tcp_seg.ack ;
seq = tcp_seg.seq ; 
tcp_seg.ack = ack;

  /* Calculating checksum  */
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum = checksum(cksum_arr1);   
  
  printf("Sequence 0x%08X\n", tcp_seg.seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);
  printf("Ack in numbers %d\n", ack);
printf("Seq in numbers %d\n", seq);
  printf("Checksum 0x%04X\n", tcp_seg.cksum);
  printf("DATA %s\n\n", tcp_seg.data);
  memset(buffer1, 0, sizeof(tcp_seg));
  memcpy(buffer1, &tcp_seg, sizeof(tcp_seg));
  n1 = write(sockfd, buffer1, sizeof(buffer1));
create_log(buffer1);

printf("-------------Receiving from server ACK 768------------------ \n\n");

memset(buffer1, 0, sizeof(tcp_seg));
recv(sockfd,buffer1, sizeof(buffer1),0);
create_log(buffer1);
/* Now read server response */

tcp_recv = (struct tcp_hdr *)&buffer1;  
tcp_seg= *tcp_recv ;

memset(buffer1, 0, sizeof(tcp_seg));
memcpy(buffer1, &tcp_seg, sizeof(tcp_seg));


  ack = tcp_seg.ack;
  seq = tcp_seg.seq;
  printf("Sequence 0x%08X\n", tcp_seg.seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);
  printf("Ack in numbers %d\n", ack);
printf("Seq in numbers %d\n", seq);

  printf("Cheksum received 0x%04X\n",tcp_seg.cksum);
cksum2 = tcp_seg.cksum;
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum =  checksum(cksum_arr1);

  printf("Checksum calculated 0x%04X\n",tcp_seg.cksum);


printf("---------Sending 768 to 896---------------  \n\n");

fp = fopen("Datafile.txt" , "a+");
fseek( fp, ack + 1, SEEK_SET );

for(i = 0; i < 127 && (c = getc(fp)) != EOF; ++i)
tcp_seg.data[i] = c;

fclose(fp);
ack = tcp_seg.seq + 1; 
tcp_seg.seq = tcp_seg.ack ;
seq = tcp_seg.seq ; 
tcp_seg.ack = ack;
  
/* Calculating checksum  */
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum = checksum(cksum_arr1); 
 
  printf("Sequence 0x%08X\n", tcp_seg.seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);
  printf("Ack in numbers %d\n", ack);
printf("Seq in numbers %d\n", seq);
  printf("Checksum 0x%04X\n", tcp_seg.cksum);
  printf("DATA %s\n\n", tcp_seg.data);

  memset(buffer1, 0, sizeof(tcp_seg));
  memcpy(buffer1, &tcp_seg, sizeof(tcp_seg));
  n1 = write(sockfd, buffer1, sizeof(buffer1));
create_log(buffer1);

printf("-------------Receiving from server ACK 896------------------ \n\n");

memset(buffer1, 0, sizeof(tcp_seg));
recv(sockfd,buffer1, sizeof(buffer1),0);
create_log(buffer1);
/* Now read server response */

tcp_recv = (struct tcp_hdr *)&buffer1;  
tcp_seg= *tcp_recv ;
cksum2 = tcp_seg.cksum;

memset(buffer1, 0, sizeof(tcp_seg));
memcpy(buffer1, &tcp_seg, sizeof(tcp_seg));


  ack = tcp_seg.ack;
  seq = tcp_seg.seq;
  printf("Sequence 0x%08X\n", tcp_seg.seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);
  printf("Ack in numbers %d\n", ack);
printf("Seq in numbers %d\n", seq);

   printf("Cheksum received 0x%04X\n",tcp_seg.cksum);
cksum2 = tcp_seg.cksum;
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum =  checksum(cksum_arr1);

  printf("Checksum calculated 0x%04X\n",tcp_seg.cksum);

printf("---------Sending 896 to 1024---------------  \n\n");

fp = fopen("Datafile.txt" , "a+");
fseek( fp, ack + 1, SEEK_SET );

for(i = 0; i < 127 && (c = getc(fp)) != EOF; ++i)
tcp_seg.data[i] = c;

fclose(fp);
ack = tcp_seg.seq + 1; 
tcp_seg.seq = tcp_seg.ack ;
seq = tcp_seg.seq ; 
tcp_seg.ack = ack;

/* Calculating checksum  */
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum = checksum(cksum_arr1);
 
  printf("Sequence 0x%08X\n", tcp_seg.seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);
  printf("Ack in numbers %d\n", ack);
printf("Seq in numbers %d\n", seq);
  printf("Checksum 0x%04X\n", tcp_seg.cksum);
  printf("DATA %s\n\n", tcp_seg.data);
  memset(buffer1, 0, sizeof(tcp_seg));
  memcpy(buffer1, &tcp_seg, sizeof(tcp_seg));
  n1 = write(sockfd, buffer1, sizeof(buffer1));
create_log(buffer1);

printf("-------------Receiving from server ACK 1024------------------ \n\n");

memset(buffer1, 0, sizeof(tcp_seg));
recv(sockfd,buffer1, sizeof(buffer1),0);
create_log(buffer1);
/* Now read server response */

tcp_recv = (struct tcp_hdr *)&buffer1;  
tcp_seg= *tcp_recv ;
memset(buffer1, 0, sizeof(tcp_seg));
memcpy(buffer1, &tcp_seg, sizeof(tcp_seg));


  ack = tcp_seg.ack;
  seq = tcp_seg.seq;
  printf("Sequence 0x%08X\n", tcp_seg.seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);
  printf("Ack in numbers %d\n", ack);
printf("Seq in numbers %d\n", seq);

  printf("Cheksum received 0x%04X\n",tcp_seg.cksum);
cksum2 = tcp_seg.cksum;
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum =  checksum(cksum_arr1);

  printf("Checksum calculated 0x%04X\n",tcp_seg.cksum);


printf("Values after implementing part 8a sending to server \n\n");
tcp_seg.seq = 0 ; 
tcp_seg.ack = 0;
tcp_seg.hdr_flags = 000000000000000001;	

/* Calculating checksum  */
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum = checksum(cksum_arr1);


  printf("Source port %d\n",  s_port); // Printing all values
  printf("Dest port %d\n",  d_port);
  printf("Sequence 0x%08X\n", tcp_seg.seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);
  printf("Flags 0x%016X\n", tcp_seg.hdr_flags);
  printf("Rec 0x%04X\n", tcp_seg.rec);
  printf("Checksum 0x%04X\n", tcp_seg.cksum);
  printf("PTR 0x%04X\n", tcp_seg.ptr);
  printf("Optional 0x%08X\n", tcp_seg.opt);
 

  memset(buffer1, 0, sizeof(tcp_seg));
  memcpy(buffer1, &tcp_seg, sizeof(tcp_seg));

 n1 = write(sockfd, buffer1, sizeof(buffer1));
 create_log(buffer1);   
   if (n1 < 0) {
      perror("ERROR writing to socket");
      exit(1);
   }


 memset(buffer1, 0, sizeof(tcp_seg));
recv(sockfd,buffer1, sizeof(buffer1),0);
create_log(buffer1);
 /* Now read server response */
tcp_recv = (struct tcp_hdr *)&buffer1;  
tcp_seg= *tcp_recv ;



printf("Values after implementing part 8b receiving from server \n\n");
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


 memset(buffer1, 0, sizeof(tcp_seg));
recv(sockfd,buffer1, sizeof(buffer1),0);
create_log(buffer1);
 /* Now read server response */
tcp_recv = (struct tcp_hdr *)&buffer1;  
tcp_seg= *tcp_recv ;



 memcpy(cksum_arr1, &tcp_seg, 24);



printf("Values after implementing part 8c receiving from server \n\n");
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


tcp_seg.seq = tcp_seg.seq + 1 ; 
//tcp_seg.ACK = 1;
tcp_seg.hdr_flags = 000000000000010000;

/* Calculating checksum  */
tcp_seg.cksum = 0;
memcpy(cksum_arr1, &tcp_seg, 24);
tcp_seg.cksum = checksum(cksum_arr1);

printf("Values after implementing part 8d sending to server \n\n");
 printf("Source port %d\n",  s_port); // Printing all values
  printf("Dest port %d\n",  d_port);
  printf("Sequence 0x%08X\n", tcp_seg.seq);
  printf("Ack 0x%08X\n", tcp_seg.ack);
  printf("Flags 0x%016X\n", tcp_seg.hdr_flags);
  printf("Rec 0x%04X\n", tcp_seg.rec);
  printf("Checksum 0x%04X\n", tcp_seg.cksum);
  printf("PTR 0x%04X\n", tcp_seg.ptr);
  printf("Optional 0x%08X\n", tcp_seg.opt);

 memset(buffer1, 0, sizeof(tcp_seg));

  memcpy(buffer1, &tcp_seg, sizeof(tcp_seg));
memcpy(buffer1, &tcp_seg, sizeof(tcp_seg));
 n1 = write(sockfd, buffer1, sizeof(buffer1));
 create_log(buffer1);   
   if (n1 < 0) {
      perror("ERROR writing to socket");
      exit(1);
   }




return 0;
}
