#include "hx711.h"

HX711::HX711(int SCK_PIN,int DT_PIN,float GapValueIn)
{
  HX711_SCK = SCK_PIN;
	HX711_DT = DT_PIN;
	ValueGap = GapValueIn;
}

//****************************************************
//initiate HX711
//****************************************************
void HX711::begin()
{
	pinMode(HX711_SCK, OUTPUT);	
	pinMode(HX711_DT, INPUT);

	Get_Maopi();
}

int HX711::Pressed(int AlarmValue)
{
	if(Get_Weight() >= AlarmValue && CurrentAlarm == 0)
	{
		CurrentAlarm = 1;
		return 1;
	}
	else if(Get_Weight() < AlarmValue)
	{
		CurrentAlarm = 0;
		return 0;
	}

	return 0;
}

//****************************************************
//get tare weight
//****************************************************
void HX711::Get_Maopi()
{
	Weight_Maopi = HX711_Read();		
} 

//****************************************************
//weigh
//****************************************************
long HX711::Get_Weight()
{
	HX711_Buffer = HX711_Read();

	Weight_Shiwu = HX711_Buffer;
	Weight_Shiwu = Weight_Shiwu - Weight_Maopi;				//weighted object AD data
	
	Weight_Shiwu = (long)((float)Weight_Shiwu/ValueGap+0.05); 	

	return Weight_Shiwu;
}

//****************************************************
//read HX711
//****************************************************
unsigned long HX711::HX711_Read()	//128 gain
{
	unsigned long count; 
	unsigned char i;
	bool Flag = 0;

	digitalWrite(HX711_DT, HIGH);
	delayMicroseconds(1);

	digitalWrite(HX711_SCK, LOW);
	delayMicroseconds(1);

  	count=0; 
  	while(digitalRead(HX711_DT)); 
  	for(i=0;i<24;i++)
	{ 
	  	digitalWrite(HX711_SCK, HIGH); 
		delayMicroseconds(1);
	  	count=count<<1; 
		digitalWrite(HX711_SCK, LOW); 
		delayMicroseconds(1);
	  	if(digitalRead(HX711_DT))
			count++; 
	} 
 	digitalWrite(HX711_SCK, HIGH); 
	delayMicroseconds(1);
	digitalWrite(HX711_SCK, LOW); 
	delayMicroseconds(1);
	count ^= 0x800000;
	return(count);
}
