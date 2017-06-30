#include "Network.h"

Network* network_init(int* layers, int nLayers){
	
	printf("Network: initiating\n");
	printArray(layers, nLayers);

	int i, iLayer, iNeuron, iLayerCurr, iLayerNext, iNeuronCurr, iNeuronNext;

	// === Setup the network
	// Malloc network
	Network* network = (Network*) malloc(sizeof(Network));
	// Malloc network->layers
	network->layers = (Neuron***) malloc(sizeof(Neuron*) * nLayers);
	// Set nLayers
	network->nLayers = nLayers;
	// Malloc network->layerSizes
	network->layerSizes = (int*) malloc(sizeof(int) * nLayers);

	// Setup propvals
	network->pv = (PropVals*) malloc(sizeof(PropVals));
	network->pv->inputs =  (float*) malloc(sizeof(float) * layers[0]);
	network->pv->targets = (float*) malloc(sizeof(float) * layers[nLayers - 1]);
	network->pv->outputs = (float*) malloc(sizeof(float) * layers[nLayers - 1]);
	

	// === Count the number of Neurons and Links
	int nNeurons = 0;
	int nLinks = 0;
	
	for(i = 0; i < nLayers; i++){
		nNeurons += layers[i];
		if(i != 0){
			nLinks += layers[i-1] * layers[i];	
		}
	}

	// printf("#neurons : %d\n", nNeurons);
	// printf("#links   : %d\n", nLinks);

	// === Setup all neurons
	// Malloc all neurons
	Neuron* neurons = (Neuron*) malloc(sizeof(Neuron) * nNeurons);   

	int atNeuron = 0;

	// For each layer
	for(iLayer = 0; iLayer < nLayers; iLayer++){

		int nInL = layers[iLayer];	// Neurons in layer
		int nIn = 0;				// Inputs per neuron
		int nOut= 0;				// Outputs per neuron

		if(iLayer > 0)
			nIn = layers[iLayer - 1];
		if(iLayer < nLayers - 1)
			nOut = layers[iLayer + 1];

		// Set layer size in network struct
		network->layerSizes[iLayer] = nInL;

		// Malloc room for pointers in layer
		network->layers[iLayer] = (Neuron**) malloc(sizeof(Neuron*) * nInL);

		// For each neuron in layer
		for(iNeuron = 0; iNeuron < nInL; iNeuron++){
			// Add pointer of neuron to layer
			network->layers[iLayer][iNeuron] = &neurons[atNeuron];
			// Init neuron
			neuron_init(&neurons[atNeuron], iLayer, iNeuron, nIn, nOut);
			atNeuron++;
		}
	}

	// === Set inputLayer and outputLayer
	network->inputLayer = network->layers[0];
	network->outputLayer = network->layers[nLayers-1];
	network->nInputs = layers[0];
	network->nOutputs = layers[nLayers-1];

	// === Setup all links
	// Malloc all links
	Link* links = (Link*) malloc(sizeof(Link) * nLinks);
	
	int atLink = 0;

	// Iterate over all layers		
	for(iLayer = 0; iLayer < nLayers - 1; iLayer++){
				
		// Get current layer
		Neuron** lCurr = network->layers[iLayer];
		// Get next layer
		Neuron** lNext = network->layers[iLayer + 1];

		int lSizeCurr = network->layerSizes[iLayer];
		int lSizeNext = network->layerSizes[iLayer + 1];

		// For each neuron in current layer
		for(iNeuronCurr = 0; iNeuronCurr < lSizeCurr; iNeuronCurr++){
			Neuron* nCurr = lCurr[iNeuronCurr];
			
			// Connect current neuron to all other neurons in next layer
			for(iNeuronNext = 0; iNeuronNext < lSizeNext; iNeuronNext++){
				Neuron* nNext = lNext[iNeuronNext];
				link_init(&links[atLink], nCurr, nNext, 1);
				
				nCurr->linksOut[iNeuronNext] = &links[atLink];
				nNext->linksIn[iNeuronCurr] = &links[atLink];

				atLink++;
			}	
		}
	}

	// network_print(network);

	printf("Network initiated\n");

	return network;

}

void network_propagate(Network* network){
	
	network_reset(network);
	
	// printf("Inputs : ");
	// printFloatArr(network->pv->inputs, network->nInputs);
	// printf("Targets: ");
	// printFloatArr(network->pv->targets, network->nOutputs);
	
	
	int i;
	for(i = 0; i < network->nOutputs; i++){
		network->pv->outputs[i] = neuron_propagate(network->outputLayer[i], network, 0);
	}
	
	// printf("Outputs: ");
	// printFloatArr(network->pv->outputs, network->nOutputs);
}

void network_train(Network* network){
	int log;
	int i, iLayer, iNeuron;
	float learningRate = 0.3;
	
	log = 0;
	// Calculate deltas for output layer
	if(log) printf("\nUpdating output layer\n");
	for(i = 0; i < network->nOutputs; i++){
		Neuron* n = network->outputLayer[i];
		float target = network->pv->targets[i];
		float out = n->out;
		
		if(log) printf("(%d,%d) %0.3f -> ", n->ilayer, n->id, n->delta);
		
		n->delta = (out - target) * out * (1 - out);
		n->bias -= n->delta * learningRate;
		
		if(log) printf(" %0.3f\n", n->delta);
	}
	

	// Update hidden layers
	for(iLayer = network->nLayers -2; 0 < iLayer; iLayer--){

		// Calculate deltas for hidden layer
		if(log) printf("\nUpdating hidden layer %d\n", iLayer);
		for(iNeuron = 0; iNeuron < network->layerSizes[iLayer]; iNeuron++){
			Neuron* n = network->layers[iLayer][iNeuron];
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
	}




	/*
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
	}*/
	
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
	int i, iLayer;
	for(i = 0; i < network->nInputs; i++){
		network->inputLayer[i]->isSet = 0;
	}
	
	for(iLayer = 1; iLayer < network->nLayers - 1; iLayer++){
		for(i = 0; i < network->layerSizes[iLayer]; i++){
			network->layers[iLayer][i]->isSet = 0;
		}
	}

	for(i = 0; i < network->nOutputs; i++){
		network->outputLayer[i]->isSet = 0;
	}
}

void network_print(Network* network){
	
	int iL = 0, iN = 0, j = 0, k = 0;		// Hold arbitrary counters
	
	int nNeurons, nLinks;			// Hold number of neurons and links

	// Hold layer sizes
	Neuron* neuron;
	Link* link;
	
	printf("\n========== Network ==========\n");
	printf("Layers : ");
	printArray(network->layerSizes, network->nLayers);

	
	for(iL = 0; iL < network->nLayers; iL++){	// For each layer
		printf("\nLayer %d\n", iL);
		nNeurons = network->layerSizes[iL];			// Get number of neurons
		
		for(iN = 0; iN < nNeurons; iN++){			// For each neuron in layer
			neuron = network->layers[iL][iN];			// Get neuron
			neuron_print(neuron);						// Print neuron
			
			nLinks = neuron->nOut;						// Get number of links
			for(k = 0; k < nLinks; k++){				// For each link of neuron
				link = neuron->linksOut[k];					// Get link
				printf("\t");
				link_print(link);							// Print link
				printf("\n");
			}
		}
	}
	
	printf("=============================\n");
	
	return;
}