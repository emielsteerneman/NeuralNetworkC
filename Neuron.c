#include "Neuron.h"

void neuron_print(Neuron* n){
	printf("Neuron (%d,%d) | ", n->ilayer, n->id);
	printf("net = %0.3f | ", n->net);
	printf("out = %0.3f | ", n->out);
	printf("delta = %0.3f | ", n->delta);
	printf("bias = %0.3f | ", n->bias);
	printf("nIn = %d | ", n->nIn);
	printf("nOut = %d\n", n->nOut);
}

void neuron_init(Neuron* n, int ilayer, int id, int nIn, int nOut){
	
	int log = 0;
	
	n->net   = 0;
	n->out   = 0;
	n->delta = 0;
	n->bias  = 0;
	n->nIn   = nIn;
	n->nOut  = nOut;
	n->isSet = 0;
	
	// Create an array of pointers
	n->linksIn  = (Link**) malloc(sizeof(Link*) * nIn);
	n->linksOut = (Link**) malloc(sizeof(Link*) * nOut);
	
	n->ilayer = ilayer;
	n->id = id;
	
	if(log) {
		printf("  new ");
		neuron_print(n);
	}
}

float neuron_propagate(Neuron* n, Network* network, int depth){
	int log = 1;
	if(log) printf("\n");
	if(log) indent(depth);
	if(log) printf("(%d,%d)", n->ilayer, n->id);
	
	// If neuron is already set
	if(n->isSet == 1){
		if(log) printf(" set! %d %d\n", n->ilayer, n->id);
	}else
	
	// If neuron is input
	if(n->nIn == 0){
		if(log) printf(" input!");
		n->out = network->pv->inputs[n->id];
	}else
	
	// Propagate down
	{
		if(log) printf("\n");
		if(log) indent(depth);
		if(log) printf("bias = %0.3f", n->bias);
		
		float net = n->bias;
		int i;
		for(i = 0; i < n->nIn; i++){
			Link* link = n->linksIn[i];
			Neuron* from = link->from;
			float out = neuron_propagate(from, network, depth+1);
			if(log) printf("\n");
			if(log) indent(depth);
			if(log) printf("%0.3f x %0.3f = %0.3f", out, link->weight, out  * link->weight);
			net += out * link->weight;
			if(log) printf(" | net = %0.3f", net);
		}
		// printf("\n");
		// indent(depth);
		// printf("net : %0.3f", net);
		
		n->net = net;
		n->out = sigmoid(net);
	}
	
	n->isSet = 1;
	if(log) printf("\n");
	if(log) indent(depth);
	if(log) printf("<- %0.3f", n->out);
	return n->out;
}

float sigmoid(float f){
	return 1 / (1 + exp(-f));
}

void indent(int depth){
	int i;
	for(i = 0; i < depth; i++){
		printf("|   ");
	}
}

void neuron_str(Neuron* n){
	printf("(%d,%d)", n->ilayer, n->id);
}
























