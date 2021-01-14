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
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>

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

//根据具体的设备修改
const char default_path[] = "/dev/ttyUSB0";
int tty_init( void );
int socket_init(int *mlfd ,int *mcfd);
//发送的数据：
struct control_date_t{
	int data1;
	int data2;
	float data3;
	float data4; //%5.2f -300.0f-300.0f
	int mode1;
	int mode2;
};
struct control_date_t control_date;

//接受的数据
struct feedback_data_t{
	float imu_angel[3]; //pitch,yaw,roll
	float imu_speed[3];
	int channel[3]; //前进，刹车，转向 -30000~+30000
	int mode0;
	int mode1;
	int mode2;
};
struct feedback_data_t feedback_data;
struct feedback_data_t translate_data;

char tx_buf[128];
char socket_buf[4096]; //用于通信

int i=0;

//#define USE_READ
//#define USE_WRITE

int send_mode_last=0;

int main(int argc, char *argv[])
{
	int fd; //tty dev的fd
	int res = -1 ,ret = -1;
	
	char buf[98];

    //初始化tty串口
    fd = tty_init();
	
    //初始化socket
    int lfd = 0,cfd = 0;
    ret = socket_init(&lfd ,&cfd);

    //计算丢包率
	static int receive_num[2]={0,0};

	while(1)
	{	
		/*read*/
		#define USE_PRINTF_TRANSLATE 1
		#if USE_PRINTF_TRANSLATE
			char receivebuf[256];
			res = read(fd,receivebuf,sizeof(receivebuf));
            sscanf((const char *)receivebuf , "D:%f,%f,%f,%f,%f,%f,%d,%d,%d,%d,%d,%d\n"
                ,feedback_data.imu_angel[0] ,
                feedback_data.imu_angel[1] ,
                feedback_data.imu_angel[2] ,
                feedback_data.imu_speed[0] ,
                feedback_data.imu_speed[1] ,
                feedback_data.imu_speed[2] ,
                feedback_data.channel[0] ,
                feedback_data.channel[1] ,
                feedback_data.channel[2] ,
                feedback_data.mode0 ,
                feedback_data.mode1 ,
                feedback_data.mode2 );
    
            printf("%s,%d\n",receivebuf ,feedback_data.mode0);
            memset(receivebuf ,0 ,strlen(receivebuf));

			printf("###############receive data %d\n",res);
			if(res >0)
			{
				receive_num[0] ++;
			}
			if( receivebuf[res-1] == '\n')
			{
				receive_num[1] ++;
				printf("receive %s",receivebuf ,strlen(receivebuf));
				printf("------available data: %5.2f\n",(float)(receive_num[1])/(float)(receive_num[0]));
			}
			memset(receivebuf ,0 ,strlen(receivebuf));
		#else
			res = read(fd,buf,98);
			printf("###############receive data %d\n",res);
			#define RS232_RX_BUF_LEN (sizeof(struct feedback_data_t)+1) 
			if(res >0)
			{
				receive_num[0] ++;
			}
			if(res == (RS232_RX_BUF_LEN) && buf[RS232_RX_BUF_LEN-1] == '\n')
			{
				receive_num[1] ++;
				memcpy((void*)(&feedback_data), (void*)(buf), sizeof(struct feedback_data_t));
				printf("receive %d, date:%5.2f , %d ",res ,feedback_data.imu_angel[0] ,feedback_data.mode0);
				printf("------available data: %5.2f\r\n",(float)(receive_num[1])/(float)(receive_num[0]));
			}
			memset(buf ,0 ,98);
		#endif

        /*write*/
		#ifdef USE_WRITE
		control_date.data1 = rand()%1024;
		control_date.data2 = rand()%1024;
		control_date.data3 = (float)(rand()%1024*0.1f);
		control_date.data4 = (float)(rand()%1024*0.1f);
		control_date.mode1 = rand()%3;
		control_date.mode2 = rand()%3;

		//打包发送会出现接受错误
		//memcpy((void*)(tx_buf), (void*)(&control_date), sizeof(struct control_date_t));
		//tx_buf[sizeof(struct control_date_t)] = '\n';
		//res = write(fd, tx_buf, sizeof(struct control_date_t)+1);
		
		memset(tx_buf ,0 ,sizeof(tx_buf));
		static int i=0;
		sprintf(tx_buf,"\nB:%d,%d,%5.2f,%5.2f,%d,%d$\n",control_date.data1,control_date.data2,control_date.data3,control_date.data4,control_date.mode1,i++);
		
		res = write(fd, tx_buf, strlen(tx_buf));
		printf("translate %d,%s\r\n",res,tx_buf);
		#endif
        
        /*socket*/
		/*
        if(send_mode_last != feedback_data.mode0)
        {
            sprintf(socket_buf ,"Server:%5.2f,%5.2f,%d,%d\n" ,(float)(rand()%1024*1.0f) ,(float)(rand()%1024*1.0f) ,rand()%1024 ,feedback_data.mode0);
            Write(cfd ,socket_buf ,strlen(socket_buf));
            memset(socket_buf ,0 ,strlen(socket_buf));
        }
        send_mode_last = feedback_data.mode0;
		*/
		
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


        /*end while*/
	}
	printf("read error,res = %d",res);
	tcflush(fd, TCIOFLUSH);
	close(fd);
	return 0;
}


int tty_init( void )
{
    int fd;
    struct termios opt;
    char *path;

    path = (char *)default_path;
    printf("Init tty dev %s.\n" ,path);

	//获取串口设备描述符
	fd = open(path, O_RDWR);
	if(fd < 0){
		printf("Fail to Open %s device\n", path);
		return 0;
	}

    //清空串口接收缓冲区
	tcflush(fd, TCIOFLUSH);
	// 获取串口参数opt
	tcgetattr(fd, &opt);

	//设置串口输出波特率
	cfsetospeed(&opt, B115200);
	//设置串口输入波特率
	cfsetispeed(&opt, B115200);
	//设置数据位数
	opt.c_cflag &= ~CSIZE;
	opt.c_cflag |= CS8;
	//校验位
	opt.c_cflag &= ~PARENB;
	opt.c_iflag &= ~INPCK;
	//设置停止位
	opt.c_cflag &= ~CSTOPB;

	//更新配置
	tcsetattr(fd, TCSANOW, &opt);  

    return fd; 
}

int socket_init(int *mlfd ,int *mcfd)
{
    int ret = -1;
    int lfd=0 ,cfd=0;
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

    //等待客户端连接
    cfd = Accept(lfd ,(struct sockaddr *)&clin_addr ,&clin_addr_len);
    
    //文件名最后加上\0,防止显示乱码
    len -= offsetof(struct sockaddr_un ,sun_path);
    clin_addr.sun_path[len] = '\0';
    printf("clinte bind filename: %s\n" ,clin_addr.sun_path);

    *mlfd = lfd;
    *mcfd = cfd;
    return 0;
}