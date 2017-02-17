#ifndef NEURON_H
#define NEURON_H

typedef struct Neuron Neuron;

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Link.h"
#include "Network.h"
#include "main.h"

struct Neuron{
	float net;
	float out;
	float delta;
	float bias;
	int   isSet;
	
	// Links
	int nIn;
	int nOut;
	Link** linksIn;
	Link** linksOut;
	
	int ilayer;
	int id;
};

void neuron_print();
void neuron_init(Neuron* n, int ilayer, int id, int nIn, int nOut);
float neuron_propagate(Neuron* n, Network* network, int depth);
float sigmoid(float f);
void indent(int depth);

#endif