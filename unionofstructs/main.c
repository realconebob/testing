struct tester {
    char *name;
    char type;

    union exclusivestruct {
        struct obj1 {
            int age;
        } obj1;


        struct obj2 {
            char *color;
        } obj2;


        struct obj3 {
            float balance;
        } obj3;

    } excstruct;

};

#include <stdio.h>

int main() {
    struct tester guy1;
    struct tester guy2;

    guy1.name = "lmao";
    guy2.name = "lmbao";
    printf("Names: %s %s\n", guy1.name, guy2.name);

    guy1.type = 1;
    guy2.type = 3;

    guy1.excstruct.obj1.age = 50;
    guy2.excstruct.obj3.balance = 40.92;

    printf("guy1's age: %d\nguy2's balance: %f\n", guy1.excstruct.obj1.age, guy2.excstruct.obj3.balance);

    return 0;
}