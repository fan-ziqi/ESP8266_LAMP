#define BLINKER_MIOT_LIGHT//支持小爱同学
#define BLINKER_WIFI

#include <Blinker.h>

char auth[] = "520206be0f0b";   //app中获取到的Secret Key(密钥)
char ssid[] = "mi5";          //WiFi账号
char pswd[] = "123456789";   //WIFI密码

int GPIO = 0; //定义继电器输入引脚为GPIO/0

// 新建组件对象
BlinkerButton Button1("button2");//注意：要和APP组件’数据键名’一致

// 按下BlinkerAPP按键即会执行该函数
void button1_callback(const String & state)
{
    BLINKER_LOG("get button state: ", state);
    digitalWrite(GPIO, !digitalRead(GPIO));
    Blinker.vibrate(); 
    if (state=="on") {
        digitalWrite(GPIO, LOW);
        // 反馈开关状态
        Button1.print("on");
    } else if(state=="off"){
        digitalWrite(GPIO, HIGH);
        // 反馈开关状态
        Button1.print("off");
    }
}

//小爱电源类操作的回调函数:
//当小爱同学向设备发起控制, 设备端需要有对应控制处理函数 
void miotPowerState(const String & state)
{
    BLINKER_LOG("need set power state: ",state);

    if (state == BLINKER_CMD_OFF) 
    {//如果语音接收到是关闭灯就设置GPIO口为高电平
        digitalWrite(GPIO, HIGH);
        BlinkerMIOT.powerState("off");
        BlinkerMIOT.print();
    }
    else if (state == BLINKER_CMD_ON) 
    {
        digitalWrite(GPIO, LOW);
        BlinkerMIOT.powerState("on");
        BlinkerMIOT.print();
    }
}


void setup() {
    // 初始化串口，并开启调试信息，调试用可以删除
    Serial.begin(115200);
    pinMode(GPIO,OUTPUT);
    digitalWrite(GPIO,HIGH);//初始化，由于继电器是低电平触发。所以刚开始设为高电平
    Blinker.begin(auth, ssid, pswd);
    Button1.attach(button1_callback);
    BlinkerMIOT.attachPowerState(miotPowerState);//这段代码一定要加，不加小爱同学控制不了,务必在回调函数中反馈该控制状态
}

void loop()
{
    Blinker.run();
}
