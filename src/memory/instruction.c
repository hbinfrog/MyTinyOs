#include "memory/instruction.h"
#include "cpu/mmu.h"
#include "cpu/register.h"
#include "memory/dram.h"

uint64_t decode_od (od_all_t od){
    if(od.type == IMM){
        return *((uint64_t *)&od.imm);
    }
    else if(od.type == REG){
        return (uint64_t)od.reg1;
    }
    else{
        uint64_t vaddr;
        switch (od.type) {
            case MM_REG:
                vaddr = od.imm;
                break;
            case MM_IMM_REG:
                vaddr = od.imm + *(od.reg1);
                break;
            case MM_REG1_REG2:
                vaddr = *(od.reg1) + *(od.reg2);
                break;
            case MM_IMM_REG1_REG2:
                vaddr = od.imm + *(od.reg1) + *(od.reg2);
                break;
            case MM_REG2_S:
                vaddr = (*(od.reg2)) * od.scal;
                break;
            case MM_IMM_REG2_S:
                vaddr = od.imm + (*(od.reg2)) * od.scal;
                break;
            case MM_REG1_REG2_S:
                vaddr = *(od.reg1) + (*(od.reg2)) * od.scal;
                break;
            case MM_IMM_REG1_REG2_S:
                vaddr = od.imm + *(od.reg1) + (*(od.reg2)) * od.scal;
                break;


            default:
                vaddr = 0;
                break;
        }
        return vaddr;
    }
}

void instruction_cycle(){
    inst_t *instr = (inst_t *)reg.rip;
    uint64_t src = decode_od(instr->src);
    uint64_t dst = decode_od(instr->dst);
    handler_t handler = handler_table[instr->op];
    handler(src, dst);
}

void init_handler_table(){
    handler_table[add_reg_reg] = &add_reg_reg_handler;
    handler_table[add_reg_mem] = &add_reg_mem_handler;
    handler_table[call] = &call_handler;
    handler_table[push_reg] = &push_reg_handler;
    handler_table[pop_reg] = &pop_reg_handler;
    handler_table[mov_reg_reg] = &mov_reg_reg_handler;

}

//mov
void mov_reg_reg_handler(uint64_t src, uint64_t dst){
    *(uint64_t *) dst =  *(uint64_t *)src;
    reg.rip = reg.rip + sizeof(inst_t);
}
void mov_reg_mem_handler(uint64_t src, uint64_t dst){
    write64bits_dram(va2pa(dst), *(uint64_t *)src);
    reg.rip = reg.rip + sizeof(inst_t);
}


//call
void call_handler(uint64_t src, uint64_t dst){
    reg.rsp = reg.rsp - 8;
    write64bits_dram(va2pa(reg.rsp), reg.rip + sizeof(inst_t));
    reg.rip = src;
}

void push_reg_handler(uint64_t src, uint64_t dst){
    reg.rsp = reg.rsp - 8;
    write64bits_dram(va2pa(reg.rsp), *(uint64_t *)src);
    reg.rip = reg.rip + sizeof(inst_t);
    
}

void pop_reg_handler(uint64_t src, uint64_t dst){


    dst = read64bits_dram(va2pa(reg.rsp));
    reg.rsp = reg.rsp + 8;
    reg.rip = reg.rip + sizeof(inst_t);
    
}

//add
void add_reg_reg_handler(uint64_t src, uint64_t dst){
    *(uint64_t *) dst = *(uint64_t *) dst + *(uint64_t *)src;
    reg.rip = reg.rip + sizeof(inst_t);
}