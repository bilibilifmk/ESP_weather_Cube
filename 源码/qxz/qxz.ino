
/*
0.96 气象站 第一版

需要配合wifi_link_tool配网工具 地址：https://github.com/bilibilifmk/wifi_link_tool
所需库：
u8g2
ArduinoJson
time(以提供)

*/


#include <wifi_link_tool.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include "zlt.h"

#include <WiFiClientSecure.h>

#include <Time.h>
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <WiFiClientSecure.h>

#include <WiFiClientSecureBearSSL.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

String  keys  = "此处填入和风天气api_key";  // 接口地址：https://console.heweather.com/app/index
String  dq  = "auto_ip"; //填入地区 文档地址 https://dev.heweather.com/docs/api/weather
/*
CN101010100
116.40,39.9
北京、 北京市、 beijing
朝阳,北京、chaoyang,beijing
60.194.130.1
auto_ip 为当前网络ip地址

*/


#define sck D1 /* 屏幕 */
#define sda D2

const int CONFIG_HTTPPORT   = 80;
const int CONFIG_HTTPSPORT  = 443;

std::unique_ptr<BearSSL::WiFiClientSecure>client( new BearSSL::WiFiClientSecure );


U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2( U8G2_R0, /* clock=*/ sck, /* data=*/ sda, /* reset=*/ U8X8_PIN_NONE );
int             col   = 999;
String          diqu    = "";
int             tq    = 0;
String          pm2   = "";
String          wendu   = "";
String          tim   = "00:00";
String          dat   = "2019/01/01";
unsigned long     previousMillis  = 0;
const long        interval  = 30000;        /* 30秒更新屏幕 */
unsigned long     previousMillis2 = 0;
const long        interval2 = 7200000;      /* 两小时更新天气 */
int         one   = 0;
int         logo    = 0;

IPAddress timeServer( 120, 25, 115, 20 );                         /* 阿里云ntp服务器 如果失效可以使用 120.25.115.19   120.25.115.20 */
#define STD_TIMEZONE_OFFSET +8                                     /* 设置中国 */
const int timeZone = 8;                                            /* 修改北京时区 */
WiFiUDP   Udp;

unsigned int  localPort = 8266;                                 /* 修改udp 有些路由器端口冲突时修改 */
int   servoLift = 1500;


void setup()
{
  WiFiServer    HTTPserver( CONFIG_HTTPPORT );
  WiFiServerSecure  HTTPSserver( CONFIG_HTTPSPORT );


  Serial.begin( 115200 );
  /*
   * 开启串口
   * ///////////////////////////////////////////////////////基础设置//////////////////////////////////////////////////////////
   */

  rstb = D3;
  /* 重置io */
  stateled = D4;
  /* 指示灯io */
  Hostname = "0.96OLED气象站";
  /* 设备名称 允许中文名称 不建议太长 */
  wxscan = true;
  /* 是否被小程序发现设备 开启意味该设备具有后台 true开启 false关闭 */

  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.setFont( u8g2_font_unifont_t_chinese2 );
  u8g2.setFontDirection( 0 );
  u8g2.clearBuffer();
  u8g2.setCursor( 0, 15 );
  u8g2.print( "开机" );
  u8g2.setCursor( 0, 30 );
  u8g2.print( "WiFi to Link" );
  u8g2.sendBuffer();


  load();
  /* 初始化WiFi link tool */
  Serial.println( "设置 UDP" );
  Udp.begin( localPort );
  Serial.print( "Local port: " );
  Serial.println( Udp.localPort() );
  Serial.println( "正在等待同步" );

  if ( link() )
  {
    u8g2.setFont( u8g2_font_unifont_t_chinese2 );
    u8g2.setFontDirection( 0 );
    u8g2.clearBuffer();
    u8g2.setCursor( 0, 15 );
    u8g2.print( "开机" );
    u8g2.setCursor( 0, 30 );
    u8g2.print( "WiFi to Link" );
    u8g2.setCursor( 0, 45 );
    u8g2.print( "OK" );
    u8g2.setCursor( 0, 60 );
    u8g2.print( WiFi.localIP() );
    u8g2.sendBuffer();
    sjfx();
    delay( 1000 );

   u8g2.sendBuffer();
for(int i =0;i<=10;i++){
u8g2.clearBuffer();
u8g2.drawXBMP(44,0,40,64,logo1);
u8g2.drawXBMP(75,51,49,11,by);
u8g2.sendBuffer(); 
  delay(50);
u8g2.clearBuffer();
u8g2.drawXBMP(44,0,40,64,logo2);
u8g2.drawXBMP(75,51,49,11,by);
u8g2.sendBuffer(); 
  delay(50);
u8g2.clearBuffer();
u8g2.drawXBMP(44,0,40,64,logo3);
u8g2.drawXBMP(75,51,49,11,by);
u8g2.sendBuffer();
  delay(50); 
u8g2.clearBuffer();
u8g2.drawXBMP(44,0,40,64,logo4);
u8g2.drawXBMP(75,51,49,11,by);
u8g2.sendBuffer(); 
  delay(50);
u8g2.clearBuffer();
u8g2.drawXBMP(44,0,40,64,logo5);
u8g2.drawXBMP(75,51,49,11,by);
u8g2.sendBuffer(); 
  delay(50);
 }
    setSyncProvider( getNtpTime );
    shuaxin();
  }else{
    u8g2.setFont( u8g2_font_unifont_t_chinese2 );
    u8g2.setFontDirection( 0 );
    u8g2.clearBuffer();
    u8g2.setCursor( 45, 15 );
    u8g2.print( "配网" );
    u8g2.setCursor( 0, 30 );
    u8g2.print( "AP TO Link:" );
    u8g2.setCursor( 0, 45 );
    u8g2.print( "wifi_link_tool" );
    u8g2.setCursor( 0, 60 );
    u8g2.print( "IP:6.6.6.6" );
    u8g2.sendBuffer();
    delay( 5000 );

    u8g2.clearBuffer();
    u8g2.drawXBMP( 0, 0, 128, 64, wx );
    u8g2.sendBuffer();
  }
}


void loop()
{
  pant(); /* WiFi link tool 服务维持函数  请勿修改位置 */
  if ( link() )
  {
    unsigned long currentMillis = millis();
    if ( currentMillis - previousMillis >= interval )
    {
      previousMillis = currentMillis;
      /*  sjfx(); */
      shuaxin();
    }
    unsigned long currentMillis2 = millis();
    if ( currentMillis2 - previousMillis2 >= interval2 )
    {
      previousMillis2 = currentMillis2;
      one   = 0;
      sjfx();
    }
  }
}


void sjfx()
{
  xx();
  delay( 1000 );
  pm();
  delay( 1000 );
  col = tq;

  Serial.println( "当前地区： " + diqu );
  Serial.println( "天气代码： " );
  Serial.println( tq );
  Serial.println( "体感温度： " + wendu );
  Serial.println( "PM2.5： " + pm2 );
}


void shuaxin()
{
  /* setSyncProvider(getNtpTime); */
  String zov = "";
  if ( hour() < 10 )
  {
    zov = "0";
  }
  if ( minute() < 10 )
  {
    tim = zov + String( hour() ) + ":0" + String( minute() );
  }else{ tim = zov + String( hour() ) + ":" + String( minute() ); }
  dat = String( year() ) + "/" + String( month() ) + "/" + String( day() );
  Serial.print( tim );    /* 输出当前网络分钟 */
  Serial.print( dat );    /* 输出当前日期 */
  u8g2.clearBuffer();
/*  */
  tubiao();


  u8g2.setFont( u8g2_font_ncenB18_tf );
  u8g2.setFontDirection( 0 );
  u8g2.setCursor( 65, 53 );
  u8g2.print( wendu );    /* 温度 */
  u8g2.setFont( u8g2_font_6x10_tf );
  u8g2.setFontDirection( 0 );
  u8g2.setCursor( 60, 64 );
  u8g2.print( dat );      /* 日期 */
  u8g2.drawXBMP( 100, 32, 25, 25, col_ssd );
  u8g2.setFont( u8g2_font_ncenR12_tf );
  u8g2.setFontDirection( 0 );
  u8g2.setCursor( 0, 48 );
  u8g2.print( "pm2.5" );
  u8g2.setCursor( 10, 62 );
  u8g2.print( pm2 );      /* pm2.5 浓度 */
  u8g2.setFont( u8g2_font_fub25_tf );
  u8g2.setFontDirection( 0 );
  u8g2.setCursor( 40, 32 );
  u8g2.print( tim );
  u8g2.sendBuffer();
}


void tubiao()
{
  if ( col == 100 )                               /* 晴 */
  {
    u8g2.drawXBMP( 0, 0, 40, 40, col_100 );
  }else if ( col == 102 || col == 101 )
  {
    u8g2.drawXBMP( 0, 0, 40, 40, col_102 ); /* 云 */
  }else if ( col == 103 )
  {
    u8g2.drawXBMP( 0, 0, 40, 40, col_103 ); /* 晴间多云 */
  }else if ( col == 104 )
  {
    u8g2.drawXBMP( 0, 0, 40, 40, col_104 ); /* 阴 */
  }else if ( col >= 200 && col <= 207 )
  {
    u8g2.drawXBMP( 0, 0, 40, 40, col_205 ); /* 风 */
  }else if ( col >= 208 && col <= 213 )
  {
    u8g2.drawXBMP( 0, 0, 40, 40, col_208 ); /* 风暴 */
  }else if ( col >= 300 && col <= 301 )
  {
    u8g2.drawXBMP( 0, 0, 40, 40, col_301 ); /* 阵雨 */
  }else if ( col >= 302 && col <= 303 )
  {
    u8g2.drawXBMP( 0, 0, 40, 40, col_302 ); /* 雷阵雨 */
  }else if ( col == 304 )
  {
    u8g2.drawXBMP( 0, 0, 40, 40, col_304 ); /* 冰雹 */
  }else if ( col == 399 || col == 314 || col == 305 || col == 306 || col == 307 || col == 315 )
  {
    u8g2.drawXBMP( 0, 0, 40, 40, col_307 ); /* 雨 */
  }else if ( (col >= 308 && col <= 313) || (col >= 316 && col <= 318) )
  {
    u8g2.drawXBMP( 0, 0, 40, 40, col_310 ); /* 暴雨 */
  }else if ( (col >= 402 && col <= 406) || col == 409 || col == 410 || col == 400 || col == 401 || col == 408 || col == 499 )
  {
    u8g2.drawXBMP( 0, 0, 40, 40, col_401 ); /* 雪 */
  }else if ( col == 407 )
  {
    u8g2.drawXBMP( 0, 0, 40, 40, col_407 ); /* 阵雪 */
  }else if ( col >= 500 && col <= 502 )
  {
    u8g2.drawXBMP( 0, 0, 40, 40, col_500 ); /* 雾霾 */
  }else if ( col >= 503 && col <= 508 )
  {
    u8g2.drawXBMP( 0, 0, 40, 40, col_503 ); /* 沙尘暴 */
  }else if ( col >= 509 && col <= 515 )
  {
    u8g2.drawXBMP( 0, 0, 40, 40, col_509 ); /*不适宜生存 */
  }else{
    u8g2.drawXBMP( 0, 0, 40, 40, col_999 ); /* 未知 */
  }
}


/* //////////////////////////////////////////////////天气数据 */
void xx()
{
  String line;

  client->setInsecure();
  HTTPClient https;
  if ( https.begin( *client, "https://free-api.heweather.net/s6/weather/now?&location=" + dq + "&key=" + keys ) )
  {
    int httpCode = https.GET();
    if ( httpCode > 0 )
    {
      if ( httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY )
      {
        line = https.getString();
      }
    }

    https.end();
  } else {
    Serial.printf( "[HTTPS]请求链接失败\n" );
  }


  Serial.println( "接口返回" + line );
  DynamicJsonBuffer jsonBuffer( 1024 );
  JsonObject &    res_json  = jsonBuffer.parseObject( line );
  String      r1    = res_json["HeWeather6"][0]["basic"]["location"];       /* 地区 */
  int     r2    = res_json["HeWeather6"][0]["now"]["cond_code"];        /* 天气 */
  String      r3    = res_json["HeWeather6"][0]["now"]["fl"];               /* 体感温度 */
  jsonBuffer.clear();
  diqu  = r1;                                                                                   /* 地区 */
  tq  = r2;                                                                                   /* 天气 */
  wendu = r3;                                                                                   /* 体感温度 */
}


void pm()
{
  String line;

  client->setInsecure();
  HTTPClient https;
  if ( https.begin( *client, "https://free-api.heweather.net/s6/air/now?&location=" + dq + "&key=" + keys ) )
  {
    int httpCode = https.GET();
    if ( httpCode > 0 )
    {
      if ( httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY )
      {
        line = https.getString();
      }
    }

    https.end();
  } else {
    Serial.printf( "[HTTPS]请求链接失败\n" );
  }


  Serial.println( "接口返回" + line );
  DynamicJsonBuffer jsonBuffer( 1024 );
  JsonObject &    res_json  = jsonBuffer.parseObject( line );
  String      r1    = res_json["HeWeather6"][0]["air_now_city"]["pm25"];    /* pm25 */
  jsonBuffer.clear();
  if ( r1 != "" )
  {
    pm2 = r1;                                                                               /* pm2.5 */
  }
}


/* ////////////////////////////////////////////////////////////////解包设置////////////////////////////////////////////////////////////////////////////////////////////////////////////// */
void digitalClockDisplay()
{
  /*  */
  Serial.print( hour() );
  printDigits( minute() );
  Serial.println();
}


void printDigits( int digits )
{
  Serial.print( ":" );
  if ( digits < 10 )
    Serial.print( '0' );
  Serial.print( digits );
}


const int NTP_PACKET_SIZE = 48;
byte    packetBuffer[NTP_PACKET_SIZE];

time_t getNtpTime()
{
  while ( Udp.parsePacket() > 0 )
    ;
  Serial.println( "连接时间服务器" );
  sendNTPpacket( timeServer );
  uint32_t beginWait = millis();
  while ( millis() - beginWait < 1500 )
  {
    int size = Udp.parsePacket();
    if ( size >= NTP_PACKET_SIZE )
    {
      Serial.println( "时间服务器应答" );
      Udp.read( packetBuffer, NTP_PACKET_SIZE );
      unsigned long secsSince1900;
      /* convert four bytes starting at location 40 to a long integer */
      secsSince1900 = (unsigned long) packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long) packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long) packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long) packetBuffer[43];
      return(secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR);
    }
  }
  Serial.println( "No NTP Response :-(" );
  return(0);
}


void sendNTPpacket( IPAddress &address )
{
  memset( packetBuffer, 0, NTP_PACKET_SIZE );

  packetBuffer[0] = 0b11100011;
  packetBuffer[1] = 0;
  packetBuffer[3] = 0xEC;

  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  Udp.beginPacket( address, 123 );
  Udp.write( packetBuffer, NTP_PACKET_SIZE );
  Udp.endPacket();
}
