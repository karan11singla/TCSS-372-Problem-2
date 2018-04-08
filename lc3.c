// lc3.c


#include<stdio.h>
#include<math.h>
#include<stdlib.h>

#include "lc3.h"
// you can define a simple memory module here for this program
unsigned short memory[32]; // 32 words of memory enough to store simple program

void controller (CPU_p cpu) {
    // check to make sure both pointers are not NULL
    
    // do any initializations here
        unsigned int opcode, Rd, Rs1, Rs2, offset ;// fields for the IR

    int state = FETCH;
    // for (;;) { // efficient endless loop to be used in the next problem
        switch (state) {
            case FETCH: // microstates 18, 33, 35 in the book
                printf("Here in FETCH\n");
                // get memory[PC] into IR - memory is a global array
                cpu->ir = memory[0];
                // increment PC
                //printf("Contents of IR = %04X\n", ir);
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// put printf statements in each state and microstate to see that it is
// working
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                state = DECODE;
                break;
            case DECODE: // microstate 32
                // get the fields out of the IR
                // make sure opcode is in integer form
                // hint: use four unsigned int variables, opcode, Rd, Rs, and
                // immed7
                // extract the bit fields from the IR into these variables
                state = EVAL_ADDR;
                break;
            case EVAL_ADDR:
                // Look at the LD instruction to see microstate 2 example
                switch (opcode) {
                // different opcodes require different handling
                // compute effective address, e.g. add sext(immed7) to
                // register
                }
                state = FETCH_OP;
                break;
            case FETCH_OP:
            // Look at ST. Microstate 23 example of getting a value out of a
            // register
                switch (opcode) {
                    // get operands out of registers into A, B of ALU
                    // or get memory for load instr.
                }
                state = EXECUTE;
                break;
            case EXECUTE: // Note that ST does not have an execute microstate
                switch (opcode) {
                    // do what the opcode is for, e.g. ADD
                    // in case of TRAP: call trap(int trap_vector) routine,
                    // see below for TRAP x25 (HALT)
                }
                state = STORE;
                break;
            case STORE: // Look at ST. Microstate 16 is the store to memory
                switch (opcode) {
                    // do what the opcode is for, e.g. ADD
                    // in case of TRAP: call trap(int trap_vector) routine,
                    // see below for TRAP x25 (HALT)
                }
                state = STORE;
                break;
        }
    // if-loop }
}

int main(int argc, char* argv[]) {
    //memory[0] = argv[1];


    printf("Here");

}

unsigned int intToBinary(unsigned int num) {
    if (num == 0) return 0;
    return (num % 2) + 10 * intToBinary(num / 2);
}

unsigned int * toArray(unsigned int num) {
    int length = log10(num) + 1;
    unsigned int *arr = calloc(16, sizeof(unsigned int));
    int i;
    for (i = 15; i > 15 - length; i--) {
        arr[i] = num % 10;
        num /= 10;
    }
    return arr;
}

void setCC() {
    if (Rd == 0) {
        cc = 2;
    } else if (Rd < 0) {
        cc = 4;
    } else {
        cc = 1;
    }
}

void executeAdd(unsigned int Rd, unsigned int Rs1, unsigned int Rs2,
                unsigned int offset, unsigned int mode, unsigned int cc) {
    if (mode == 0) { // need to parse mode from IR
        Rd = Rs1 + Rs2;
    } else {
        Rd = Rs1 + offset;
    }
    setCC();
}

void executeAnd(unsigned int Rd, unsigned int Rs1, unsigned int Rs2,
                unsigned int offset, unsigned int mode, unsigned int cc) {
    unsigned int s1Bin = intToBinary(Rs1);
    unsigned int *s1Arr = toArray(s1Bin);
    unsigned int resultBin;
    unsigned int resultInt;
    int i;
    int j;
    int k;
    if (mode == 0) { // need to parse mode from IR
        unsigned int s2Bin = intToBinary(Rs2);
        unsigned int *s2Arr = toArray(s2Bin);
        for (i = 15; i >= 0; i--) {
            if (s1Arr[i] == 1 && s2Arr[i] == 1) {
                resultBin += pow(10, 15 - i);
            }
        }
    } else {
        unsigned int imm5Bin = intToBinary(offset);
        unsigned int *imm5Arr = toArray(imm5Bin);
        for (i = 15; i >= 11; i--) {
            if (imm5Arr[11] == 1) { // if first bit is 1, sign extend
                for (k = 0; k <= 10; k++) {
                    imm5Arr[k] = 1;
                }
            }
            if (s1Arr[i] == 1 && imm5Arr[i] == 1) {
                resultBin += pow(10, 15 - i);
            }
        }
    }
    unsigned int *resultArr = toArray(resultBin);
    for (j = 15; j > 0; j--) {
        resultInt += resultArr[j] * pow(10, 15 - j);
    }
    if (resultArr[0] == 1) {
        resultInt -= 32768;
    }
    Rd = resultInt;
    setCC();
}

void executeNot(unsigned int Rd, unsigned int Rs1) {
    unsigned int *s1Arr = toArray(Rs1);
    unsigned int resultInt;
    int i;
    int j;
    for (i = 0; i < 16; i++) {
        if (s1Arr[i] == 0) {
            s1Arr[i] = 1;
        } else {
            s1Arr[i] = 0;
        }
    }
    for (j = 15; j > 0; j--) {
        resultInt += s1Arr[j] * pow(10, 15 - j);
    }
    if (resultArr[0] == 1) {
        resultInt -= 32768;
    }
    Rd = resultInt;
    setCC();
}
