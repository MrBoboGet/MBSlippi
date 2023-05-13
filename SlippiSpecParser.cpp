#include "SlippiSpecParser.h"
const bool LOOKTable[112][2][36] = {{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,true,true,true,false,false,true,false,true,true,true,true,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,true,true,true,false,false,true,false,false,false,false,true,false,false,},{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,},{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,true,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,false,false,false,false,false,true,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,true,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,false,false,true,false,false,false,false,false,false,false,false,false,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,false,false,true,false,false,false,false,false,false,false,false,false,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,false,false,true,false,false,false,false,false,false,false,false,false,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,false,false,true,false,false,false,false,false,false,false,false,false,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,false,false,true,false,false,false,false,false,false,false,false,false,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,false,false,true,false,false,false,false,false,false,false,false,false,false,false,true,false,true,false,true,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,false,true,true,false,false,true,false,false,false,false,false,false,false,false,true,false,true,false,true,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,false,false,true,false,false,false,false,false,false,false,false,false,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,true,true,true,false,false,true,true,false,false,false,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,true,true,true,false,false,true,true,false,false,false,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,true,true,true,false,false,true,true,false,false,false,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,true,true,true,false,false,true,true,false,false,false,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,true,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,true,true,false,false,false,true,true,false,false,true,false,false,false,false,false,false,false,false,false,false,true,false,true,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,true,true,false,false,false,true,true,false,false,true,false,false,false,false,false,false,false,false,false,false,true,false,true,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,true,true,true,false,false,true,true,false,false,false,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,true,true,true,false,false,true,true,false,false,false,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,true,true,false,false,false,true,true,false,false,true,true,true,false,true,true,true,false,false,true,true,true,false,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,true,true,false,false,false,true,true,false,false,true,false,false,false,false,false,false,false,false,false,false,true,false,true,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,true,true,true,false,false,true,true,false,false,false,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,true,true,false,false,false,true,true,false,false,true,true,true,false,true,true,true,false,false,true,true,true,false,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,true,true,false,false,false,true,true,false,false,true,true,true,false,true,true,true,false,false,true,true,true,false,true,true,false,true,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,false,false,false,false,true,true,false,false,false,true,true,false,true,true,true,true,true,true,true,true,false,false,true,true,true,false,true,true,false,true,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,true,true,false,false,false,true,true,false,false,true,true,true,false,true,true,true,false,false,true,true,true,false,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,true,true,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,true,true,false,true,true,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,true,true,false,true,true,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,true,true,false,true,true,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,true,true,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},},};
Operator ParseOperator(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if (LOOKTable[1][0][Tokenizer.Peek().Type]&& LOOKTable[1][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseOperator_0(Tokenizer);
    }
    else if (LOOKTable[2][0][Tokenizer.Peek().Type]&& LOOKTable[2][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseOperator_1(Tokenizer);
    }
    else if (LOOKTable[3][0][Tokenizer.Peek().Type]&& LOOKTable[3][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseOperator_2(Tokenizer);
    }
    else if (LOOKTable[4][0][Tokenizer.Peek().Type]&& LOOKTable[4][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseOperator_3(Tokenizer);
    }
    else if (LOOKTable[5][0][Tokenizer.Peek().Type]&& LOOKTable[5][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseOperator_4(Tokenizer);
    }
    else if (LOOKTable[6][0][Tokenizer.Peek().Type]&& LOOKTable[6][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseOperator_5(Tokenizer);
    }
    else if (LOOKTable[7][0][Tokenizer.Peek().Type]&& LOOKTable[7][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseOperator_6(Tokenizer);
    }
    else if (LOOKTable[8][0][Tokenizer.Peek().Type]&& LOOKTable[8][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseOperator_7(Tokenizer);
    }
    else if (LOOKTable[9][0][Tokenizer.Peek().Type]&& LOOKTable[9][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseOperator_8(Tokenizer);
    }
    else if (LOOKTable[10][0][Tokenizer.Peek().Type]&& LOOKTable[10][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseOperator_9(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Operator","Operator");
    }
    return(ReturnValue);
}
Operator ParseOperator_0(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 21)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Operator","less");
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseOperator_1(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 31)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Operator","pipe");
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseOperator_2(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 32)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Operator","AND");
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseOperator_3(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 30)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Operator","OR");
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseOperator_4(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 33)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Operator","add");
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseOperator_5(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 24)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Operator","great");
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseOperator_6(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 28)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Operator","eq");
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseOperator_7(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 20)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Operator","leq");
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseOperator_8(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 23)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Operator","geq");
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseOperator_9(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 25)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Operator","neq");
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseComparator(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if (LOOKTable[12][0][Tokenizer.Peek().Type]&& LOOKTable[12][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseComparator_0(Tokenizer);
    }
    else if (LOOKTable[13][0][Tokenizer.Peek().Type]&& LOOKTable[13][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseComparator_1(Tokenizer);
    }
    else if (LOOKTable[14][0][Tokenizer.Peek().Type]&& LOOKTable[14][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseComparator_2(Tokenizer);
    }
    else if (LOOKTable[15][0][Tokenizer.Peek().Type]&& LOOKTable[15][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseComparator_3(Tokenizer);
    }
    else if (LOOKTable[16][0][Tokenizer.Peek().Type]&& LOOKTable[16][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseComparator_4(Tokenizer);
    }
    else if (LOOKTable[17][0][Tokenizer.Peek().Type]&& LOOKTable[17][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseComparator_5(Tokenizer);
    }
    else if (LOOKTable[18][0][Tokenizer.Peek().Type]&& LOOKTable[18][1][Tokenizer.Peek(1).Type])
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
    if(Tokenizer.Peek().Type != 21)
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
    if(Tokenizer.Peek().Type != 33)
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
    if(Tokenizer.Peek().Type != 24)
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
    if(Tokenizer.Peek().Type != 28)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Comparator","eq");
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseComparator_4(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if(Tokenizer.Peek().Type != 20)
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
    if(Tokenizer.Peek().Type != 23)
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
    if(Tokenizer.Peek().Type != 25)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Comparator","neq");
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Operator ParseCombinator(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    if (LOOKTable[20][0][Tokenizer.Peek().Type]&& LOOKTable[20][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseCombinator_0(Tokenizer);
    }
    else if (LOOKTable[21][0][Tokenizer.Peek().Type]&& LOOKTable[21][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseCombinator_1(Tokenizer);
    }
    else if (LOOKTable[22][0][Tokenizer.Peek().Type]&& LOOKTable[22][1][Tokenizer.Peek(1).Type])
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
    if(Tokenizer.Peek().Type != 31)
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
    if(Tokenizer.Peek().Type != 32)
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
    if(Tokenizer.Peek().Type != 30)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Combinator","OR");
    }
    ReturnValue.Op = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Filter_Arg ParseFilter_Arg_Extra(MBCC::Tokenizer& Tokenizer)
{
    Filter_Arg ReturnValue;
    ReturnValue = ParseFilter_Arg_Extra_0(Tokenizer);
    return(ReturnValue);
}
Filter_Arg ParseFilter_Arg_Extra_0(MBCC::Tokenizer& Tokenizer)
{
    Filter_Arg ReturnValue;
    if(Tokenizer.Peek().Type != 27)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg_Extra","comma");
    }
    Tokenizer.ConsumeToken();
    if(LOOKTable[25][0][Tokenizer.Peek().Type]&& LOOKTable[25][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseFilter_Arg(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg_Extra","Filter_Arg");
    }
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
    if(LOOKTable[28][0][Tokenizer.Peek().Type]&& LOOKTable[28][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseFilter_Arg_Named(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg","Filter_Arg_Named");
    }
    return(ReturnValue);
}
Filter_Arg ParseFilter_Arg_1(MBCC::Tokenizer& Tokenizer)
{
    Filter_Arg ReturnValue;
    if(LOOKTable[30][0][Tokenizer.Peek().Type]&& LOOKTable[30][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseFilter_Arg_Positional(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg","Filter_Arg_Positional");
    }
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
    if(Tokenizer.Peek().Type != 18)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg_Named","idf");
    }
    ReturnValue.Name = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    if(Tokenizer.Peek().Type != 28)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg_Named","eq");
    }
    Tokenizer.ConsumeToken();
    ReturnValue.ValuePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 7)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg_Named","string");
    }
    ReturnValue.Value = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
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
    ReturnValue.ValuePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 7)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg_Positional","string");
    }
    ReturnValue.Value = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
ArgList ParseFilter_ArgList_Arguments(MBCC::Tokenizer& Tokenizer)
{
    ArgList ReturnValue;
    ReturnValue = ParseFilter_ArgList_Arguments_0(Tokenizer);
    return(ReturnValue);
}
ArgList ParseFilter_ArgList_Arguments_0(MBCC::Tokenizer& Tokenizer)
{
    ArgList ReturnValue;
    if(LOOKTable[25][0][Tokenizer.Peek().Type]&& LOOKTable[25][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.Args.push_back(ParseFilter_Arg(Tokenizer));
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_ArgList_Arguments","Filter_Arg");
    }
    while(LOOKTable[23][0][Tokenizer.Peek().Type]&& LOOKTable[23][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.Args.push_back(ParseFilter_Arg_Extra(Tokenizer));
    }
    return(ReturnValue);
}
ArgList ParseFilter_ArgList(MBCC::Tokenizer& Tokenizer)
{
    ArgList ReturnValue;
    ReturnValue = ParseFilter_ArgList_0(Tokenizer);
    return(ReturnValue);
}
ArgList ParseFilter_ArgList_0(MBCC::Tokenizer& Tokenizer)
{
    ArgList ReturnValue;
    if(Tokenizer.Peek().Type != 0)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_ArgList","lpar");
    }
    Tokenizer.ConsumeToken();
    if(LOOKTable[32][0][Tokenizer.Peek().Type]&& LOOKTable[32][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseFilter_ArgList_Arguments(Tokenizer);
    }
    if(Tokenizer.Peek().Type != 1)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_ArgList","rpar");
    }
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Filter_Component ParseFilter_Component_And(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component ReturnValue;
    ReturnValue = ParseFilter_Component_And_0(Tokenizer);
    return(ReturnValue);
}
Filter_Component ParseFilter_Component_And_0(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component ReturnValue;
    std::string Operator;
    if(Tokenizer.Peek().Type != 33)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component_And","add");
    }
    Operator = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    if(LOOKTable[49][0][Tokenizer.Peek().Type]&& LOOKTable[49][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseFilter_Component(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component_And","Filter_Component");
    }
    ReturnValue.Operator = std::move(Operator);
    return(ReturnValue);
}
Filter_Component ParseFilter_Component_Or(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component ReturnValue;
    ReturnValue = ParseFilter_Component_Or_0(Tokenizer);
    return(ReturnValue);
}
Filter_Component ParseFilter_Component_Or_0(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component ReturnValue;
    std::string Operator;
    if(Tokenizer.Peek().Type != 31)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component_Or","pipe");
    }
    Operator = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    if(LOOKTable[49][0][Tokenizer.Peek().Type]&& LOOKTable[49][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseFilter_Component(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component_Or","Filter_Component");
    }
    ReturnValue.Operator = std::move(Operator);
    return(ReturnValue);
}
Filter_OperatorList ParseFilter_OperatorList(MBCC::Tokenizer& Tokenizer)
{
    Filter_OperatorList ReturnValue;
    if (LOOKTable[41][0][Tokenizer.Peek().Type]&& LOOKTable[41][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseFilter_OperatorList_0(Tokenizer);
    }
    else if (LOOKTable[42][0][Tokenizer.Peek().Type]&& LOOKTable[42][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseFilter_OperatorList_1(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_OperatorList","Filter_OperatorList");
    }
    return(ReturnValue);
}
Filter_OperatorList ParseFilter_OperatorList_0(MBCC::Tokenizer& Tokenizer)
{
    Filter_OperatorList ReturnValue;
    do
    {
        ReturnValue.Components.push_back(ParseFilter_Component_And(Tokenizer));
    }
    while(LOOKTable[36][0][Tokenizer.Peek().Type]&& LOOKTable[36][1][Tokenizer.Peek(1).Type]);
    return(ReturnValue);
}
Filter_OperatorList ParseFilter_OperatorList_1(MBCC::Tokenizer& Tokenizer)
{
    Filter_OperatorList ReturnValue;
    do
    {
        ReturnValue.Components.push_back(ParseFilter_Component_Or(Tokenizer));
    }
    while(LOOKTable[38][0][Tokenizer.Peek().Type]&& LOOKTable[38][1][Tokenizer.Peek(1).Type]);
    return(ReturnValue);
}
Filter_Component_Function ParseFilter_Component_Function(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component_Function ReturnValue;
    ReturnValue = ParseFilter_Component_Function_0(Tokenizer);
    return(ReturnValue);
}
Filter_Component_Function ParseFilter_Component_Function_0(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component_Function ReturnValue;
    ReturnValue.NamePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 18)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component_Function","idf");
    }
    ReturnValue.FilterName = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Filter_Component_Data ParseFilter_Component_Data(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component_Data ReturnValue;
    ReturnValue = ParseFilter_Component_Data_0(Tokenizer);
    return(ReturnValue);
}
Filter_Component_Data ParseFilter_Component_Data_0(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component_Data ReturnValue;
    if(LOOKTable[43][0][Tokenizer.Peek().Type]&& LOOKTable[43][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseFilter_Component_Function(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component_Data","Filter_Component_Function");
    }
    return(ReturnValue);
}
Filter_Component ParseFilter_Component_Base(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component ReturnValue;
    ReturnValue = ParseFilter_Component_Base_0(Tokenizer);
    return(ReturnValue);
}
Filter_Component ParseFilter_Component_Base_0(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component ReturnValue;
    if(LOOKTable[45][0][Tokenizer.Peek().Type]&& LOOKTable[45][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.Data = ParseFilter_Component_Data(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component_Base","Filter_Component_Data");
    }
    if(LOOKTable[34][0][Tokenizer.Peek().Type]&& LOOKTable[34][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.ArgumentList.Arguments = ParseFilter_ArgList(Tokenizer).Args;
    }
    return(ReturnValue);
}
Filter_Component ParseFilter_Component(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component ReturnValue;
    if (LOOKTable[50][0][Tokenizer.Peek().Type]&& LOOKTable[50][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseFilter_Component_0(Tokenizer);
    }
    else if (LOOKTable[51][0][Tokenizer.Peek().Type]&& LOOKTable[51][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseFilter_Component_1(Tokenizer);
    }
    else if (LOOKTable[52][0][Tokenizer.Peek().Type]&& LOOKTable[52][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseFilter_Component_2(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component","Filter_Component");
    }
    return(ReturnValue);
}
Filter_Component ParseFilter_Component_0(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component ReturnValue;
    if(Tokenizer.Peek().Type != 0)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component","lpar");
    }
    Tokenizer.ConsumeToken();
    if(LOOKTable[49][0][Tokenizer.Peek().Type]&& LOOKTable[49][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.ExtraTerms.push_back(ParseFilter_Component(Tokenizer));
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component","Filter_Component");
    }
    if(Tokenizer.Peek().Type != 1)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component","rpar");
    }
    Tokenizer.ConsumeToken();
    if(LOOKTable[40][0][Tokenizer.Peek().Type]&& LOOKTable[40][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.ExtraTerms = ParseFilter_OperatorList(Tokenizer).Components;
    }
    return(ReturnValue);
}
Filter_Component ParseFilter_Component_1(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component ReturnValue;
    if(LOOKTable[47][0][Tokenizer.Peek().Type]&& LOOKTable[47][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseFilter_Component_Base(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component","Filter_Component_Base");
    }
    std::vector<Filter_Component> ExtraTerms;
    if(LOOKTable[40][0][Tokenizer.Peek().Type]&& LOOKTable[40][1][Tokenizer.Peek(1).Type])
    {
        ExtraTerms = ParseFilter_OperatorList(Tokenizer).Components;
    }
    ReturnValue.ExtraTerms = std::move(ExtraTerms);
    return(ReturnValue);
}
Filter_Component ParseFilter_Component_2(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component ReturnValue;
    if(Tokenizer.Peek().Type != 22)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component","bang");
    }
    Tokenizer.ConsumeToken();
    if(LOOKTable[49][0][Tokenizer.Peek().Type]&& LOOKTable[49][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseFilter_Component(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component","Filter_Component");
    }
    ReturnValue.Negated=true;
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
    if(Tokenizer.Peek().Type != 15)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter","FILTER");
    }
    Tokenizer.ConsumeToken();
    if(LOOKTable[49][0][Tokenizer.Peek().Type]&& LOOKTable[49][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.Component = ParseFilter_Component(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter","Filter_Component");
    }
    return(ReturnValue);
}
AttributeComponent ParseAttribute_Base(MBCC::Tokenizer& Tokenizer)
{
    AttributeComponent ReturnValue;
    ReturnValue = ParseAttribute_Base_0(Tokenizer);
    return(ReturnValue);
}
AttributeComponent ParseAttribute_Base_0(MBCC::Tokenizer& Tokenizer)
{
    AttributeComponent ReturnValue;
    ReturnValue.NamePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 18)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Attribute_Base","idf");
    }
    ReturnValue.Name = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
AttributeComponent ParseAttribute_Extension(MBCC::Tokenizer& Tokenizer)
{
    AttributeComponent ReturnValue;
    ReturnValue = ParseAttribute_Extension_0(Tokenizer);
    return(ReturnValue);
}
AttributeComponent ParseAttribute_Extension_0(MBCC::Tokenizer& Tokenizer)
{
    AttributeComponent ReturnValue;
    if(Tokenizer.Peek().Type != 29)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Attribute_Extension","dot");
    }
    Tokenizer.ConsumeToken();
    ReturnValue.NamePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 18)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Attribute_Extension","idf");
    }
    ReturnValue.Name = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
AttributeList ParseAttribute_List(MBCC::Tokenizer& Tokenizer)
{
    AttributeList ReturnValue;
    ReturnValue = ParseAttribute_List_0(Tokenizer);
    return(ReturnValue);
}
AttributeList ParseAttribute_List_0(MBCC::Tokenizer& Tokenizer)
{
    AttributeList ReturnValue;
    if(LOOKTable[55][0][Tokenizer.Peek().Type]&& LOOKTable[55][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.Attributes.push_back(ParseAttribute_Base(Tokenizer));
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Attribute_List","Attribute_Base");
    }
    while(LOOKTable[57][0][Tokenizer.Peek().Type]&& LOOKTable[57][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.Attributes.push_back(ParseAttribute_Extension(Tokenizer));
    }
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
    std::string Operator;
    if(Tokenizer.Peek().Type != 30)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Or","OR");
    }
    Operator = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    if(LOOKTable[77][0][Tokenizer.Peek().Type]&& LOOKTable[77][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseGameInfoPredicate(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Or","GameInfoPredicate");
    }
    ReturnValue.Operator = std::move(Operator);
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
    std::string Operator;
    if(Tokenizer.Peek().Type != 32)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_And","AND");
    }
    Operator = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    if(LOOKTable[77][0][Tokenizer.Peek().Type]&& LOOKTable[77][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseGameInfoPredicate(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_And","GameInfoPredicate");
    }
    ReturnValue.Operator = std::move(Operator);
    return(ReturnValue);
}
GameInfoPredicate_OperatorList ParseGameInfoPredicate_OperatorList(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate_OperatorList ReturnValue;
    if (LOOKTable[66][0][Tokenizer.Peek().Type]&& LOOKTable[66][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseGameInfoPredicate_OperatorList_0(Tokenizer);
    }
    else if (LOOKTable[67][0][Tokenizer.Peek().Type]&& LOOKTable[67][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseGameInfoPredicate_OperatorList_1(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_OperatorList","GameInfoPredicate_OperatorList");
    }
    return(ReturnValue);
}
GameInfoPredicate_OperatorList ParseGameInfoPredicate_OperatorList_0(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate_OperatorList ReturnValue;
    do
    {
        ReturnValue.Terms.push_back(ParseGameInfoPredicate_Or(Tokenizer));
    }
    while(LOOKTable[61][0][Tokenizer.Peek().Type]&& LOOKTable[61][1][Tokenizer.Peek(1).Type]);
    return(ReturnValue);
}
GameInfoPredicate_OperatorList ParseGameInfoPredicate_OperatorList_1(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate_OperatorList ReturnValue;
    do
    {
        ReturnValue.Terms.push_back(ParseGameInfoPredicate_And(Tokenizer));
    }
    while(LOOKTable[63][0][Tokenizer.Peek().Type]&& LOOKTable[63][1][Tokenizer.Peek(1).Type]);
    return(ReturnValue);
}
GameInfoPredicate_Variable ParseGameInfoPredicate_Variable(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate_Variable ReturnValue;
    ReturnValue = ParseGameInfoPredicate_Variable_0(Tokenizer);
    return(ReturnValue);
}
GameInfoPredicate_Variable ParseGameInfoPredicate_Variable_0(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate_Variable ReturnValue;
    ReturnValue.VariablePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 18)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Variable","idf");
    }
    ReturnValue.VariableName = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
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
    if(LOOKTable[59][0][Tokenizer.Peek().Type]&& LOOKTable[59][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.Attribute = ParseAttribute_List(Tokenizer).Attributes;
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Direct","Attribute_List");
    }
    if(LOOKTable[11][0][Tokenizer.Peek().Type]&& LOOKTable[11][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.Comparison = ParseComparator(Tokenizer).Op;
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Direct","Comparator");
    }
    ReturnValue.ValuePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 7)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Direct","string");
    }
    ReturnValue.Value = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
GameInfoPredicate_Data ParseGameInfoPredicate_Data(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate_Data ReturnValue;
    if (LOOKTable[73][0][Tokenizer.Peek().Type]&& LOOKTable[73][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseGameInfoPredicate_Data_0(Tokenizer);
    }
    else if (LOOKTable[74][0][Tokenizer.Peek().Type]&& LOOKTable[74][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseGameInfoPredicate_Data_1(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Data","GameInfoPredicate_Data");
    }
    return(ReturnValue);
}
GameInfoPredicate_Data ParseGameInfoPredicate_Data_0(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate_Data ReturnValue;
    if(LOOKTable[68][0][Tokenizer.Peek().Type]&& LOOKTable[68][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseGameInfoPredicate_Variable(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Data","GameInfoPredicate_Variable");
    }
    return(ReturnValue);
}
GameInfoPredicate_Data ParseGameInfoPredicate_Data_1(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate_Data ReturnValue;
    if(LOOKTable[70][0][Tokenizer.Peek().Type]&& LOOKTable[70][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseGameInfoPredicate_Direct(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Data","GameInfoPredicate_Direct");
    }
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
    if(LOOKTable[72][0][Tokenizer.Peek().Type]&& LOOKTable[72][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.Data = ParseGameInfoPredicate_Data(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Base","GameInfoPredicate_Data");
    }
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicate(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    if (LOOKTable[78][0][Tokenizer.Peek().Type]&& LOOKTable[78][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseGameInfoPredicate_0(Tokenizer);
    }
    else if (LOOKTable[79][0][Tokenizer.Peek().Type]&& LOOKTable[79][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseGameInfoPredicate_1(Tokenizer);
    }
    else if (LOOKTable[80][0][Tokenizer.Peek().Type]&& LOOKTable[80][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseGameInfoPredicate_2(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate","GameInfoPredicate");
    }
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicate_0(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    if(Tokenizer.Peek().Type != 0)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate","lpar");
    }
    Tokenizer.ConsumeToken();
    if(LOOKTable[77][0][Tokenizer.Peek().Type]&& LOOKTable[77][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.ExtraTerms.push_back(ParseGameInfoPredicate(Tokenizer));
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate","GameInfoPredicate");
    }
    if(Tokenizer.Peek().Type != 1)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate","rpar");
    }
    Tokenizer.ConsumeToken();
    if(LOOKTable[65][0][Tokenizer.Peek().Type]&& LOOKTable[65][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.ExtraTerms = ParseGameInfoPredicate_OperatorList(Tokenizer).Terms;
    }
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicate_1(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    if(LOOKTable[75][0][Tokenizer.Peek().Type]&& LOOKTable[75][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseGameInfoPredicate_Base(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate","GameInfoPredicate_Base");
    }
    std::vector<GameInfoPredicate> ExtraTerms;
    if(LOOKTable[65][0][Tokenizer.Peek().Type]&& LOOKTable[65][1][Tokenizer.Peek(1).Type])
    {
        ExtraTerms = ParseGameInfoPredicate_OperatorList(Tokenizer).Terms;
    }
    ReturnValue.ExtraTerms = std::move(ExtraTerms);
    return(ReturnValue);
}
GameInfoPredicate ParseGameInfoPredicate_2(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    if(Tokenizer.Peek().Type != 22)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate","bang");
    }
    Tokenizer.ConsumeToken();
    if(LOOKTable[77][0][Tokenizer.Peek().Type]&& LOOKTable[77][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseGameInfoPredicate(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate","GameInfoPredicate");
    }
    ReturnValue.Negated=true;
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
    if(LOOKTable[83][0][Tokenizer.Peek().Type]&& LOOKTable[83][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.Assignment = ParsePlayerAssignment(Tokenizer);
    }
    ReturnValue.SelectPosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 10)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameSelection","SELECT");
    }
    Tokenizer.ConsumeToken();
    if(LOOKTable[77][0][Tokenizer.Peek().Type]&& LOOKTable[77][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.GameCondition = ParseGameInfoPredicate(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameSelection","GameInfoPredicate");
    }
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
    if(Tokenizer.Peek().Type != 18)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"PlayerAssignment","idf");
    }
    ReturnValue.AffectedPlayer = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    if(LOOKTable[77][0][Tokenizer.Peek().Type]&& LOOKTable[77][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.PlayerCondition = ParseGameInfoPredicate(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"PlayerAssignment","GameInfoPredicate");
    }
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
    if(Tokenizer.Peek().Type != 11)
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
    if(Tokenizer.Peek().Type != 16)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result_Print","PRINT");
    }
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
Result ParseResult(MBCC::Tokenizer& Tokenizer)
{
    Result ReturnValue;
    if (LOOKTable[90][0][Tokenizer.Peek().Type]&& LOOKTable[90][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseResult_0(Tokenizer);
    }
    else if (LOOKTable[91][0][Tokenizer.Peek().Type]&& LOOKTable[91][1][Tokenizer.Peek(1).Type])
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
    if(LOOKTable[85][0][Tokenizer.Peek().Type]&& LOOKTable[85][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseResult_Record(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result","Result_Record");
    }
    return(ReturnValue);
}
Result ParseResult_1(MBCC::Tokenizer& Tokenizer)
{
    Result ReturnValue;
    if(LOOKTable[87][0][Tokenizer.Peek().Type]&& LOOKTable[87][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseResult_Print(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result","Result_Print");
    }
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
    if(LOOKTable[81][0][Tokenizer.Peek().Type]&& LOOKTable[81][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.Games = ParseGameSelection(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Selection","GameSelection");
    }
    if(LOOKTable[53][0][Tokenizer.Peek().Type]&& LOOKTable[53][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.SituationFilter = ParseFilter(Tokenizer);
    }
    if(LOOKTable[89][0][Tokenizer.Peek().Type]&& LOOKTable[89][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.Output = ParseResult(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Selection","Result");
    }
    if(Tokenizer.Peek().Type != 19)
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
    if(Tokenizer.Peek().Type != 15)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_Filter","FILTER");
    }
    Tokenizer.ConsumeToken();
    ReturnValue.NamePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 18)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_Filter","idf");
    }
    ReturnValue.Name = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    if(Tokenizer.Peek().Type != 28)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_Filter","eq");
    }
    Tokenizer.ConsumeToken();
    if(LOOKTable[49][0][Tokenizer.Peek().Type]&& LOOKTable[49][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.Component = ParseFilter_Component(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_Filter","Filter_Component");
    }
    if(Tokenizer.Peek().Type != 19)
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
    if(Tokenizer.Peek().Type != 12)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameList","GAMES");
    }
    Tokenizer.ConsumeToken();
    ReturnValue.NamePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 18)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameList","idf");
    }
    ReturnValue.Name = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    if(Tokenizer.Peek().Type != 28)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameList","eq");
    }
    Tokenizer.ConsumeToken();
    if(LOOKTable[81][0][Tokenizer.Peek().Type]&& LOOKTable[81][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.Selection = ParseGameSelection(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameList","GameSelection");
    }
    if(Tokenizer.Peek().Type != 19)
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
    if(Tokenizer.Peek().Type != 14)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_PlayerSelection","PlayerSelection");
    }
    Tokenizer.ConsumeToken();
    ReturnValue.NamePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 18)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_PlayerSelection","idf");
    }
    ReturnValue.Name = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    if(Tokenizer.Peek().Type != 28)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_PlayerSelection","eq");
    }
    Tokenizer.ConsumeToken();
    if(LOOKTable[77][0][Tokenizer.Peek().Type]&& LOOKTable[77][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.Predicate = ParseGameInfoPredicate(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_PlayerSelection","GameInfoPredicate");
    }
    if(Tokenizer.Peek().Type != 19)
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
    if(Tokenizer.Peek().Type != 13)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameInfoPredicate","GamePredicate");
    }
    Tokenizer.ConsumeToken();
    ReturnValue.NamePosition = Tokenizer.Peek().Position;
    if(Tokenizer.Peek().Type != 18)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameInfoPredicate","idf");
    }
    ReturnValue.Name = Tokenizer.Peek().Value;
    Tokenizer.ConsumeToken();
    if(Tokenizer.Peek().Type != 28)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameInfoPredicate","eq");
    }
    Tokenizer.ConsumeToken();
    if(LOOKTable[77][0][Tokenizer.Peek().Type]&& LOOKTable[77][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.Predicate = ParseGameInfoPredicate(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameInfoPredicate","GameInfoPredicate");
    }
    if(Tokenizer.Peek().Type != 19)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameInfoPredicate","semi");
    }
    Tokenizer.ConsumeToken();
    return(ReturnValue);
}
VariableDeclaration ParseVariableDeclaration(MBCC::Tokenizer& Tokenizer)
{
    VariableDeclaration ReturnValue;
    if (LOOKTable[103][0][Tokenizer.Peek().Type]&& LOOKTable[103][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseVariableDeclaration_0(Tokenizer);
    }
    else if (LOOKTable[104][0][Tokenizer.Peek().Type]&& LOOKTable[104][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseVariableDeclaration_1(Tokenizer);
    }
    else if (LOOKTable[105][0][Tokenizer.Peek().Type]&& LOOKTable[105][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseVariableDeclaration_2(Tokenizer);
    }
    else if (LOOKTable[106][0][Tokenizer.Peek().Type]&& LOOKTable[106][1][Tokenizer.Peek(1).Type])
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
    if(LOOKTable[94][0][Tokenizer.Peek().Type]&& LOOKTable[94][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseVariableDeclaration_Filter(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration","VariableDeclaration_Filter");
    }
    return(ReturnValue);
}
VariableDeclaration ParseVariableDeclaration_1(MBCC::Tokenizer& Tokenizer)
{
    VariableDeclaration ReturnValue;
    if(LOOKTable[96][0][Tokenizer.Peek().Type]&& LOOKTable[96][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseVariableDeclaration_GameList(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration","VariableDeclaration_GameList");
    }
    return(ReturnValue);
}
VariableDeclaration ParseVariableDeclaration_2(MBCC::Tokenizer& Tokenizer)
{
    VariableDeclaration ReturnValue;
    if(LOOKTable[100][0][Tokenizer.Peek().Type]&& LOOKTable[100][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseVariableDeclaration_GameInfoPredicate(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration","VariableDeclaration_GameInfoPredicate");
    }
    return(ReturnValue);
}
VariableDeclaration ParseVariableDeclaration_3(MBCC::Tokenizer& Tokenizer)
{
    VariableDeclaration ReturnValue;
    if(LOOKTable[98][0][Tokenizer.Peek().Type]&& LOOKTable[98][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseVariableDeclaration_PlayerSelection(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration","VariableDeclaration_PlayerSelection");
    }
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
    while(LOOKTable[109][0][Tokenizer.Peek().Type]&& LOOKTable[109][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.Statements.push_back(ParseStatement(Tokenizer));
    }
    return(ReturnValue);
}
Statement ParseStatement(MBCC::Tokenizer& Tokenizer)
{
    Statement ReturnValue;
    if (LOOKTable[110][0][Tokenizer.Peek().Type]&& LOOKTable[110][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseStatement_0(Tokenizer);
    }
    else if (LOOKTable[111][0][Tokenizer.Peek().Type]&& LOOKTable[111][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseStatement_1(Tokenizer);
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
    if(LOOKTable[102][0][Tokenizer.Peek().Type]&& LOOKTable[102][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseVariableDeclaration(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Statement","VariableDeclaration");
    }
    return(ReturnValue);
}
Statement ParseStatement_1(MBCC::Tokenizer& Tokenizer)
{
    Statement ReturnValue;
    if(LOOKTable[92][0][Tokenizer.Peek().Type]&& LOOKTable[92][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue = ParseSelection(Tokenizer);
    }
    else
    {
         throw MBCC::ParsingException(Tokenizer.Peek().Position,"Statement","Selection");
    }
    return(ReturnValue);
}
