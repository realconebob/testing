#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>

int main() {
	int echeck;
	int rnum;

	echeck = getrandom(&rnum, sizeof(rnum), 0);
    	if(echeck < 1) {
        	perror("Couldn't generate random number");
        	exit(EXIT_FAILURE);
    	}
    	rnum = rnum % (100 - 0) + 1 + 0;
    	// Linear transformation to eliminate bias in generation
    	//rnum = (PORT_MAX - IPPORT_USERRESERVED) * (rnum - IPPORT_USERRESERVED) / (PORT_MAX - IPPORT_USERRESERVED) + IPPORT_USERRESERVED;

	printf("%d\n", rnum);

	int nums[100];

	return 0;
}
