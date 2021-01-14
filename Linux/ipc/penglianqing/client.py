#!/usr/bin/env python
# coding=utf-8
# client 客户端

from socket import* #socket
import os   #文件操作
import random   #random随机数
import time #延时
import re

#任务开始
print('task start.')


# 监听消息
while True:
    #延时
    time.sleep(1.0)


    
    #通过正则表达式读取数据
    receive_data = 'Server:115.00,105.00,966,359'
    (a, b, c, d ,e) = [t(s) for t,s in zip((str,float,float,int,int),re.search('^(\w+):([\d.]+),([\d.]+),(\d+),(\d+)$',receive_data).groups())]
    print (a, b, c, d ,e)

#关闭套接字

print('all done.')
