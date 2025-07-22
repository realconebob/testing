#include <stdio.h>

#define LISTOFstuff \
	X(value1) \
	X(value2) \
	X(value3)

#define X(name) int name;
LISTOFstuff
#undef X

void printvars(void) {
	#define X(name) printf("%s = %d\n", #name, name);
	LISTOFstuff
	#undef X
}



#define FORLISTOFstuff \
	DO(id1, name1) \
	DO(id2, name2) \
	DO(id3, name3)

#define DEFINE_NAME_VAR(id, name, ...) int name;
FORLISTOFstuff(DEFINE_NAME_VAR)

void printvars2(void) {
	#define PRINTNAMEANDVAL(id, name, ...) printf("%s = %d\n", #name, name);
	FORLISTOFstuff(PRINTNAMEANDVAL)
}

int main(void) {
	value1 = 0;
	value2 = 1;
	value3 = 2;

	printvars();
	return 0;
}
