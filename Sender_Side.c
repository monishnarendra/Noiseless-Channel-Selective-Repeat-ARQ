#include <stdio.h> 
#include <stdlib.h> 
#include <sys/socket.h>                           //definitions of structures needed for sockets 
#include <netinet/in.h>                           //structures needed for internet domain addresses 
#include <string.h> 
 
int win; 
char array1[10],array2[10]; 
 
void update_array1(int n) 
{ 
    int i,j,n1,end; 
    n1 = n;                                       //number of frames 
    i=0;                                          //initialize to 0 
    while(n1 > 0)                                 //when 'n1' frame size is greater than 0 
    { 
        i++;                                      //increment i by 1 
        n1=n1/10; 
    } 
    end = i; 
    i--;                                          //Decrement i by 1 
    while(n > 0)                                  //when 'n' frame size is greater than 0 
    { 
        n1=n%10; 
        array1[i]=n1+48;                          //convert to char 
        i--; 
        n=n/10;
    } 
    array1[end] = '\0';                           //null character or terminator
} 
 
int convert_array_to_int() 
{ 
    char array3[9]; 
    int i=1; 
    while(array1[i]!='\0') 
    { 
        array3[i-1]=array1[i]; 
        i++; 
    } 
    array3[i-1]='\0'; 
    i=atoi(array3); 
    return i; 
} 
 
int main() 
{ 
    int n,s,frame_count,win1,i,p=0,e=0; 
    frame_count = 1; 
 
    struct sockaddr_in serv_addr;                       //Server Address... serv_addr is a structure of type Struct sockaddr_in. 
 
    s=socket(AF_INET,SOCK_STREAM,0);                    //SOCK_STREAM: TCP... Domain: AF_INET...Protocol:0 
 
    serv_addr.sin_family=AF_INET;                       //contains a code for the address family.It should always be set to AF_INET. 
    serv_addr.sin_port=6500;                            //contain the port number. 
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");   //convert the string to an integer which can be used as an Internet address. 
 
connect(s,(struct sockaddr *)&serv_addr, sizeof(serv_addr));//connect() is called by the client to establish a connection to the server 
                                                                    //connect() returns 0 on success and -1 
 
    printf("\nSuccessful Connection \n"); 
    printf("Enter the Number of Frames \n"); 
    scanf("%d",&n);                                                 //number of frames 
    printf("Enter the Window Size \n"); 
    scanf("%d",&win);                                               //window size 
 
    update_array1(win); 
    send(s,array1,sizeof(array1),0);                                //Send a array to its Peer window size
 
    update_array1(n); 
    send(s,array1,sizeof(array1),0);                                //Send a array to its Peer number of Frames
 
    strcpy(array2,"Time Out ");                                     //Copies the string to array2 
 
    while(1) 
    { 
        for(i = 0;i<win;i++) 
        { 
            update_array1(frame_count); 
            send(s,array1,sizeof(array1),0);                        //Send a Message to its Peer 
            if(frame_count<=n) 
            { 
                printf("\nFrame Number: %d Sent",frame_count); 
                frame_count++; 
            } 
        } 
        win1 = win; 
        for(i=0;i<win;i++) 
        { 
            recv(s,array1,sizeof(array1),0); 
            p=atoi(array1);                                         //takes a string as an argument and returns its as an integer value. 
            if(array1[0]=='N') 
            { 
                e=convert_array_to_int(); 
                if(e<n) 
                { 
                    printf("\nNegative Acknowledgment Received %d",e); 
                    printf("\nResending Frame Number: %d sent",e); 
                    i--; 
                } 
            } 
            else 
            { 
                if(p<=n) 
                { 
                    printf("\nFrame Number: %s Acknowledgment Received",array1); 
                    win1--; 
                } 
                else 
                { 
                    break; 
                } 
            } 
            if(p>n) 
            { 
                break; 
            } 
        } 
        if(win1==0 && frame_count>n) 
        { 
            send(s,array2,sizeof(array2),0); 
            printf("\nTime Out Error-- %s Frame Not Received",array2); 
            break; 
        } 
        else 
        { 
            frame_count=frame_count-win1; 
            win1=win; 
        } 
    } 
    close(s);                                       //To Close Connection 
    return 0; 
}
