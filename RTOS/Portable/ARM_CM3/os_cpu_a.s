;********************************************************************************
; file: os_cpu_a.s
; brief:   RTOS 的底层汇编接口
;********************************************************************************

; 1. 声明：承诺堆栈是 8 字节对齐的
    PRESERVE8

; 2. 声明：我们要使用 Thumb 指令集 (Cortex-M3 必须用这个)
    THUMB

; 3. 定义段 (Section)
;    AREA: 告诉汇编器这是一段代码
;    |.text|: 段的名字 (C语言编译出来的代码通常也放在这个段)
;    CODE: 类型是代码
;    READONLY: 只读 (放在 Flash 里)
    AREA    |.text|, CODE, READONLY

; 4. 导出符号 (相当于 C 语言的头文件声明，让别人能调用它)
;    PendSV_Handler 是 STM32 启动文件里默认的中断名
    EXPORT  PendSV_Handler


; 5. 引入外部符号 (相当于 C 语言的 extern，我们要访问 C 里的变量)
    IMPORT  CurrentTCB  ; 在 C 里定义的全局变量叫 CurrentTCB
    IMPORT  NextTCB


;===============================================================================
; 函数实现
;===============================================================================

; -----------------------------------------
; 函数：PendSV_Handler
; -----------------------------------------
PendSV_Handler  PROC  ; PROC代表函数的开头
    CPSID I ; 关中断
    MRS R0, PSP
    ISB ; 指令同步隔离，确保程序生效

    LDR R2, =CurrentTCB ; 现在R2里存的是CurrentTCB的地址
    LDR R1, [R2] ; 把R2（CurrentTCB）地址中所存的值（就是TCB的首地址，也就是sp变量的地址）存到R1里

    CMP R1, #0          ; 比较R1和0
    BEQ RestoreContext  ; 如果相等 (Z标志位为1)，直接跳转到恢复上下文部分

    STMDB R0!, {R4-R11}
    STR R0, [R1] ; 把现在的R0（也就是PSP最终指向的地址）存到R1指向的地址（也就是存进sp变量）

RestoreContext
    LDR R2, =NextTCB ; 现在R2里存的是NextTCB的地址
    LDR R3, =CurrentTCB ; 现在R3里存的是CurrentTCB的地址
    LDR R1, [R2] ; 把R2（NextTCB）地址中所存的值存到R1里
    STR R1, [R3] ; 把R1（NextTCB的sp变量）存到CurrentTCB的地址所对应的内存中，现在CurrentTCB已经是新的任务了
    LDR R0, [R1] ; 从R1（NextTCB的sp变量）所对应的内存中读取NextTCB（实际上就是CurrentTCB）的sp变量到R0
    LDMIA R0!, {R4-R11}
    MSR PSP, R0
    ORR LR, LR, #0x04   ; 将LR的第2位置1，返回时使用PSP
    CPSIE I
    BX LR
    ENDP






; 6. 文件结束 (必须有，且必须放在最后一行)
    END