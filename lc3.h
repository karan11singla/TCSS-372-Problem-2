// lc3.h

#define FETCH 0
#define DECODE 1
#define EVAL_ADDR 2
#define FETCH_OP 3
#define EXECUTE 4
#define STORE 5

struct cpu {
	unsigned short IR;
	unsigned int PC;
	unsigned int registers[8];
};

typedef struct cpu CPU_p;
// etc.
