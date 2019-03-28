#include "ASW.h"
#include "mcal_encoder.h"

struct _car car;

void vInit_Everything()
{
    car.f16Speed = 20 + TIM2_u16GetCount() % 40; //generate a random speed [20 - 60]]
    car.u8IdRoad = 0;
    car.state = S1;
    car.bWait = FALSE;
   
    RTE_RF_vInit();
	RTE_vMotorInit();
	RTE_vSetMotorDir(FORWARD);
	RTE_vSetMotorSpeed(car.f16Speed);
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
                if(car.state == S3)
                    switch(car.direction)
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
        //TBD
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
        car.u8IdRoad++;
        bPrevWasBlack = FALSE;
    }
    else if (u8ReadPins == 0b111111)
    {
        //go in next state
        car.state = S2;
        car.direction = TIM2_u16GetCount() % 3; //generate a random direction
    }
    else
    {
        bPrevWasBlack = TRUE;
    }
}

void vDoS2Action()
{
    //communication logic
    T_U8 u8Message = 0b10000;
    u8Message = u8Message | (car.direction << 2);
    u8Message = u8Message | (car.u8IdRoad);
    //send message
    COM_vSendMessage(u8Message);
    //receive messages
    COM_vStartListening();
    COM_vCheckIRQ();
    COM_vProcessFIFO();
                
    T_U8 u8ReadPins = RTE_LF_u8ReadPins();
    if (u8ReadPins == 0b111111)
    {
        switch(car.direction)
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
        if(car.bWait == TRUE)
        {
            RTE_vSetMotorSpeed(0);
        }
        else
        {
            RTE_vSetMotorSpeed(car.f16Speed);
            //go in next state
            car.state = S3;
        }
    }
}

void vDoS3Action()
{
    T_U8 u8ReadPins = RTE_LF_u8ReadPins();
    if(u8ReadPins == 0)
    {
        //tell other that i have finished intersection
        T_U8 u8Message = 0;
        COM_vSendMessage(u8Message);
        RTE_vSetMotorSpeed(0);
    }
}

void vFSM()
{
    switch(car.state)
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