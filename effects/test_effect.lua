
EFFECT_NAME = "TestEffect"
INIT_PARAM = "int,int,int"
INIT_PARAM_META = "R,G,B"

segment = 2

R = 0
G = 0
B = 0

function initialize(r,g,b)
	R = r
	G = g
	B = b
end

function render(current_time)
	for i = 0, length do
		setLED(i,R,G,B)
	end
	return true
	
end

