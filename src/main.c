#include <stdio.h>
#include <stdint.h>
#include "cpu/register.h"
#include "memory/instruction.h"
#include "memory/dram.h"
#include "cpu/mmu.h"
#include "disk/elf.h"


int main(){
    //printf("test\n");
    init_handler_table();
    reg.rax = 0x12340000;
    reg.rbx = 0x0;
    reg.rcx = 0x8000660;
    reg.rdx = 0xabcd;
    reg.rsi = 0x7ffffffee2f8;
    reg.rdi = 0x1;
    reg.rbp = 0x7ffffffee210;
    reg.rsp = 0x7ffffffee1f0;

    reg.rip = (uint64_t)&program[11];
    write64bits_dram(va2pa(0x7ffffffee210), 0x08000660);
    write64bits_dram(va2pa(0x7ffffffee208), 0x0);
    write64bits_dram(va2pa(0x7ffffffee200), 0x0000abcd);
    write64bits_dram(va2pa(0x7ffffffee1f8), 0x12340000);
    write64bits_dram(va2pa(0x7ffffffee1f0), 0x80000660);

    //test
    // uint64_t pa = va2pa(0x7ffffffee210);

    // printf("%16lx\n", *(uint64_t *)(&mm[pa]));
    // printf("%16lx\n", read64bits_dram(pa));
    print_register();
    print_stack();




    // mm[va2pa(0x7ffffffee210)] = 0x08000660;//rbp
    // mm[va2pa(0x7ffffffee208)] = 0x0;
    // mm[va2pa(0x7ffffffee200)] = 0x0000abcd;
    // mm[va2pa(0x7ffffffee1f8)] = 0x12340000;
    // mm[va2pa(0x7ffffffee1f0)] = 0x80000660;//rsp

    for(int i = 0; i < 7; i++){
        instruction_cycle();
        print_register();
        print_stack();

    }

    //verify

    int match = 1;

    match = match && (reg.rax == 0x1234abcd);
    match = match && (reg.rbx == 0x0);
    match = match && (reg.rcx == 0x8000660);
    match = match && (reg.rdx == 0x12340000);
    match = match && (reg.rsi == 0xabcd);
    match = match && (reg.rdi == 0x12340000);
    match = match && (reg.rbp == 0x7ffffffee210);
    match = match && (reg.rsp == 0x7ffffffee1f0);
    //match = match && (reg.rbx == 0x0);
    if(match == 1){
        printf("Register match\n");
    }
    else{
        printf("Register not match\n");

    }
    match = match && (read64bits_dram(va2pa(0x7ffffffee210)) == 0x8000660);
    match = match && (read64bits_dram(va2pa(0x7ffffffee208)) == 0x1234abcd);
    match = match && (read64bits_dram(va2pa(0x7ffffffee200)) == 0xabcd);
    match = match && (read64bits_dram(va2pa(0x7ffffffee1f8)) == 0x12340000);
    match = match && (read64bits_dram(va2pa(0x7ffffffee1f0)) == 0x80000660);//rsp
    if(match == 1){
        printf("Memory match\n");
    }
    else{
        printf("Memory not match\n");

    }


    return 0;
}