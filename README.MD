# 前言
本程序现已经支持ESP8266的微信Airkiss配网与局域网发现功能
（！！！代码与微信硬件云无关，无需去微信硬件云中生成设备id。只与微信的jssdk有关！！！）

# 技术栈
1. 基于[Arduino Esp8266](https://github.com/esp8266/Arduino){:target="_blank"} 固件开发，
版本2.4.2
2. 硬件NodeMCU

# 技术测试截图
1. Airkiss微信页面由jssdk掉起（自己写js调用微信，拉起配网页面）
![Image text](./img/airkiss.png =240x320)
2. 微信NFF局域网发现（自己写js调用微信，等待微信调用js结果）
![Image text](./img/nff.png)
3. 调试时控制台数据
![Image text](./img/pc.jpg)


# 参考文献
1. 微信硬件云 https://iot.weixin.qq.com/wiki/new/index.html
2. 微信NFF http://wiki.jackslab.org/ESP8266_AirKiss_NFF
