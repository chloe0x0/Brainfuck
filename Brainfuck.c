#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEM_LEN 30000

// Stack struct to store indecies of open brackets (start of loops)
typedef struct{
    unsigned capacity;
    int* stack_array;
    int top;
}stack;

stack* new_stack(unsigned capacity){
    stack* Stack = malloc(sizeof(stack));
    Stack->capacity = capacity;
    Stack->stack_array = malloc( capacity * sizeof(int) );
    Stack->top = -1;
    return Stack;
}

void push(int x, stack* Stack){
    Stack->stack_array[++Stack->top] = x;
}

int pop(stack* Stack){
    return Stack->stack_array[--Stack->top];
}

int peek(stack* Stack){
    return Stack->stack_array[Stack->top];
}

void interp(const char* input){
    char memory[MEM_LEN] = {0};
    char *ptr = memory;
    int instruction_pointer = 0;
    stack* jump_stack = new_stack(strlen(input));

    while(1){
        char ch = input[instruction_pointer];
        if (ch == '>'){
            ++ptr;
        }

        if (ch == '<'){
            --ptr;
        }

        if (ch == '+'){
            if (*ptr != 254){
                ++*ptr;
            }
        }

        if (ch == '-'){
            if (*ptr != 0){
                --*ptr;
            }
        }

        if (ch == '.'){
            putchar(*ptr);
        }

        if (ch == ','){
            *ptr = getchar();
        }

        if (ch == '['){
            push(instruction_pointer, jump_stack);
        }

        if (ch == ']'){
            if (*ptr){
                instruction_pointer = peek(jump_stack);
            }
            else{
                pop(jump_stack);
            }
        }

        ++instruction_pointer;
        if (instruction_pointer >= strlen(input)+1 ){
            break;
        }
    }   
}



int main(void){
    char* input = "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.";
    printf("Interpreting %s\n", input);
    interp("++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.");
    
}