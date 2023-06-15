#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

Adafruit_MPU6050 mpu;

//接收方MAC地址 根据自己的板子修改
uint8_t broadcastAddress[] = {0x2C, 0xF4, 0x32, 0x30, 0xBF, 0xFA};

//发送数据的结构体
typedef struct {
  float x;
  float y;
  float z;
} struct_message;

//创建一个新的类型变量
struct_message myData;

//这是一个回调函数，将在发送消息时执行。
//在这种情况下，无论是否成功发送该消息，都会简单地打印出来
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

void setup() {
  //初始化串行监视器以进行调试
  Serial.begin(115200);
  while (!Serial)
    delay(10);

  //将设备设置为Wi-Fi站点
  WiFi.mode(WIFI_STA);

  //立即初始化ESP
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  //设置ESP8266角色
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);

  //注册回调函数
  esp_now_register_send_cb(OnDataSent);
  
  //与另一个ESP-NOW设备配对以发送数据
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  Wire.begin();////
if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
      Serial.println("Failed to find MPU6050 chip");
    }  
}
else
    Serial.println("success to find MPU6050 chip");

    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

  // 将陀螺仪范围设置为 +- 500 度/秒
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);

  // 将滤波器带宽设置为 21 Hz
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {
  //float fsrReading = analogRead(0);//
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  myData.x=a.acceleration.x;
  myData.y=a.acceleration.y;
  myData.z=a.acceleration.z;

  //myData.Z=fsrReading;//

    //发送消息
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  

  delay(1000);
}