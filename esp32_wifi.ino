/*
介绍：主代码部分
功能：完成esp32自动连接WiFi功能，并在开机时以100hz速度向电脑发送字符数据"esp32"
运行环境：Arduino IDE 
参考代码链接：
https://www.bilibili.com/video/BV1kY411b7bg/?spm_id_from=333.337.search-card.all.click&vd_source=2d70cb2f3e11560f3f8e3c05d272732f
https://blog.csdn.net/Beihai_Van/article/details/125793806?ops_request_misc=&request_id=e7f3bb23d2644b2ca8a5cc370f5ae15e&biz_id=&utm_medium=distribute.pc_search_result.none-task-blog-2~blog~koosearch~default-6-125793806-null-null.268^v1^control&utm_term=esp32%E5%AE%9A%E6%97%B6%E5%99%A8%E4%B8%AD%E6%96%AD%E5%87%BD%E6%95%B0&spm=1018.2226.3001.4450
https://blog.csdn.net/qq_30316889/article/details/123451261?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522169088080216782425195369%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=169088080216782425195369&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~blog~sobaiduend~default-3-123451261-null-null.268^v1^control&utm_term=esp32%E5%AE%9A%E6%97%B6%E5%99%A8%E4%B8%AD%E6%96%AD%E5%87%BD%E6%95%B0&spm=1018.2226.3001.4450
*/

#include "esp32_sf_fun.h"

#define BUTTON_PIN 0 //设置深度睡眠引脚号

const char* WiFi_name = "One Plus" ;//输入要连接的网络名称
const char* WiFi_pass = "qwertyuiop";  //输入要连接的网络密码

hw_timer_t *timer = NULL; //创建硬件定时器
bool flag = false; //创建中断标志位




//定时中断回调函数
void IRAM_ATTR timer_flag()
{
  flag = true;//进入定时中断，触发标志位
}


void setup() {
  Serial.begin(115200); //启动串口通讯
  WiFi.mode(WIFI_STA);//开启wifi STA 模式
  WiFi.begin(WiFi_name, WiFi_pass); //esp32开始连接wifi
  my_wifi_connect();//WiFi连接并在WiFi连接后串口打印esp32在网络上的IP地址
  
  //定时器设置
  //初始化定时器
  timer = timerBegin(0, 80, true);//配置定时器，这里使用0号定时器，还可使用1-3号定时器；80为分频系数，定时器基础频率为80MHz，设置80可以保证1us记录一次；ture为向上计数，也可设置false为向下计数
  //配置定时器
  timerAttachInterrupt(timer, &timer_flag, true);//true表示边沿触发，可以设置false改为电平触发
  //设置定时模式
  timerAlarmWrite(timer, 10*1000, true);//10*1000表示10ms(及100hz)触发一次中断，true表示每0.01ms触发一次，可以设置为false改为单次触发
  //启动定时器
  timerAlarmEnable(timer);

  //将睡眠引脚号设置为输入模式
  pinMode(BUTTON_PIN, INPUT);

  //设置唤醒引脚，并打印唤醒原因
  print_wakeup_reason();//打印唤醒原因
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_13,1); //设置引脚13为唤醒源，1表示为高电平唤醒，当引脚13置为高电平时，esp32唤醒。
  
}

void loop()
 {
   if(flag)//判断定时器中断标志位是否触发
  {
    sendDataToPC();// 将数据发送给电脑
    flag = false;//复原标志位
  }
  if(digitalRead(BUTTON_PIN) == LOW)//判断睡眠引脚是否为低电平，是则进入深度睡眠
  {
    Serial.println("Going to sleep now");
    esp_deep_sleep_start();//进入深度睡眠
    BUTTON_PIN == LOW;
  }
}

