#ifndef BSP_CAN_H
#define BSP_CAN_H
#include "struct_typedef.h"
#include "main.h"

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

void can_init(void);

void can_filter_init(void);

#define CHASSIS_CAN hcan1
#define GIMBAL_CAN hcan2

/* CAN send and receive ID */
typedef enum
{
    CAN_CHASSIS_ALL_ID = 0x200,
    CAN_3508_M1_ID = 0x201,
    CAN_3508_M2_ID = 0x202,
    CAN_3508_M3_ID = 0x203,
    CAN_3508_M4_ID = 0x204,

    CAN_6020_M1_ID = 0x205,
    CAN_6020_M2_ID = 0x206,
    CAN_6020_M3_ID = 0x207,
		CAN_6020_M4_ID = 0x208,
	
	//���Ʊ���0x1FF��6020���idΪ1��2��3��4ʱ��
    CAN_GIMBAL_ALL_ID = 0x1FF,

} can_msg_id_e;

#define FILTER_BUF_LEN		5
typedef struct{
	int16_t	 	speed_rpm;
  float  	  real_current;
  int16_t  	given_current;
  uint8_t  	hall;
	uint16_t 	angle;				//abs angle range:[0,8191]
	uint16_t 	last_angle;	//abs angle range:[0,8191]
	uint16_t	offset_angle;
	int32_t		round_cnt;
	int32_t		total_angle;
	uint8_t		buf_idx;
	uint16_t	angle_buf[FILTER_BUF_LEN];
	uint16_t	fited_angle;
	uint32_t	msg_cnt;
}motor_measure_t;

/*
* ������ת�� 320rpm
* �ת����132rpm
*/
typedef struct __moto_position_t
{
		int32_t	offset_data;      //����Ƕ�У׼ƫ��
		int32_t moto_result_data;	//������ԽǶȴ�����ֵ
	
    float moto_result_angle;		//������ԽǶ�	
		float moto_result_angle_last;		//������ԽǶ�	
    float moto_target_angle;		//���Ŀ��Ƕ�		
	
		float moto_target_speed;
		float moto_result_speed;
	
		float moto_history_speed[8];
		float control_time;
	
		uint8_t control_mode;
	
}moto_position_t;

extern motor_measure_t motor_chassis[7];
extern moto_position_t moto_position[7];

void CAN_cmd_can1(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
void CAN_cmd_can2(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
const motor_measure_t *get_6020_moto1_measure_point(void);

#endif
