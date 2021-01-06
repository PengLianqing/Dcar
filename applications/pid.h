/**
  ****************************(C) COPYRIGHT 2021 Peng****************************
  * @file				pid.c/h
  * @version		V1.0.0
  * @brief   		����PID�� ����/����ϰ���Խ�get/measure/real/fdb,
						  ��������һ���set/target/ref 
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Jan-1-2021      Peng            1. ���
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2021 Peng****************************
	*/
	
#ifndef __pid_H
#define __pid_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/**
  * @brief          pid_t�ṹ����enum
  */
enum{
    LLAST	= 0,
    LAST 	= 1,
    NOW 	= 2,
    
    POSITION_PID,
    DELTA_PID,
};
typedef struct __pid_t
{
    float p;
    float i;
    float d;
    
    float set[3];				//Ŀ��ֵ,����NOW�� LAST�� LLAST���ϴ�
    float get[3];				//����ֵ
    float err[3];				//���
	
    
    float pout;							//p���
    float iout;							//i���
    float dout;							//d���
    
    float pos_out;						//����λ��ʽ���
    float last_pos_out;				//�ϴ����
    float delta_u;						//��������ֵ
    float delta_out;					//��������ʽ��� = last_delta_out + delta_u
    float last_delta_out;
    
	  float max_err;
	  float deadband;				//err < deadband return
    uint32_t pid_mode;
    float MaxOutput;				//����޷�
    float IntegralLimit;		//�����޷�
    
    void (*f_param_init)(struct __pid_t *pid,  //PID������ʼ��
                    uint32_t pid_mode,
                    uint32_t maxOutput,
                    uint32_t integralLimit,
                    float p,
                    float i,
                    float d);
    void (*f_pid_reset)(struct __pid_t *pid, float p, float i, float d);		//pid���������޸�

}pid_t; 

/**
  * @brief          �û������pid
  */
extern pid_t pid_6020_moto1_velocity;
extern pid_t pid_6020_moto1_position;

		
/**
  * @brief          ��;���Ĳ����趨(����)
  * @param[in]      *pid ,kp ,ki ,kd
  * @retval         none
  */
static void pid_reset(pid_t	*pid, float kp, float ki, float kd);
		
/**
  * @brief          ���pid��������
  * @param[in]      *pid
  * @retval         err��set��get��out��last_out
  */
void pid_clear(pid_t	*pid);
		
/**
  * @brief          pid����
  * @param[in]      *pid ,get ,set
  * @retval         none
  */
float pid_calc(pid_t* pid, float get, float set);
		
/**
  * @brief          ʹ�������ǽ��ٶȵ�pid����
  * @param[in]      *pid ,get ,set
  * @retval         none
  */
float pid_sp_calc(pid_t* pid, float get, float set, float gyro);
		
/**
  * @brief          pid�ṹ���ʼ��
  * @param[in]      PID_struct_init(&pid_6020_moto1_velocity, DELTA_PID, 30000.0, 10000.0,
									6.0f,	10.0f,	2.0f	); 
										PID_struct_init(&pid_6020_moto1_position, POSITION_PID, 300.0, 100.0,
									5.0f,	0.0f,	10.0f	); 
  * @retval         none
  */
void PID_struct_init(
    pid_t* pid,
    uint32_t mode,
    uint32_t maxout,
    uint32_t intergral_limit,
    
    float 	kp, 
    float 	ki, 
    float 	kd);
		
#endif

