"""
介绍：电脑运行代码
功能：监听esp32是否发送数据给电脑
      收到数据后自动保存数据
      在按下电脑键盘上相应按键后会将数据自动保存为.txt格式
运行环境：Pycharm
"""
# 使用库
import socket  # 导入socket库，用于网络通信
import keyboard  # 导入keyboard库，用于监听键盘输入
HOST = ''  # 创建监听变量，监听所有网络接口
PORT = 6666  # 设置端口号，与Arduino IDE代码中设置的端口号保持一致

data = ""  # 声明一个变量用于存储接收到的数据
i = 0  # 记录从ESP32接收到的数据次数

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:  # 创建TCP socket对象s，并指定地址簇为IPv4和传输协议为TCP
    s.bind((HOST, PORT))  # 绑定指定的主机和端口
    s.listen(1)  # 开始监听
    print('等待ESP32连接...')  # 打印提示信息

    conn, addr = s.accept()  # 接受客户端连接请求，并返回新的socket对象conn和客户端的地址addr
    print('已连接：', addr)  # 打印客户端地址信息
    print('已连接,正在接收数据。')  # 打印客户端地址信息

    while True:
        received_data = conn.recv(1024)  # 接收从ESP32发送的数据，每次最多接收1024字节
        if not received_data:  # 如果接收到的数据为空，表示连接已断开，退出循环
            break
        data += received_data.decode()  # 将接收到的数据转换为字符串类型并追加到data变量中
        i += 1  # 计数器加1，记录接收到的数据次数

print('', i)
def listen_keyboard():
    if keyboard.is_pressed('w'):  # 判断按下的键盘是否为设置的键盘符号，此处定义为"W"
        with open('E:/桌面/esp32_reception.txt', 'a') as file:  # 打开文件，以写入模式
            for _ in range(i):  # 根据接收到的数据次数循环写入数据
                file.write(data + '\n')  # 将接收到的数据写入文件，并在末尾添加换行符
        print('数据已保存为esp32_reception.txt') # 打印提示信息
        return True  # 返回True表示按键被按下
    return False  # 返回False表示按键未被按下

while True:
    if listen_keyboard():  # 循环监听键盘输入
        break  # 如果键盘按键被按下，退出循环