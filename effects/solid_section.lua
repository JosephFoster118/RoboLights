
EFFECT_NAME = "SolidSection"
INIT_PARAM = "int,int,int,int,int,float"
INIT_PARAM_META = "R,G,B,Start,Size,Duration"

R = 0
G = 0
B = 0
START = 0
SIZE = 0;
DURATION = 0.0;

function initialize(r,g,b,start,size,duration)
	R = r
	G = g
	B = b
	START = start
	SIZE = size
	DURATION = duration
end

function render(current_time)
	local time_dif =  current_time - start_time
	if time_dif > DURATION then
		return true
	end
	
	
	
	return false
end




