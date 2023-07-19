#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <bits/elf_arm.h>
#include <elf.h>
#include <dlfcn.h>
#include <sys/mman.h>

uint32_t ret_addr;
uint32_t arg0;
uint32_t arg1;

/*
0   cpsr       ------>sp
4   R0
8   R1
c   R2
10  R3
14  R4
18  R5
1c  R6
20  R7
24  R8
28  R9
2c  R10
30  R11
34  R12
38  LR
3c  PC
----------
40  R4     ---> hook 前sp的位置
    R5
    R6
    LR
*/

void  fun3() {
    //printf("handle  ....\n");
    asm("STMFD sp!, {R4-R6,LR}");
    asm("sub sp , sp , #0x10"); 
    asm("STMFD sp!, {R0-R12, LR, PC}");
    asm("mrs r0, cpsr");
    asm("STMFD sp!, {R0}");


    asm("ldr r4, [sp, #4]");
    asm("str r4, %0":"=m"(arg0));
    asm("ldr r4, [sp, #8]");
    asm("str r4, %0":"=m"(arg1));

    asm("ldr r9, %0"::"m"(ret_addr));
    asm("str r9, [sp, #0x3c]");

    //asm("add sp , sp , #0x30"); // sub sp, sp, #0x10 
    //mov r5, r0
    asm("ldr r0, [sp, #0x4]");
    asm("str r0, [sp, #0x18]");

    asm("ldr r0, =0x3f7b8");



    // asm("ldr r0, [sp, #4]");
    // asm("str r0, [sp, #0x1c]");
    // asm("ldr r0, [sp, #8]");
    // asm("str r0, [sp, #4]");
    // asm("mov r0, sp");
    // asm("add r0, r0, #0x44");
    // asm("str r0, [sp, #8]");



    asm("LDMFD sp!, {R0}");
    asm("msr cpsr, r0");
    asm("ldr r0, =0x3f7b8");
    asm("add sp , sp , #0x4"); 
    asm("LDMFD sp!, {R1-R12,LR,PC}");

}

void fun2(int a , int b){
    // asm("push {R0,R1, R4-R6, LR}");
    // asm("mov R0, #12");
    // asm("mov R2, R0");
    // asm("add R2, R2, #123");    
    // asm("add R1, R2, #123");
    // asm("add R0, R1, #123");
    // asm("mov LR, #0");
    //asm("mov sp, #0");    
    // for(int i = 0; i < 5; i ++){
    //    printf("%d\n", a+b);
    // }
    // printf("%d\n", a+b);
    // getchar();


}

int fun(int a, int b, int c, int d, int e, int f){
    printf("%d\n", a);
    int n = d + e ;
    int k = PF_ARM_ENTRY;
    fun2(a, b);
    return n;
}

int main(){

    // while (1)
    // {
       printf("hello\n"); 
    //    getchar();
    //    int n = 7;
    //    fun(1,2,3,4,5,6);
   // }
    

    void* handle = dlopen("libc.so", RTLD_NOW);
    void* hook_addr = dlsym(handle, "fopen");
    mprotect((void*)((int)hook_addr & 0xfffff000) , 0x1000, PROT_READ|PROT_EXEC|PROT_WRITE);

    printf("hook_addr = %p\n", hook_addr);
    printf("fun3 = %p\n", fun3);
    *(uint32_t *)(hook_addr - 1) = 0xf000f8df;
    *(uint32_t *)(hook_addr - 1 + 4)  = (uint32_t)fun3;
    ret_addr = (uint32_t)hook_addr - 1 + 8 + 1;
    //-1 是因为开始是基数地址，+1 是thumb模式
    getchar();

    FILE *fp = fopen("/data/local/tmp/android_server", "rb");
    uint32_t data;
    fread(&data, 4, 1, fp);
    fclose(fp);
    printf("data : %p\n", fp);


    printf("arg0 : %s\n", arg0);
    printf("arg1 : %s\n", arg1);

    return 0 ;
}

