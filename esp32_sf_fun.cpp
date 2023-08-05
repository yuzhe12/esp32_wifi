/*
介绍：主代码自定义函数内容
功能：完成esp32自动连接wifi功能
      自动向电脑发送“esp32”字符串
运行环境：Arduino IDE 
*/
#include "esp32_sf_fun.h"

void my_wifi_connect()//WiFi连接并在WiFi连接后串口打印esp32在网络上的IP地址
{
  Serial.println("正在等待ESP32接入热点!");//提示等待热点接入
  int sec = 0;//设置esp32接入wifi的时间变量
  while(WiFi.status() != WL_CONNECTED)//判断WiFi是否接入成功，不成功会一直等待
  {
    Serial.println(".");
    delay(1000);
    sec++;//计算等待时间，单位：秒
  } 
  Serial.println("ESP32与电脑wifi连接成功,本次接入时长（秒）：");
  Serial.println(sec);
  Serial.println("接入IP:");
  Serial.println(WiFi.localIP());//打印esp32在网络上的IP地址
}

void sendDataToPC() {
  // 连接到电脑的 IP 地址和端口号
  const char* serverIP = "192.168.204.254";  // 修改为你电脑的 IP 地址
  const int serverPort = 6666;              // 修改为你希望使用的端口号
  
  // 创建 TCP 客户端
  WiFiClient client;

  //判断esp32是否连接电脑服务器
  if (client.connect(serverIP, serverPort)) {
    Serial.println("已连接到服务器");
    client.print("esp32");// 发送字符数据到服务器
    client.stop();//断开服务器连接
    Serial.println("已断开连接");
  } else {
    Serial.println("无法连接到服务器");
  }
}