#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEM_LEN 30000
#define STACK_SIZE 100

/*		
	|| instruction Set ||

		> : Increment the data pointer to the right by one cell
			| before command 
			[0, 0, 0, 0,]
			 ^
			| after command
			[0, 0, 0, 0]
			    ^
		< : Decrement the data pointer to the left by one cell:
			| before command 
			[0, 0, 0, 0,]
			    ^
			| after command
			[0, 0, 0, 0]
			 ^
		+ : Increment the cell value at the data pointer
			| before command
			[0, 0, 0, 0]
			 ^ 
			| after command
			[1, 0, 0, 0]
		- : Decrement the cell value at the data pointer
			| before command
			[1, 0, 0, 0]
			 ^ 
			| after command
			[0, 0, 0, 0]
			 ^
		. : prints the current cell value (implemented in C as putchar(*ptr); )
			| before command
			[65, 0, 0, 0,]
			 ^ 
			| after command
			[65, 0, 0, 0]
			 ^ 
			>>> A 
		, : reads a char as input and inserts it to the current cell
		  | before command
		  [0, 0, 0, 0]
		   ^
		  | after command
		  >>> reads in 'A'
		  [65, 0, 0, 0]
		[ : Marks the begining of a loop

		] : Marks the end of a loop

		(loops execute as long as the Memory pointer is nonzero)
		C equivalent:
			while(*ptr){
				// code
			}

	>>> Example Programs/ Constructs <<<
		Hello World: ++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.
		Transfer cell value over to the cell to its right: [->+<]
	

	!!! Resources <3 !!!

		Wonderful Stack Overflow question on the implementation of loops:
			https://stackoverflow.com/questions/2588163/implementing-brainfuck-loops-in-an-interpreter
				Answer:
					https://stackoverflow.com/a/2588195
			(Marked as a duplicate of)
				https://stackoverflow.com/questions/1055758/creating-a-brainfuck-parser-whats-the-best-method-of-parsing-loop-operators

		Wikipedia Page:
			https://en.wikipedia.org/wiki/Brainfuck
		
		Brainfuck Visualizer. Used For Debugging this Interpreter:
			https://fatiherikli.github.io/brainfuck-visualizer/#KysrKysrKytbPisrKytbPisrPisrKz4rKys+Kzw8PDwtXT4rPis+LT4+K1s8XTwtXT4+Lg==
		
		For information on the Stack Data Structure:
			https://en.wikipedia.org/wiki/Stack_(abstract_data_type)
*/

// Stack struct. Empty values are defaulted to 0
typedef struct{
	unsigned int capacity; // the max capacity of the stack
	int head; // the index of the head of the stack
	int* array; // the array the stack operates on
}stack;

stack constr_stack(int capacity){
	stack stc;

	stc.capacity = capacity;
	stc.head = -1;
	stc.array = malloc( capacity * sizeof(int) );

	return stc;
}

// returns the top element of the stack
int peek(stack* stc){
	return stc->array[stc->head];
}

void push(stack* stc, int x){
	if (stc->head+1 >= stc->capacity){
		fprintf(stderr, "Error: Stack capacity exceeded. {Stack Capacity=%d, Current Size=%d}", stc->capacity, stc->head);
	}

	stc->array[++stc->head] = x;
}

// pop the head value off the stack
void pop(stack* stc){
	stc->array[stc->head] = 0;
	--stc->head;
}

// interpret BF code
void terp(const char* bf){
	char MEM[MEM_LEN] = {0};
	char* ptr = MEM;
	size_t ip = 0;
	
	stack loop_stack = constr_stack(STACK_SIZE);
	
	while (ip < strlen(bf)){
		switch (bf[ip]){
			case '>':
				++ptr;
				break;
			case '<':
				--ptr;
				break;
			case '+':
				++*ptr;
				break;
			case '-':
				--*ptr;
				break;
			case '.':
				putchar(*ptr);
				break;
			case ',':
				*ptr = getchar();
				break;
			case '[':
				if (*ptr){
					push(&loop_stack, ip);
				}
				else{
					// search for next matching ] token
					int brackets = 1;
					while (brackets){
						switch(bf[++ip]){
							case '[':
								++brackets;
								break;
							case ']':
								--brackets;
								break;
							default:
								break;
						}
					}
				}
				break;
			case ']':
				if (*ptr){
					ip = peek(&loop_stack);
				}
				else{
					pop(&loop_stack);
				}
				break;
			default:
				break;
		}
		++ip;
	}
}

/* TODO
	Interpret .bf Files
	Further test the interpreter
	Pre-compute the jump table before interpreting the code
*/

// Brainfuck -f <filepath>
// Brainfuck -i <string>
int main(int argc, char* argv[]){
	char* input;

	if (!strcmp(argv[1], "-f")){
		char* path = argv[2];

		FILE *fp;
		fp = fopen(path, "r");
		fscanf(fp, "%s", input);

	}else if (!strcmp(argv[1], "-i")){
		input = argv[2];
	}

	terp(input);
}
