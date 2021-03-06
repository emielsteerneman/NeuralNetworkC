#include "main.h"

// Neuron* inputLayer[nInput];
// Neuron* hiddenLayer[nHidden];
// Neuron* outputLayer[nOutput];
// Link* links[nLinks];

int main(int argc, char* argv[]){
	// Seed random
	srand(time(NULL));

	// Handle arguments
	int iterations = 20000000;
	char* filename = NULL;
	
	int fflag = 0;
	
	int flags, opt;
	int nsecs, tfnd;

	while ((opt = getopt(argc, argv, "n:f:")) != -1) {
		switch (opt) {
		case 'n':
			iterations = atoi(optarg);
			printf("iteration: %d\n", iterations);
			break;
		case 'f':
			fflag = 1;
			filename = optarg;
			printf("filename %s\n", filename);
			break;
		case '?':
			if (optopt == 'n' || optopt == 'f')
				fprintf (stderr, "Option -%c requires an argument.\n", optopt);
			else if (isprint (optopt))
				fprintf (stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
			break;
		default: /* '?' */
			fprintf(stderr, "Usage: %s [-t nsecs] [-n] name\n", argv[0]);
			exit(EXIT_FAILURE);
	   }
	}

	Network* network;
	
	if(fflag){
		network = fileHander_readNetwork(filename);
		if(network == NULL){
			printf("File does not contain a neural network\n");
			exit(-1);
		}
	}else{
		printf("\nNew network\n");
		network = network_init(2, 5, 1);
	}
	
	// network_print(network);
	
	int i;
	for(i = 0; i < iterations; i++){
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
		
		if(i % (iterations/100) == 0){
			int counter = 0;
			int progress = i / (iterations/100);
			
			printf("[");
			for(counter = 0; counter < 100; counter++){
				if(counter <= progress) printf("-");
				else printf(" ");
			}
			printf("] %d \r", i); fflush(stdout);
		}
		
		if(iterations - i <= 4){
			printf("\n");
			printf("Inputs : ");
			printFloatArr(network->pv->inputs, network->nInput);
			printf("Targets: ");
			printFloatArr(network->pv->targets, network->nOutput);
			printf("Outputs: ");
			printFloatArr(network->pv->outputs, network->nOutput);
			printf("\n");
		}
	}
	
	// network_print(network);
	
	// fileHander_writeNetwork("networks/XOR.txt", network);
	
	exit(EXIT_SUCCESS);
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
