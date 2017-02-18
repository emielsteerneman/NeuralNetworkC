#include <stdio.h>

#include "Link.h"

void link_print(Link* link){
	printf("Link "); neuron_str(link->from); printf("->"); neuron_str(link->to); printf(" | %0.3f", link->weight);
}

void link_init(Link* link, Neuron* from, Neuron* to, float weight){
	int log = 0;
	
	link->from = from;
	link->to = to;
	link->weight = weight;
	
	if(log)  {
		printf("  new ");
		link_print(link);
		printf("\n");
	}
}