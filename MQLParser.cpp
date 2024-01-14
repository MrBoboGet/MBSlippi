#include "MQLParser.h"
const bool LOOKTable[142][2][46] = {{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,true,true,true,false,false,false,false,false,false,false,true,false,false,false,false,true,false,false,},{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,},{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,true,true,true,true,true,true,true,false,false,true,false,false,false,true,false,true,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,true,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,true,true,false,false,false,false,false,false,true,false,false,false,false,true,true,true,false,true,true,true,true,true,true,true,true,true,true,true,true,false,true,true,true,true,false,true,false,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,},},{{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,true,true,false,false,false,false,false,false,true,false,false,false,false,true,true,true,false,true,true,true,true,true,true,true,true,true,true,true,true,false,true,true,false,true,false,true,false,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,},},{{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,true,true,false,false,false,false,false,false,true,false,false,false,false,true,true,true,false,true,true,true,true,true,true,true,true,true,true,true,true,false,true,true,false,true,false,true,false,true,false,true,},},{{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,true,true,false,false,false,false,false,false,true,false,false,false,false,true,true,true,false,true,true,true,true,true,true,true,true,true,true,true,true,false,true,true,false,true,false,true,false,true,false,true,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,true,true,false,false,false,false,false,false,true,false,false,false,false,true,true,true,false,true,true,true,true,true,true,true,true,true,true,true,true,false,true,true,true,true,false,true,false,true,false,true,},},{{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,true,true,false,false,false,false,false,false,true,false,false,false,false,true,true,true,false,true,true,true,true,true,true,true,true,true,true,true,true,false,true,true,true,true,false,true,false,true,false,true,},},{{false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,true,true,false,false,true,true,true,false,true,true,true,true,true,true,true,false,true,true,false,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,true,true,false,false,true,true,true,false,true,true,true,true,true,true,true,false,true,true,false,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,true,true,false,false,true,true,true,false,true,true,true,true,true,true,true,false,true,true,false,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,true,true,false,false,true,true,true,false,true,true,true,true,true,true,true,false,true,true,false,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,true,true,false,false,true,true,true,false,true,true,true,true,true,true,true,false,true,true,false,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,true,true,false,false,true,true,true,false,true,true,true,true,true,true,true,false,true,true,false,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,true,true,false,false,true,true,true,false,true,true,true,true,true,true,true,false,true,true,false,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,true,true,false,false,true,true,true,false,true,true,true,true,true,true,true,false,true,true,false,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,true,true,false,false,true,true,true,false,true,true,true,true,true,true,true,false,true,true,false,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,true,true,false,false,true,true,true,false,true,true,true,true,true,true,true,false,true,true,false,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,true,},},{{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,true,true,false,false,false,false,false,false,true,false,false,false,false,true,true,true,false,true,true,true,true,true,true,true,true,true,true,true,true,false,true,true,false,true,false,true,false,true,false,true,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,true,},},{{false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,true,true,false,false,true,true,true,false,true,true,true,true,true,true,true,false,true,true,false,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,true,true,false,false,true,true,true,false,true,true,true,true,true,true,true,false,true,true,false,false,false,true,false,true,false,true,},},{{false,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,true,true,false,false,true,true,true,false,true,true,true,true,true,true,true,false,true,true,false,false,false,true,false,true,false,true,},},{{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,true,true,false,false,false,false,false,false,true,false,false,false,false,true,true,true,false,true,true,true,true,true,true,true,true,true,true,true,true,false,true,true,false,true,false,true,false,true,false,true,},},{{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,true,true,false,false,false,false,false,false,true,false,false,false,false,true,true,true,false,true,true,true,true,true,true,true,true,true,true,true,true,false,true,true,false,true,false,true,false,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,true,true,true,true,true,true,true,false,false,true,false,false,false,true,false,true,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,true,true,true,true,true,true,true,false,false,true,false,false,false,true,false,true,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,true,true,true,false,false,false,false,false,false,false,true,false,false,false,false,true,false,false,},{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,true,true,true,false,false,false,false,false,false,false,true,false,false,false,false,true,false,false,},{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,true,false,false,true,true,true,false,false,false,true,true,true,false,false,false,true,true,true,false,true,true,true,false,false,false,false,false,true,false,true,true,true,false,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,true,false,false,true,true,true,false,false,false,true,true,true,false,false,false,true,true,true,false,true,true,true,false,false,false,false,false,true,false,true,true,true,false,true,true,false,true,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,false,false,false,true,false,false,true,true,true,false,false,false,true,true,true,false,false,true,true,true,true,true,true,true,true,false,false,false,false,false,true,false,true,true,true,false,true,true,false,true,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,false,false,false,true,false,false,true,true,true,false,false,false,true,true,true,false,false,true,true,true,true,true,true,true,true,false,false,false,false,false,true,false,true,true,true,false,true,true,false,true,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,false,false,false,true,false,false,true,true,true,false,false,false,true,true,true,false,false,true,true,true,true,true,true,true,true,false,false,false,false,false,true,false,true,true,true,false,true,true,false,true,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,true,false,false,true,true,true,false,false,false,true,true,true,false,false,false,true,true,true,false,true,true,true,false,false,false,false,false,true,false,true,true,true,false,true,true,false,true,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,false,false,false,true,false,false,true,true,true,false,false,false,true,true,true,false,false,true,true,true,true,true,true,true,true,false,false,false,false,false,true,false,true,true,true,false,true,true,false,true,},},{{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,false,false,false,true,false,false,true,true,true,false,false,false,true,true,true,false,false,true,true,true,true,true,true,true,true,false,false,false,false,false,true,false,true,true,true,false,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,true,false,true,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,true,false,true,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,true,true,false,true,false,false,true,true,true,false,false,false,true,true,true,true,false,true,true,true,true,true,true,true,true,true,true,true,true,false,true,true,true,true,true,true,true,true,false,true,},},{{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,true,false,false,false,false,true,true,false,true,false,false,true,true,true,false,false,false,true,true,true,true,false,true,true,true,true,true,true,true,true,true,true,true,true,false,true,true,true,true,true,true,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,true,false,false,true,true,true,false,false,false,true,true,true,false,false,false,true,true,true,false,true,true,true,false,false,false,false,false,true,false,true,true,true,false,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,true,false,false,true,true,true,false,false,false,true,true,true,false,false,false,true,true,true,false,true,true,true,false,false,false,false,false,true,false,true,true,true,false,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,true,false,true,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,true,false,true,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,true,true,true,false,true,false,true,true,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,true,true,true,false,true,false,true,true,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,true,false,false,true,true,true,false,false,false,true,true,true,false,false,false,true,true,true,false,true,true,true,false,false,false,false,false,true,false,true,true,true,false,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,true,false,false,true,true,true,false,false,false,true,true,true,false,false,false,true,true,true,false,true,true,true,false,false,false,false,false,true,false,true,true,true,false,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,true,false,false,true,true,true,false,false,false,true,true,true,false,false,false,true,true,true,false,true,true,true,false,false,false,false,false,true,false,true,true,true,false,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,true,false,false,false,false,false,false,false,true,false,false,true,true,true,false,false,false,true,true,true,false,false,false,true,true,true,false,true,true,true,false,false,false,false,false,true,false,true,true,true,false,true,true,false,true,},},{{false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,true,true,true,false,true,false,true,true,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,true,true,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,true,false,true,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},{{false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,},},};
void  FillComparator_0(Operator& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(Tokenizer.Peek(0).Type!=26)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Comparator","less");
        
    }
    ReturnValue.Op = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
void  FillComparator_1(Operator& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(Tokenizer.Peek(0).Type!=43)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Comparator","add");
        
    }
    ReturnValue.Op = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
void  FillComparator_2(Operator& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(Tokenizer.Peek(0).Type!=29)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Comparator","great");
        
    }
    ReturnValue.Op = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
void  FillComparator_3(Operator& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(Tokenizer.Peek(0).Type!=38)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Comparator","eq");
        
    }
    ReturnValue.Op = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
void  FillComparator_4(Operator& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(Tokenizer.Peek(0).Type!=25)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Comparator","leq");
        
    }
    ReturnValue.Op = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
void  FillComparator_5(Operator& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(Tokenizer.Peek(0).Type!=28)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Comparator","geq");
        
    }
    ReturnValue.Op = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
void  FillComparator_6(Operator& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(Tokenizer.Peek(0).Type!=30)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Comparator","neq");
        
    }
    ReturnValue.Op = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
void  FillComparator(Operator& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(LOOKTable[1][0][Tokenizer.Peek(0).Type] && LOOKTable[1][1][Tokenizer.Peek(1).Type])
    {
        FillComparator_0(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[2][0][Tokenizer.Peek(0).Type] && LOOKTable[2][1][Tokenizer.Peek(1).Type])
    {
        FillComparator_1(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[3][0][Tokenizer.Peek(0).Type] && LOOKTable[3][1][Tokenizer.Peek(1).Type])
    {
        FillComparator_2(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[4][0][Tokenizer.Peek(0).Type] && LOOKTable[4][1][Tokenizer.Peek(1).Type])
    {
        FillComparator_3(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[5][0][Tokenizer.Peek(0).Type] && LOOKTable[5][1][Tokenizer.Peek(1).Type])
    {
        FillComparator_4(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[6][0][Tokenizer.Peek(0).Type] && LOOKTable[6][1][Tokenizer.Peek(1).Type])
    {
        FillComparator_5(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[7][0][Tokenizer.Peek(0).Type] && LOOKTable[7][1][Tokenizer.Peek(1).Type])
    {
        FillComparator_6(ReturnValue,Tokenizer);
        
    }
    else 
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Comparator","Comparator");
        
    }
    
}
Operator ParseComparator(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    FillComparator(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillExprOperator_0(Operator& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(Tokenizer.Peek(0).Type!=31)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ExprOperator","plus");
        
    }
    ReturnValue.Op = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
void  FillExprOperator_1(Operator& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(Tokenizer.Peek(0).Type!=32)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ExprOperator","minus");
        
    }
    ReturnValue.Op = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
void  FillExprOperator_2(Operator& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(Tokenizer.Peek(0).Type!=33)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ExprOperator","mul");
        
    }
    ReturnValue.Op = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
void  FillExprOperator_3(Operator& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(Tokenizer.Peek(0).Type!=34)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ExprOperator","div");
        
    }
    ReturnValue.Op = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
void  FillExprOperator_4(Operator& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(Tokenizer.Peek(0).Type!=26)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ExprOperator","less");
        
    }
    ReturnValue.Op = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
void  FillExprOperator_5(Operator& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(Tokenizer.Peek(0).Type!=29)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ExprOperator","great");
        
    }
    ReturnValue.Op = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
void  FillExprOperator_6(Operator& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(Tokenizer.Peek(0).Type!=37)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ExprOperator","eqq");
        
    }
    ReturnValue.Op = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
void  FillExprOperator_7(Operator& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(Tokenizer.Peek(0).Type!=25)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ExprOperator","leq");
        
    }
    ReturnValue.Op = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
void  FillExprOperator_8(Operator& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(Tokenizer.Peek(0).Type!=28)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ExprOperator","geq");
        
    }
    ReturnValue.Op = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
void  FillExprOperator_9(Operator& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(Tokenizer.Peek(0).Type!=30)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ExprOperator","neq");
        
    }
    ReturnValue.Op = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
void  FillExprOperator_10(Operator& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(Tokenizer.Peek(0).Type!=41)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ExprOperator","pipe");
        
    }
    ReturnValue.Op = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
void  FillExprOperator_11(Operator& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(Tokenizer.Peek(0).Type!=43)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ExprOperator","add");
        
    }
    ReturnValue.Op = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
void  FillExprOperator(Operator& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(LOOKTable[9][0][Tokenizer.Peek(0).Type] && LOOKTable[9][1][Tokenizer.Peek(1).Type])
    {
        FillExprOperator_0(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[10][0][Tokenizer.Peek(0).Type] && LOOKTable[10][1][Tokenizer.Peek(1).Type])
    {
        FillExprOperator_1(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[11][0][Tokenizer.Peek(0).Type] && LOOKTable[11][1][Tokenizer.Peek(1).Type])
    {
        FillExprOperator_2(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[12][0][Tokenizer.Peek(0).Type] && LOOKTable[12][1][Tokenizer.Peek(1).Type])
    {
        FillExprOperator_3(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[13][0][Tokenizer.Peek(0).Type] && LOOKTable[13][1][Tokenizer.Peek(1).Type])
    {
        FillExprOperator_4(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[14][0][Tokenizer.Peek(0).Type] && LOOKTable[14][1][Tokenizer.Peek(1).Type])
    {
        FillExprOperator_5(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[15][0][Tokenizer.Peek(0).Type] && LOOKTable[15][1][Tokenizer.Peek(1).Type])
    {
        FillExprOperator_6(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[16][0][Tokenizer.Peek(0).Type] && LOOKTable[16][1][Tokenizer.Peek(1).Type])
    {
        FillExprOperator_7(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[17][0][Tokenizer.Peek(0).Type] && LOOKTable[17][1][Tokenizer.Peek(1).Type])
    {
        FillExprOperator_8(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[18][0][Tokenizer.Peek(0).Type] && LOOKTable[18][1][Tokenizer.Peek(1).Type])
    {
        FillExprOperator_9(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[19][0][Tokenizer.Peek(0).Type] && LOOKTable[19][1][Tokenizer.Peek(1).Type])
    {
        FillExprOperator_10(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[20][0][Tokenizer.Peek(0).Type] && LOOKTable[20][1][Tokenizer.Peek(1).Type])
    {
        FillExprOperator_11(ReturnValue,Tokenizer);
        
    }
    else 
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ExprOperator","ExprOperator");
        
    }
    
}
Operator ParseExprOperator(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    FillExprOperator(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillCombinator_0(Operator& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(Tokenizer.Peek(0).Type!=41)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Combinator","pipe");
        
    }
    ReturnValue.Op = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
void  FillCombinator_1(Operator& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(Tokenizer.Peek(0).Type!=42)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Combinator","AND");
        
    }
    ReturnValue.Op = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
void  FillCombinator_2(Operator& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(Tokenizer.Peek(0).Type!=40)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Combinator","OR");
        
    }
    ReturnValue.Op = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
void  FillCombinator(Operator& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(LOOKTable[22][0][Tokenizer.Peek(0).Type] && LOOKTable[22][1][Tokenizer.Peek(1).Type])
    {
        FillCombinator_0(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[23][0][Tokenizer.Peek(0).Type] && LOOKTable[23][1][Tokenizer.Peek(1).Type])
    {
        FillCombinator_1(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[24][0][Tokenizer.Peek(0).Type] && LOOKTable[24][1][Tokenizer.Peek(1).Type])
    {
        FillCombinator_2(ReturnValue,Tokenizer);
        
    }
    else 
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Combinator","Combinator");
        
    }
    
}
Operator ParseCombinator(MBCC::Tokenizer& Tokenizer)
{
    Operator ReturnValue;
    FillCombinator(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillFilter_Arg_0(Filter_Arg& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(!(LOOKTable[28][0][Tokenizer.Peek(0).Type] && LOOKTable[28][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg","Filter_Arg_Named");
        
    }
    FillFilter_Arg_Named(ReturnValue.GetOrAssign<Filter_Arg_Named>(),Tokenizer);
    
}
void  FillFilter_Arg_1(Filter_Arg& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(!(LOOKTable[30][0][Tokenizer.Peek(0).Type] && LOOKTable[30][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg","Filter_Arg_Positional");
        
    }
    FillFilter_Arg_Positional(ReturnValue.GetOrAssign<Filter_Arg_Positional>(),Tokenizer);
    
}
void  FillFilter_Arg(Filter_Arg& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(LOOKTable[26][0][Tokenizer.Peek(0).Type] && LOOKTable[26][1][Tokenizer.Peek(1).Type])
    {
        FillFilter_Arg_0(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[27][0][Tokenizer.Peek(0).Type] && LOOKTable[27][1][Tokenizer.Peek(1).Type])
    {
        FillFilter_Arg_1(ReturnValue,Tokenizer);
        
    }
    else 
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg","Filter_Arg");
        
    }
    
}
Filter_Arg ParseFilter_Arg(MBCC::Tokenizer& Tokenizer)
{
    Filter_Arg ReturnValue;
    FillFilter_Arg(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillFilter_Arg_Named_0(Filter_Arg_Named& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.NamePosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=23)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg_Named","idf");
        
    }
    ReturnValue.Name = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    if(Tokenizer.Peek(0).Type!=38)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg_Named","eq");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[63][0][Tokenizer.Peek(0).Type] && LOOKTable[63][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg_Named","Filter_Component");
        
    }
    FillFilter_Component(ReturnValue.Argument.GetOrAssign<Filter_OperatorList>(),Tokenizer);
    
}
void  FillFilter_Arg_Named(Filter_Arg_Named& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.NamePosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=23)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg_Named","idf");
        
    }
    ReturnValue.Name = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    if(Tokenizer.Peek(0).Type!=38)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg_Named","eq");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[63][0][Tokenizer.Peek(0).Type] && LOOKTable[63][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg_Named","Filter_Component");
        
    }
    FillFilter_Component(ReturnValue.Argument.GetOrAssign<Filter_OperatorList>(),Tokenizer);
    
}
Filter_Arg_Named ParseFilter_Arg_Named(MBCC::Tokenizer& Tokenizer)
{
    Filter_Arg_Named ReturnValue;
    FillFilter_Arg_Named(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillFilter_Arg_Positional_0(Filter_Arg_Positional& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(!(LOOKTable[63][0][Tokenizer.Peek(0).Type] && LOOKTable[63][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg_Positional","Filter_Component");
        
    }
    FillFilter_Component(ReturnValue.Argument.GetOrAssign<Filter_OperatorList>(),Tokenizer);
    
}
void  FillFilter_Arg_Positional(Filter_Arg_Positional& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(!(LOOKTable[63][0][Tokenizer.Peek(0).Type] && LOOKTable[63][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Arg_Positional","Filter_Component");
        
    }
    FillFilter_Component(ReturnValue.Argument.GetOrAssign<Filter_OperatorList>(),Tokenizer);
    
}
Filter_Arg_Positional ParseFilter_Arg_Positional(MBCC::Tokenizer& Tokenizer)
{
    Filter_Arg_Positional ReturnValue;
    FillFilter_Arg_Positional(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillFilter_ArgList_0(Filter_ArgList& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(Tokenizer.Peek(0).Type!=0)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_ArgList","lpar");
        
    }
    Tokenizer.ConsumeToken();
    if(LOOKTable[36][0][Tokenizer.Peek(0).Type] && LOOKTable[36][1][Tokenizer.Peek(1).Type])
    {
        if(LOOKTable[37][0][Tokenizer.Peek(0).Type] && LOOKTable[37][1][Tokenizer.Peek(1).Type])
        {
            if(!(LOOKTable[25][0][Tokenizer.Peek(0).Type] && LOOKTable[25][1][Tokenizer.Peek(1).Type]))
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L2","Filter_Arg");
                
            }
            ReturnValue.Arguments.push_back(Filter_Arg());
            FillFilter_Arg(ReturnValue.Arguments.back(),Tokenizer);
            while(LOOKTable[34][0][Tokenizer.Peek(0).Type] && LOOKTable[34][1][Tokenizer.Peek(1).Type])
            {
                if(LOOKTable[35][0][Tokenizer.Peek(0).Type] && LOOKTable[35][1][Tokenizer.Peek(1).Type])
                {
                    if(Tokenizer.Peek(0).Type!=36)
                    {
                        throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L1","comma");
                        
                    }
                    Tokenizer.ConsumeToken();
                    if(!(LOOKTable[25][0][Tokenizer.Peek(0).Type] && LOOKTable[25][1][Tokenizer.Peek(1).Type]))
                    {
                        throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L1","Filter_Arg");
                        
                    }
                    ReturnValue.Arguments.push_back(Filter_Arg());
                    FillFilter_Arg(ReturnValue.Arguments.back(),Tokenizer);
                    
                }
                else 
                {
                    throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L2","_L1");
                    
                }
                
            }
            
        }
        else 
        {
            throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_ArgList","_L2");
            
        }
        
    }
    if(Tokenizer.Peek(0).Type!=1)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_ArgList","rpar");
        
    }
    Tokenizer.ConsumeToken();
    
}
void  FillFilter_ArgList(Filter_ArgList& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(Tokenizer.Peek(0).Type!=0)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_ArgList","lpar");
        
    }
    Tokenizer.ConsumeToken();
    if(LOOKTable[36][0][Tokenizer.Peek(0).Type] && LOOKTable[36][1][Tokenizer.Peek(1).Type])
    {
        if(LOOKTable[37][0][Tokenizer.Peek(0).Type] && LOOKTable[37][1][Tokenizer.Peek(1).Type])
        {
            if(!(LOOKTable[25][0][Tokenizer.Peek(0).Type] && LOOKTable[25][1][Tokenizer.Peek(1).Type]))
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L2","Filter_Arg");
                
            }
            ReturnValue.Arguments.push_back(Filter_Arg());
            FillFilter_Arg(ReturnValue.Arguments.back(),Tokenizer);
            while(LOOKTable[34][0][Tokenizer.Peek(0).Type] && LOOKTable[34][1][Tokenizer.Peek(1).Type])
            {
                if(LOOKTable[35][0][Tokenizer.Peek(0).Type] && LOOKTable[35][1][Tokenizer.Peek(1).Type])
                {
                    if(Tokenizer.Peek(0).Type!=36)
                    {
                        throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L1","comma");
                        
                    }
                    Tokenizer.ConsumeToken();
                    if(!(LOOKTable[25][0][Tokenizer.Peek(0).Type] && LOOKTable[25][1][Tokenizer.Peek(1).Type]))
                    {
                        throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L1","Filter_Arg");
                        
                    }
                    ReturnValue.Arguments.push_back(Filter_Arg());
                    FillFilter_Arg(ReturnValue.Arguments.back(),Tokenizer);
                    
                }
                else 
                {
                    throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L2","_L1");
                    
                }
                
            }
            
        }
        else 
        {
            throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_ArgList","_L2");
            
        }
        
    }
    if(Tokenizer.Peek(0).Type!=1)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_ArgList","rpar");
        
    }
    Tokenizer.ConsumeToken();
    
}
Filter_ArgList ParseFilter_ArgList(MBCC::Tokenizer& Tokenizer)
{
    Filter_ArgList ReturnValue;
    FillFilter_ArgList(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillLiteral_0(Literal& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(!(LOOKTable[42][0][Tokenizer.Peek(0).Type] && LOOKTable[42][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Literal","Literal_String");
        
    }
    FillLiteral_String(ReturnValue.GetOrAssign<Literal_String>(),Tokenizer);
    
}
void  FillLiteral_1(Literal& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(!(LOOKTable[44][0][Tokenizer.Peek(0).Type] && LOOKTable[44][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Literal","Literal_Symbol");
        
    }
    FillLiteral_Symbol(ReturnValue.GetOrAssign<Literal_Symbol>(),Tokenizer);
    
}
void  FillLiteral_2(Literal& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(!(LOOKTable[46][0][Tokenizer.Peek(0).Type] && LOOKTable[46][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Literal","Literal_Number");
        
    }
    FillLiteral_Number(ReturnValue.GetOrAssign<Literal_Number>(),Tokenizer);
    
}
void  FillLiteral(Literal& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(LOOKTable[39][0][Tokenizer.Peek(0).Type] && LOOKTable[39][1][Tokenizer.Peek(1).Type])
    {
        FillLiteral_0(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[40][0][Tokenizer.Peek(0).Type] && LOOKTable[40][1][Tokenizer.Peek(1).Type])
    {
        FillLiteral_1(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[41][0][Tokenizer.Peek(0).Type] && LOOKTable[41][1][Tokenizer.Peek(1).Type])
    {
        FillLiteral_2(ReturnValue,Tokenizer);
        
    }
    else 
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Literal","Literal");
        
    }
    
}
Literal ParseLiteral(MBCC::Tokenizer& Tokenizer)
{
    Literal ReturnValue;
    FillLiteral(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillLiteral_String_0(Literal_String& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.ValuePosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=7)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Literal_String","string");
        
    }
    ReturnValue.Value = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
void  FillLiteral_String(Literal_String& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.ValuePosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=7)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Literal_String","string");
        
    }
    ReturnValue.Value = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
Literal_String ParseLiteral_String(MBCC::Tokenizer& Tokenizer)
{
    Literal_String ReturnValue;
    FillLiteral_String(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillLiteral_Symbol_0(Literal_Symbol& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.ValuePosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=23)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Literal_Symbol","idf");
        
    }
    ReturnValue.Value = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
void  FillLiteral_Symbol(Literal_Symbol& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.ValuePosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=23)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Literal_Symbol","idf");
        
    }
    ReturnValue.Value = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
Literal_Symbol ParseLiteral_Symbol(MBCC::Tokenizer& Tokenizer)
{
    Literal_Symbol ReturnValue;
    FillLiteral_Symbol(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillLiteral_Number_0(Literal_Number& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.ValuePosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=6)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Literal_Number","number");
        
    }
    ReturnValue.Value = std::stoi(Tokenizer.Peek(0).Value);
    Tokenizer.ConsumeToken();
    
}
void  FillLiteral_Number(Literal_Number& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.ValuePosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=6)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Literal_Number","number");
        
    }
    ReturnValue.Value = std::stoi(Tokenizer.Peek(0).Value);
    Tokenizer.ConsumeToken();
    
}
Literal_Number ParseLiteral_Number(MBCC::Tokenizer& Tokenizer)
{
    Literal_Number ReturnValue;
    FillLiteral_Number(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillIdentifier2_0(Identifier& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(!(LOOKTable[52][0][Tokenizer.Peek(0).Type] && LOOKTable[52][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Identifier2","Token2");
        
    }
    ReturnValue.Parts.push_back(Token());
    FillToken2(ReturnValue.Parts.back(),Tokenizer);
    while(LOOKTable[50][0][Tokenizer.Peek(0).Type] && LOOKTable[50][1][Tokenizer.Peek(1).Type])
    {
        if(LOOKTable[51][0][Tokenizer.Peek(0).Type] && LOOKTable[51][1][Tokenizer.Peek(1).Type])
        {
            if(Tokenizer.Peek(0).Type!=39)
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L3","dot");
                
            }
            Tokenizer.ConsumeToken();
            if(!(LOOKTable[52][0][Tokenizer.Peek(0).Type] && LOOKTable[52][1][Tokenizer.Peek(1).Type]))
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L3","Token2");
                
            }
            ReturnValue.Parts.push_back(Token());
            FillToken2(ReturnValue.Parts.back(),Tokenizer);
            
        }
        else 
        {
            throw MBCC::ParsingException(Tokenizer.Peek().Position,"Identifier2","_L3");
            
        }
        
    }
    
}
void  FillIdentifier2(Identifier& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(!(LOOKTable[52][0][Tokenizer.Peek(0).Type] && LOOKTable[52][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Identifier2","Token2");
        
    }
    ReturnValue.Parts.push_back(Token());
    FillToken2(ReturnValue.Parts.back(),Tokenizer);
    while(LOOKTable[50][0][Tokenizer.Peek(0).Type] && LOOKTable[50][1][Tokenizer.Peek(1).Type])
    {
        if(LOOKTable[51][0][Tokenizer.Peek(0).Type] && LOOKTable[51][1][Tokenizer.Peek(1).Type])
        {
            if(Tokenizer.Peek(0).Type!=39)
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L3","dot");
                
            }
            Tokenizer.ConsumeToken();
            if(!(LOOKTable[52][0][Tokenizer.Peek(0).Type] && LOOKTable[52][1][Tokenizer.Peek(1).Type]))
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L3","Token2");
                
            }
            ReturnValue.Parts.push_back(Token());
            FillToken2(ReturnValue.Parts.back(),Tokenizer);
            
        }
        else 
        {
            throw MBCC::ParsingException(Tokenizer.Peek().Position,"Identifier2","_L3");
            
        }
        
    }
    
}
Identifier ParseIdentifier2(MBCC::Tokenizer& Tokenizer)
{
    Identifier ReturnValue;
    FillIdentifier2(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillToken2_0(Token& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.Position = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=23)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Token2","idf");
        
    }
    ReturnValue.Value = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
void  FillToken2(Token& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.Position = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=23)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Token2","idf");
        
    }
    ReturnValue.Value = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
Token ParseToken2(MBCC::Tokenizer& Tokenizer)
{
    Token ReturnValue;
    FillToken2(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillFilter_Component_Func_0(Filter_Component_Func& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.Begin = Tokenizer.Peek(0).Position;
    ReturnValue.NamePosition = Tokenizer.Peek(0).Position;
    if(!(LOOKTable[48][0][Tokenizer.Peek(0).Type] && LOOKTable[48][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component_Func","Identifier2");
        
    }
    FillIdentifier2(ReturnValue.FilterName,Tokenizer);
    if(!(LOOKTable[32][0][Tokenizer.Peek(0).Type] && LOOKTable[32][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component_Func","Filter_ArgList");
        
    }
    FillFilter_ArgList(ReturnValue.ArgumentList,Tokenizer);
    ReturnValue.End = Tokenizer.Peek(0).Position;
    
}
void  FillFilter_Component_Func(Filter_Component_Func& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.Begin = Tokenizer.Peek(0).Position;
    ReturnValue.NamePosition = Tokenizer.Peek(0).Position;
    if(!(LOOKTable[48][0][Tokenizer.Peek(0).Type] && LOOKTable[48][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component_Func","Identifier2");
        
    }
    FillIdentifier2(ReturnValue.FilterName,Tokenizer);
    if(!(LOOKTable[32][0][Tokenizer.Peek(0).Type] && LOOKTable[32][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component_Func","Filter_ArgList");
        
    }
    FillFilter_ArgList(ReturnValue.ArgumentList,Tokenizer);
    ReturnValue.End = Tokenizer.Peek(0).Position;
    
}
Filter_Component_Func ParseFilter_Component_Func(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component_Func ReturnValue;
    FillFilter_Component_Func(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillFilter_Term_0(Filter_Component& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(Tokenizer.Peek(0).Type!=0)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Term","lpar");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[63][0][Tokenizer.Peek(0).Type] && LOOKTable[63][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Term","Filter_Component");
        
    }
    FillFilter_Component(ReturnValue.GetOrAssign<Filter_OperatorList>(),Tokenizer);
    if(Tokenizer.Peek(0).Type!=1)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Term","rpar");
        
    }
    Tokenizer.ConsumeToken();
    
}
void  FillFilter_Term_1(Filter_Component& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(!(LOOKTable[54][0][Tokenizer.Peek(0).Type] && LOOKTable[54][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Term","Filter_Component_Func");
        
    }
    FillFilter_Component_Func(ReturnValue.GetOrAssign<Filter_Component_Func>(),Tokenizer);
    
}
void  FillFilter_Term_2(Filter_Component& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(!(LOOKTable[61][0][Tokenizer.Peek(0).Type] && LOOKTable[61][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Term","Filter_Component_Literal");
        
    }
    FillFilter_Component_Literal(ReturnValue.GetOrAssign<Filter_Component_Literal>(),Tokenizer);
    
}
void  FillFilter_Term_3(Filter_Component& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    bool i_Negated;
    if(Tokenizer.Peek(0).Type!=27)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Term","bang");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[56][0][Tokenizer.Peek(0).Type] && LOOKTable[56][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Term","Filter_Term");
        
    }
    FillFilter_Term(ReturnValue,Tokenizer);
    i_Negated = true;
    ReturnValue.GetBase().Negated = i_Negated;
    
}
void  FillFilter_Term(Filter_Component& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(LOOKTable[57][0][Tokenizer.Peek(0).Type] && LOOKTable[57][1][Tokenizer.Peek(1).Type])
    {
        FillFilter_Term_0(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[58][0][Tokenizer.Peek(0).Type] && LOOKTable[58][1][Tokenizer.Peek(1).Type])
    {
        FillFilter_Term_1(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[59][0][Tokenizer.Peek(0).Type] && LOOKTable[59][1][Tokenizer.Peek(1).Type])
    {
        FillFilter_Term_2(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[60][0][Tokenizer.Peek(0).Type] && LOOKTable[60][1][Tokenizer.Peek(1).Type])
    {
        FillFilter_Term_3(ReturnValue,Tokenizer);
        
    }
    else 
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Term","Filter_Term");
        
    }
    
}
Filter_Component ParseFilter_Term(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component ReturnValue;
    FillFilter_Term(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillFilter_Component_Literal_0(Filter_Component_Literal& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(!(LOOKTable[38][0][Tokenizer.Peek(0).Type] && LOOKTable[38][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component_Literal","Literal");
        
    }
    FillLiteral(ReturnValue.Value,Tokenizer);
    
}
void  FillFilter_Component_Literal(Filter_Component_Literal& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(!(LOOKTable[38][0][Tokenizer.Peek(0).Type] && LOOKTable[38][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component_Literal","Literal");
        
    }
    FillLiteral(ReturnValue.Value,Tokenizer);
    
}
Filter_Component_Literal ParseFilter_Component_Literal(MBCC::Tokenizer& Tokenizer)
{
    Filter_Component_Literal ReturnValue;
    FillFilter_Component_Literal(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillFilter_Component_0(Filter_OperatorList& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.Begin = Tokenizer.Peek(0).Position;
    if(!(LOOKTable[56][0][Tokenizer.Peek(0).Type] && LOOKTable[56][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component","Filter_Term");
        
    }
    ReturnValue.Components.push_back(Filter_Component());
    FillFilter_Term(ReturnValue.Components.back(),Tokenizer);
    while(LOOKTable[65][0][Tokenizer.Peek(0).Type] && LOOKTable[65][1][Tokenizer.Peek(1).Type])
    {
        if(LOOKTable[66][0][Tokenizer.Peek(0).Type] && LOOKTable[66][1][Tokenizer.Peek(1).Type])
        {
            if(!(LOOKTable[8][0][Tokenizer.Peek(0).Type] && LOOKTable[8][1][Tokenizer.Peek(1).Type]))
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L4","ExprOperator");
                
            }
            ReturnValue.Operators.push_back(ParseExprOperator(Tokenizer).Op);
            if(!(LOOKTable[56][0][Tokenizer.Peek(0).Type] && LOOKTable[56][1][Tokenizer.Peek(1).Type]))
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L4","Filter_Term");
                
            }
            ReturnValue.Components.push_back(Filter_Component());
            FillFilter_Term(ReturnValue.Components.back(),Tokenizer);
            
        }
        else 
        {
            throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component","_L4");
            
        }
        
    }
    ReturnValue.End = Tokenizer.Peek(0).Position;
    
}
void  FillFilter_Component(Filter_OperatorList& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.Begin = Tokenizer.Peek(0).Position;
    if(!(LOOKTable[56][0][Tokenizer.Peek(0).Type] && LOOKTable[56][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component","Filter_Term");
        
    }
    ReturnValue.Components.push_back(Filter_Component());
    FillFilter_Term(ReturnValue.Components.back(),Tokenizer);
    while(LOOKTable[65][0][Tokenizer.Peek(0).Type] && LOOKTable[65][1][Tokenizer.Peek(1).Type])
    {
        if(LOOKTable[66][0][Tokenizer.Peek(0).Type] && LOOKTable[66][1][Tokenizer.Peek(1).Type])
        {
            if(!(LOOKTable[8][0][Tokenizer.Peek(0).Type] && LOOKTable[8][1][Tokenizer.Peek(1).Type]))
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L4","ExprOperator");
                
            }
            ReturnValue.Operators.push_back(ParseExprOperator(Tokenizer).Op);
            if(!(LOOKTable[56][0][Tokenizer.Peek(0).Type] && LOOKTable[56][1][Tokenizer.Peek(1).Type]))
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L4","Filter_Term");
                
            }
            ReturnValue.Components.push_back(Filter_Component());
            FillFilter_Term(ReturnValue.Components.back(),Tokenizer);
            
        }
        else 
        {
            throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter_Component","_L4");
            
        }
        
    }
    ReturnValue.End = Tokenizer.Peek(0).Position;
    
}
Filter_OperatorList ParseFilter_Component(MBCC::Tokenizer& Tokenizer)
{
    Filter_OperatorList ReturnValue;
    FillFilter_Component(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillFilter_0(Filter& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.FilterPosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=18)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter","FILTER");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[63][0][Tokenizer.Peek(0).Type] && LOOKTable[63][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter","Filter_Component");
        
    }
    FillFilter_Component(ReturnValue.Component.GetOrAssign<Filter_OperatorList>(),Tokenizer);
    
}
void  FillFilter(Filter& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.FilterPosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=18)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter","FILTER");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[63][0][Tokenizer.Peek(0).Type] && LOOKTable[63][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Filter","Filter_Component");
        
    }
    FillFilter_Component(ReturnValue.Component.GetOrAssign<Filter_OperatorList>(),Tokenizer);
    
}
Filter ParseFilter(MBCC::Tokenizer& Tokenizer)
{
    Filter ReturnValue;
    FillFilter(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillGameInfoPredicate_Or_0(GameInfoPredicate& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    std::string i_Operator;
    if(Tokenizer.Peek(0).Type!=40)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Or","OR");
        
    }
    i_Operator = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[77][0][Tokenizer.Peek(0).Type] && LOOKTable[77][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Or","GameInfoPredicateOr");
        
    }
    FillGameInfoPredicateOr(ReturnValue,Tokenizer);
    ReturnValue.Operator = i_Operator;
    
}
void  FillGameInfoPredicate_Or(GameInfoPredicate& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    std::string i_Operator;
    if(Tokenizer.Peek(0).Type!=40)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Or","OR");
        
    }
    i_Operator = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[77][0][Tokenizer.Peek(0).Type] && LOOKTable[77][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Or","GameInfoPredicateOr");
        
    }
    FillGameInfoPredicateOr(ReturnValue,Tokenizer);
    ReturnValue.Operator = i_Operator;
    
}
GameInfoPredicate ParseGameInfoPredicate_Or(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    FillGameInfoPredicate_Or(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillGameInfoPredicate_And_0(GameInfoPredicate& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    std::string i_Operator;
    if(Tokenizer.Peek(0).Type!=42)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_And","AND");
        
    }
    i_Operator = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[83][0][Tokenizer.Peek(0).Type] && LOOKTable[83][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_And","GameInfoPredicate");
        
    }
    FillGameInfoPredicate(ReturnValue,Tokenizer);
    ReturnValue.Operator = i_Operator;
    
}
void  FillGameInfoPredicate_And(GameInfoPredicate& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    std::string i_Operator;
    if(Tokenizer.Peek(0).Type!=42)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_And","AND");
        
    }
    i_Operator = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[83][0][Tokenizer.Peek(0).Type] && LOOKTable[83][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_And","GameInfoPredicate");
        
    }
    FillGameInfoPredicate(ReturnValue,Tokenizer);
    ReturnValue.Operator = i_Operator;
    
}
GameInfoPredicate ParseGameInfoPredicate_And(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    FillGameInfoPredicate_And(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillGameInfoPredicate_Direct_0(GameInfoPredicate_Direct& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(!(LOOKTable[0][0][Tokenizer.Peek(0).Type] && LOOKTable[0][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Direct","Comparator");
        
    }
    ReturnValue.Comparison = ParseComparator(Tokenizer).Op;
    ReturnValue.ValuePosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=7)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Direct","string");
        
    }
    ReturnValue.Value = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
void  FillGameInfoPredicate_Direct(GameInfoPredicate_Direct& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(!(LOOKTable[0][0][Tokenizer.Peek(0).Type] && LOOKTable[0][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Direct","Comparator");
        
    }
    ReturnValue.Comparison = ParseComparator(Tokenizer).Op;
    ReturnValue.ValuePosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=7)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Direct","string");
        
    }
    ReturnValue.Value = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
GameInfoPredicate_Direct ParseGameInfoPredicate_Direct(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate_Direct ReturnValue;
    FillGameInfoPredicate_Direct(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillGameInfoPredicate_Base_0(GameInfoPredicate& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.Begin = Tokenizer.Peek(0).Position;
    if(!(LOOKTable[124][0][Tokenizer.Peek(0).Type] && LOOKTable[124][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Base","Identifier");
        
    }
    FillIdentifier(ReturnValue.Attribute,Tokenizer);
    if(LOOKTable[73][0][Tokenizer.Peek(0).Type] && LOOKTable[73][1][Tokenizer.Peek(1).Type])
    {
        FillGameInfoPredicate_Direct(ReturnValue.Data.GetOrAssign<GameInfoPredicate_Direct>(),Tokenizer);
        
    }
    ReturnValue.End = Tokenizer.Peek(0).Position;
    
}
void  FillGameInfoPredicate_Base(GameInfoPredicate& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.Begin = Tokenizer.Peek(0).Position;
    if(!(LOOKTable[124][0][Tokenizer.Peek(0).Type] && LOOKTable[124][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Base","Identifier");
        
    }
    FillIdentifier(ReturnValue.Attribute,Tokenizer);
    if(LOOKTable[73][0][Tokenizer.Peek(0).Type] && LOOKTable[73][1][Tokenizer.Peek(1).Type])
    {
        FillGameInfoPredicate_Direct(ReturnValue.Data.GetOrAssign<GameInfoPredicate_Direct>(),Tokenizer);
        
    }
    ReturnValue.End = Tokenizer.Peek(0).Position;
    
}
GameInfoPredicate ParseGameInfoPredicate_Base(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    FillGameInfoPredicate_Base(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillGameInfoPredicateOr_0(GameInfoPredicate& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.Begin = Tokenizer.Peek(0).Position;
    ReturnValue.Operator = "||";
    if(!(LOOKTable[79][0][Tokenizer.Peek(0).Type] && LOOKTable[79][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicateOr","GameInfoPredicate_Term");
        
    }
    ReturnValue.ExtraTerms.push_back(GameInfoPredicate());
    FillGameInfoPredicate_Term(ReturnValue.ExtraTerms.back(),Tokenizer);
    while(LOOKTable[69][0][Tokenizer.Peek(0).Type] && LOOKTable[69][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.ExtraTerms.push_back(GameInfoPredicate());
        FillGameInfoPredicate_Or(ReturnValue.ExtraTerms.back(),Tokenizer);
        
    }
    ReturnValue.End = Tokenizer.Peek(0).Position;
    
}
void  FillGameInfoPredicateOr(GameInfoPredicate& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.Begin = Tokenizer.Peek(0).Position;
    ReturnValue.Operator = "||";
    if(!(LOOKTable[79][0][Tokenizer.Peek(0).Type] && LOOKTable[79][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicateOr","GameInfoPredicate_Term");
        
    }
    ReturnValue.ExtraTerms.push_back(GameInfoPredicate());
    FillGameInfoPredicate_Term(ReturnValue.ExtraTerms.back(),Tokenizer);
    while(LOOKTable[69][0][Tokenizer.Peek(0).Type] && LOOKTable[69][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.ExtraTerms.push_back(GameInfoPredicate());
        FillGameInfoPredicate_Or(ReturnValue.ExtraTerms.back(),Tokenizer);
        
    }
    ReturnValue.End = Tokenizer.Peek(0).Position;
    
}
GameInfoPredicate ParseGameInfoPredicateOr(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    FillGameInfoPredicateOr(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillGameInfoPredicate_Term_0(GameInfoPredicate& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(Tokenizer.Peek(0).Type!=0)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Term","lpar");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[83][0][Tokenizer.Peek(0).Type] && LOOKTable[83][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Term","GameInfoPredicate");
        
    }
    FillGameInfoPredicate(ReturnValue,Tokenizer);
    if(Tokenizer.Peek(0).Type!=1)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Term","rpar");
        
    }
    Tokenizer.ConsumeToken();
    
}
void  FillGameInfoPredicate_Term_1(GameInfoPredicate& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    bool i_Negated;
    if(Tokenizer.Peek(0).Type!=27)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Term","bang");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[79][0][Tokenizer.Peek(0).Type] && LOOKTable[79][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Term","GameInfoPredicate_Term");
        
    }
    FillGameInfoPredicate_Term(ReturnValue,Tokenizer);
    i_Negated = true;
    ReturnValue.Negated = i_Negated;
    
}
void  FillGameInfoPredicate_Term_2(GameInfoPredicate& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(!(LOOKTable[75][0][Tokenizer.Peek(0).Type] && LOOKTable[75][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Term","GameInfoPredicate_Base");
        
    }
    FillGameInfoPredicate_Base(ReturnValue,Tokenizer);
    
}
void  FillGameInfoPredicate_Term(GameInfoPredicate& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(LOOKTable[80][0][Tokenizer.Peek(0).Type] && LOOKTable[80][1][Tokenizer.Peek(1).Type])
    {
        FillGameInfoPredicate_Term_0(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[81][0][Tokenizer.Peek(0).Type] && LOOKTable[81][1][Tokenizer.Peek(1).Type])
    {
        FillGameInfoPredicate_Term_1(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[82][0][Tokenizer.Peek(0).Type] && LOOKTable[82][1][Tokenizer.Peek(1).Type])
    {
        FillGameInfoPredicate_Term_2(ReturnValue,Tokenizer);
        
    }
    else 
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate_Term","GameInfoPredicate_Term");
        
    }
    
}
GameInfoPredicate ParseGameInfoPredicate_Term(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    FillGameInfoPredicate_Term(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillGameInfoPredicate_0(GameInfoPredicate& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.Begin = Tokenizer.Peek(0).Position;
    ReturnValue.Operator = "&&";
    if(!(LOOKTable[77][0][Tokenizer.Peek(0).Type] && LOOKTable[77][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate","GameInfoPredicateOr");
        
    }
    ReturnValue.ExtraTerms.push_back(GameInfoPredicate());
    FillGameInfoPredicateOr(ReturnValue.ExtraTerms.back(),Tokenizer);
    while(LOOKTable[71][0][Tokenizer.Peek(0).Type] && LOOKTable[71][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.ExtraTerms.push_back(GameInfoPredicate());
        FillGameInfoPredicate_And(ReturnValue.ExtraTerms.back(),Tokenizer);
        
    }
    ReturnValue.End = Tokenizer.Peek(0).Position;
    
}
void  FillGameInfoPredicate(GameInfoPredicate& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.Begin = Tokenizer.Peek(0).Position;
    ReturnValue.Operator = "&&";
    if(!(LOOKTable[77][0][Tokenizer.Peek(0).Type] && LOOKTable[77][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameInfoPredicate","GameInfoPredicateOr");
        
    }
    ReturnValue.ExtraTerms.push_back(GameInfoPredicate());
    FillGameInfoPredicateOr(ReturnValue.ExtraTerms.back(),Tokenizer);
    while(LOOKTable[71][0][Tokenizer.Peek(0).Type] && LOOKTable[71][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.ExtraTerms.push_back(GameInfoPredicate());
        FillGameInfoPredicate_And(ReturnValue.ExtraTerms.back(),Tokenizer);
        
    }
    ReturnValue.End = Tokenizer.Peek(0).Position;
    
}
GameInfoPredicate ParseGameInfoPredicate(MBCC::Tokenizer& Tokenizer)
{
    GameInfoPredicate ReturnValue;
    FillGameInfoPredicate(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillGameSelection_0(GameSelection& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(LOOKTable[130][0][Tokenizer.Peek(0).Type] && LOOKTable[130][1][Tokenizer.Peek(1).Type])
    {
        FillUsingDirective(ReturnValue.Using,Tokenizer);
        
    }
    if(LOOKTable[87][0][Tokenizer.Peek(0).Type] && LOOKTable[87][1][Tokenizer.Peek(1).Type])
    {
        FillPlayerAssignment(ReturnValue.Assignment,Tokenizer);
        
    }
    ReturnValue.SelectPosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=13)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameSelection","SELECT");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[83][0][Tokenizer.Peek(0).Type] && LOOKTable[83][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameSelection","GameInfoPredicate");
        
    }
    FillGameInfoPredicate(ReturnValue.GameCondition,Tokenizer);
    
}
void  FillGameSelection(GameSelection& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(LOOKTable[130][0][Tokenizer.Peek(0).Type] && LOOKTable[130][1][Tokenizer.Peek(1).Type])
    {
        FillUsingDirective(ReturnValue.Using,Tokenizer);
        
    }
    if(LOOKTable[87][0][Tokenizer.Peek(0).Type] && LOOKTable[87][1][Tokenizer.Peek(1).Type])
    {
        FillPlayerAssignment(ReturnValue.Assignment,Tokenizer);
        
    }
    ReturnValue.SelectPosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=13)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameSelection","SELECT");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[83][0][Tokenizer.Peek(0).Type] && LOOKTable[83][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"GameSelection","GameInfoPredicate");
        
    }
    FillGameInfoPredicate(ReturnValue.GameCondition,Tokenizer);
    
}
GameSelection ParseGameSelection(MBCC::Tokenizer& Tokenizer)
{
    GameSelection ReturnValue;
    FillGameSelection(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillPlayerAssignment_0(PlayerAssignment& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.WithPosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=9)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"PlayerAssignment","WITH");
        
    }
    Tokenizer.ConsumeToken();
    ReturnValue.PlayerPosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=23)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"PlayerAssignment","idf");
        
    }
    ReturnValue.AffectedPlayer = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[83][0][Tokenizer.Peek(0).Type] && LOOKTable[83][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"PlayerAssignment","GameInfoPredicate");
        
    }
    FillGameInfoPredicate(ReturnValue.PlayerCondition,Tokenizer);
    
}
void  FillPlayerAssignment(PlayerAssignment& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.WithPosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=9)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"PlayerAssignment","WITH");
        
    }
    Tokenizer.ConsumeToken();
    ReturnValue.PlayerPosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=23)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"PlayerAssignment","idf");
        
    }
    ReturnValue.AffectedPlayer = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[83][0][Tokenizer.Peek(0).Type] && LOOKTable[83][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"PlayerAssignment","GameInfoPredicate");
        
    }
    FillGameInfoPredicate(ReturnValue.PlayerCondition,Tokenizer);
    
}
PlayerAssignment ParsePlayerAssignment(MBCC::Tokenizer& Tokenizer)
{
    PlayerAssignment ReturnValue;
    FillPlayerAssignment(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillResult_Record_0(Result_Record& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.RecordPosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=14)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result_Record","RECORD");
        
    }
    Tokenizer.ConsumeToken();
    ReturnValue.FilePosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=7)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result_Record","string");
        
    }
    ReturnValue.OutFile = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
void  FillResult_Record(Result_Record& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.RecordPosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=14)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result_Record","RECORD");
        
    }
    Tokenizer.ConsumeToken();
    ReturnValue.FilePosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=7)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result_Record","string");
        
    }
    ReturnValue.OutFile = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
Result_Record ParseResult_Record(MBCC::Tokenizer& Tokenizer)
{
    Result_Record ReturnValue;
    FillResult_Record(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillResult_Print_0(Result_Print& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.PrintPosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=19)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result_Print","PRINT");
        
    }
    Tokenizer.ConsumeToken();
    
}
void  FillResult_Print(Result_Print& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.PrintPosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=19)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result_Print","PRINT");
        
    }
    Tokenizer.ConsumeToken();
    
}
Result_Print ParseResult_Print(MBCC::Tokenizer& Tokenizer)
{
    Result_Print ReturnValue;
    FillResult_Print(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillColumnSpec_0(ColumnSpec& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(LOOKTable[95][0][Tokenizer.Peek(0).Type] && LOOKTable[95][1][Tokenizer.Peek(1).Type])
    {
        if(LOOKTable[96][0][Tokenizer.Peek(0).Type] && LOOKTable[96][1][Tokenizer.Peek(1).Type])
        {
            if(!(LOOKTable[128][0][Tokenizer.Peek(0).Type] && LOOKTable[128][1][Tokenizer.Peek(1).Type]))
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L5","Token");
                
            }
            FillToken(ReturnValue.Name,Tokenizer);
            if(Tokenizer.Peek(0).Type!=38)
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L5","eq");
                
            }
            Tokenizer.ConsumeToken();
            
        }
        else 
        {
            throw MBCC::ParsingException(Tokenizer.Peek().Position,"ColumnSpec","_L5");
            
        }
        
    }
    if(!(LOOKTable[63][0][Tokenizer.Peek(0).Type] && LOOKTable[63][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ColumnSpec","Filter_Component");
        
    }
    FillFilter_Component(ReturnValue.Metric.GetOrAssign<Filter_OperatorList>(),Tokenizer);
    
}
void  FillColumnSpec(ColumnSpec& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(LOOKTable[95][0][Tokenizer.Peek(0).Type] && LOOKTable[95][1][Tokenizer.Peek(1).Type])
    {
        if(LOOKTable[96][0][Tokenizer.Peek(0).Type] && LOOKTable[96][1][Tokenizer.Peek(1).Type])
        {
            if(!(LOOKTable[128][0][Tokenizer.Peek(0).Type] && LOOKTable[128][1][Tokenizer.Peek(1).Type]))
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L5","Token");
                
            }
            FillToken(ReturnValue.Name,Tokenizer);
            if(Tokenizer.Peek(0).Type!=38)
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L5","eq");
                
            }
            Tokenizer.ConsumeToken();
            
        }
        else 
        {
            throw MBCC::ParsingException(Tokenizer.Peek().Position,"ColumnSpec","_L5");
            
        }
        
    }
    if(!(LOOKTable[63][0][Tokenizer.Peek(0).Type] && LOOKTable[63][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ColumnSpec","Filter_Component");
        
    }
    FillFilter_Component(ReturnValue.Metric.GetOrAssign<Filter_OperatorList>(),Tokenizer);
    
}
ColumnSpec ParseColumnSpec(MBCC::Tokenizer& Tokenizer)
{
    ColumnSpec ReturnValue;
    FillColumnSpec(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillResult_Tabulate_0(Result_Tabulate& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.TabulatePos = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=20)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result_Tabulate","TABULATE");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[93][0][Tokenizer.Peek(0).Type] && LOOKTable[93][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result_Tabulate","ColumnSpec");
        
    }
    ReturnValue.Columns.push_back(ColumnSpec());
    FillColumnSpec(ReturnValue.Columns.back(),Tokenizer);
    while(LOOKTable[99][0][Tokenizer.Peek(0).Type] && LOOKTable[99][1][Tokenizer.Peek(1).Type])
    {
        if(LOOKTable[100][0][Tokenizer.Peek(0).Type] && LOOKTable[100][1][Tokenizer.Peek(1).Type])
        {
            if(Tokenizer.Peek(0).Type!=36)
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L6","comma");
                
            }
            Tokenizer.ConsumeToken();
            if(!(LOOKTable[93][0][Tokenizer.Peek(0).Type] && LOOKTable[93][1][Tokenizer.Peek(1).Type]))
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L6","ColumnSpec");
                
            }
            ReturnValue.Columns.push_back(ColumnSpec());
            FillColumnSpec(ReturnValue.Columns.back(),Tokenizer);
            
        }
        else 
        {
            throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result_Tabulate","_L6");
            
        }
        
    }
    if(LOOKTable[101][0][Tokenizer.Peek(0).Type] && LOOKTable[101][1][Tokenizer.Peek(1).Type])
    {
        if(LOOKTable[102][0][Tokenizer.Peek(0).Type] && LOOKTable[102][1][Tokenizer.Peek(1).Type])
        {
            ReturnValue.IntoPos = Tokenizer.Peek(0).Position;
            if(Tokenizer.Peek(0).Type!=21)
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L7","INTO");
                
            }
            Tokenizer.ConsumeToken();
            if(!(LOOKTable[42][0][Tokenizer.Peek(0).Type] && LOOKTable[42][1][Tokenizer.Peek(1).Type]))
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L7","Literal_String");
                
            }
            FillLiteral_String(ReturnValue.OutFile,Tokenizer);
            
        }
        else 
        {
            throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result_Tabulate","_L7");
            
        }
        
    }
    
}
void  FillResult_Tabulate(Result_Tabulate& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.TabulatePos = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=20)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result_Tabulate","TABULATE");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[93][0][Tokenizer.Peek(0).Type] && LOOKTable[93][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result_Tabulate","ColumnSpec");
        
    }
    ReturnValue.Columns.push_back(ColumnSpec());
    FillColumnSpec(ReturnValue.Columns.back(),Tokenizer);
    while(LOOKTable[99][0][Tokenizer.Peek(0).Type] && LOOKTable[99][1][Tokenizer.Peek(1).Type])
    {
        if(LOOKTable[100][0][Tokenizer.Peek(0).Type] && LOOKTable[100][1][Tokenizer.Peek(1).Type])
        {
            if(Tokenizer.Peek(0).Type!=36)
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L6","comma");
                
            }
            Tokenizer.ConsumeToken();
            if(!(LOOKTable[93][0][Tokenizer.Peek(0).Type] && LOOKTable[93][1][Tokenizer.Peek(1).Type]))
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L6","ColumnSpec");
                
            }
            ReturnValue.Columns.push_back(ColumnSpec());
            FillColumnSpec(ReturnValue.Columns.back(),Tokenizer);
            
        }
        else 
        {
            throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result_Tabulate","_L6");
            
        }
        
    }
    if(LOOKTable[101][0][Tokenizer.Peek(0).Type] && LOOKTable[101][1][Tokenizer.Peek(1).Type])
    {
        if(LOOKTable[102][0][Tokenizer.Peek(0).Type] && LOOKTable[102][1][Tokenizer.Peek(1).Type])
        {
            ReturnValue.IntoPos = Tokenizer.Peek(0).Position;
            if(Tokenizer.Peek(0).Type!=21)
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L7","INTO");
                
            }
            Tokenizer.ConsumeToken();
            if(!(LOOKTable[42][0][Tokenizer.Peek(0).Type] && LOOKTable[42][1][Tokenizer.Peek(1).Type]))
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L7","Literal_String");
                
            }
            FillLiteral_String(ReturnValue.OutFile,Tokenizer);
            
        }
        else 
        {
            throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result_Tabulate","_L7");
            
        }
        
    }
    
}
Result_Tabulate ParseResult_Tabulate(MBCC::Tokenizer& Tokenizer)
{
    Result_Tabulate ReturnValue;
    FillResult_Tabulate(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillResult_0(Result& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(!(LOOKTable[89][0][Tokenizer.Peek(0).Type] && LOOKTable[89][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result","Result_Record");
        
    }
    FillResult_Record(ReturnValue.GetOrAssign<Result_Record>(),Tokenizer);
    
}
void  FillResult_1(Result& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(!(LOOKTable[91][0][Tokenizer.Peek(0).Type] && LOOKTable[91][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result","Result_Print");
        
    }
    FillResult_Print(ReturnValue.GetOrAssign<Result_Print>(),Tokenizer);
    
}
void  FillResult_2(Result& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(!(LOOKTable[97][0][Tokenizer.Peek(0).Type] && LOOKTable[97][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result","Result_Tabulate");
        
    }
    FillResult_Tabulate(ReturnValue.GetOrAssign<Result_Tabulate>(),Tokenizer);
    
}
void  FillResult(Result& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(LOOKTable[104][0][Tokenizer.Peek(0).Type] && LOOKTable[104][1][Tokenizer.Peek(1).Type])
    {
        FillResult_0(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[105][0][Tokenizer.Peek(0).Type] && LOOKTable[105][1][Tokenizer.Peek(1).Type])
    {
        FillResult_1(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[106][0][Tokenizer.Peek(0).Type] && LOOKTable[106][1][Tokenizer.Peek(1).Type])
    {
        FillResult_2(ReturnValue,Tokenizer);
        
    }
    else 
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Result","Result");
        
    }
    
}
Result ParseResult(MBCC::Tokenizer& Tokenizer)
{
    Result ReturnValue;
    FillResult(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillSelection_0(Selection& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(!(LOOKTable[85][0][Tokenizer.Peek(0).Type] && LOOKTable[85][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Selection","GameSelection");
        
    }
    FillGameSelection(ReturnValue.Games,Tokenizer);
    if(LOOKTable[67][0][Tokenizer.Peek(0).Type] && LOOKTable[67][1][Tokenizer.Peek(1).Type])
    {
        FillFilter(ReturnValue.SituationFilter,Tokenizer);
        
    }
    if(!(LOOKTable[103][0][Tokenizer.Peek(0).Type] && LOOKTable[103][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Selection","Result");
        
    }
    FillResult(ReturnValue.Output,Tokenizer);
    if(Tokenizer.Peek(0).Type!=24)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Selection","semi");
        
    }
    Tokenizer.ConsumeToken();
    
}
void  FillSelection(Selection& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(!(LOOKTable[85][0][Tokenizer.Peek(0).Type] && LOOKTable[85][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Selection","GameSelection");
        
    }
    FillGameSelection(ReturnValue.Games,Tokenizer);
    if(LOOKTable[67][0][Tokenizer.Peek(0).Type] && LOOKTable[67][1][Tokenizer.Peek(1).Type])
    {
        FillFilter(ReturnValue.SituationFilter,Tokenizer);
        
    }
    if(!(LOOKTable[103][0][Tokenizer.Peek(0).Type] && LOOKTable[103][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Selection","Result");
        
    }
    FillResult(ReturnValue.Output,Tokenizer);
    if(Tokenizer.Peek(0).Type!=24)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Selection","semi");
        
    }
    Tokenizer.ConsumeToken();
    
}
Selection ParseSelection(MBCC::Tokenizer& Tokenizer)
{
    Selection ReturnValue;
    FillSelection(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillVariableDeclaration_Filter_0(VariableDeclaration_Filter& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.FilterPosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=18)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_Filter","FILTER");
        
    }
    Tokenizer.ConsumeToken();
    ReturnValue.NamePosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=23)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_Filter","idf");
        
    }
    ReturnValue.Name = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[32][0][Tokenizer.Peek(0).Type] && LOOKTable[32][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_Filter","Filter_ArgList");
        
    }
    FillFilter_ArgList(ReturnValue.Arguments,Tokenizer);
    if(Tokenizer.Peek(0).Type!=38)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_Filter","eq");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[63][0][Tokenizer.Peek(0).Type] && LOOKTable[63][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_Filter","Filter_Component");
        
    }
    FillFilter_Component(ReturnValue.Component.GetOrAssign<Filter_OperatorList>(),Tokenizer);
    if(Tokenizer.Peek(0).Type!=24)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_Filter","semi");
        
    }
    Tokenizer.ConsumeToken();
    
}
void  FillVariableDeclaration_Filter(VariableDeclaration_Filter& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.FilterPosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=18)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_Filter","FILTER");
        
    }
    Tokenizer.ConsumeToken();
    ReturnValue.NamePosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=23)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_Filter","idf");
        
    }
    ReturnValue.Name = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[32][0][Tokenizer.Peek(0).Type] && LOOKTable[32][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_Filter","Filter_ArgList");
        
    }
    FillFilter_ArgList(ReturnValue.Arguments,Tokenizer);
    if(Tokenizer.Peek(0).Type!=38)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_Filter","eq");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[63][0][Tokenizer.Peek(0).Type] && LOOKTable[63][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_Filter","Filter_Component");
        
    }
    FillFilter_Component(ReturnValue.Component.GetOrAssign<Filter_OperatorList>(),Tokenizer);
    if(Tokenizer.Peek(0).Type!=24)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_Filter","semi");
        
    }
    Tokenizer.ConsumeToken();
    
}
VariableDeclaration_Filter ParseVariableDeclaration_Filter(MBCC::Tokenizer& Tokenizer)
{
    VariableDeclaration_Filter ReturnValue;
    FillVariableDeclaration_Filter(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillVariableDeclaration_GameList_0(VariableDeclaration_GameList& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.GamePosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=15)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameList","GAMES");
        
    }
    Tokenizer.ConsumeToken();
    ReturnValue.NamePosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=23)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameList","idf");
        
    }
    ReturnValue.Name = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    if(Tokenizer.Peek(0).Type!=38)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameList","eq");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[85][0][Tokenizer.Peek(0).Type] && LOOKTable[85][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameList","GameSelection");
        
    }
    FillGameSelection(ReturnValue.Selection,Tokenizer);
    if(Tokenizer.Peek(0).Type!=24)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameList","semi");
        
    }
    Tokenizer.ConsumeToken();
    
}
void  FillVariableDeclaration_GameList(VariableDeclaration_GameList& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.GamePosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=15)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameList","GAMES");
        
    }
    Tokenizer.ConsumeToken();
    ReturnValue.NamePosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=23)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameList","idf");
        
    }
    ReturnValue.Name = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    if(Tokenizer.Peek(0).Type!=38)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameList","eq");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[85][0][Tokenizer.Peek(0).Type] && LOOKTable[85][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameList","GameSelection");
        
    }
    FillGameSelection(ReturnValue.Selection,Tokenizer);
    if(Tokenizer.Peek(0).Type!=24)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameList","semi");
        
    }
    Tokenizer.ConsumeToken();
    
}
VariableDeclaration_GameList ParseVariableDeclaration_GameList(MBCC::Tokenizer& Tokenizer)
{
    VariableDeclaration_GameList ReturnValue;
    FillVariableDeclaration_GameList(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillVariableDeclaration_PlayerSelection_0(VariableDeclaration_PlayerSelection& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.GamePosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=17)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_PlayerSelection","PlayerSelection");
        
    }
    Tokenizer.ConsumeToken();
    ReturnValue.NamePosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=23)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_PlayerSelection","idf");
        
    }
    ReturnValue.Name = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    if(Tokenizer.Peek(0).Type!=38)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_PlayerSelection","eq");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[83][0][Tokenizer.Peek(0).Type] && LOOKTable[83][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_PlayerSelection","GameInfoPredicate");
        
    }
    FillGameInfoPredicate(ReturnValue.Predicate,Tokenizer);
    if(Tokenizer.Peek(0).Type!=24)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_PlayerSelection","semi");
        
    }
    Tokenizer.ConsumeToken();
    
}
void  FillVariableDeclaration_PlayerSelection(VariableDeclaration_PlayerSelection& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.GamePosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=17)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_PlayerSelection","PlayerSelection");
        
    }
    Tokenizer.ConsumeToken();
    ReturnValue.NamePosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=23)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_PlayerSelection","idf");
        
    }
    ReturnValue.Name = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    if(Tokenizer.Peek(0).Type!=38)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_PlayerSelection","eq");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[83][0][Tokenizer.Peek(0).Type] && LOOKTable[83][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_PlayerSelection","GameInfoPredicate");
        
    }
    FillGameInfoPredicate(ReturnValue.Predicate,Tokenizer);
    if(Tokenizer.Peek(0).Type!=24)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_PlayerSelection","semi");
        
    }
    Tokenizer.ConsumeToken();
    
}
VariableDeclaration_PlayerSelection ParseVariableDeclaration_PlayerSelection(MBCC::Tokenizer& Tokenizer)
{
    VariableDeclaration_PlayerSelection ReturnValue;
    FillVariableDeclaration_PlayerSelection(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillVariableDeclaration_GameInfoPredicate_0(VariableDeclaration_GameInfoPredicate& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.GamePosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=16)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameInfoPredicate","GamePredicate");
        
    }
    Tokenizer.ConsumeToken();
    ReturnValue.NamePosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=23)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameInfoPredicate","idf");
        
    }
    ReturnValue.Name = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    if(Tokenizer.Peek(0).Type!=38)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameInfoPredicate","eq");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[83][0][Tokenizer.Peek(0).Type] && LOOKTable[83][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameInfoPredicate","GameInfoPredicate");
        
    }
    FillGameInfoPredicate(ReturnValue.Predicate,Tokenizer);
    if(Tokenizer.Peek(0).Type!=24)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameInfoPredicate","semi");
        
    }
    Tokenizer.ConsumeToken();
    
}
void  FillVariableDeclaration_GameInfoPredicate(VariableDeclaration_GameInfoPredicate& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.GamePosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=16)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameInfoPredicate","GamePredicate");
        
    }
    Tokenizer.ConsumeToken();
    ReturnValue.NamePosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=23)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameInfoPredicate","idf");
        
    }
    ReturnValue.Name = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    if(Tokenizer.Peek(0).Type!=38)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameInfoPredicate","eq");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[83][0][Tokenizer.Peek(0).Type] && LOOKTable[83][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameInfoPredicate","GameInfoPredicate");
        
    }
    FillGameInfoPredicate(ReturnValue.Predicate,Tokenizer);
    if(Tokenizer.Peek(0).Type!=24)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration_GameInfoPredicate","semi");
        
    }
    Tokenizer.ConsumeToken();
    
}
VariableDeclaration_GameInfoPredicate ParseVariableDeclaration_GameInfoPredicate(MBCC::Tokenizer& Tokenizer)
{
    VariableDeclaration_GameInfoPredicate ReturnValue;
    FillVariableDeclaration_GameInfoPredicate(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillVariableDeclaration_0(VariableDeclaration& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(!(LOOKTable[109][0][Tokenizer.Peek(0).Type] && LOOKTable[109][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration","VariableDeclaration_Filter");
        
    }
    FillVariableDeclaration_Filter(ReturnValue.GetOrAssign<VariableDeclaration_Filter>(),Tokenizer);
    
}
void  FillVariableDeclaration_1(VariableDeclaration& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(!(LOOKTable[111][0][Tokenizer.Peek(0).Type] && LOOKTable[111][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration","VariableDeclaration_GameList");
        
    }
    FillVariableDeclaration_GameList(ReturnValue.GetOrAssign<VariableDeclaration_GameList>(),Tokenizer);
    
}
void  FillVariableDeclaration_2(VariableDeclaration& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(!(LOOKTable[115][0][Tokenizer.Peek(0).Type] && LOOKTable[115][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration","VariableDeclaration_GameInfoPredicate");
        
    }
    FillVariableDeclaration_GameInfoPredicate(ReturnValue.GetOrAssign<VariableDeclaration_GameInfoPredicate>(),Tokenizer);
    
}
void  FillVariableDeclaration_3(VariableDeclaration& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(!(LOOKTable[113][0][Tokenizer.Peek(0).Type] && LOOKTable[113][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration","VariableDeclaration_PlayerSelection");
        
    }
    FillVariableDeclaration_PlayerSelection(ReturnValue.GetOrAssign<VariableDeclaration_PlayerSelection>(),Tokenizer);
    
}
void  FillVariableDeclaration(VariableDeclaration& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(LOOKTable[118][0][Tokenizer.Peek(0).Type] && LOOKTable[118][1][Tokenizer.Peek(1).Type])
    {
        FillVariableDeclaration_0(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[119][0][Tokenizer.Peek(0).Type] && LOOKTable[119][1][Tokenizer.Peek(1).Type])
    {
        FillVariableDeclaration_1(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[120][0][Tokenizer.Peek(0).Type] && LOOKTable[120][1][Tokenizer.Peek(1).Type])
    {
        FillVariableDeclaration_2(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[121][0][Tokenizer.Peek(0).Type] && LOOKTable[121][1][Tokenizer.Peek(1).Type])
    {
        FillVariableDeclaration_3(ReturnValue,Tokenizer);
        
    }
    else 
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"VariableDeclaration","VariableDeclaration");
        
    }
    
}
VariableDeclaration ParseVariableDeclaration(MBCC::Tokenizer& Tokenizer)
{
    VariableDeclaration ReturnValue;
    FillVariableDeclaration(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillModule_0(Module& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    while(LOOKTable[138][0][Tokenizer.Peek(0).Type] && LOOKTable[138][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.Statements.push_back(Statement());
        FillStatement(ReturnValue.Statements.back(),Tokenizer);
        
    }
    
}
void  FillModule(Module& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    while(LOOKTable[138][0][Tokenizer.Peek(0).Type] && LOOKTable[138][1][Tokenizer.Peek(1).Type])
    {
        ReturnValue.Statements.push_back(Statement());
        FillStatement(ReturnValue.Statements.back(),Tokenizer);
        
    }
    
}
Module ParseModule(MBCC::Tokenizer& Tokenizer)
{
    Module ReturnValue;
    FillModule(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillIdentifier_0(Identifier& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(!(LOOKTable[128][0][Tokenizer.Peek(0).Type] && LOOKTable[128][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Identifier","Token");
        
    }
    ReturnValue.Parts.push_back(Token());
    FillToken(ReturnValue.Parts.back(),Tokenizer);
    while(LOOKTable[126][0][Tokenizer.Peek(0).Type] && LOOKTable[126][1][Tokenizer.Peek(1).Type])
    {
        if(LOOKTable[127][0][Tokenizer.Peek(0).Type] && LOOKTable[127][1][Tokenizer.Peek(1).Type])
        {
            if(Tokenizer.Peek(0).Type!=39)
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L8","dot");
                
            }
            Tokenizer.ConsumeToken();
            if(!(LOOKTable[128][0][Tokenizer.Peek(0).Type] && LOOKTable[128][1][Tokenizer.Peek(1).Type]))
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L8","Token");
                
            }
            ReturnValue.Parts.push_back(Token());
            FillToken(ReturnValue.Parts.back(),Tokenizer);
            
        }
        else 
        {
            throw MBCC::ParsingException(Tokenizer.Peek().Position,"Identifier","_L8");
            
        }
        
    }
    ReturnValue.End = Tokenizer.Peek(0).Position;
    
}
void  FillIdentifier(Identifier& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(!(LOOKTable[128][0][Tokenizer.Peek(0).Type] && LOOKTable[128][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Identifier","Token");
        
    }
    ReturnValue.Parts.push_back(Token());
    FillToken(ReturnValue.Parts.back(),Tokenizer);
    while(LOOKTable[126][0][Tokenizer.Peek(0).Type] && LOOKTable[126][1][Tokenizer.Peek(1).Type])
    {
        if(LOOKTable[127][0][Tokenizer.Peek(0).Type] && LOOKTable[127][1][Tokenizer.Peek(1).Type])
        {
            if(Tokenizer.Peek(0).Type!=39)
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L8","dot");
                
            }
            Tokenizer.ConsumeToken();
            if(!(LOOKTable[128][0][Tokenizer.Peek(0).Type] && LOOKTable[128][1][Tokenizer.Peek(1).Type]))
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L8","Token");
                
            }
            ReturnValue.Parts.push_back(Token());
            FillToken(ReturnValue.Parts.back(),Tokenizer);
            
        }
        else 
        {
            throw MBCC::ParsingException(Tokenizer.Peek().Position,"Identifier","_L8");
            
        }
        
    }
    ReturnValue.End = Tokenizer.Peek(0).Position;
    
}
Identifier ParseIdentifier(MBCC::Tokenizer& Tokenizer)
{
    Identifier ReturnValue;
    FillIdentifier(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillToken_0(Token& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.Position = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=23)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Token","idf");
        
    }
    ReturnValue.Value = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
void  FillToken(Token& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.Position = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=23)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Token","idf");
        
    }
    ReturnValue.Value = Tokenizer.Peek(0).Value;
    Tokenizer.ConsumeToken();
    
}
Token ParseToken(MBCC::Tokenizer& Tokenizer)
{
    Token ReturnValue;
    FillToken(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillUsingDirective_0(UsingDirective& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.UsingPosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=11)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"UsingDirective","USING");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[128][0][Tokenizer.Peek(0).Type] && LOOKTable[128][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"UsingDirective","Token");
        
    }
    ReturnValue.GameSets.push_back(Token());
    FillToken(ReturnValue.GameSets.back(),Tokenizer);
    while(LOOKTable[132][0][Tokenizer.Peek(0).Type] && LOOKTable[132][1][Tokenizer.Peek(1).Type])
    {
        if(LOOKTable[133][0][Tokenizer.Peek(0).Type] && LOOKTable[133][1][Tokenizer.Peek(1).Type])
        {
            if(Tokenizer.Peek(0).Type!=36)
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L9","comma");
                
            }
            Tokenizer.ConsumeToken();
            if(!(LOOKTable[128][0][Tokenizer.Peek(0).Type] && LOOKTable[128][1][Tokenizer.Peek(1).Type]))
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L9","Token");
                
            }
            ReturnValue.GameSets.push_back(Token());
            FillToken(ReturnValue.GameSets.back(),Tokenizer);
            
        }
        else 
        {
            throw MBCC::ParsingException(Tokenizer.Peek().Position,"UsingDirective","_L9");
            
        }
        
    }
    ReturnValue.End = Tokenizer.Peek(0).Position;
    
}
void  FillUsingDirective(UsingDirective& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.UsingPosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=11)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"UsingDirective","USING");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[128][0][Tokenizer.Peek(0).Type] && LOOKTable[128][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"UsingDirective","Token");
        
    }
    ReturnValue.GameSets.push_back(Token());
    FillToken(ReturnValue.GameSets.back(),Tokenizer);
    while(LOOKTable[132][0][Tokenizer.Peek(0).Type] && LOOKTable[132][1][Tokenizer.Peek(1).Type])
    {
        if(LOOKTable[133][0][Tokenizer.Peek(0).Type] && LOOKTable[133][1][Tokenizer.Peek(1).Type])
        {
            if(Tokenizer.Peek(0).Type!=36)
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L9","comma");
                
            }
            Tokenizer.ConsumeToken();
            if(!(LOOKTable[128][0][Tokenizer.Peek(0).Type] && LOOKTable[128][1][Tokenizer.Peek(1).Type]))
            {
                throw MBCC::ParsingException(Tokenizer.Peek().Position,"_L9","Token");
                
            }
            ReturnValue.GameSets.push_back(Token());
            FillToken(ReturnValue.GameSets.back(),Tokenizer);
            
        }
        else 
        {
            throw MBCC::ParsingException(Tokenizer.Peek().Position,"UsingDirective","_L9");
            
        }
        
    }
    ReturnValue.End = Tokenizer.Peek(0).Position;
    
}
UsingDirective ParseUsingDirective(MBCC::Tokenizer& Tokenizer)
{
    UsingDirective ReturnValue;
    FillUsingDirective(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillImportBinding_0(ImportBinding& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.AsPosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=12)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ImportBinding","AS");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[128][0][Tokenizer.Peek(0).Type] && LOOKTable[128][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ImportBinding","Token");
        
    }
    FillToken(ReturnValue.ImportName,Tokenizer);
    
}
void  FillImportBinding(ImportBinding& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.AsPosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=12)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ImportBinding","AS");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[128][0][Tokenizer.Peek(0).Type] && LOOKTable[128][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"ImportBinding","Token");
        
    }
    FillToken(ReturnValue.ImportName,Tokenizer);
    
}
ImportBinding ParseImportBinding(MBCC::Tokenizer& Tokenizer)
{
    ImportBinding ReturnValue;
    FillImportBinding(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillImport_0(Import& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.ImportPosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=10)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Import","IMPORT");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[124][0][Tokenizer.Peek(0).Type] && LOOKTable[124][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Import","Identifier");
        
    }
    FillIdentifier(ReturnValue.ImportPath,Tokenizer);
    if(LOOKTable[134][0][Tokenizer.Peek(0).Type] && LOOKTable[134][1][Tokenizer.Peek(1).Type])
    {
        FillImportBinding(ReturnValue.Binding,Tokenizer);
        
    }
    if(Tokenizer.Peek(0).Type!=24)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Import","semi");
        
    }
    Tokenizer.ConsumeToken();
    ReturnValue.End = Tokenizer.Peek(0).Position;
    
}
void  FillImport(Import& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    ReturnValue.ImportPosition = Tokenizer.Peek(0).Position;
    if(Tokenizer.Peek(0).Type!=10)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Import","IMPORT");
        
    }
    Tokenizer.ConsumeToken();
    if(!(LOOKTable[124][0][Tokenizer.Peek(0).Type] && LOOKTable[124][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Import","Identifier");
        
    }
    FillIdentifier(ReturnValue.ImportPath,Tokenizer);
    if(LOOKTable[134][0][Tokenizer.Peek(0).Type] && LOOKTable[134][1][Tokenizer.Peek(1).Type])
    {
        FillImportBinding(ReturnValue.Binding,Tokenizer);
        
    }
    if(Tokenizer.Peek(0).Type!=24)
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Import","semi");
        
    }
    Tokenizer.ConsumeToken();
    ReturnValue.End = Tokenizer.Peek(0).Position;
    
}
Import ParseImport(MBCC::Tokenizer& Tokenizer)
{
    Import ReturnValue;
    FillImport(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
void  FillStatement_0(Statement& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(!(LOOKTable[117][0][Tokenizer.Peek(0).Type] && LOOKTable[117][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Statement","VariableDeclaration");
        
    }
    FillVariableDeclaration(ReturnValue.GetOrAssign<VariableDeclaration>(),Tokenizer);
    
}
void  FillStatement_1(Statement& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(!(LOOKTable[107][0][Tokenizer.Peek(0).Type] && LOOKTable[107][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Statement","Selection");
        
    }
    FillSelection(ReturnValue.GetOrAssign<Selection>(),Tokenizer);
    
}
void  FillStatement_2(Statement& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(!(LOOKTable[136][0][Tokenizer.Peek(0).Type] && LOOKTable[136][1][Tokenizer.Peek(1).Type]))
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Statement","Import");
        
    }
    FillImport(ReturnValue.GetOrAssign<Import>(),Tokenizer);
    
}
void  FillStatement(Statement& ReturnValue, MBCC::Tokenizer& Tokenizer)
{
    if(LOOKTable[139][0][Tokenizer.Peek(0).Type] && LOOKTable[139][1][Tokenizer.Peek(1).Type])
    {
        FillStatement_0(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[140][0][Tokenizer.Peek(0).Type] && LOOKTable[140][1][Tokenizer.Peek(1).Type])
    {
        FillStatement_1(ReturnValue,Tokenizer);
        
    }
    else if(LOOKTable[141][0][Tokenizer.Peek(0).Type] && LOOKTable[141][1][Tokenizer.Peek(1).Type])
    {
        FillStatement_2(ReturnValue,Tokenizer);
        
    }
    else 
    {
        throw MBCC::ParsingException(Tokenizer.Peek().Position,"Statement","Statement");
        
    }
    
}
Statement ParseStatement(MBCC::Tokenizer& Tokenizer)
{
    Statement ReturnValue;
    FillStatement(ReturnValue,Tokenizer);
    return ReturnValue;
    
}
