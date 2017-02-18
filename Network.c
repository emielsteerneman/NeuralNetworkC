#include "Network.h"

Network* network_init(int nInput, int nHidden, int nOutput){
	int log = 0;
	Network* network = (Network*) malloc(sizeof(Network));
	
	int nLinks = (nInput * nHidden + nHidden * nOutput);
	network->nInput = nInput;
	network->nHidden = nHidden;
	network->nOutput = nOutput;
	network->nLinks = nLinks;
	
	network->pv = (PropVals*) malloc(sizeof(PropVals));
	network->pv->inputs =  (float*) malloc(sizeof(float) * nInput);
	network->pv->targets = (float*) malloc(sizeof(float) * nOutput);
	network->pv->outputs = (float*) malloc(sizeof(float) * nOutput);
	
	network->inputLayer  = (Neuron**) malloc(sizeof(Neuron*) * nInput);
	network->hiddenLayer = (Neuron**) malloc(sizeof(Neuron*) * nHidden);
	network->outputLayer = (Neuron**) malloc(sizeof(Neuron*) * nOutput);
	network->links = (Link**) malloc(sizeof(Link*) * nLinks);

	if(log) printf("New network\n  nInput = %d\n  nHidden = %d\n  nOutput = %d\n  nLinks = %d\n", network->nInput, network->nHidden, network->nOutput, network->nLinks);
	
	int i, j, offset = 0;
	
	// Input layer
	if(log) printf("\nInput layer\n");
	for(i = 0; i < nInput; i++){						// For number of neurons in input layer
		Neuron* n = (Neuron*) malloc(sizeof(Neuron));		// Create neuron
		neuron_init(n, 0, i, 0, nHidden);					// Init neuron
		network->inputLayer[i] = n;							// Add neuron to input layer
	}
	
	// Hidden layer
	if(log) printf("\nHidden layer\n");
	for(i = 0; i < nHidden; i++){						// For number of neurons in hidden layer
		Neuron* n = (Neuron*) malloc(sizeof(Neuron));		// Create neuron
		neuron_init(n, 1, i, nInput, nOutput);				// Init neuron
		network->hiddenLayer[i] = n;						// Add neuron to hidden layer
	}
	
	// Output layer
	if(log) printf("\nOutput layer\n");
	for(i = 0; i < nOutput; i++){						// For number of neurons in output layer
		Neuron* n = (Neuron*) malloc(sizeof(Neuron));		// Create neuron
		neuron_init(n, 2, i, nHidden, 0);					// Init neuron
		network->outputLayer[i] = n;						// Add neuron to output layer
	}
	
	// Create links between input layer and hidden layer
	int linkCounter = 0;
	if(log) printf("\nInput layer -> Hidden layer\n");
	for(i = 0; i < nInput; i++){						// For number of neurons in input layer
		Neuron* nFrom = network->inputLayer[i];				// Get from-neuron
		for(j = 0; j < nHidden; j++){						// For number of neurons in output layer
			Neuron* nTo = network->hiddenLayer[j];				// Get to-neuron
			Link* l = (Link*) malloc(sizeof(Link));				// Create link
			link_init(l, nFrom, nTo, randomWeight(1));			// Init link
			network->links[linkCounter++] = l;					// Add link to array of links
			nFrom->linksOut[j] = l;								// Add link to from-neuron->linksOut
			nTo->linksIn[i] = l;								// Add link to to-neuron->linksIn
		}
	}
	
	// Create links between hidden layer and output layer
	if(log) printf("\nHidden layer -> Output layer\n");
	for(i = 0; i < nHidden; i++){						// For number of neurons in hidden layer
		Neuron* nFrom = network->hiddenLayer[i];			// Get from-neuron
		for(j = 0; j < nOutput; j++){						// For number of neurons in output layer
			Neuron* nTo = network->outputLayer[j];				// Get to-neuron
			Link* l = (Link*) malloc(sizeof(Link));				// Create link
			link_init(l, nFrom, nTo, randomWeight(1));			// Init link
			network->links[linkCounter++] = l;					// Add link to array of links
			nFrom->linksOut[j] = l;								// Add link to from-neuron->linksOut
			nTo->linksIn[i] = l;								// Add link to to-neuron->linksIn
		}
	}
	return network;
}

void network_propagate(Network* network){
	// printf("\nNetwork propagating\n");
	
	network_reset(network);
	
	// printf("Inputs : ");
	// printFloatArr(network->pv->inputs, network->nInput);
	// printf("Targets: ");
	// printFloatArr(network->pv->targets, network->nOutput);
	
	int i;
	for(i = 0; i < network->nOutput; i++){
		network->pv->outputs[i] = neuron_propagate(network->outputLayer[i], network, 0);
	}
	
	// printf("\nNetwork propagated\n");
	// printf("Outputs: ");
	// printFloatArr(network->pv->outputs, network->nOutput);
}

void network_train(Network* network){
	int log;
	int i;
	float learningRate = 0.3;
	
	log = 0;
	// Calculate deltas for output layer
	if(log) printf("\nUpdating output layer\n");
	for(i = 0; i < network->nOutput; i++){
		Neuron* n = network->outputLayer[i];
		float target = network->pv->targets[i];
		float out = n->out;
		
		if(log) printf("(%d,%d) %0.3f -> ", n->ilayer, n->id, n->delta);
		
		n->delta = (out - target) * out * (1 - out);
		n->bias -= n->delta * learningRate;
		
		if(log) printf(" %0.3f\n", n->delta);
	}
	
	// Calculate deltas for hidden layer
	if(log) printf("\nUpdating hidden layer\n");
	for(i = 0; i < network->nHidden; i++){
		Neuron* n = network->hiddenLayer[i];
		if(log) printf("(%d,%d)\n", n->ilayer, n->id);
		
		float out = n->out;
		float sum = 0.0f;
		int j;
		for(j = 0; j < n->nOut; j++){
			Link* link = n->linksOut[j];
			if(log) printf("  ");
			if(log) neuron_str(link->to);
			if(log) printf(" %0.3f * %0.3f = %0.3f\n", link->to->delta, link->weight, link->to->delta * link->weight);
			sum += link->to->delta * link->weight;
		}
		if(log) printf("  sum %0.3f\n", sum);
		if(log) printf("  delta %0.3f -> ", n->delta);
		n->delta = sum * out * (1 - out);
		n->bias -= n->delta * learningRate;
		if(log) printf(" %0.3f\n", n->delta);
	}
	
	log = 0;
	// Update link weights
	if(log) printf("\nUpdating link weights\n");
	for(i = 0; i < network->nLinks; i++){
		
		Link* link = network->links[i];
		if(log) link_print(link);
		
		float delta = link->to->delta;
		float x = link->from->out;
		
		link->weight -= (learningRate * delta * x);
		if(log) printf(" -> %0.3f\n", link->weight);
		if(log) printf(" deltaWeight = %0.3f\n", (learningRate * delta * x));
	}
}

void network_reset(Network* network){
	int i;
	for(i = 0; i < network->nInput; i++){
		network->inputLayer[i]->isSet = 0;
	}
	for(i = 0; i < network->nHidden; i++){
		network->hiddenLayer[i]->isSet = 0;
	}
	for(i = 0; i < network->nOutput; i++){
		network->outputLayer[i]->isSet = 0;
	}
}

void network_print(Network* network){
	
	int i = 0, j = 0, k = 0;		// Hold arbitrary counters
	
	int nNeurons, nLinks;			// Hold number of neurons and links

	// Hold layer sizes
	int layerSizes[3] = {
		network->nInput,
		network->nHidden,
		network->nOutput
	};
	// Hold pointers to layers
	Neuron** layers[3] = {
		network->inputLayer,
		network->hiddenLayer,
		network->outputLayer
	};
	
	Neuron* neuron;
	Link* link;
	
	printf("========== Network ==========\n");
	printf("Layers : %d %d %d\n", network->nInput, network->nHidden, network->nOutput);
	
	for(i = 0; i < 3; i++){			// For each layer
		printf("\nLayer %d\n", i);
		nNeurons = layerSizes[i];		// Get number of neurons
		
		for(j = 0; j < nNeurons; j++){	// For each neuron in layer
			neuron = layers[i][j];			// Get neuron
			neuron_print(neuron);			// Print neuron
			
			nLinks = neuron->nOut;		// Get number of links
			for(k = 0; k < nLinks; k++){	// For each link of neuron
				link = neuron->linksOut[k];		// Get link
				printf("\t");
				link_print(link);				// Print link
				printf("\n");
			}
		}
	}
	
	printf("=============================\n");
	
	return;
}