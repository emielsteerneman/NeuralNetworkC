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
	int* layerSizes; 
	int nLayers;
	int nLinks;

	Neuron** inputLayer;
	Neuron** outputLayer;
	int nInputs;
	int nOutputs;

	PropVals* pv;
	
	Neuron*** layers;
	Link** links;
};

// Functions
//Network* network_init(int nInputs, int nHiddens, int nOutputs);
Network* network_init(int* layers, int nLayers);

void network_propagate(Network* network);
void network_train(Network* network);
void network_reset(Network* network);
void network_print(Network* network);

#endif