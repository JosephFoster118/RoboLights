#include <stdio.h>
#include <lua.hpp>
#include <stdlib.h>
#include <unistd.h>
#include "RoboLights.h"
#include "LightEffect.h"

int main()
{
	printf("RoboLights\n");
	RoboLights* robo_lights = new RoboLights(4,8,8,8,16);
	robo_lights->printLEDs();
	
	//LightEffect* test = LightEffect::getEffect("SolidSection");
	//robo_lights->insertEffect(0,1,"TestEffect","1,2,20");
	robo_lights->insertEffect(1,0,"SolidSection","200,118,64,2,2,100.0");
	//robo_lights->insertEffect(3,0,"sweep","200,118,64,4,45.0,-1");
	//robo_lights->insertEffect(2,0,"sweep","200,118,64,4,-30.0,-1");
	robo_lights->insertEffect(3,0,"block build","255,255,255,2,.25");
	robo_lights->insertEffect(3,0,"block build","255,255,255,2,.25");
	robo_lights->insertEffect(3,0,"block build","255,255,255,2,.25");
	robo_lights->insertEffect(3,0,"block build","255,255,255,2,.25");
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			robo_lights->insertEffect(i,j,"block build","255,255,255,2,.25");
		}
	}

	for(int i = 0;i < 30*4; i++)
	{
		usleep(33333);
		//printf("Frame %8d\n",i);
		robo_lights->renderLights();
		robo_lights->printLEDs();
	}
	delete robo_lights;
	return 0;
}





