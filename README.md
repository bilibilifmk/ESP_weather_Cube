### 请尊重开源协议，谢谢！！！！
# 基于esp8266+0.96oled的气象站桌面时钟

## 关于固件版本信息  
* v1 接口将在2021.12.30日后失效在次之前是能用的
* v2 新接口 请勿使用，发现致命问题，接口变化没有改源代码。
* v3 1.修复了图标显示问题，2.删除了ota功能（空间有限 无法提供足够的ota空间 请保留rx tx 复用升级是没有问题的）3.优化了变量空间使其更稳定 4.修复配网重置bug（需要升级配网库）
* v3b:该固件与v3核心部分一致，为哔哩哔哩up主专用固件，删除了pm2.5显示，增加了哔哩哔哩粉丝显示，本来想做负一屏的可惜i2c刷新太慢了动画不流畅，直接刷新太过突兀只能牺牲pm2.5了，由于删除了一个https请求，整体会更加稳定，更不容易发生32位处理器处理溢出问题。
* v4 增加对pc性能显示，其他功能不变，需要使用上位机和aida64，采用pc请求气象站方式，大幅减少因串口通信或建立web通信所需要的性能损失。  





# v4固件视频： https://www.bilibili.com/video/BV1N54y1B7WW/
# 视频：https://www.bilibili.com/video/BV1WC4y1h7k4/ 
# 量产版视频： https://www.bilibili.com/video/BV1T54y1S7PU/
![image](https://github.com/bilibilifmk/ESP_weather_Cube/blob/master/%E5%B0%81%E9%9D%A2.jpg)


### 公共天气API_KEY： e98f6584cc5c4430a084904315907cf8  
（接口限制每天最多访问1000次如需稳定请自己申请（免费））



# 关于量产版本！！！
###  电路设计及pcb 请访问立创eda：https://oshwhub.com/hbk444/qi-xiang-zhan01  
### pcb板兼容1.3寸oled屏 
 
 
 ![image](https://github.com/bilibilifmk/ESP_weather_Cube/blob/master/4545.png)  
 
 ![image](https://github.com/bilibilifmk/ESP_weather_Cube/blob/master/54.png)  
 
 ![image](https://github.com/bilibilifmk/ESP_weather_Cube/blob/master/pcb.png)  
 
 
 ![image](https://github.com/bilibilifmk/ESP_weather_Cube/blob/master/%E9%87%8F%E4%BA%A7%E7%89%88/1.png)  
 



## 说明
源码需要配合WiFi_link_tool配网库进行开发  
可直接访问设备的ip或http://OLED气象站/ 进行访问
所需要的库：
* u8g2
* ArduinoJson
* time(以提供)  
天气API来自和风天气：console.heweather.com  
提供的固件是针对esp_01s的，其他开发板请下载源码进行编译！    

# pc性能显示
 需要配合aida64，并且开启内存共享  
 ![image](https://github.com/bilibilifmk/ESP_weather_Cube/blob/master/PC%E4%B8%8A%E4%BD%8D%E6%9C%BA/1.png)  
 ![image](https://github.com/bilibilifmk/ESP_weather_Cube/blob/master/PC%E4%B8%8A%E4%BD%8D%E6%9C%BA/2.png)  
 
 注 上位机部分代码来自他人项目，所以就不开源了。 项目地址：https://gitee.com/optimouskiller/AIDA64InfoReader  
 

# 关于b站up主专用固件（v3b）
效果  
![image](https://github.com/bilibilifmk/ESP_weather_Cube/blob/master/v3bb.png)  
后台  
![image](https://github.com/bilibilifmk/ESP_weather_Cube/blob/master/uid.png)    
获取uid 电脑个人主页后数字，手机版在个人资料中  
![image](https://github.com/bilibilifmk/ESP_weather_Cube/blob/master/buid.png)      

# 如果不想折腾直接使用编译好的固件请往下看  
### esp01s连线图：
![image](https://github.com/bilibilifmk/ESP_weather_Cube/blob/master/esp01s%E7%94%B5%E8%B7%AF.png)
### 烧录注意事项
esp01s与ttl链接方法：  
01s   ttl    
xt----rx  
rx----tx  
vcc---3.3  
ch----3.3  
io0---gnd  
gnd---gnd  

### 烧录配置
![image](https://github.com/bilibilifmk/ESP_weather_Cube/blob/master/%E7%83%A7%E5%BD%95%E9%85%8D%E7%BD%AE.png)
### 烧录流程
1.擦除2.烧录  
![image](https://github.com/bilibilifmk/ESP_weather_Cube/blob/master/%E7%83%A7%E5%BD%95%E6%B5%81%E7%A8%8B.png)
