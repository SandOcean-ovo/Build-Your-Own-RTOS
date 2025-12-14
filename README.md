
# 🛠️ Build Your Own RTOS: 从零手写 Cortex-M3 实时操作系统

![Language](https://img.shields.io/badge/language-C%20%2F%20Assembly-blue.svg)
![Platform](https://img.shields.io/badge/platform-STM32%20%7C%20Cortex--M3-green.svg)
![License](https://img.shields.io/badge/license-MIT-orange.svg)
![Status](https://img.shields.io/badge/status-Active%20Dev-brightgreen.svg)

> **"What I cannot create, I do not understand."** — Richard Feynman

这是一个用于学习和探索实时操作系统（RTOS）底层原理的项目。
本项目不仅仅是一个 OS 内核，更是一份**循序渐进的学习指南**。我从第一行汇编代码开始，一步步构建任务切换、调度器、时间管理以及进程间通信机制。

---

## 🗺️ 项目进度与路线图 (Roadmap)

本项目严格遵循 **8 周实战指南**，并将开发过程划分为四个核心阶段。你可以通过 Git Tag 穿越回 RTOS 诞生的每一个关键时刻。

### 🏗️ Phase 1: 地基与汇编热身 (Week 1-2)
*目标：祛除对汇编的恐惧，亲眼见证硬件自动压栈过程。*

- [ ] **Week 1: 深入 Cortex-M3 架构**
  - [ ] 掌握寄存器组 (R0-R12, R13/SP, R14/LR, R15/PC)
  - [ ] 区分 **MSP** (主栈指针) 与 **PSP** (进程栈指针)
  - [ ] 汇编实战：使用 `MRS`/`MSR` 指令读取特殊寄存器

- [ ] **Week 2: 裸机中断与堆栈帧**
  - [ ] 触发 PendSV 或外部中断
  - [ ] **内存取证**：验证硬件自动压栈顺序 (`xPSR` -> `PC` -> `LR` -> `R12` -> `R3`...`R0`)
  - [ ] 理解 `EXC_RETURN` (如 `0xFFFFFFF9`) 的特殊含义

### ⚙️ Phase 2: 内核核心——上下文切换 (Week 3-4)
*目标：摆脱编译器依赖，手动“伪造”现场，实现第一次手动任务切换。*

- [ ] **Week 3: 任务定义与堆栈伪造**
  - [ ] 定义任务控制块 (**TCB**) 结构体
  - [ ] 实现 `Task_Stack_Init`：在内存中手动构建符合规范的伪造堆栈
  - [ ] 确保 `xPSR` 的 **T-bit** 正确置位 (防止 HardFault)

- [ ] **Week 4: RTOS 的心脏 (PendSV)**
  - [ ] 编写纯汇编的 `PendSV_Handler`
  - [ ] 实现核心逻辑：保存上文 (R4-R11) -> 切换栈指针 -> 恢复下文
  - [ ] 验证 `OS_Yield()`：实现 TaskA 到 TaskB 的手动切换

### ⏱️ Phase 3: 自动化调度与时间管理 (Week 5-6)
*目标：引入 SysTick 心跳，变“被动切换”为“主动抢占”。*

- [ ] **Week 5: 时间片轮转 (Round Robin)**
  - [ ] 配置 SysTick (1ms 中断周期)
  - [ ] 实现基础调度算法：`Next_Task = (Current + 1) % Count`
  - [ ] 成果：双任务 LED 自动并行闪烁

- [ ] **Week 6: 阻塞延时与空闲任务**
  - [ ] 实现 `osDelay(ticks)`：让出 CPU 算力而非死等
  - [ ] 扩展 TCB：增加 `DelayTicks` 计数器
  - [ ] 实现 **Idle Task** (空闲任务)：当所有任务延时时执行 `WFI` 低功耗指令

### 🛡️ Phase 4: 进程间通信与工程化 (Week 7-8)
*目标：解决并发竞争，完成从 Demo 到 Project 的蜕变。*

- [ ] **Week 7: 临界区与信号量**
  - [ ] 实现 `OS_EnterCritical` / `OS_ExitCritical` (关中断保护)
  - [ ] 实现 **信号量 (Semaphore)**：`OS_SemWait` / `OS_SemPost`
  - [ ] 解决生产者-消费者模型下的资源竞争问题

- [ ] **Week 8: 总结与文档**
  - [ ] 代码重构：分离 `RTOS` 内核层与 `User` 应用层
  - [ ] 绘制原理图：任务切换流程图、堆栈内存布局图

---

## 📂 目录结构 (Project Structure)

本项目遵循模块化设计，将内核代码与硬件移植层分离。

```text
MyRTOS/
├── MDK-ARM/               # Keil MDK 工程文件
├── RTOS/                  # RTOS 核心源码
│   ├── Include/           # 内核头文件 (os_core.h 等)
│   ├── Source/            # 内核逻辑实现 (调度算法、时基管理)
│   └── Portable/          # 硬件移植层 (最核心的汇编代码在这里)
│        └── ARM_CM3/      # 针对 Cortex-M3 的 PendSV 实现与栈初始化
├── Core/                  # 用户应用层 (main.c)
└── README.md              # 项目说明文档

```

---

## 🚀 如何使用 (Getting Started)

### 1. 克隆仓库
```bash
git clone https://github.com/SandOcean/Build-Your-Own-RTOS.git
```

### 2. 切换到特定学习阶段
本项目使用 Git Tag 标记每个学习阶段的完成状态。如果你想查看**第一周完成时**的纯净代码（刚搞定寄存器）：

```bash
git checkout v1.0-week1
```

如果你想查看**最新的开发进度**：

```bash
git checkout main
```

如果你想**从零开始自己写代码**：

```bash
git checkout template
```

### 3. 编译与下载
* **IDE**: Keil MDK 5
* **硬件**: STM32F103C8T6 (Cortex-M3) 或其他兼容开发板
* 打开 `Project` 目录下的工程文件，编译并下载即可。

---

## 📝 开发日志
手写RTOS的全流程，我都记录在[我的个人博客](https://www.sandocean.cn/categories/Build-Your-Own-RTOS-%E7%B3%BB%E5%88%97/)中。

---

## 🤝 贡献与反馈
这是一个个人学习项目，但非常欢迎 Issues 和 Pull Requests！如果你发现任何 Bug 或有更好的优化思路，请随时告诉我。

## 📜 许可证
本项目使用MIT License。



