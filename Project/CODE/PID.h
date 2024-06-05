#ifndef __PID_H
#define __PID_H


typedef struct
{
	float Err;			  //定义偏差值
	float Err_last;		  //定义上一次偏差值
    float Kpa,Kpb;
	float Ki,Kd;		  //定义比例、积分、微分
	float Integral;       // 积分项
	float Integral_Last;  // 上一次积分
	float PID_Out;		  //定义总输出量
	
	float Out_Limit;      // 输出限幅
	float Integral_Limit;  // 积分限幅 
}PID_InitTypeDef;


//extern PID_InitTypeDef Left_Wheel_PID;
//extern PID_InitTypeDef Right_Wheel_PID;
extern PID_InitTypeDef Turn_PID;

//PID初始化
void PID_Init(PID_InitTypeDef *PID_Struct, float Kpa,float Kpb, float Ki, float Kd,float Out_Limit, float Integral_Limit);	
//PID计算
void PID_Calculate(PID_InitTypeDef *PID_Struct, float Exp_Val, float Act_Val);		



//***********************************************************增量式PID**************************************************//
typedef struct{
    float Kp;
    float Ki;
    float Kd;
	
    float error;
    float last_error;
    float last_last_error;
    float last_out;
	
    float P_Out,I_Out,D_Out;		//定义PID输出量
    float out;
    float outmax;
    float outmin;
	
    float Integral_Limit;
    uint8 use_lowpass_filter;
    float lowpass_filter_factor;
	
} PID_Incremental;


extern PID_Incremental Left_Wheel;
extern PID_Incremental Right_Wheel;




void PID_Incremental_Init(PID_Incremental *pid, float Kp, float Ki, float Kd,float Out_Limit, float Integral_Limit,uint8 use_lowpass_filter);
float PID_Incremental_Calc(PID_Incremental *pid, float setpoint, float input_value);






#endif