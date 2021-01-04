#!/usr/bin/env python
# coding=utf-8

'''
    发送端
'''

from socket import *

#　确保两边使用相同的套接字文件
#sock_file = "./sock"
sock_file = "../server/serv.socket"

sockfd = socket(AF_UNIX,SOCK_STREAM)
sockfd.connect(sock_file)

while True:
    msg = input(">>")
    if not msg:
        break
    sockfd.send(msg.encode())

sockfd.close()