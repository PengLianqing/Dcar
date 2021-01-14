#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include "wrap.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h> //本地套接字头文件
#include <stddef.h> //offsetof
#include <arpa/inet.h>
int ret = -1;

int main()
{
    char buf[4096]; //BUFSIZ 4096字节
    int cfd = 0;
    ssize_t len;
    struct sockaddr_un serv_addr ,clin_addr; //地址结构
    
    //clin地址结构
    bzero(&clin_addr ,sizeof(clin_addr));
    clin_addr.sun_family = AF_UNIX;
    strcpy(clin_addr.sun_path ,"clin.socket");
    len = offsetof(struct sockaddr_un ,sun_path) + strlen(clin_addr.sun_path); //计算客户端地址结构的有效地址
    unlink("clin.socket");
    
    //绑定客户端地址结构
    cfd = Socket(AF_UNIX,SOCK_STREAM,0);
    Bind(cfd ,(struct sockaddr *)&clin_addr ,len);
    
    //serv地址结构
    bzero(&serv_addr ,sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path ,"serv.socket");
    len = offsetof(struct sockaddr_un ,sun_path) + strlen(serv_addr.sun_path);
    //unlink("serv.socket"); //client中不能unlinkserv.socket，否则把server创建的socket文件删除了
    
    //连接服务器端
    Connect(cfd ,(struct sockaddr *)&serv_addr ,len);

    int i = 10 ;
    while(i--)
    {   
        sprintf(buf,"client write: %d, %5.2f ,%5.2f \n",rand()%1024,(float)(rand()%1024*0.1f),(float)(rand()%1024*0.1f));
        ret = Write(cfd , buf ,sizeof("server write: %d, %5.2f ,%5.2f \n"));
        printf("write:%d ,%s",ret ,buf);
        
        printf("Wait for Read.\n");
        ret = read(cfd ,buf ,sizeof(buf));
        write(STDOUT_FILENO,buf,ret);

        sleep(1);
    }
    close(cfd); //关闭cfd
    printf("All Done\n");
    return 0;
}

