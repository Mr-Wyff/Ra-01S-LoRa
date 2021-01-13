STM32F103C8T6demo

这个是sx126x的驱动demo

硬件连接(使用stm32f103c8t6芯片驱动)
PA9/10	接电脑串口
PB_12	接led指示灯
PB_13	接按键（低电平有效）
SPICS	PA_4
SPISCK	PA_5
SPIMISO	PA_6
SPIMOSI	PA_7
RADIO_RESET	PB_1
RADIO_BUSY	PA_0
RADIO_DIO1	PA_1
RADIO_DIO3	PA_11

测试方法
两个模组修改main.c文件中的bool EnableMaster=false;//主从选择
主模组设置为true，从模组设置为false，分别编译并下载
将两个模组都上电，主模组按一次按键(PB_13拉低)发送一次数据，从机收到数据后改变LED状态(按一次点亮LED，再按一次熄灭)

资源：
48脚
64K FLASH 20K RAM
定时器：高级TIM1通用TIM2.TIM3,TIM4
SPI:两个
IIC:两个
USART:3个
USB:一个2.0
CAN:一个2.0B主动
GPIO：37个
ADC:2个（10通道）

STMF103系列库函数使用都相同，高等级的兼容低等级的（低等级的功能在高等级的MCU上使用完全相同可以完全使用）
STMF103ZE有144脚，512K FLASH 64K RAM