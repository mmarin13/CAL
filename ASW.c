#include "ASW.h"
#include "mcal_encoder.h"

T_F16 f16Speed;
T_U8 u8IdRoad;
enum _Direction direction;
enum _State state;

void vInit_Everything()
{
    f16Speed = 10 + rand() % 20; //generate a random speed [20 - 60]]
    u8IdRoad = 0;
    direction = rand() % 3; //generate random direction
    state = S3;
    
	RTE_vMotorInit();
	RTE_vSetMotorDir(FORWARD);
	RTE_vSetMotorSpeed(f16Speed);
	RTE_vSetAngle(90);
}

void vFollower()
{
	T_U8 u8ReadPins = RTE_LF_u8ReadPins();
    //u8ReadPins = u8ReadPins & RTE_LF_u8ReadPins(); //try to avoid noise
	static T_U8 u8Angle = 90;
    static T_U8 u8PrevAngle = 0;
	if(u8ReadPins != 0)
	{	
		switch(u8ReadPins)
		{
			case 0b001100:
			case 0b001000:
				u8Angle = 90;
				break;
			case 0b000110: 
			case 0b000100:
				u8Angle = 110;
				break;
			case 0b000011:
			case 0b000001:  
				u8Angle = 120;
				break;
			case 0b011000:
				u8Angle = 80;
				break;
			case 0b110000: 
			case 0b010000:
				u8Angle = 60;
				break;
			case 0b100000: 
				u8Angle = 60;
				break;
			case 0b111111: 
				u8Angle = 90;
				break;
			default:
                if(state == S3)
                    switch(direction)
                    {
                        case LEFT:
                          u8Angle = 60; 
                          break;
                        case CENTER:
                            u8Angle = 90;
                            break;
                        case RIGHT:
                            u8Angle = 120;
                    }                    
		}
        if(u8Angle != u8PrevAngle)
        {
            RTE_vSetAngle(u8Angle);
            u8PrevAngle = u8Angle;
        }		
	}
	else
	{
        //to do
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

void vDoS1Action()
{
    static BOOL bPrevWasBlack = TRUE; 
    T_U8 u8ReadPins = RTE_LF_u8ReadPins();
    if(u8ReadPins == 0 && bPrevWasBlack)
    {
        u8IdRoad++;
        bPrevWasBlack = FALSE;
    }
    else if (u8ReadPins == 0b01111110)
    {
        //go in next state
        state = S2;
    }
    else
    {
        bPrevWasBlack = TRUE;
    }
}

void vDoS2Action()
{
    //communication logic
    //T_U8 message = (direction << 2) | u8IdRoad;
            
    T_U8 u8ReadPins = RTE_LF_u8ReadPins();
    if (u8ReadPins == 0b01111110)
    {
        switch(direction)
        {
            case LEFT:
              RTE_vSetAngle(60); 
              break;
            case CENTER:
                RTE_vSetAngle(90);
                break;
            case RIGHT:
                RTE_vSetAngle(120);
        }
        //go in next state
        state = S3;
    }
}

void vDoS3Action()
{
    T_U8 u8ReadPins = RTE_LF_u8ReadPins();
    if(u8ReadPins == 0)
    {
        RTE_vSetMotorSpeed(0);
    }
}

void vFSM()
{
    switch(state)
    {
        case S1:
            vDoS1Action();
            break;
        case S2:
            vDoS2Action();
            break;
        case S3:
            vDoS3Action();
            break;
    }
}