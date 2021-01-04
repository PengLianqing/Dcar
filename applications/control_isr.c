#include "control_isr.h"
#include "math.h"
#include "pid.h"

#include "usb_task.h"

pid_t pid_6020_moto1_velocity;
pid_t pid_6020_moto1_position;
void control_init(void)
{
	
	PID_struct_init(&pid_6020_moto1_velocity, DELTA_PID, 20000.0, 10000.0,
									//10.0f,	25.0f,	0.0f	); 
									6.0f,	15.0f,	0.0f	); 
	PID_struct_init(&pid_6020_moto1_position, POSITION_PID, 300.0, 100.0,
									10.0f,	0.0f,	1.0f	); 
}


moto_position_t moto_position[7];
void moto_control_data_process(moto_position_t *moto,motor_measure_t *ptr)
{
	moto->control_time = 500.0f; //控制频率500Hz
	
	moto->moto_result_angle_last = moto->moto_result_angle;
	moto->moto_result_angle = (float)(ptr->total_angle*360.0f*0.000122085215f);
	
	moto->moto_history_speed[1] = moto->moto_history_speed[0];
	moto->moto_history_speed[0] = (moto->moto_result_angle - moto->moto_result_angle_last)*moto->control_time*60.0f/360.0f;
	
	//moto->moto_history_speed[2] = moto->moto_history_speed[1];
	//moto->moto_history_speed[3] = moto->moto_history_speed[2];
	
	//moto->moto_result_speed = (moto->moto_result_angle - moto->moto_result_angle_last)*moto->control_time;
	moto->moto_result_speed = (moto->moto_history_speed[0]*0.4f + moto->moto_history_speed[1]*0.6f);
}

#define MaxVelocity 132.0f
#define MaxPosition 8191
const motor_measure_t *M6020_Moto1_Measure;


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static int time=0;
	static int16_t rc_ctrl_last=0;
	float rc_ctrl_target = 0.0f;
	
	if(htim==(&htim2))
	{
		if(++time>=1000 ) time=0;
		/****************************1000HZ***********************/
			
		/****************************1000HZ***********************/
		if(time%2==0)
		{
		/***************************500HZ***********************/
			led1_toggle();	
			M6020_Moto1_Measure = get_6020_moto1_measure_point();
			
			moto_control_data_process(&moto_position[4],&motor_chassis[4]);
			
			pid_calc(&pid_6020_moto1_position,moto_position[4].moto_result_angle,moto_position[4].moto_target_angle);
			
			moto_position[4].moto_target_speed = pid_6020_moto1_position.pos_out;
			
			pid_calc(&pid_6020_moto1_velocity,moto_position[4].moto_result_speed,moto_position[4].moto_target_speed);
			
			CAN_cmd_can1(pid_6020_moto1_velocity.delta_out,0,0,0);
			
		/***************************500HZ***********************/
		}
		if(time%5==0)
		{
		/***************************200HZ***********************/
			
		/***************************200HZ***********************/
		}
		if(time%10==0)
		{
		/***************************100HZ***********************/
			rc_ctrl_target = (float)(rc_ctrl_last*0.6f)+(float)(rc_ctrl.rc.ch[0]*0.4f);
			moto_position[4].moto_target_angle = (rc_ctrl_target*360.0f/660.0f);
			rc_ctrl_last = rc_ctrl.rc.ch[0];
			
		/***************************100HZ***********************/
		}
		if(time%100==0)
		{
		/***************************10HZ************************/
		led2_toggle();
			
			feedback_data.channel[0]=1.0;//rand()%1024;
			feedback_data.channel[1]=rand()%1024;
			feedback_data.channel[2]=rand()%1024;
			feedback_data.imu_angel [0] = 1;//(float)(rand()%1024*0.1f);
			feedback_data.imu_angel [1] = (float)(rand()%1024*0.1f);
			feedback_data.imu_angel [2] = (float)(rand()%1024*0.1f);
			feedback_data.mode0 = time%100;
			feedback_data.mode1 = time%100;
			BBB_data_send(&feedback_data);
		
		/***************************10HZ************************/
		}
	}
}
#include "FreeRTOS.h"
#include "timers.h"
void SoftTimer10HzCallback(TimerHandle_t xTimer)
{
	static TickType_t xTimeNow;
	xTimeNow = xTaskGetTickCount();
			
	feedback_data.channel[0]=1.0;//rand()%1024;
	feedback_data.channel[1]=rand()%1024;
	feedback_data.channel[2]=rand()%1024;
	feedback_data.imu_angel [0] = 1;//(float)(rand()%1024*0.1f);
	feedback_data.imu_angel [1] = (float)(rand()%1024*0.1f);
	feedback_data.imu_angel [2] = (float)(rand()%1024*0.1f);
	feedback_data.mode0 = rand()%3;
	feedback_data.mode1 = rand()%3;
	BBB_data_send(&feedback_data);
}

void SoftTimer100HzCallback(TimerHandle_t xTimer)
{
	static TickType_t xTimeNow;
	xTimeNow = xTaskGetTickCount();
	
	static int16_t rc_ctrl_last=0;
	float rc_ctrl_target = 0.0f;
	rc_ctrl_target = (float)(rc_ctrl_last*0.6f)+(float)(rc_ctrl.rc.ch[0]*0.4f);
	moto_position[4].moto_target_angle = (rc_ctrl_target*360.0f/660.0f);
	rc_ctrl_last = rc_ctrl.rc.ch[0];
}

#define TRIAL_MODE 0
void SoftTimer1000HzCallback(TimerHandle_t xTimer)
{
	static TickType_t xTimeNow;
	xTimeNow = xTaskGetTickCount();
	
	led1_toggle();	
	M6020_Moto1_Measure = get_6020_moto1_measure_point();
	
	moto_control_data_process(&moto_position[4],&motor_chassis[4]);
	#if TRIAL_MODE
	pid_calc(&pid_6020_moto1_position,moto_position[4].moto_result_angle,moto_position[4].moto_target_angle);
	
	moto_position[4].moto_target_speed = pid_6020_moto1_position.pos_out;
	pid_calc(&pid_6020_moto1_velocity,moto_position[4].moto_result_speed,moto_position[4].moto_target_speed);
	CAN_cmd_can1(pid_6020_moto1_velocity.delta_out,0,0,0);
	#else
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