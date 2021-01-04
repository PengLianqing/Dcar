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

#　确定本地套接字文件
sock_file = "./sock"

#　判断文件是否存在，存在就删除
if os.path.exists(sock_file):
    os.remove(sock_file)

#　创建本地套接字
sockfd = socket(AF_UNIX,SOCK_STREAM)

#　绑定本地套接字
sockfd.bind(sock_file)

# 连接服务器端
serv_file = "../server/serv.socket"
sockfd.connect(serv_file)

# 监听消息
while True:
    #延时
    time.sleep(1.0)

    #发送数据'x:%4.2f,%d'%(random.random(),random.randint(1,10))
    tramsmit_data='client:%4.2f,%4.2f,%d,%d'%(random.random(),random.random(),random.randint(1,10),random.randint(1,10))
    sockfd.send(tramsmit_data)
    print(tramsmit_data)

    #接收数据
    receive_data = sockfd.recv(1024)
    if not receive_data:
        break
    print(receive_data)
    
    #通过正则表达式读取数据
    #receive_data = 'Server:115.00,105.00,966,359'
    (a, b, c, d ,e) = [t(s) for t,s in zip((str,float,float,int,int),re.search('^(\w+):([\d.]+),([\d.]+),(\d+),(\d+)$',receive_data).groups())]
    print (a, b, c, d ,e)

#关闭套接字
sockfd.close()

print('all done.')
