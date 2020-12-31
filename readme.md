v1.实现功能
    stm32f407：
        1.dbus遥控器通信
        2.can通信，控制DJI M6020电机
        3.串级pid控制电机
        4.与beagbone进行rs232通信
    beagbone：
        1.实现了rs232的双向通信
    *展望功能：
        1.canopen通信
        2.freertos
        3.beagbone实现稳定的进程间通信
        4.串口调参
    2020.12.31

    问题：
        beagbone发送二进制丢包严重且无法触发空闲中断。

v2.实现功能