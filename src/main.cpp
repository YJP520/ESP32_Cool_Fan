/*
 * Time : 2023/05/07
 * Project : PWM fan
 * Author : Yu.J.P
 */

#include <Arduino.h>


#include "DHT11.h"                //温湿度传感器头文件

// 定义引脚
const int motorPin = 0;   //定义motorPin引脚为9，驱动电机
const int speedPin = 2;   //定义speedPin引脚为2，电机转速
const int dht11Pin = 4;   //定义dht11Pin引脚为4，温度

char speed_rank = 'A';    //初始转速
int rate = 127;           //占空比，这里取0-255
int speedState = 0;       //电机转速

DHT11 dht11;              //创建DHT11对象
int Temperature = 0;      //温度
int Humidity = 0;         //湿度

void setup() {
  //初始化serial，该串口用于与计算机连接通信
  Serial.begin(115200);

  //输入输出状态定义
  pinMode(speedPin, INPUT);
  pinMode(dht11Pin, INPUT);
  pinMode(motorPin, OUTPUT);

  analogWrite(motorPin, rate); //将a的值赋给motorPin
}

/* 缓慢改变转速 */
void gradual_change(int target) {
  if(rate > target) {
      for (; rate > target; --rate) {
        analogWrite(motorPin, rate); //将a的值赋给motorPin
        delay(50);
      }
  } else {
    for (; rate < target; ++rate) {
      analogWrite(motorPin, rate); //将a的值赋给motorPin
      delay(50);
    }
  }
}

/* 设置风扇转速 */
void set_fan_speed_gradually(char speed_rank) {
  if (speed_rank == 'A') {
    gradual_change(0);
  } else if (speed_rank == 'B') {
    gradual_change(52);
  }else if (speed_rank == 'C') {
    gradual_change(104);
  } else if (speed_rank == 'D') {
    gradual_change(156);
  } else if (speed_rank == 'E') {
    gradual_change(208);
  } else if (speed_rank == 'F') {
    gradual_change(255);
  }
}

/* 温度调控转速 */
void set_speed_with_temperature() {
  if (Temperature <= 26) {
    set_fan_speed_gradually('A');
  } else if (Temperature <= 28) {
    set_fan_speed_gradually('B');
  } else if (Temperature <= 30) {
    set_fan_speed_gradually('C');
  } else if (Temperature <= 32) {
    set_fan_speed_gradually('D');
  } else if (Temperature <= 34) {
    set_fan_speed_gradually('E');
  } else {  //大于34度 满转
    set_fan_speed_gradually('F');
  }
}

void loop() {
  // 串口监视器打印电机转速
  speedState = analogRead(speedPin);    //获取转速
  
  dht11.read(dht11Pin);                 //读引脚
  Temperature = dht11.temperature;      //获得温度
  Humidity = dht11.humidity;            //获得湿度

  Serial.printf("[ 电机转速 ] - %d\n", speedState);
  Serial.printf("[ 当前温度 ] - %d\n", Temperature);
  Serial.printf("[ 当前湿度 ] - %d\n\n", Humidity);
  delay(3000);

  set_speed_with_temperature();
}


// String str = "";  // 获得命令
// void get_commod() {
//   while (Serial.available() > 0) {
//     str += char(Serial.read());   // read是剪切，而不是复制
//     delay(10);  // 延时
//   }
//   if (str.length() > 0) {
//     Serial.print(F("命令行发送内容："));
//     Serial.println(str);
//   }
// }


// speed_rank = char(Serial.read());
// if (speed_rank == 'A') {
//   rate = 0;
// } else if (speed_rank == 'B') {
//   rate = 52;
// }else if (speed_rank == 'C') {
//   rate = 104;
// } else if (speed_rank == 'D') {
//   rate = 156;
// } else if (speed_rank == 'E') {
//   rate = 208;
// } else if (speed_rank == 'F') {
//   rate = 255;
// }
// analogWrite(motorPin, rate); //将a的值赋给motorPin


/* 设置风扇转速 */
// void set_fan_speed(char speed_rank) {
//   if (speed_rank == 'A') {
//     rate = 0;
//   } else if (speed_rank == 'B') {
//     rate = 52;
//   }else if (speed_rank == 'C') {
//     rate = 104;
//   } else if (speed_rank == 'D') {
//     rate = 156;
//   } else if (speed_rank == 'E') {
//     rate = 208;
//   } else if (speed_rank == 'F') {
//     rate = 255;
//   }
//   analogWrite(motorPin, rate); //将a的值赋给motorPin
// }