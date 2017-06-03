#include "FileHandler.h"

/*
	Files are not checked for consistency in any way!
*/

Network* fileHandler_readNetwork(char* filename){
	FILE* fp;					// Create file pointer
	fp = fopen(filename, "r");	// Open file for reading
	if(fp == NULL){
		printf("fileHander_readNetwork : Error while opening '%s'\n", filename);
		return NULL;
	}

	Network* network;			// Hold network
	int layerSizes[3];			// Hold layer sizes
	int i = 0, j = 0, k = 0;	// Hold arbitrary counters
	float f = 0;				// Hold float values
	
	int nNeurons, nLinks;		// Hold number of neurons and links
	
	// Read layer sizes from file
	while(i < 3 && fscanf(fp, "%d", &j) != 0){
		layerSizes[i++] = j;
	}
	// Goto next line
	fscanf(fp, ";(\n)?");
	
	// Initialize network
	network = network_init(layerSizes[0], layerSizes[1], layerSizes[2]);
	
	printf("FileHandler : %d %d %d\n", layerSizes[0], layerSizes[1], layerSizes[2]);

	// Hold pointers to layers
	Neuron** layers[3] = {
		network->inputLayer,
		network->hiddenLayer,
		network->outputLayer
	};
	
	for(i = 0; i < 3; i++){			// For each layer
		nNeurons = layerSizes[i];			// Get number of neurons
		for(j = 0; j < nNeurons; j++){		// For each neuron in layer
			Neuron* neuron = layers[i][j];		// Get neuron
			fscanf(fp, "%f", &f);				// Read bias
			neuron->bias = f;					// Set Bias
		
			nLinks = neuron->nOut;				// Get number of links
			for(k = 0; k < nLinks; k++){		// For each link of neuron
				fscanf(fp, "%f", &f);				// Read weight
				neuron->linksOut[k]->weight = f;	// Set Weight
			}
			
			fscanf(fp, ";(\n)?");		// Goto next line
		}
	}
	
	fclose(fp);					// Close file
	
	return network;
}

int fileHandler_writeNetwork(char* filename, Network* network){
	FILE* fp;					// Create file pointer
	fp = fopen(filename, "w");	// Open file for reading
	if(fp == NULL){
		printf("fileHander_writeNetwork : Error while opening '%s'\n", filename);
		return 1;
	}
	
	int i = 0, j = 0, k = 0;	// Hold arbitrary counters
	int nNeurons, nLinks;		// Hold number of neurons and links
	Neuron* neuron;				// Hold current neuron
	Link* link;					// Hold current link
	
	int layerSizes[3] = {		// Hold layer sizes
		network->nInput,
		network->nHidden,
		network->nOutput
	};
	
	Neuron** layers[3] = {		// Hold pointers to layers
		network->inputLayer,
		network->hiddenLayer,
		network->outputLayer
	};
	
	// Write network size
	fprintf(fp, "%d %d %d;\n\n", network->nInput, network->nHidden, network->nOutput);
	
	for(i = 0; i < 3; i++){				// For each layer
		nNeurons = layerSizes[i];			// Get number of neurons
		
		for(j = 0; j < nNeurons; j++){		// For each neuron in layer
			neuron = layers[i][j];				// Get neuron
			fprintf(fp, "%f", neuron->bias);	// Write neuron bias
			
			nLinks = neuron->nOut;				// Get number of links
			for(k = 0; k < nLinks; k++){		// For each link of neuron
				link = neuron->linksOut[k];			// Get link
				fprintf(fp, " %f", link->weight);	// Write link weight
			}
			
			fprintf(fp, ";\n");				// Write new line
		}
	}
	
	fclose(fp);					// Close file
	
	return 0;
}

























