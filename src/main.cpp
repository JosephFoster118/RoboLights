#include <stdio.h>
#include <lua.hpp>
#include <stdlib.h>
#include <unistd.h>
#include "RoboLights.h"
#include "LightEffect.h"

int main()
{
	printf("RoboLights\n");
	RoboLights* robo_lights = new RoboLights(4,8,8,8,8);
	robo_lights->printLEDs();
	
	LightEffect* test = LightEffect::getEffect("SolidSection");
	robo_lights->insertEffect(0,0,"TestEffect","1,2,20");
	robo_lights->insertEffect(1,0,"SolidSection","1,2,20,2,2,10.0");
	for(int i = 0; i < 12; i++)
	{
		sleep(1);
		robo_lights->renderLights();
		//robo_lights->printLEDs();
	}
	delete robo_lights;
	return 0;
}





