// lc3.c


#include<stdio.h>
#include<math.h>
#include<stdlib.h>

#include "lc3.h"
// you can define a simple memory module here for this program
unsigned short memory[32]; // 32 words of memory enough to store simple program

signed int twosCBinToInt(unsigned int num) {
    unsigned int n = num;
    signed int result;
    if (n / 1000000000000000 == 1) {
        n -= 1000000000000000;
        result -= 32768;
    }
    int i;
    for (i = 14; i >= 0; i--) {
        if (n / pow(10, i) == 1) {
            n -= pow(10, i);
            result += pow(2, i);
        }
    }
    return result;
}

unsigned int * toArray(unsigned int num) {
    unsigned int n = num;
    unsigned int length = log10(num) + 1;
    unsigned int *arr = calloc(16, sizeof(unsigned int));
    int i;
    for (i = 15; i > 15 - length; i--) {
        arr[i] = n % 10;
        n /= 10;
    }
    return arr;
}

unsigned int executeNot(unsigned int Rs1) {
    unsigned int *s1Arr = toArray(Rs1);
    unsigned int resultBin;
    int i;
    int j;
    for (i = 0; i < 16; i++) {
        if (s1Arr[i] == 0) {
            s1Arr[i] = 1;
        } else {
            s1Arr[i] = 0;
        }
    }
    for (j = 15; j >= 0; j--) {
        resultBin += s1Arr[j] * pow(10, 15 - j);
    }
    return resultBin;
}

unsigned int twosCIntToBinary(signed int num) {
    int n = num;
    int neg = 0;
    if (num == 0) {
        return 0;
    } //else if (num == -1) {
        //return 1111111111111111;
    //}
    if (num < 0) {
        neg = 1;
        n = -1 * num;
    }
    unsigned int bin;
    while (n != 0) {
        bin *= 10;
        bin += (n % 2);
        n /= 2;
    } // bin = binary rep of |num|
    if (neg) {
        unsigned int bin2 = executeNot(bin);
        unsigned int *arr = toArray(bin2);
        int i = 15;
        while (arr[i] == 1 && i >= 0) {
            arr[i] = 0;
            i--;
        }
        arr[i] = 1;
        bin2 = 0;
        int j;
        for (j = 15; j >= 0; j--) {
            bin2 += arr[i] * pow(2, 15 - j);
        }
        return bin2;
    }
    return bin;
}

unsigned int executeAdd(unsigned int Rs1, unsigned int Rs2,
                        unsigned int offset, unsigned int mode) {
    signed int resultInt;
    unsigned int o = offset;
    if (mode == 0) {
        resultInt = twosCBinToInt(Rs1) + twosCBinToInt(Rs2);
    } else {
        if (offset / 10000 == 1) {
            o += 1111111111100000;
        }
        resultInt = twosCBinToInt(Rs1) + twosCBinToInt(o);
    }
    return twosCIntToBinary(resultInt); // ???
}

unsigned int executeAnd(unsigned int Rs1, unsigned int Rs2,
                        unsigned int offset, unsigned int mode) {
    unsigned int *s1Arr = toArray(Rs1);
    unsigned int resultBin;
    unsigned int o = offset;
    int i;
    if (mode == 0) { // need to parse mode from IR
        unsigned int *s2Arr = toArray(Rs2);
        for (i = 15; i >= 0; i--) {
            if (s1Arr[i] == 1 && s2Arr[i] == 1) {
                resultBin += pow(10, 15 - i);
            }
        }
    } else {
        if (offset / 10000 == 1) {
            o += 1111111111100000;
        }
        unsigned int *imm5Arr = toArray(o);
        for (i = 15; i >= 11; i--) {
            if (s1Arr[i] == 1 && imm5Arr[i] == 1) {
                resultBin += pow(10, 15 - i);
            }
        }
    }
    return resultBin;
}

unsigned int setCC(unsigned int Rd) {
    if (twosCBinToInt(Rd) == 0) {
        return 2;
    } else if (twosCBinToInt(Rd) < 0) {
        return 4;
    } else {
        return 1;
    }
}

void controller (CPU_p *cpu) {
    // check to make sure both pointers are not NULL
    // do any initializations here
    // initialize registers r0-r7
        cpu->PC = 0; // set starting address of PC
        unsigned int opcode, Rd, Rs1, Rs2, offset;// fields for the IR
        unsigned int cc; // condition codes
        unsigned int mode;
        int state = FETCH;
    // for (;;) { // efficient endless loop to be used in the next problem
        switch (state) {
            case FETCH: // microstates 18, 33, 35 in the book
                printf("Here in FETCH\n");
                // get memory[PC] into IR - memory is a global array
                cpu->IR = memory[cpu->PC];
                // increment PC
                cpu->PC++;
                printf("Contents of IR = %04X\n", cpu->IR);
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
                    case 1:
                        Rd = executeAdd(Rs1, Rs2, offset, mode);
                        cc = setCC(Rd);
                        break;
                    case 5:
                        Rd = executeAnd(Rs1, Rs2, offset, mode);
                        cc = setCC(Rd);
                        break;
                    case 9:
                        Rd = executeNot(Rs1);
                        cc = setCC(Rd);
                        break;
                    // do what the opcode is for, e.g. ADD
                    // in case of TRAP: call trap(int trap_vector) routine,
                    // see below for TRAP x25 (HALT)
                }
                state = STORE;
                break;
            case STORE: // Look at ST. Microstate 16 is the store to memory
                switch (opcode) {
                    // write back to register or store MDR into memory
                }
                // do any clean up here in prep for the next complete cycle
                state = FETCH;
                break;
        }
    // if-loop }
}

int main(int argc, char* argv[]) {
    char *garbage;
    memory[0] = strtol(argv[1],&garbage,16);
    printf("Here");
    CPU_p cpu;
    controller(&cpu);
}
