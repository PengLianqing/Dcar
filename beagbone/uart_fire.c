#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <string.h>
#include <sys/ioctl.h>

//根据具体的设备修改
const char default_path[] = "/dev/ttyO2";

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

char tx_buf[34];

int i=0;

#define USE_READ
#define USE_WRITE

int main(int argc, char *argv[])
{
	int fd;
	int res;
	struct termios opt;
	char *path;
	char buf[98] = "Embedfire tty send test.\n";

	//若无输入参数则使用默认终端设备
	if(argc > 1)
		path = argv[1];
	else
		path = (char *)default_path;

	//获取串口设备描述符
	printf("This is tty/usart program running.\n");
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
	
	//printf("Device %s is set to 9600bps.\n",path);
	printf("%s\n",path);

	printf("receive %d , %s\n", res , buf);
	static int receive_num[2]={0,0};
	
	while(1)
	{
		//sleep(1);
		//sprintf(buf,"translate %d , %5.2f \n",i,rand()%1024*0.1f);
		//res = write(fd, buf, sizeof(buf));
		//printf("write %d,%s\n",res,buf);
		
		/*read*/
		#ifdef USE_READ
		res = read(fd,buf,98);
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
		#endif

		usleep(20000);
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
		sprintf(tx_buf,"B:%d , %d ,%5.2f ,%5.2f ,%d ,%d \n",control_date.data1,control_date.data2,control_date.data3,control_date.data4,control_date.mode1,control_date.mode2);
		res = write(fd, tx_buf, sizeof(tx_buf));
		printf("translate %d\r\n",res);
		#endif
		
		//sprintf(tx_buf,"B:%d , %d ,%5.2f ,%5.2f\n",i,i,rand()%1024*0.1f,rand()%1024*0.1f);
		//res = write(fd, tx_buf, sizeof(tx_buf));
		//printf("translate %d , %d ,%s\r\n",res,sizeof(tx_buf),tx_buf);

	}
	printf("read error,res = %d",res);

	close(fd);
	return 0;
}