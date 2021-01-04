/**
  ****************************(C) COPYRIGHT 2021 Peng****************************
  * @file       client.c
  * @brief      
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Jan-4-2021      Peng            1. 完成
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2021 Peng****************************
*/
#include <stdio.h>
#include "wrap.h"
#include <strings.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <sys/wait.h>
#include <stddef.h>

#include <arpa/inet.h>
#include <sys/un.h>
    
char buf[4096]; //用于通信
int ret = -1; 

/**
  * @brief          本地套接字通信服务器端
  * @param[out]     none
  * @retval         none
  */
int main()
{
    int lfd = 0,cfd = 0;
    
    //配置服务器地址结构
    struct sockaddr_un serv_addr , clin_addr;
    //memset(&serv_addr ,0 ,sizeof(serv_addr));
    bzero(&serv_addr ,sizeof(serv_addr)); //将地址结构清零 <strings.h>
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path ,"serv.socket");
    size_t len = offsetof(struct sockaddr_un ,sun_path) + strlen("serv.socket");
    unlink("serv.socket");

    //创建socket文件描述符
    lfd = Socket(AF_UNIX,SOCK_STREAM,0);
    //为socket文件描述符绑定服务器端地址结构
    ret = Bind(lfd ,(struct sockaddr *)&serv_addr ,len);
    //设置与服务器建立的最大连接数
    Listen(lfd ,128);

    socklen_t clin_addr_len = sizeof(clin_addr);
    
    while(1)
    {
        //等待客户端连接
        cfd = Accept(lfd ,(struct sockaddr *)&clin_addr ,&clin_addr_len);
        
        //文件名最后加上\0,防止显示乱码
        len -= offsetof(struct sockaddr_un ,sun_path);
        clin_addr.sun_path[len] = '\0';
        printf("clinte bind filename: %s\n" ,clin_addr.sun_path);

        //Read=0时说明连接已断开
        while( ( ret=Read(cfd ,buf ,sizeof(buf)) ) > 0 )
        {
            printf("read %d \n",ret);
            Write(STDOUT_FILENO ,buf ,ret);

            sprintf(buf ,"Server:%5.2f,%5.2f,%d,%d\n" ,(float)(rand()%1024*1.0f) ,(float)(rand()%1024*1.0f) ,rand()%1024 ,rand()%1024);
            Write(cfd ,buf ,strlen(buf));
            memset(buf ,0 ,strlen(buf));
        }
        close(cfd);
        printf("disconnect from client.\n");
    }
    close(lfd);
    printf("All done\n");
    return 0;
}
