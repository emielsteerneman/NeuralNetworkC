#ifndef NETWORK_H
#define NETWORK_H

// Struct declarations
typedef struct Network Network;

#include <stdio.h>

#include "Neuron.h"
#include "Link.h"
#include "main.h"

// Struct definitions
struct Network {
	int nInput;
	int nHidden;
	int nOutput;
	int nLinks;
	
	PropVals* pv;
	
	Neuron** inputLayer;
	Neuron** hiddenLayer;
	Neuron** outputLayer;
	Link** links;
};

// Functions
Network* network_init(int nInputs, int nHiddens, int nOutputs);
void network_propagate(Network* network);
void network_train(Network* network);
void network_reset(Network* network);

#endif