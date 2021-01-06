/**
  ****************************(C) COPYRIGHT 2021 Peng****************************
  * @file				pid.c/h
  * @version		V1.0.0
  * @brief   		对于PID， 反馈/测量习惯性叫get/measure/real/fdb,
						  期望输入一般叫set/target/ref 
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Jan-1-2021      Peng            1. 完成
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
  * @brief          pid_t结构体与enum
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
    
    float set[3];				//目标值,包含NOW， LAST， LLAST上上次
    float get[3];				//测量值
    float err[3];				//误差
	
    
    float pout;							//p输出
    float iout;							//i输出
    float dout;							//d输出
    
    float pos_out;						//本次位置式输出
    float last_pos_out;				//上次输出
    float delta_u;						//本次增量值
    float delta_out;					//本次增量式输出 = last_delta_out + delta_u
    float last_delta_out;
    
	  float max_err;
	  float deadband;				//err < deadband return
    uint32_t pid_mode;
    float MaxOutput;				//输出限幅
    float IntegralLimit;		//积分限幅
    
    void (*f_param_init)(struct __pid_t *pid,  //PID参数初始化
                    uint32_t pid_mode,
                    uint32_t maxOutput,
                    uint32_t integralLimit,
                    float p,
                    float i,
                    float d);
    void (*f_pid_reset)(struct __pid_t *pid, float p, float i, float d);		//pid三个参数修改

}pid_t; 

/**
  * @brief          用户定义的pid
  */
extern pid_t pid_6020_moto1_velocity;
extern pid_t pid_6020_moto1_position;

		
/**
  * @brief          中途更改参数设定(调试)
  * @param[in]      *pid ,kp ,ki ,kd
  * @retval         none
  */
static void pid_reset(pid_t	*pid, float kp, float ki, float kd);
		
/**
  * @brief          清空pid计算数据
  * @param[in]      *pid
  * @retval         err、set、get、out、last_out
  */
void pid_clear(pid_t	*pid);
		
/**
  * @brief          pid计算
  * @param[in]      *pid ,get ,set
  * @retval         none
  */
float pid_calc(pid_t* pid, float get, float set);
		
/**
  * @brief          使用陀螺仪角速度的pid计算
  * @param[in]      *pid ,get ,set
  * @retval         none
  */
float pid_sp_calc(pid_t* pid, float get, float set, float gyro);
		
/**
  * @brief          pid结构体初始化
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

