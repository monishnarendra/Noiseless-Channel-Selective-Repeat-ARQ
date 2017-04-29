#include<stdio.h> 
#include<sys/socket.h>                //definitions of structures needed for sockets 
#include<netinet/in.h>                //structures needed for internet domain addresses 
#include<string.h> 
#include<time.h> 
#include<stdlib.h> 
 
char array1[10], array2[10]; 
 
void update_array1(int n) 
{ 
    int n1,i=0,end; 
    n1=n; 
    while(n1>0) 
    { 
        i++; 
        n1=n1/10; 
    } 
    end=i; 
    i--; 
    while(n>0) 
    { 
        n1=n%10; 
        array1[i]=n1+48; 
        i--; 
        n=n/10; 
    } 
    array1[end]='\0'; 
} 
 
void update_array2(int n) 
{ 
    int n1,i=1,end; 
    n1=n; 
    array2[0]='N'; 
    while(n1>0) 
    { 
        i++; 
        n1=n1/10; 
    } 
    end=i; 
    i--; 
    while(n>0) 
    { 
        n1=n%10; 
        array2[i]=n1+48; 
        i--; 
        n=n/10; 
    } 
    array2[end]='\0'; 
} 
 
int main() 
{ 
    int s,n,sock,i,j,frame_count=1,frame_size,window_size = 5,P1 = 50,P2 = 10; 
    unsigned int s1; 
     
    struct sockaddr_in ser,cli;                     //Server Address... serv_addr is a structure of type Struct sockaddr_in. 
     
    s=socket(AF_INET,SOCK_STREAM,0);                //SOCK_STREAM: TCP... Domain: AF_INET...Protocol:0 
    ser.sin_family=AF_INET;                         //contains a code for the address family.It should always be set to AF_INET. 
    ser.sin_port=6500;                              //contain the port number. 
    ser.sin_addr.s_addr=inet_addr("127.0.0.1");     //convert the string to an integer which can be used as an Internet address. 
    bind(s,(struct sockaddr *) &ser, sizeof(ser)); 
    listen(s,1); 
    n=sizeof(cli); 
    sock=accept(s,(struct sockaddr *)&cli, &n); 
    printf("\nSuccessful Connection\n");
    
    s1=(unsigned int) time(NULL);                   //To Prevent Sequence of Similar Number  
    srand(s1);                                      //To Generate Random Number             
    
    strcpy(array2,"Time Out ");                     //Copy 'Time Out' String to array2
 
    recv(sock,array1,sizeof(array1),0);             //receives number of window size 
    window_size=atoi(array1);     
 
    recv(sock,array1,sizeof(array1),0);             //receives number of frames 
    frame_size=atoi(array1); 
 
    while(1) 
    { 
        for(i=0;i<window_size;i++) 
        { 
            recv(sock,array1,sizeof(array1),0); 
            if(strcmp(array1,array2)==0)            //If the data recived is equalent to   
            { 
                break; 
            } 
        } 
        for(i=0;i<window_size;i++) 
        { 
            L: 
                j=rand()%P1; 
            if(j<P2) 
            { 
                update_array2(frame_count); 
                printf("\nTimeOut Error %s Frame Not Recived",array2); 
                send(sock,array2,sizeof(array2),0); 
                goto L; 
            } 
            else 
            { 
                update_array1(frame_count); 
                if(frame_count<=frame_size) 
                { 
                    printf("\nFrame Number: %s Received ",array1); 
                    send(sock,array1,sizeof(array1),0); 
                } 
                else 
                { 
                    break; 
                } 
                frame_count++; 
            } 
            if(frame_count>frame_size) 
            { 
                break; 
            } 
        } 
    } 
    close(sock);                                  //Close the Connection 
    close(s);                                     //Close the Connection
    return 0; 
}
