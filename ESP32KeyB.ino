#include <BleKeyboard.h>
BleKeyboard bleKeyboard;
//按键映射表
byte KeyMappingTable[10] = {13, 4, 18, 19, 32, 22, 23, 21, 5, 12}; //22,
uint8_t RealKeyMappingTable[10] = {KEY_LEFT_SHIFT, KEY_LEFT_CTRL, KEY_LEFT_GUI, KEY_LEFT_ALT, 'z', 'c', 'v', KEY_RETURN, 'r', 'e'}; //
byte KetState[10];
//初始化按键io
void SetupPin() {
  for (byte i = 0; i < sizeof(KeyMappingTable) / sizeof(KeyMappingTable[0]); i++) {
    pinMode(KeyMappingTable[i], INPUT_PULLUP);
    KetState[i] = 1;
  }
}

//扫描按键io
void ScanKey() {
  for (byte i = 0; i < sizeof(KeyMappingTable) / sizeof(KeyMappingTable[0]); i++) {
    if (digitalRead(KeyMappingTable[i]) == LOW) {
       KetState[i]++;
       if (KetState[i] > 254) KetState[i]=100;
    }
    else KetState[i] = 0;
    Serial.print(KetState[i]);
    Serial.print(", ");
  }
  Serial.println("");
};

//发送蓝牙按键
void SendKey() {
  for (byte i = 0; i < sizeof(KeyMappingTable) / sizeof(KeyMappingTable[0]); i++) {
    if (KetState[i] % 25 == 0 && KetState[i] != 0 || KetState[i] >= 100&&KetState[i] % 10 == 0) bleKeyboard.press(RealKeyMappingTable[i]);
  }
  bleKeyboard.releaseAll();
}
void setup() { 
  Serial.begin(115200);
  SetupPin();
  Serial.println("Starting BLE work!");
  bleKeyboard.begin();
  
}
void loop() {
  if (bleKeyboard.isConnected()) {
    Serial.print(".");
    ScanKey();
    SendKey();

  }
}
