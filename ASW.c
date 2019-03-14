#include "ASW.h"
#include "mcal_encoder.h"

T_F16 speed;

void vInit_Everything()
{
    speed = 30; //generate a random speed
    
	RTE_vMotorInit();
	RTE_vSetMotorDir(FORWARD);
	RTE_vSetMotorSpeed(speed);
	RTE_vSetAngle(90);
}

void vFollower()
{
	T_U8 readPins = RTE_LF_u8ReadPins();
	static T_U8 angle = 90;
	static T_U8 lastAngle = 90;
	if(readPins != 0)
	{	
		RTE_vSetMotorSpeed(speed);
		switch(readPins)
		{
			case 0b001100: 
			case 0b000100:
			case 0b001000:
				angle = 90;
				lastAngle = angle;
				break;
			case 0b000110: 
			case 0b000010:
				angle = 110;
				lastAngle = angle;
				break;
			case 0b000011:
			case 0b000001:  
				angle = 120;
				lastAngle = angle;
				break;
			case 0b011000:
				angle = 80;
				lastAngle = angle;
				break;
			case 0b110000: 
			case 0b010000:
				angle = 60;
				lastAngle = angle;
				break;
			case 0b100000: 
				angle = 60;
				lastAngle = angle;
				break;
			case 0b111111: 
				angle = 90;
				lastAngle = angle;
				break;
			default:
				angle = 120;
				lastAngle = angle;
		}
		RTE_vSetAngle(angle);
	}
	else
	{
//		RTE_vSetMotorSpeed(0);
//		RTE_vSetAngle(lastAngle);
	}
	
    //PID implementation
	/*int v[63] = { 0 }, cmd, err, kp, kd, ki, P, D, ref;
	static int perr = 0, I = 0;
	T_U8 val;
	v[1] = 1; v[3] = 2;	v[2] = 3; v[5] = 4; v[4] = 5; //linia e in dreapta
	v[12] = 6; //referinta
	v[8] = 7; v[24] = 8; v[16] = 9; v[48] = 10; v[32] =11; //linia e in stanga
	
	ref = 6;
	kp = 200;
	ki = 0;
	kd = 0;
	
	val = RTE_LF_u8ReadPins();
	if (val == 0)
	{
		RTE_vSetMotorSpeed(0);
	}
	else
	{
		RTE_vSetMotorSpeed(speed);
	}
        
	if (v[val] != 0)
	{
		err = ref - v[val];
		P = kp*err;
		I = I + err;
		D = err - perr;
		cmd = P + ki*I + kd*D;
		cmd = cmd + 7500;
		if(cmd > 9500)
		{
			cmd = 9500;
		}
		if(cmd < 5500)
		{
			cmd = 5500;
		}
		
		//P1DC1 = cmd;
		RTE_vSetAngle(60*cmd/3000-60);
		perr = err;
	}*/
    
    //stop if there is a obstacle
    RTE_vHandleObst();
}

