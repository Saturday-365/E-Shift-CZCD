# 软件工程说明

## 当前维护版本

`Eshift_byCyberMotor_F407` 是当前唯一维护的主工程，目标芯片为 STM32F407VET6。

主要入口：

- `STM32F407VET6_CUBMX.ioc`：CubeMX 外设与时钟配置。
- `MDK-ARM/STM32F407VET6_CUBMX.uvprojx`：Keil MDK 工程。
- `Core/Src/main.c`：初始化与主循环。
- `Sourse_Library/1_SA_Library/SA_E_Shift.c`：换挡流程。
- `Sourse_Library/1_SA_Library/SA_CANDataprocess.c`：ECU CAN 数据解析。
- `Sourse_Library/4_XMCyberGear_Libary/`：CyberGear 通信与控制。

> `Sourse_Library` 是早期工程沿用的目录名。为了避免一次性破坏 Keil 工程引用，当前暂未更名。

## Legacy 工程

`legacy` 只用于保存早期方案和迁移依据，不作为新功能开发入口。

| 目录 | 用途 | 状态 |
| --- | --- | --- |
| `legacy/BasicTEST` | F103 外设与电机基础测试 | 停止维护 |
| `legacy/Eshift_byCyberMotor_F103` | 早期 F103 换挡方案 | 停止维护 |

新贡献应基于 F407 主工程。若需修改引脚、时钟或外设，请在 CubeMX 中修改 `.ioc` 后重新生成，不直接修改生成的外设配置代码。
