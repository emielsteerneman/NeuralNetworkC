#ifndef FILERHANDLER_H
#define FILERHANDLER_H

#include <stdio.h>
#include <stdlib.h>

#include "Network.h"

// Functions
Network* fileHandler_readNetwork(char* filename);
int fileHandler_writeNetwork(char* filename, Network* network);

#endif