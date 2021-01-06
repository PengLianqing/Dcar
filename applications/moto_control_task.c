/**
  ****************************(C) COPYRIGHT 2021 Peng****************************
  * @file       moto_control_task.c/h
  * @brief      FreeRTOS任务
  * @note       
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
#include "moto_control_task.h"


pid_t pid_6020_moto1_velocity;
pid_t pid_6020_moto1_position;
/**
  * @brief          初始化pid指针
  * @param[in]      pid指针、参数
  * @retval         null
  */
void control_init(void)
{
	
	PID_struct_init(&pid_6020_moto1_velocity, DELTA_PID, 30000.0, 10000.0,
									6.0f,	10.0f,	2.0f	); 
	PID_struct_init(&pid_6020_moto1_position, POSITION_PID, 300.0, 100.0,
									5.0f,	0.0f,	10.0f	); 
}

moto_position_t moto_position[7];

/**
  * @brief          处理电机原始数据
  * @param[in]      电机控制数据指针, 电机原始数据指针
  * @retval         null
  */
void moto_control_data_process(moto_position_t *moto,motor_measure_t *ptr)
{
	
	moto->control_time = 1000.0f; //电机信息发送频率1000Hz
	
	moto->moto_result_angle_last = moto->moto_result_angle;
	moto->moto_result_angle = (float)(ptr->total_angle*360.0f*0.000122085215f);
	
	moto->moto_history_speed[1] = moto->moto_history_speed[0];
	moto->moto_history_speed[0] = (moto->moto_result_angle - moto->moto_result_angle_last)*moto->control_time*60.0f/360.0f;
	
	//使用can传回来的速度值.
	//moto->moto_history_speed[0] = ptr->speed_rpm;
	
	moto->moto_result_speed = (moto->moto_history_speed[0]*0.4f + moto->moto_history_speed[1]*0.6f);
}

#define MaxVelocity 132.0f
#define MaxPosition 8191
const motor_measure_t *M6020_Moto1_Measure;
/**
  * @brief          freertos 200hz 软件定时器任务
	* 实现遥控数据的平滑、变换、限位处理
  * @param[in]      null
  * @retval         null
  */
void SoftTimer200HzCallback(TimerHandle_t xTimer)
{
	static TickType_t xTimeNow;
	xTimeNow = xTaskGetTickCount();
	
	static int16_t rc_ctrl_last=0;
	float rc_ctrl_target = 0.0f;
	rc_ctrl_target = (float)(rc_ctrl_last*0.9f)+(float)(rc_ctrl.rc.ch[0]*0.1f);
	
	//机械限位-100°-+75°
	if(rc_ctrl_target >=0)
	{
		moto_position[4].moto_target_angle = (rc_ctrl_target*75.0f/660.0f);
	}
	else
	{
		moto_position[4].moto_target_angle = (rc_ctrl_target*100.0f/660.0f);
	}
	rc_ctrl_last = rc_ctrl.rc.ch[0];
}

/**
  * @brief          freertos 500hz 软件定时器任务
	* 实现电机控制
  * @param[in]      null
  * @retval         null
  */
#define TRIAL_MODE 1
void SoftTimer500HzCallback(TimerHandle_t xTimer)
{
	static TickType_t xTimeNow;
	xTimeNow = xTaskGetTickCount();
	
	led1_toggle();	
	M6020_Moto1_Measure = get_6020_moto1_measure_point();
	#if TRIAL_MODE
//	if(rc_ctrl.rc.s[0]==3)
//	{
//		moto_position[4].moto_target_angle = 0.0f;
//	}
//	else if(rc_ctrl.rc.s[0]==1)
//	{
//		moto_position[4].moto_target_angle = 180.0f;
//	}
//	else if(rc_ctrl.rc.s[0]==2)
//	{
//		moto_position[4].moto_target_angle = -360.0f;
//	}
	pid_calc(&pid_6020_moto1_position,moto_position[4].moto_result_angle,moto_position[4].moto_target_angle);
	
	moto_position[4].moto_target_speed = pid_6020_moto1_position.pos_out;
	pid_calc(&pid_6020_moto1_velocity,moto_position[4].moto_result_speed,moto_position[4].moto_target_speed);
	if(rc_ctrl.rc.s[0]==3)
	{
		CAN_cmd_can1(pid_6020_moto1_velocity.delta_out,0,0,0);
	}		
	else 
	{
		CAN_cmd_can1(0,0,0,0);
	}
	#else
	if(rc_ctrl.rc.s[0]==3)
	{
		moto_position[4].moto_target_speed = 0.0f;
	}
	else if(rc_ctrl.rc.s[0]==1)
	{
		moto_position[4].moto_target_speed = 100.0f;
	}
	else if(rc_ctrl.rc.s[0]==2)
	{
		moto_position[4].moto_target_speed = 200.0f;
	}
	pid_calc(&pid_6020_moto1_velocity,moto_position[4].moto_result_speed,moto_position[4].moto_target_speed);
	CAN_cmd_can1(pid_6020_moto1_velocity.delta_out,0,0,0);
	#endif
}

/**
  * @brief          返回电机控制数据指针
  * @param[in]      none
  * @retval         电机控制数据指针
  */
const moto_position_t *get_moto1_position_point(void)
{
    return &moto_position[0];
}

/**
  * @brief          返回pid数据指针
  * @param[in]      none
  * @retval         pid数据指针
  */
pid_t *get_moto1_position_pid_point(void)
{
    return &pid_6020_moto1_position;
}
pid_t *get_moto1_velocity_pid_point(void)
{
    return &pid_6020_moto1_velocity;
}
