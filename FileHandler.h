#ifndef FILERHANDLER_H
#define FILERHANDLER_H

#include <stdio.h>
#include <stdlib.h>

#include "Network.h"

// Functions
Network* fileHander_readNetwork(char* filename);
int fileHander_writeNetwork(char* filename, Network* network);

#endif