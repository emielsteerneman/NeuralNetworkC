#ifndef MAIN_H
#define MAIN_H

// Struct declarations
typedef struct PropVals PropVals;

#include <stdio.h>
#include <stdlib.h>

#include "Neuron.h"
#include "Link.h"
#include "Network.h"

// Struct definitions
struct PropVals{
	float* inputs;
	float* targets;
	float* outputs;
};

// Functions
void printArray(int *array, int size);
float randomWeight(int range);
void printFloatArr(float *array, int size);

#endif