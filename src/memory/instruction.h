#ifndef instruction_guard
#define instruction_guard

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define NUM_INSTRTYPE 30


typedef enum operator{
    mov_reg_reg,
    mov_reg_mem,
    mov_mem_reg,
    push_reg,
    pop_reg,
    call,
    ret,
    add_reg_reg,
}op_t;
typedef enum operand_type{
    EMPTY,
    IMM, REG, MM_REG, MM_IMM_REG, MM_REG1_REG2, MM_IMM_REG1_REG2, MM_REG2_S, MM_IMM_REG2_S, MM_REG1_REG2_S, MM_IMM_REG1_REG2_S
}od_t;

typedef struct operand_all
{
    od_t type;
    int64_t imm;
    int64_t scal;
    uint64_t *reg1;
    uint64_t *reg2;

}od_all_t;

typedef struct instruct{
    op_t op;
    od_all_t src;
    od_all_t dst;
    char code[100];
}inst_t;

uint64_t decode_od (od_all_t od);

typedef void(*handler_t)(uint64_t, uint64_t);
handler_t handler_table[NUM_INSTRTYPE];


void instruction_cycle();
void init_handler_table();


void mov_reg_reg_handler(uint64_t src, uint64_t dst);
void mov_reg_mem_handler(uint64_t src, uint64_t dst);
void call_handler(uint64_t src, uint64_t dst);
void push_reg_handler(uint64_t src, uint64_t dst);
void pop_reg_handler(uint64_t src, uint64_t dst);
void add_reg_reg_handler(uint64_t src, uint64_t dst);


#endif