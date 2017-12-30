#pragma once

#define SAFE_DELETE(var)\
if( var != NULL)\
{\
	delete var;\
	var = NULL;\
}

#define SAFE_DELETE_ARRAY(var)\
if( var != NULL)\
{\
	delete [] var;\
	var = NULL;\
}

