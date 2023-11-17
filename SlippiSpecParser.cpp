#include "SlippiSpecParser.h"
const bool LOOKTable[133][2][44] = {{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,true,true,true,false,false,false,false,false,false,true,false,false,false,false,false,true,false,false,},{true,true,false,false,false,false,false,true,false,false,false,false,false,true,true,false,false,false,true,true,false,true,true,true,true,true,true,true,true,true,true,true,true,false,false,true,false,false,true,true,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,false,true,false,false,false,false,false,true,true,false,false,false,true,true,false,true,true,true,true,true,true,true,true,true,true,true,true,false,false,true,false,false,true,true,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,},{true,true,false,false,false,false,false,true,false,false,false,false,false,true,true,false,false,false,true,true,false,true,true,true,true,true,true,true,true,true,true,true,true,false,false,true,false,false,true,true,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,false,true,false,false,false,false,false,true,true,false,false,false,true,true,false,true,true,true,true,true,true,true,true,true,true,true,true,false,false,true,false,false,true,true,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,false,true,false,false,false,false,false,true,true,false,false,false,true,true,false,true,true,true,true,true,true,true,true,true,true,true,true,false,false,true,false,false,true,true,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,false,true,false,false,false,false,false,true,true,false,false,false,true,true,false,true,true,true,true,true,true,true,true,true,true,true,true,false,false,true,false,false,true,true,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,false,true,false,false,false,false,false,true,true,false,false,false,true,true,false,true,true,true,true,true,true,true,true,true,true,true,true,false,false,true,false,false,true,true,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,false,true,false,false,false,false,false,true,true,false,false,false,true,true,false,true,true,true,true,true,true,true,true,true,true,true,true,false,false,true,false,false,true,true,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,true,true,true,true,true,true,true,false,false,true,false,false,false,true,false,true,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,true,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,false,false,true,true,true,false,true,true,true,true,true,true,true,false,true,true,true,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,false,false,true,true,true,false,true,true,true,true,true,true,true,false,true,true,false,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,false,false,true,true,true,false,true,true,true,true,true,true,true,false,true,true,false,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,false,false,true,true,true,false,true,true,true,true,true,true,true,false,true,true,false,false,false,true,false,true,false,true,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,false,false,true,true,true,false,true,true,true,true,true,true,true,false,true,true,true,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,false,false,true,true,true,false,true,true,true,true,true,true,true,false,true,true,true,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,false,false,true,true,true,false,true,true,true,true,true,true,true,false,true,true,false,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,false,false,true,true,true,false,true,true,true,true,true,true,true,false,true,true,false,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,false,false,true,true,true,false,true,true,true,true,true,true,true,false,true,true,false,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,false,false,true,true,true,false,true,true,true,true,true,true,true,false,true,true,false,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,false,false,true,true,true,false,true,true,true,true,true,true,true,false,true,true,false,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,false,false,true,true,true,false,true,true,true,true,true,true,true,false,true,true,false,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,false,false,true,true,true,false,true,true,true,true,true,true,true,false,true,true,false,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,false,false,true,true,true,false,true,true,true,true,true,true,true,false,true,true,false,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,false,false,true,true,true,false,true,true,true,true,true,true,true,false,true,true,false,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,false,false,true,true,true,false,true,true,true,true,true,true,true,false,true,true,false,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,true,},},{{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,true,true,false,false,false,false,false,false,true,false,false,false,false,true,false,true,true,true,true,true,true,true,true,true,true,true,true,false,true,true,false,true,false,true,false,true,false,true,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,true,},},{{false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,false,false,true,true,true,false,true,true,true,true,true,true,true,false,true,true,false,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,false,false,true,true,true,false,true,true,true,true,true,true,true,false,true,true,false,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,false,false,true,true,true,false,true,true,true,true,true,true,true,false,true,true,false,false,false,true,false,true,false,true,},},{{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,true,true,false,false,false,false,false,false,true,false,false,false,false,true,false,true,true,true,true,true,true,true,true,true,true,true,true,false,true,true,false,true,false,true,false,true,false,true,},},{{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,true,true,false,false,false,false,false,false,true,false,false,false,false,true,false,true,true,true,true,true,true,true,true,true,true,true,true,false,true,true,false,true,false,true,false,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,true,true,true,true,true,true,true,false,false,true,false,false,false,true,false,true,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,true,true,true,true,true,true,true,false,false,true,false,false,false,true,false,true,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,true,true,true,false,false,false,false,false,false,true,false,false,false,false,false,true,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,true,true,true,false,false,false,false,false,false,true,false,false,false,false,false,true,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,true,true,true,false,false,false,false,false,false,true,false,false,false,false,false,true,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,true,true,true,false,false,false,false,false,false,true,false,false,false,false,false,true,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,true,true,true,false,false,false,false,false,false,true,false,false,false,false,false,true,false,false,},{true,true,false,false,false,false,false,true,false,false,false,false,false,true,true,false,false,false,true,true,false,true,true,true,true,true,true,true,true,true,true,true,true,false,false,true,false,false,true,true,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,true,true,true,false,false,false,false,false,false,true,false,false,false,false,false,true,false,false,},{true,true,false,false,false,false,false,true,false,false,false,false,false,true,true,false,false,false,true,true,false,true,true,true,true,true,true,true,true,true,true,true,true,false,false,true,false,false,true,true,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,true,false,false,true,true,true,false,false,false,true,true,false,false,true,true,true,false,true,true,true,false,false,false,false,false,true,true,false,true,true,false,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,true,false,false,true,true,true,false,false,false,true,true,false,false,true,true,true,false,true,true,true,false,false,false,false,false,true,true,false,true,true,false,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,true,true,true,false,false,false,false,false,false,true,false,false,false,false,false,true,false,false,},{true,true,false,false,false,false,false,true,false,false,false,false,false,true,true,false,false,false,true,true,false,true,true,true,true,true,true,true,true,true,true,true,true,false,false,true,false,false,true,true,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,true,true,true,false,false,false,false,false,false,true,false,false,false,false,false,true,false,false,},{true,true,false,false,false,false,false,true,false,false,false,false,false,true,true,false,false,false,true,true,false,true,true,true,true,true,true,true,true,true,true,true,true,false,false,true,false,false,true,true,true,true,false,true,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,false,false,false,true,false,false,true,true,true,false,false,false,true,true,false,true,true,true,true,true,true,true,true,false,false,false,false,false,true,true,false,true,true,false,true,true,false,true,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,true,true,true,false,false,false,false,false,false,true,false,false,false,false,false,true,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,true,false,false,true,true,true,false,false,false,true,true,false,false,true,true,true,false,true,true,true,false,false,false,false,false,true,true,false,true,true,false,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,true,true,true,false,false,false,false,false,false,true,false,false,false,false,false,true,false,false,},{true,true,false,false,false,false,false,true,false,false,false,false,false,true,true,false,false,false,true,true,false,true,true,true,true,true,true,true,true,true,true,true,true,false,false,true,false,false,true,true,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,true,true,true,false,false,false,false,false,false,true,false,false,false,false,false,true,false,false,},{true,true,false,false,false,false,false,true,false,false,false,false,false,true,true,false,false,false,true,true,false,true,true,true,true,true,true,true,true,true,true,true,true,false,false,true,false,false,true,true,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,true,false,true,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,true,true,false,true,true,true,false,false,false,false,false,false,true,false,false,false,false,false,true,false,false,},},{{false,false,false,false,false,false,false,false,false,true,false,true,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,true,true,false,true,true,true,false,false,false,false,false,false,true,false,false,false,false,false,true,false,false,},},{{false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,true,false,true,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,true,true,false,true,true,true,false,false,false,false,false,false,true,false,false,false,false,false,true,false,false,},},{{false,false,false,false,false,false,false,false,false,true,false,true,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,true,true,false,true,true,true,false,false,false,false,false,false,true,false,false,false,false,false,true,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,true,true,true,false,true,false,true,true,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,true,true,false,true,true,true,false,false,false,false,false,false,true,false,false,false,false,false,true,false,false,},},{{false,false,false,false,false,false,false,false,false,true,true,true,false,true,false,true,true,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,true,true,false,true,true,true,false,false,false,false,false,false,true,false,false,false,false,false,true,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,true,false,false,true,true,true,false,false,false,true,true,false,false,true,true,true,false,true,true,true,false,false,false,false,false,true,true,false,true,true,false,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,true,false,false,true,true,true,false,false,false,true,true,false,false,true,true,true,false,true,true,true,false,false,false,false,false,true,true,false,true,true,false,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,true,false,false,true,true,true,false,false,false,true,true,false,false,true,true,true,false,true,true,true,false,false,false,false,false,true,true,false,true,true,false,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,true,false,false,true,true,true,false,false,false,true,true,false,false,true,true,true,false,true,true,true,false,false,false,false,false,true,true,false,true,true,false,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,true,false,false,true,true,true,false,false,false,true,true,false,false,true,true,true,false,true,true,true,false,false,false,false,false,true,true,false,true,true,false,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,true,false,false,true,true,true,false,false,false,true,true,false,false,true,true,true,false,true,true,true,false,false,false,false,false,true,true,false,true,true,false,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,true,true,true,false,true,false,true,true,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,true,true,false,true,true,true,false,false,false,false,false,false,true,false,false,false,false,false,true,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,true,false,true,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,true,true,false,true,true,true,false,false,false,false,false,false,true,false,false,false,false,false,true,false,false,},},{{false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},};
Operator ParseComparator(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if (LOOKTable[1][0][Tokenizer.Peek().Type]&& LOOKTable[1][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseComparator_0(Tokenizer);
    }
    else if (LOOKTable[2][0][Tokenizer.Peek().Type]&& LOOKTable[2][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseComparator_1(Tokenizer);
    }
    else if (LOOKTable[3][0][Tokenizer.Peek().Type]&& LOOKTable[3][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseComparator_2(Tokenizer);
    }
    else if (LOOKTable[4][0][Tokenizer.Peek().Type]&& LOOKTable[4][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseComparator_3(Tokenizer);
    }
    else if (LOOKTable[5][0][Tokenizer.Peek().Type]&& LOOKTable[5][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseComparator_4(Tokenizer);
    }
    else if (LOOKTable[6][0][Tokenizer.Peek().Type]&& LOOKTable[6][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseComparator_5(Tokenizer);
    }
    else if (LOOKTable[7][0][Tokenizer.Peek().Type]&& LOOKTable[7][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseComparator_6(Tokenizer);
    }
    else
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Comparator","Comparator");
        
    }
    return(ReturnValue);
}
Operator ParseComparator_0(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 24)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Comparator","less");
        
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseComparator_1(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 41)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Comparator","add");
        
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseComparator_2(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 27)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Comparator","great");
        
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseComparator_3(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 35)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Comparator","eqq");
        
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseComparator_4(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 23)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Comparator","leq");
        
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseComparator_5(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 26)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Comparator","geq");
        
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseComparator_6(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 28)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Comparator","neq");
        
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseExprOperator(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if (LOOKTable[9][0][Tokenizer.Peek().Type]&& LOOKTable[9][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseExprOperator_0(Tokenizer);
    }
    else if (LOOKTable[10][0][Tokenizer.Peek().Type]&& LOOKTable[10][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseExprOperator_1(Tokenizer);
    }
    else if (LOOKTable[11][0][Tokenizer.Peek().Type]&& LOOKTable[11][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseExprOperator_2(Tokenizer);
    }
    else if (LOOKTable[12][0][Tokenizer.Peek().Type]&& LOOKTable[12][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseExprOperator_3(Tokenizer);
    }
    else if (LOOKTable[13][0][Tokenizer.Peek().Type]&& LOOKTable[13][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseExprOperator_4(Tokenizer);
    }
    else if (LOOKTable[14][0][Tokenizer.Peek().Type]&& LOOKTable[14][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseExprOperator_5(Tokenizer);
    }
    else if (LOOKTable[15][0][Tokenizer.Peek().Type]&& LOOKTable[15][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseExprOperator_6(Tokenizer);
    }
    else if (LOOKTable[16][0][Tokenizer.Peek().Type]&& LOOKTable[16][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseExprOperator_7(Tokenizer);
    }
    else if (LOOKTable[17][0][Tokenizer.Peek().Type]&& LOOKTable[17][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseExprOperator_8(Tokenizer);
    }
    else if (LOOKTable[18][0][Tokenizer.Peek().Type]&& LOOKTable[18][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseExprOperator_9(Tokenizer);
    }
    else if (LOOKTable[19][0][Tokenizer.Peek().Type]&& LOOKTable[19][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseExprOperator_10(Tokenizer);
    }
    else if (LOOKTable[20][0][Tokenizer.Peek().Type]&& LOOKTable[20][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseExprOperator_11(Tokenizer);
    }
    else
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ExprOperator","ExprOperator");
        
    }
    return(ReturnValue);
}
Operator ParseExprOperator_0(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 29)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ExprOperator","plus");
        
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseExprOperator_1(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 30)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ExprOperator","minus");
        
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseExprOperator_2(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 31)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ExprOperator","mul");
        
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseExprOperator_3(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 32)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ExprOperator","div");
        
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseExprOperator_4(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 24)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ExprOperator","less");
        
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseExprOperator_5(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 27)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ExprOperator","great");
        
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseExprOperator_6(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 35)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ExprOperator","eqq");
        
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseExprOperator_7(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 23)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ExprOperator","leq");
        
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseExprOperator_8(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 26)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ExprOperator","geq");
        
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseExprOperator_9(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 28)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ExprOperator","neq");
        
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseExprOperator_10(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 39)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ExprOperator","pipe");
        
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseExprOperator_11(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 41)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ExprOperator","add");
        
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseCombinator(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if (LOOKTable[22][0][Tokenizer.Peek().Type]&& LOOKTable[22][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseCombinator_0(Tokenizer);
    }
    else if (LOOKTable[23][0][Tokenizer.Peek().Type]&& LOOKTable[23][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseCombinator_1(Tokenizer);
    }
    else if (LOOKTable[24][0][Tokenizer.Peek().Type]&& LOOKTable[24][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseCombinator_2(Tokenizer);
    }
    else
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Combinator","Combinator");
        
    }
    return(ReturnValue);
}
Operator ParseCombinator_0(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 39)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Combinator","pipe");
        
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseCombinator_1(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 40)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Combinator","AND");
        
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseCombinator_2(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 38)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Combinator","OR");
        
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Filter_Arg ParseFilter_Arg(MBCC::Tokenizer& Tokenizer)
{
    Filter_Arg ReturnValue;
    if (LOOKTable[26][0][Tokenizer.Peek().Type]&& LOOKTable[26][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseFilter_Arg_0(Tokenizer);
    }
    else if (LOOKTable[27][0][Tokenizer.Peek().Type]&& LOOKTable[27][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseFilter_Arg_1(Tokenizer);
    }
    else
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg","Filter_Arg");
        
    }
    return(ReturnValue);
}
Filter_Arg ParseFilter_Arg_0(MBCC::Tokenizer& Tokenizer)
{
    Filter_Arg ReturnValue;
    if(!(LOOKTable[28][0][Tokenizer.Peek().Type]&& LOOKTable[28][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg","Filter_Arg_Named");
        
    }
    ReturnValue = ParseFilter_Arg_Named(Tokenizer);
    return(ReturnValue);
}
Filter_Arg ParseFilter_Arg_1(MBCC::Tokenizer& Tokenizer)
{
    Filter_Arg ReturnValue;
    if(!(LOOKTable[30][0][Tokenizer.Peek().Type]&& LOOKTable[30][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg","Filter_Arg_Positional");
        
    }
    ReturnValue = ParseFilter_Arg_Positional(Tokenizer);
    return(ReturnValue);
}
Filter_Arg_Named ParseFilter_Arg_Named(MBCC::Tokenizer& Tokenizer)
{
    Filter_Arg_Named ReturnValue;
    ReturnValue = ParseFilter_Arg_Named_0(Tokenizer);
    return(ReturnValue);
}
Filter_Arg_Named ParseFilter_Arg_Named_0(MBCC::Tokenizer& Tokenizer)
{
    Filter_Arg_Named ReturnValue;
    ReturnValue.NamePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 21)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg_Named","idf");
        
    }
    ReturnValue.Name = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    if(Tokenizer.Peek().Type != 36)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg_Named","eq");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[38][0][Tokenizer.Peek().Type]&& LOOKTable[38][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg_Named","Literal");
        
    }
    ReturnValue.Argument = ParseLiteral(Tokenizer);
    return(ReturnValue);
}
Filter_Arg_Positional ParseFilter_Arg_Positional(MBCC::Tokenizer& Tokenizer)
{
    Filter_Arg_Positional ReturnValue;
    ReturnValue = ParseFilter_Arg_Positional_0(Tokenizer);
    return(ReturnValue);
}
Filter_Arg_Positional ParseFilter_Arg_Positional_0(MBCC::Tokenizer& Tokenizer)
{
    Filter_Arg_Positional ReturnValue;
    if(!(LOOKTable[38][0][Tokenizer.Peek().Type]&& LOOKTable[38][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg_Positional","Literal");
        
    }
    ReturnValue.Argument = ParseLiteral(Tokenizer);
    return(ReturnValue);
}
Filter_ArgList ParseFilter_ArgList(MBCC::Tokenizer& Tokenizer)
{
    Filter_ArgList ReturnValue;
    ReturnValue = ParseFilter_ArgList_0(Tokenizer);
    return(ReturnValue);
}
Filter_ArgList ParseFilter_ArgList_0(MBCC::Tokenizer& Tokenizer)
{
    Filter_ArgList ReturnValue;
    if(Tokenizer.Peek().Type != 0)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_ArgList","lpar");
        
    }
    Tokenizer.ConsumeToken();
    if(LOOKTable[36][0][Tokenizer.Peek().Type]&& LOOKTable[36][1][Tokenizer.Peek(1).Type])
    {
        if (LOOKTable[37][0][Tokenizer.Peek().Type]&& LOOKTable[37][1][Tokenizer.Peek(1).Type])
        {
            if(!(LOOKTable[25][0][Tokenizer.Peek().Type]&& LOOKTable[25][1][Tokenizer.Peek(1).Type]))
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L2","Filter_Arg");
                
            }
            ReturnValue.Arguments.push_back(ParseFilter_Arg(Tokenizer));
            while(LOOKTable[34][0][Tokenizer.Peek().Type]&& LOOKTable[34][1][Tokenizer.Peek(1).Type])
            {
                if (LOOKTable[35][0][Tokenizer.Peek().Type]&& LOOKTable[35][1][Tokenizer.Peek(1).Type])
                {
                    if(Tokenizer.Peek().Type != 34)
                    {
                        throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L1","comma");
                        
                    }
                    Tokenizer.ConsumeToken();
                    if(!(LOOKTable[25][0][Tokenizer.Peek().Type]&& LOOKTable[25][1][Tokenizer.Peek(1).Type]))
                    {
                        throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L1","Filter_Arg");
                        
                    }
                    ReturnValue.Arguments.push_back(ParseFilter_Arg(Tokenizer));
                    
                }
                else
                {
                    throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L2","_L2");
                    
                }
                
            }
            
        }
        else
        {
            throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_ArgList","Filter_ArgList");
            
        }
        
    }
    if(Tokenizer.Peek().Type != 1)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_ArgList","rpar");
        
    }
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Literal ParseLiteral(MBCC::Tokenizer& Tokenizer)
{
    Literal ReturnValue;
    if (LOOKTable[39][0][Tokenizer.Peek().Type]&& LOOKTable[39][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseLiteral_0(Tokenizer);
    }
    else if (LOOKTable[40][0][Tokenizer.Peek().Type]&& LOOKTable[40][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseLiteral_1(Tokenizer);
    }
    else if (LOOKTable[41][0][Tokenizer.Peek().Type]&& LOOKTable[41][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseLiteral_2(Tokenizer);
    }
    else
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Literal","Literal");
        
    }
    return(ReturnValue);
}
Literal ParseLiteral_0(MBCC::Tokenizer& Tokenizer)
{
    Literal ReturnValue;
    if(!(LOOKTable[42][0][Tokenizer.Peek().Type]&& LOOKTable[42][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Literal","Literal_String");
        
    }
    ReturnValue = ParseLiteral_String(Tokenizer);
    return(ReturnValue);
}
Literal ParseLiteral_1(MBCC::Tokenizer& Tokenizer)
{
    Literal ReturnValue;
    if(!(LOOKTable[44][0][Tokenizer.Peek().Type]&& LOOKTable[44][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Literal","Literal_Symbol");
        
    }
    ReturnValue = ParseLiteral_Symbol(Tokenizer);
    return(ReturnValue);
}
Literal ParseLiteral_2(MBCC::Tokenizer& Tokenizer)
{
    Literal ReturnValue;
    if(!(LOOKTable[46][0][Tokenizer.Peek().Type]&& LOOKTable[46][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Literal","Literal_Number");
        
    }
    ReturnValue = ParseLiteral_Number(Tokenizer);
    return(ReturnValue);
}
Literal_String ParseLiteral_String(MBCC::Tokenizer& Tokenizer)
{
    Literal_String ReturnValue;
    ReturnValue = ParseLiteral_String_0(Tokenizer);
    return(ReturnValue);
}
Literal_String ParseLiteral_String_0(MBCC::Tokenizer& Tokenizer)
{
    Literal_String ReturnValue;
    ReturnValue.ValuePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 7)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Literal_String","string");
        
    }
    ReturnValue.Value = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Literal_Symbol ParseLiteral_Symbol(MBCC::Tokenizer& Tokenizer)
{
    Literal_Symbol ReturnValue;
    ReturnValue = ParseLiteral_Symbol_0(Tokenizer);
    return(ReturnValue);
}
Literal_Symbol ParseLiteral_Symbol_0(MBCC::Tokenizer& Tokenizer)
{
    Literal_Symbol ReturnValue;
    ReturnValue.ValuePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 21)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Literal_Symbol","idf");
        
    }
    ReturnValue.Value = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Literal_Number ParseLiteral_Number(MBCC::Tokenizer& Tokenizer)
{
    Literal_Number ReturnValue;
    ReturnValue = ParseLiteral_Number_0(Tokenizer);
    return(ReturnValue);
}
Literal_Number ParseLiteral_Number_0(MBCC::Tokenizer& Tokenizer)
{
    Literal_Number ReturnValue;
    ReturnValue.ValuePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 6)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Literal_Number","number");
        
    }
    ReturnValue.Value = std::stoi(Tokenizer.Peek().Value);
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Identifier ParseIdentifier2(MBCC::Tokenizer& Tokenizer)
{
    Identifier ReturnValue;
    ReturnValue = ParseIdentifier2_0(Tokenizer);
    return(ReturnValue);
}
Identifier ParseIdentifier2_0(MBCC::Tokenizer& Tokenizer)
{
    Identifier ReturnValue;
    if(!(LOOKTable[52][0][Tokenizer.Peek().Type]&& LOOKTable[52][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Identifier2","Token2");
        
    }
    ReturnValue.Parts.push_back(ParseToken2(Tokenizer));
    while(LOOKTable[50][0][Tokenizer.Peek().Type]&& LOOKTable[50][1][Tokenizer.Peek(1).Type])
    {
        if (LOOKTable[51][0][Tokenizer.Peek().Type]&& LOOKTable[51][1][Tokenizer.Peek(1).Type])
        {
            if(Tokenizer.Peek().Type != 37)
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L3","dot");
                
            }
            Tokenizer.ConsumeToken();
            if(!(LOOKTable[52][0][Tokenizer.Peek().Type]&& LOOKTable[52][1][Tokenizer.Peek(1).Type]))
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L3","Token2");
                
            }
            ReturnValue.Parts.push_back(ParseToken2(Tokenizer));
            
        }
        else
        {
            throw MBCC::ParsingException(Tokenizer.Peek().Position,"Identifier2","Identifier2");
            
        }
        
    }
    return(ReturnValue);
}
Token ParseToken2(MBCC::Tokenizer& Tokenizer)
{
    Token ReturnValue;
    ReturnValue = ParseToken2_0(Tokenizer);
    return(ReturnValue);
}
Token ParseToken2_0(MBCC::Tokenizer& Tokenizer)
{
    Token ReturnValue;
    ReturnValue.Position = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 21)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Token2","idf");
        
    }
    ReturnValue.Value = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Filter_Component_Func ParseFilter_Component_Func(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component_Func ReturnValue;
    ReturnValue = ParseFilter_Component_Func_0(Tokenizer);
    return(ReturnValue);
}
Filter_Component_Func ParseFilter_Component_Func_0(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component_Func ReturnValue;
    ReturnValue.NamePosition = Tokenizer.Peek().Position;
    if(!(LOOKTable[48][0][Tokenizer.Peek().Type]&& LOOKTable[48][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component_Func","Identifier2");
        
    }
    ReturnValue.FilterName = ParseIdentifier2(Tokenizer);
    if(!(LOOKTable[32][0][Tokenizer.Peek().Type]&& LOOKTable[32][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component_Func","Filter_ArgList");
        
    }
    ReturnValue.ArgumentList = ParseFilter_ArgList(Tokenizer);
    return(ReturnValue);
}
Filter_Component ParseFilter_Term(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component ReturnValue;
    if (LOOKTable[57][0][Tokenizer.Peek().Type]&& LOOKTable[57][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseFilter_Term_0(Tokenizer);
    }
    else if (LOOKTable[58][0][Tokenizer.Peek().Type]&& LOOKTable[58][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseFilter_Term_1(Tokenizer);
    }
    else if (LOOKTable[59][0][Tokenizer.Peek().Type]&& LOOKTable[59][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseFilter_Term_2(Tokenizer);
    }
    else if (LOOKTable[60][0][Tokenizer.Peek().Type]&& LOOKTable[60][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseFilter_Term_3(Tokenizer);
    }
    else
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Term","Filter_Term");
        
    }
    return(ReturnValue);
}
Filter_Component ParseFilter_Term_0(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component ReturnValue;
    if(Tokenizer.Peek().Type != 0)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Term","lpar");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[63][0][Tokenizer.Peek().Type]&& LOOKTable[63][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Term","Filter_Component");
        
    }
    ReturnValue = ParseFilter_Component(Tokenizer);
    if(Tokenizer.Peek().Type != 1)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Term","rpar");
        
    }
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Filter_Component ParseFilter_Term_1(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component ReturnValue;
    if(!(LOOKTable[54][0][Tokenizer.Peek().Type]&& LOOKTable[54][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Term","Filter_Component_Func");
        
    }
    ReturnValue = ParseFilter_Component_Func(Tokenizer);
    return(ReturnValue);
}
Filter_Component ParseFilter_Term_2(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component ReturnValue;
    if(!(LOOKTable[61][0][Tokenizer.Peek().Type]&& LOOKTable[61][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Term","Filter_Component_Literal");
        
    }
    ReturnValue = ParseFilter_Component_Literal(Tokenizer);
    return(ReturnValue);
}
Filter_Component ParseFilter_Term_3(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component ReturnValue;
    if(Tokenizer.Peek().Type != 25)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Term","bang");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[56][0][Tokenizer.Peek().Type]&& LOOKTable[56][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Term","Filter_Term");
        
    }
    ReturnValue = ParseFilter_Term(Tokenizer);
    bool MBCC_TempVar0;
    MBCC_TempVar0 = true;
    ReturnValue.GetBase().Negated = std::move(MBCC_TempVar0);
    return(ReturnValue);
}
Filter_Component_Literal ParseFilter_Component_Literal(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component_Literal ReturnValue;
    ReturnValue = ParseFilter_Component_Literal_0(Tokenizer);
    return(ReturnValue);
}
Filter_Component_Literal ParseFilter_Component_Literal_0(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component_Literal ReturnValue;
    if(!(LOOKTable[38][0][Tokenizer.Peek().Type]&& LOOKTable[38][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component_Literal","Literal");
        
    }
    ReturnValue.Value = ParseLiteral(Tokenizer);
    return(ReturnValue);
}
Filter_OperatorList ParseFilter_Component(MBCC::Tokenizer& Tokenizer)
{
    Filter_OperatorList ReturnValue;
    ReturnValue = ParseFilter_Component_0(Tokenizer);
    return(ReturnValue);
}
Filter_OperatorList ParseFilter_Component_0(MBCC::Tokenizer& Tokenizer)
{
    Filter_OperatorList ReturnValue;
    if(!(LOOKTable[56][0][Tokenizer.Peek().Type]&& LOOKTable[56][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component","Filter_Term");
        
    }
    ReturnValue.Components.push_back(ParseFilter_Term(Tokenizer));
    while(LOOKTable[65][0][Tokenizer.Peek().Type]&& LOOKTable[65][1][Tokenizer.Peek(1).Type])
    {
        if (LOOKTable[66][0][Tokenizer.Peek().Type]&& LOOKTable[66][1][Tokenizer.Peek(1).Type])
        {
            if(!(LOOKTable[8][0][Tokenizer.Peek().Type]&& LOOKTable[8][1][Tokenizer.Peek(1).Type]))
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L4","ExprOperator");
                
            }
            ReturnValue.Operators.push_back(ParseExprOperator(Tokenizer).Op);
            if(!(LOOKTable[56][0][Tokenizer.Peek().Type]&& LOOKTable[56][1][Tokenizer.Peek(1).Type]))
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L4","Filter_Term");
                
            }
            ReturnValue.Components.push_back(ParseFilter_Term(Tokenizer));
            
        }
        else
        {
            throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component","Filter_Component");
            
        }
        
    }
    return(ReturnValue);
}
Filter ParseFilter(MBCC::Tokenizer& Tokenizer)
{
    Filter ReturnValue;
    ReturnValue = ParseFilter_0(Tokenizer);
    return(ReturnValue);
}
Filter ParseFilter_0(MBCC::Tokenizer& Tokenizer)
{
    Filter ReturnValue;
    ReturnValue.FilterPosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 18)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter","FILTER");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[63][0][Tokenizer.Peek().Type]&& LOOKTable[63][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter","Filter_Component");
        
    }
    ReturnValue.Component = ParseFilter_Component(Tokenizer);
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicate_Or(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    ReturnValue = ParseGameInfoPredicate_Or_0(Tokenizer);
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicate_Or_0(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    std::string MBCC_TempVar1;
    if(Tokenizer.Peek().Type != 38)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Or","OR");
        
    }
    MBCC_TempVar1 = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[77][0][Tokenizer.Peek().Type]&& LOOKTable[77][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Or","GameInfoPredicateOr");
        
    }
    ReturnValue = ParseGameInfoPredicateOr(Tokenizer);
    ReturnValue.Operator = std::move(MBCC_TempVar1);
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicate_And(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    ReturnValue = ParseGameInfoPredicate_And_0(Tokenizer);
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicate_And_0(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    std::string MBCC_TempVar2;
    if(Tokenizer.Peek().Type != 40)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_And","AND");
        
    }
    MBCC_TempVar2 = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[83][0][Tokenizer.Peek().Type]&& LOOKTable[83][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_And","GameInfoPredicate");
        
    }
    ReturnValue = ParseGameInfoPredicate(Tokenizer);
    ReturnValue.Operator = std::move(MBCC_TempVar2);
    return(ReturnValue);
}
GameInfoPredicate_Direct ParseGameInfoPredicate_Direct(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate_Direct ReturnValue;
    ReturnValue = ParseGameInfoPredicate_Direct_0(Tokenizer);
    return(ReturnValue);
}
GameInfoPredicate_Direct ParseGameInfoPredicate_Direct_0(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate_Direct ReturnValue;
    if(!(LOOKTable[0][0][Tokenizer.Peek().Type]&& LOOKTable[0][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Direct","Comparator");
        
    }
    ReturnValue.Comparison = ParseComparator(Tokenizer).Op;
    ReturnValue.ValuePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 7)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Direct","string");
        
    }
    ReturnValue.Value = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicate_Base(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    ReturnValue = ParseGameInfoPredicate_Base_0(Tokenizer);
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicate_Base_0(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    if(!(LOOKTable[113][0][Tokenizer.Peek().Type]&& LOOKTable[113][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Base","Identifier");
        
    }
    ReturnValue.Attribute = ParseIdentifier(Tokenizer);
    if(LOOKTable[73][0][Tokenizer.Peek().Type]&& LOOKTable[73][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.Data = ParseGameInfoPredicate_Direct(Tokenizer);
        
    }
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicateOr(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    ReturnValue = ParseGameInfoPredicateOr_0(Tokenizer);
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicateOr_0(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    ReturnValue.Operator = "||";
    if(!(LOOKTable[79][0][Tokenizer.Peek().Type]&& LOOKTable[79][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicateOr","GameInfoPredicate_Term");
        
    }
    ReturnValue.ExtraTerms.push_back(ParseGameInfoPredicate_Term(Tokenizer));
    while(LOOKTable[69][0][Tokenizer.Peek().Type]&& LOOKTable[69][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.ExtraTerms.push_back(ParseGameInfoPredicate_Or(Tokenizer));
        
    }
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicate_Term(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    if (LOOKTable[80][0][Tokenizer.Peek().Type]&& LOOKTable[80][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseGameInfoPredicate_Term_0(Tokenizer);
    }
    else if (LOOKTable[81][0][Tokenizer.Peek().Type]&& LOOKTable[81][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseGameInfoPredicate_Term_1(Tokenizer);
    }
    else if (LOOKTable[82][0][Tokenizer.Peek().Type]&& LOOKTable[82][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseGameInfoPredicate_Term_2(Tokenizer);
    }
    else
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Term","GameInfoPredicate_Term");
        
    }
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicate_Term_0(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    if(Tokenizer.Peek().Type != 0)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Term","lpar");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[83][0][Tokenizer.Peek().Type]&& LOOKTable[83][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Term","GameInfoPredicate");
        
    }
    ReturnValue = ParseGameInfoPredicate(Tokenizer);
    if(Tokenizer.Peek().Type != 1)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Term","rpar");
        
    }
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicate_Term_1(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    if(Tokenizer.Peek().Type != 25)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Term","bang");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[79][0][Tokenizer.Peek().Type]&& LOOKTable[79][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Term","GameInfoPredicate_Term");
        
    }
    ReturnValue = ParseGameInfoPredicate_Term(Tokenizer);
    bool MBCC_TempVar3;
    MBCC_TempVar3 = true;
    ReturnValue.Negated = std::move(MBCC_TempVar3);
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicate_Term_2(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    if(!(LOOKTable[75][0][Tokenizer.Peek().Type]&& LOOKTable[75][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Term","GameInfoPredicate_Base");
        
    }
    ReturnValue = ParseGameInfoPredicate_Base(Tokenizer);
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicate(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    ReturnValue = ParseGameInfoPredicate_0(Tokenizer);
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicate_0(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    ReturnValue.Operator = "&&";
    if(!(LOOKTable[77][0][Tokenizer.Peek().Type]&& LOOKTable[77][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate","GameInfoPredicateOr");
        
    }
    ReturnValue.ExtraTerms.push_back(ParseGameInfoPredicateOr(Tokenizer));
    while(LOOKTable[71][0][Tokenizer.Peek().Type]&& LOOKTable[71][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.ExtraTerms.push_back(ParseGameInfoPredicate_And(Tokenizer));
        
    }
    return(ReturnValue);
}
GameSelection ParseGameSelection(MBCC::Tokenizer& Tokenizer)
{
    GameSelection ReturnValue;
    ReturnValue = ParseGameSelection_0(Tokenizer);
    return(ReturnValue);
}
GameSelection ParseGameSelection_0(MBCC::Tokenizer& Tokenizer)
{
    GameSelection ReturnValue;
    if(LOOKTable[123][0][Tokenizer.Peek().Type]&& LOOKTable[123][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.Using = ParseUsingDirective(Tokenizer);
        
    }
    if(LOOKTable[87][0][Tokenizer.Peek().Type]&& LOOKTable[87][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.Assignment = ParsePlayerAssignment(Tokenizer);
        
    }
    ReturnValue.SelectPosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 13)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameSelection","SELECT");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[83][0][Tokenizer.Peek().Type]&& LOOKTable[83][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameSelection","GameInfoPredicate");
        
    }
    ReturnValue.GameCondition = ParseGameInfoPredicate(Tokenizer);
    return(ReturnValue);
}
PlayerAssignment ParsePlayerAssignment(MBCC::Tokenizer& Tokenizer)
{
    PlayerAssignment ReturnValue;
    ReturnValue = ParsePlayerAssignment_0(Tokenizer);
    return(ReturnValue);
}
PlayerAssignment ParsePlayerAssignment_0(MBCC::Tokenizer& Tokenizer)
{
    PlayerAssignment ReturnValue;
    ReturnValue.WithPosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 9)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"PlayerAssignment","WITH");
        
    }
    Tokenizer.ConsumeToken();
    ReturnValue.PlayerPosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 21)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"PlayerAssignment","idf");
        
    }
    ReturnValue.AffectedPlayer = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[83][0][Tokenizer.Peek().Type]&& LOOKTable[83][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"PlayerAssignment","GameInfoPredicate");
        
    }
    ReturnValue.PlayerCondition = ParseGameInfoPredicate(Tokenizer);
    return(ReturnValue);
}
Result_Record ParseResult_Record(MBCC::Tokenizer& Tokenizer)
{
    Result_Record ReturnValue;
    ReturnValue = ParseResult_Record_0(Tokenizer);
    return(ReturnValue);
}
Result_Record ParseResult_Record_0(MBCC::Tokenizer& Tokenizer)
{
    Result_Record ReturnValue;
    ReturnValue.RecordPosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 14)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result_Record","RECORD");
        
    }
    Tokenizer.ConsumeToken();
    ReturnValue.FilePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 7)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result_Record","string");
        
    }
    ReturnValue.OutFile = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Result_Print ParseResult_Print(MBCC::Tokenizer& Tokenizer)
{
    Result_Print ReturnValue;
    ReturnValue = ParseResult_Print_0(Tokenizer);
    return(ReturnValue);
}
Result_Print ParseResult_Print_0(MBCC::Tokenizer& Tokenizer)
{
    Result_Print ReturnValue;
    ReturnValue.PrintPosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 19)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result_Print","PRINT");
        
    }
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Result ParseResult(MBCC::Tokenizer& Tokenizer)
{
    Result ReturnValue;
    if (LOOKTable[94][0][Tokenizer.Peek().Type]&& LOOKTable[94][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseResult_0(Tokenizer);
    }
    else if (LOOKTable[95][0][Tokenizer.Peek().Type]&& LOOKTable[95][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseResult_1(Tokenizer);
    }
    else
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result","Result");
        
    }
    return(ReturnValue);
}
Result ParseResult_0(MBCC::Tokenizer& Tokenizer)
{
    Result ReturnValue;
    if(!(LOOKTable[89][0][Tokenizer.Peek().Type]&& LOOKTable[89][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result","Result_Record");
        
    }
    ReturnValue = ParseResult_Record(Tokenizer);
    return(ReturnValue);
}
Result ParseResult_1(MBCC::Tokenizer& Tokenizer)
{
    Result ReturnValue;
    if(!(LOOKTable[91][0][Tokenizer.Peek().Type]&& LOOKTable[91][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result","Result_Print");
        
    }
    ReturnValue = ParseResult_Print(Tokenizer);
    return(ReturnValue);
}
Selection ParseSelection(MBCC::Tokenizer& Tokenizer)
{
    Selection ReturnValue;
    ReturnValue = ParseSelection_0(Tokenizer);
    return(ReturnValue);
}
Selection ParseSelection_0(MBCC::Tokenizer& Tokenizer)
{
    Selection ReturnValue;
    if(!(LOOKTable[85][0][Tokenizer.Peek().Type]&& LOOKTable[85][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Selection","GameSelection");
        
    }
    ReturnValue.Games = ParseGameSelection(Tokenizer);
    if(LOOKTable[67][0][Tokenizer.Peek().Type]&& LOOKTable[67][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.SituationFilter = ParseFilter(Tokenizer);
        
    }
    if(!(LOOKTable[93][0][Tokenizer.Peek().Type]&& LOOKTable[93][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Selection","Result");
        
    }
    ReturnValue.Output = ParseResult(Tokenizer);
    if(Tokenizer.Peek().Type != 22)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Selection","semi");
        
    }
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
VariableDeclaration_Filter ParseVariableDeclaration_Filter(MBCC::Tokenizer& Tokenizer)
{
    VariableDeclaration_Filter ReturnValue;
    ReturnValue = ParseVariableDeclaration_Filter_0(Tokenizer);
    return(ReturnValue);
}
VariableDeclaration_Filter ParseVariableDeclaration_Filter_0(MBCC::Tokenizer& Tokenizer)
{
    VariableDeclaration_Filter ReturnValue;
    ReturnValue.FilterPosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 18)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_Filter","FILTER");
        
    }
    Tokenizer.ConsumeToken();
    ReturnValue.NamePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 21)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_Filter","idf");
        
    }
    ReturnValue.Name = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[32][0][Tokenizer.Peek().Type]&& LOOKTable[32][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_Filter","Filter_ArgList");
        
    }
    ReturnValue.Arguments = ParseFilter_ArgList(Tokenizer);
    if(Tokenizer.Peek().Type != 36)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_Filter","eq");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[63][0][Tokenizer.Peek().Type]&& LOOKTable[63][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_Filter","Filter_Component");
        
    }
    ReturnValue.Component = ParseFilter_Component(Tokenizer);
    if(Tokenizer.Peek().Type != 22)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_Filter","semi");
        
    }
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
VariableDeclaration_GameList ParseVariableDeclaration_GameList(MBCC::Tokenizer& Tokenizer)
{
    VariableDeclaration_GameList ReturnValue;
    ReturnValue = ParseVariableDeclaration_GameList_0(Tokenizer);
    return(ReturnValue);
}
VariableDeclaration_GameList ParseVariableDeclaration_GameList_0(MBCC::Tokenizer& Tokenizer)
{
    VariableDeclaration_GameList ReturnValue;
    ReturnValue.GamePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 15)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameList","GAMES");
        
    }
    Tokenizer.ConsumeToken();
    ReturnValue.NamePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 21)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameList","idf");
        
    }
    ReturnValue.Name = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    if(Tokenizer.Peek().Type != 36)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameList","eq");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[85][0][Tokenizer.Peek().Type]&& LOOKTable[85][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameList","GameSelection");
        
    }
    ReturnValue.Selection = ParseGameSelection(Tokenizer);
    if(Tokenizer.Peek().Type != 22)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameList","semi");
        
    }
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
VariableDeclaration_PlayerSelection ParseVariableDeclaration_PlayerSelection(MBCC::Tokenizer& Tokenizer)
{
    VariableDeclaration_PlayerSelection ReturnValue;
    ReturnValue = ParseVariableDeclaration_PlayerSelection_0(Tokenizer);
    return(ReturnValue);
}
VariableDeclaration_PlayerSelection ParseVariableDeclaration_PlayerSelection_0(MBCC::Tokenizer& Tokenizer)
{
    VariableDeclaration_PlayerSelection ReturnValue;
    ReturnValue.GamePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 17)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_PlayerSelection","PlayerSelection");
        
    }
    Tokenizer.ConsumeToken();
    ReturnValue.NamePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 21)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_PlayerSelection","idf");
        
    }
    ReturnValue.Name = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    if(Tokenizer.Peek().Type != 36)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_PlayerSelection","eq");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[83][0][Tokenizer.Peek().Type]&& LOOKTable[83][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_PlayerSelection","GameInfoPredicate");
        
    }
    ReturnValue.Predicate = ParseGameInfoPredicate(Tokenizer);
    if(Tokenizer.Peek().Type != 22)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_PlayerSelection","semi");
        
    }
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
VariableDeclaration_GameInfoPredicate ParseVariableDeclaration_GameInfoPredicate(MBCC::Tokenizer& Tokenizer)
{
    VariableDeclaration_GameInfoPredicate ReturnValue;
    ReturnValue = ParseVariableDeclaration_GameInfoPredicate_0(Tokenizer);
    return(ReturnValue);
}
VariableDeclaration_GameInfoPredicate ParseVariableDeclaration_GameInfoPredicate_0(MBCC::Tokenizer& Tokenizer)
{
    VariableDeclaration_GameInfoPredicate ReturnValue;
    ReturnValue.GamePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 16)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameInfoPredicate","GamePredicate");
        
    }
    Tokenizer.ConsumeToken();
    ReturnValue.NamePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 21)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameInfoPredicate","idf");
        
    }
    ReturnValue.Name = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    if(Tokenizer.Peek().Type != 36)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameInfoPredicate","eq");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[83][0][Tokenizer.Peek().Type]&& LOOKTable[83][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameInfoPredicate","GameInfoPredicate");
        
    }
    ReturnValue.Predicate = ParseGameInfoPredicate(Tokenizer);
    if(Tokenizer.Peek().Type != 22)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameInfoPredicate","semi");
        
    }
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
VariableDeclaration ParseVariableDeclaration(MBCC::Tokenizer& Tokenizer)
{
    VariableDeclaration ReturnValue;
    if (LOOKTable[107][0][Tokenizer.Peek().Type]&& LOOKTable[107][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseVariableDeclaration_0(Tokenizer);
    }
    else if (LOOKTable[108][0][Tokenizer.Peek().Type]&& LOOKTable[108][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseVariableDeclaration_1(Tokenizer);
    }
    else if (LOOKTable[109][0][Tokenizer.Peek().Type]&& LOOKTable[109][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseVariableDeclaration_2(Tokenizer);
    }
    else if (LOOKTable[110][0][Tokenizer.Peek().Type]&& LOOKTable[110][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseVariableDeclaration_3(Tokenizer);
    }
    else
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration","VariableDeclaration");
        
    }
    return(ReturnValue);
}
VariableDeclaration ParseVariableDeclaration_0(MBCC::Tokenizer& Tokenizer)
{
    VariableDeclaration ReturnValue;
    if(!(LOOKTable[98][0][Tokenizer.Peek().Type]&& LOOKTable[98][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration","VariableDeclaration_Filter");
        
    }
    ReturnValue = ParseVariableDeclaration_Filter(Tokenizer);
    return(ReturnValue);
}
VariableDeclaration ParseVariableDeclaration_1(MBCC::Tokenizer& Tokenizer)
{
    VariableDeclaration ReturnValue;
    if(!(LOOKTable[100][0][Tokenizer.Peek().Type]&& LOOKTable[100][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration","VariableDeclaration_GameList");
        
    }
    ReturnValue = ParseVariableDeclaration_GameList(Tokenizer);
    return(ReturnValue);
}
VariableDeclaration ParseVariableDeclaration_2(MBCC::Tokenizer& Tokenizer)
{
    VariableDeclaration ReturnValue;
    if(!(LOOKTable[104][0][Tokenizer.Peek().Type]&& LOOKTable[104][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration","VariableDeclaration_GameInfoPredicate");
        
    }
    ReturnValue = ParseVariableDeclaration_GameInfoPredicate(Tokenizer);
    return(ReturnValue);
}
VariableDeclaration ParseVariableDeclaration_3(MBCC::Tokenizer& Tokenizer)
{
    VariableDeclaration ReturnValue;
    if(!(LOOKTable[102][0][Tokenizer.Peek().Type]&& LOOKTable[102][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration","VariableDeclaration_PlayerSelection");
        
    }
    ReturnValue = ParseVariableDeclaration_PlayerSelection(Tokenizer);
    return(ReturnValue);
}
Module ParseModule(MBCC::Tokenizer& Tokenizer)
{
    Module ReturnValue;
    ReturnValue = ParseModule_0(Tokenizer);
    return(ReturnValue);
}
Module ParseModule_0(MBCC::Tokenizer& Tokenizer)
{
    Module ReturnValue;
    while(LOOKTable[129][0][Tokenizer.Peek().Type]&& LOOKTable[129][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.Statements.push_back(ParseStatement(Tokenizer));
        
    }
    return(ReturnValue);
}
Identifier ParseIdentifier(MBCC::Tokenizer& Tokenizer)
{
    Identifier ReturnValue;
    ReturnValue = ParseIdentifier_0(Tokenizer);
    return(ReturnValue);
}
Identifier ParseIdentifier_0(MBCC::Tokenizer& Tokenizer)
{
    Identifier ReturnValue;
    if(!(LOOKTable[117][0][Tokenizer.Peek().Type]&& LOOKTable[117][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Identifier","Token");
        
    }
    ReturnValue.Parts.push_back(ParseToken(Tokenizer));
    while(LOOKTable[115][0][Tokenizer.Peek().Type]&& LOOKTable[115][1][Tokenizer.Peek(1).Type])
    {
        if (LOOKTable[116][0][Tokenizer.Peek().Type]&& LOOKTable[116][1][Tokenizer.Peek(1).Type])
        {
            if(Tokenizer.Peek().Type != 37)
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L5","dot");
                
            }
            Tokenizer.ConsumeToken();
            if(!(LOOKTable[117][0][Tokenizer.Peek().Type]&& LOOKTable[117][1][Tokenizer.Peek(1).Type]))
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L5","Token");
                
            }
            ReturnValue.Parts.push_back(ParseToken(Tokenizer));
            
        }
        else
        {
            throw MBCC::ParsingException(Tokenizer.Peek().Position,"Identifier","Identifier");
            
        }
        
    }
    return(ReturnValue);
}
Token ParseToken(MBCC::Tokenizer& Tokenizer)
{
    Token ReturnValue;
    ReturnValue = ParseToken_0(Tokenizer);
    return(ReturnValue);
}
Token ParseToken_0(MBCC::Tokenizer& Tokenizer)
{
    Token ReturnValue;
    ReturnValue.Position = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 21)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Token","idf");
        
    }
    ReturnValue.Value = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
UsingDirective ParseGameList(MBCC::Tokenizer& Tokenizer)
{
    UsingDirective ReturnValue;
    ReturnValue = ParseGameList_0(Tokenizer);
    return(ReturnValue);
}
UsingDirective ParseGameList_0(MBCC::Tokenizer& Tokenizer)
{
    UsingDirective ReturnValue;
    if(!(LOOKTable[117][0][Tokenizer.Peek().Type]&& LOOKTable[117][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameList","Token");
        
    }
    ReturnValue.GameSets.push_back(ParseToken(Tokenizer));
    while(LOOKTable[121][0][Tokenizer.Peek().Type]&& LOOKTable[121][1][Tokenizer.Peek(1).Type])
    {
        if (LOOKTable[122][0][Tokenizer.Peek().Type]&& LOOKTable[122][1][Tokenizer.Peek(1).Type])
        {
            if(Tokenizer.Peek().Type != 34)
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L6","comma");
                
            }
            Tokenizer.ConsumeToken();
            if(!(LOOKTable[117][0][Tokenizer.Peek().Type]&& LOOKTable[117][1][Tokenizer.Peek(1).Type]))
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L6","Token");
                
            }
            ReturnValue.GameSets.push_back(ParseToken(Tokenizer));
            
        }
        else
        {
            throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameList","GameList");
            
        }
        
    }
    return(ReturnValue);
}
UsingDirective ParseUsingDirective(MBCC::Tokenizer& Tokenizer)
{
    UsingDirective ReturnValue;
    ReturnValue = ParseUsingDirective_0(Tokenizer);
    return(ReturnValue);
}
UsingDirective ParseUsingDirective_0(MBCC::Tokenizer& Tokenizer)
{
    UsingDirective ReturnValue;
    MBCC::TokenPosition MBCC_TempVar4;
    MBCC_TempVar4 = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 11)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"UsingDirective","USING");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[119][0][Tokenizer.Peek().Type]&& LOOKTable[119][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"UsingDirective","GameList");
        
    }
    ReturnValue = ParseGameList(Tokenizer);
    ReturnValue.UsingPosition = std::move(MBCC_TempVar4);
    return(ReturnValue);
}
ImportBinding ParseImportBinding(MBCC::Tokenizer& Tokenizer)
{
    ImportBinding ReturnValue;
    ReturnValue = ParseImportBinding_0(Tokenizer);
    return(ReturnValue);
}
ImportBinding ParseImportBinding_0(MBCC::Tokenizer& Tokenizer)
{
    ImportBinding ReturnValue;
    ReturnValue.AsPosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 12)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ImportBinding","AS");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[117][0][Tokenizer.Peek().Type]&& LOOKTable[117][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ImportBinding","Token");
        
    }
    ReturnValue.ImportName = ParseToken(Tokenizer);
    return(ReturnValue);
}
Import ParseImport(MBCC::Tokenizer& Tokenizer)
{
    Import ReturnValue;
    ReturnValue = ParseImport_0(Tokenizer);
    return(ReturnValue);
}
Import ParseImport_0(MBCC::Tokenizer& Tokenizer)
{
    Import ReturnValue;
    ReturnValue.ImportPosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 10)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Import","IMPORT");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[113][0][Tokenizer.Peek().Type]&& LOOKTable[113][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Import","Identifier");
        
    }
    ReturnValue.ImportPath = ParseIdentifier(Tokenizer);
    if(LOOKTable[125][0][Tokenizer.Peek().Type]&& LOOKTable[125][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.Binding = ParseImportBinding(Tokenizer);
        
    }
    if(Tokenizer.Peek().Type != 22)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Import","semi");
        
    }
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Statement ParseStatement(MBCC::Tokenizer& Tokenizer)
{
    Statement ReturnValue;
    if (LOOKTable[130][0][Tokenizer.Peek().Type]&& LOOKTable[130][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseStatement_0(Tokenizer);
    }
    else if (LOOKTable[131][0][Tokenizer.Peek().Type]&& LOOKTable[131][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseStatement_1(Tokenizer);
    }
    else if (LOOKTable[132][0][Tokenizer.Peek().Type]&& LOOKTable[132][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseStatement_2(Tokenizer);
    }
    else
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Statement","Statement");
        
    }
    return(ReturnValue);
}
Statement ParseStatement_0(MBCC::Tokenizer& Tokenizer)
{
    Statement ReturnValue;
    if(!(LOOKTable[106][0][Tokenizer.Peek().Type]&& LOOKTable[106][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Statement","VariableDeclaration");
        
    }
    ReturnValue = ParseVariableDeclaration(Tokenizer);
    return(ReturnValue);
}
Statement ParseStatement_1(MBCC::Tokenizer& Tokenizer)
{
    Statement ReturnValue;
    if(!(LOOKTable[96][0][Tokenizer.Peek().Type]&& LOOKTable[96][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Statement","Selection");
        
    }
    ReturnValue = ParseSelection(Tokenizer);
    return(ReturnValue);
}
Statement ParseStatement_2(MBCC::Tokenizer& Tokenizer)
{
    Statement ReturnValue;
    if(!(LOOKTable[127][0][Tokenizer.Peek().Type]&& LOOKTable[127][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Statement","Import");
        
    }
    ReturnValue = ParseImport(Tokenizer);
    return(ReturnValue);
}
