#ifndef LINK_H
#define LINK_H

typedef struct Link Link;

#include "Neuron.h"

struct Link {
	Neuron* from;
	Neuron* to;
	float weight;
};

void link_print(Link* link);
void link_init(Link* link, Neuron* from, Neuron* to, float weight);

#endif