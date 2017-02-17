#include "main.h"

// Neuron* inputLayer[nInput];
// Neuron* hiddenLayer[nHidden];
// Neuron* outputLayer[nOutput];
// Link* links[nLinks];

int main(int argc, char* argv[]){
	// Seed random
	srand(time(NULL));

	int nInput = 2;
	int nHidden = 2;
	int nOutput = 1;
	
	if(argc > 2){
		nInput = atoi(argv[1]);
		nHidden = atoi(argv[2]);
		nOutput = atoi(argv[3]);
	}

	Network* network = network_init(nInput, nHidden, nOutput);
	
	int i;
	for(i = 0; i < 1000000; i++){
		int iPv = i % 4;
		
		if(iPv == 0){
			network->pv->inputs[0] = 0.0f;
			network->pv->inputs[1] = 0.0f;
			network->pv->targets[0] = 0.0f;
		}else
		if(iPv == 1){
			network->pv->inputs[0] = 0.0f;
			network->pv->inputs[1] = 1.0f;
			network->pv->targets[0] = 1.0f;
		}else
		if(iPv == 2){
			network->pv->inputs[0] = 1.0f;
			network->pv->inputs[1] = 0.0f;
			network->pv->targets[0] = 1.0f;
		}else
		if(iPv == 3){
			network->pv->inputs[0] = 1.0f;
			network->pv->inputs[1] = 1.0f;
			network->pv->targets[0] = 0.0f;
		}
		
		network_propagate(network);
		network_train(network);
		
		if(1000000 - i <= 4){
			printf("\n");
			printf("Inputs : ");
			printFloatArr(network->pv->inputs, network->nInput);
			printf("Targets: ");
			printFloatArr(network->pv->targets, network->nOutput);
			printf("Outputs: ");
			printFloatArr(network->pv->outputs, network->nOutput);
		}
	}
	return 0;
}

void printFloatArr(float *array, int size){
	int i;
	printf("[");
	for(i = 0; i < size-1; i++){
		printf("%0.3f, ", array[i]);
	}
	printf("%0.3f]\n", array[i]);
}

void printArray(int *array, int size){
	int i;
	printf("[");
	for(i = 0; i < size-1; i++){
		printf("%d, ", array[i]);
	}
	printf("%d]\n", array[i]);
}

float randomWeight(int range){
	return ((float)rand()/(float)(RAND_MAX)) * 2 * range - range;
}
