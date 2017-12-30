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
	robo_lights->insertEffect(0,0,"TestEffect","255,255,255");
	sleep(2);
	robo_lights->renderLights();
	delete robo_lights;
	return 0;
}





