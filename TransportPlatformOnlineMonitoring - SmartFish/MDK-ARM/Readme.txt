2018/02/01
1.修复ADXL345读取数据的bug，原因一方面在于高低8位数据转换出错，另一方面在于读取三轴加速度数据时
误将设备ID寄存器当作数据起始寄存器，最后一个原因是读取ADXL345时将欲读取寄存器做了不必要的转换使得
转换结果出错。

2018/01/31
1.修复printf发送时无法发送最后一个字符的bug，原因在于fputc函数没有等待最后一个字符发送完成。
2.修改工程文件结构，将每一个任务单独存放于一个.c与.h文件中，便于代码的编写。

2018/01/26
1.添加18B20温度传感器驱动代码，在使用过程中要注意使用临界区代码。原因是其驱动使用了过多的
延时函数。

2018/01/19
1.通过分析，NandFlash的扇区大小虽然为512,但是其读取是按页读取，所以不能选择按扇区读取的方
式进行读取，这样会产生大量的重复读取工作，从而导致NandFlash的寿命成倍数的降低，所以将其扇
区大小设置成2048，即一个页的大小即可。

2018/01/18
1.完成对485芯片的调试工作
2.完成加速度传感器验证工作
3.修改ADXL345读取、写入函数，以适应两块ADXL345
4.增加Fatfs文件系统  注意 FF_FS_NORTC 这个宏定义跟时间有关

2018/01/17
1.根据新的电路板更改部分程序驱动。
2.增加485通信驱动，完成发送函数编写，需要编写接收函数，入队和出队的规则等。（考虑是否使用
  DMA发送）

2018/01/08
1.STM32F4Discovery开发板，由于PA9复用成了USB-OTG的vbus，线路上有电容，导致不能正常用作USART
2.Sim868命令发送与检查驱动完成，采用DMA方式

2017/12/27
1.添加SD卡驱动文件
2.添加NandFlash驱动文件
3.增加命令，初始化时选择SD卡或者Flash
4.完成对NandFlash的驱动程序的编写与测试

2017/12/26
1.完成硬件IIC的配置与调试，根据电路以及双寻址模式意义，使用两块ADXL345时
也可以禁止双寻址模式。
2.完成串口1、串口6的配置
3.

2017/12/07
1.新建工程，模板按照STM32CubeMX工程文件创建
2.排除植入FreeRTOS后出现的问题，问题原因是因为STM32F4xx.h库中存在着
部分宏定义，使用了“U”导致该问题。问题是：
port.c(483)_ error_ A1586E_ Bad operand types (..
3.使能IO口
4.使能定时器2，调试中断函数
5.使能串口1并测试
6.初始化SPI\I2C\IWDG等模块
7.新增ReadMe.txt文件


