EFFECT_NAME = "JitterBug"
INIT_PARAM = "int,int,int,int,float,float"
INIT_PARAM_META = "R,G,B,Size,Speed,Duration"


R = 0
G = 0
B = 0
SIZE = 0
SPEED = 0
DURATION = 0.0

function initialize(r,g,b,size,speed,duration)
	R = r
	G = g
	B = b
	SPEED = speed
	SIZE = size
	DURATION = duration
end

function render(current_time)
	local time_dif =  current_time - start_time
	if time_dif > DURATION and DURATION > 0 then
		return true
	end
	for i = 0, length - 1, 1 do
		if time_dif%(SPEED*2) > SPEED then
			
			if i%(SIZE*2) < SIZE then
				setLED(i, R, G, B)
			end
		else
			if i%(SIZE*2) >= SIZE then
				setLED(i, R, G, B)
			end
		end
	end
end


