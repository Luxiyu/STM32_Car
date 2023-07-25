# Untitled - By: User - 周三 4月 19 2023
 
import sensor, image,lcd
from machine import UART #串口库函数
from fpioa_manager import fm # GPIO重定向函数
 
fm.register(18, fm.fpioa.UART1_TX, force=True)
fm.register(19, fm.fpioa.UART1_RX, force=True)
uart_A = UART(UART.UART1, 115200, 8, 0, 1, timeout=1000, read_buf_len=4096)
 
green_threshold = ((0, 190))           #黑色
roi1            = [0,100,320,16]       #巡线敏感区
roi2            = [0,180,320,8]        #关键点敏感区
expectedValue   = 160                  #巡线位置期望
err             = 0                    #本次误差
old_err         = 0                    #上次误差
Kp              = 0.046                #PID比例系数
Kd              = 0                    #PID微分系数
Speed           = 0                    #期望速度
Speed_left      = 0                    #左轮速度
Speed_right     = 0                    #右轮速度
Flag            = 0                    #用于关键点标志
 
def sending_data(x,y,z):
    global uart;
    FH = bytearray([0x2C,0x12,x,y,z,0x5B])
    uart_A.write(FH);
 
lcd.init()
sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.QVGA) # 320x240
sensor.skip_frames(time = 3000 )#跳过3000张图片
sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking
sensor.run(1)
 
while True:
    img=sensor.snapshot()
    statistics1 = img.find_blobs([green_threshold],roi=roi1,area_threshold=200,merge=True)
    statistics2 = img.find_blobs([green_threshold],roi=roi2,area_threshold=120,merge=True,margin=120)
    if statistics1:
        for b in statistics1:
            tmp=img.draw_rectangle(b[0:4])
            tmp=img.draw_cross(b[5], b[6])
 
            #PID计算
 
            actualValue=b[5]
            err=actualValue-expectedValue
            Speed_left = Speed - (Kp*err+Kd*(err-old_err))
            Speed_right = Speed + (Kp*err+Kd*(err-old_err))
            old_err= err
            print("Speed_left,Speed_right")
            print(int(Speed_left),int(Speed_right))
    if statistics2:
        for b in statistics2:
            tmp=img.draw_rectangle(b[0:4])
            tmp=img.draw_cross(b[5], b[6])
            if b[2] >50:
                Flag = 1
    sending_data(int(Speed_left),int(Speed_right),Flag)
