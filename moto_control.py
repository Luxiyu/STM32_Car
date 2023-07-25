# Timer_V1.0 - By: FITQY - 周二 8 月 23 日 2022
#__________________________________________________________________
# 导入模块
import sensor, time, image                                  # 导入感光元件模块 sensor 跟踪运行时间模块 time 机器视觉模块 image
import utime                                                # 导入延时模块 utime
from fpioa_manager import fm                                # 从 GPIO 模块中导入 引脚注册模块 fm
from Maix import GPIO                                       # 从 Maix 模块中导入 模块 GPIO
import lcd                                                  # 导入 LCD 模块
from machine import Timer, UART                             # 从 machine 模块中导入 定时器模块 Timer 脉宽调制模块 PWM
fm.register(15, fm.fpioa.UART1_TX, force=True)
fm.register(16, fm.fpioa.UART1_RX, force=True)
uart_A = UART(UART.UART1, 115200, 8, 0, 1, timeout=1000, read_buf_len=4096)

#__________________________________________________________________
# 感光元件设置
#sensor.reset()                                              # 重置并初始化感光元件 默认设置为 摄像头频率 24M 不开启双缓冲模式
sensor.reset(freq=24000000, dual_buff=True)                # 设置摄像头频率 24M 开启双缓冲模式 会提高帧率 但内存占用增加

sensor.set_pixformat(sensor.RGB565)                         # 设置图像格式为 RGB565 (彩色) 除此之外 还可设置格式为 GRAYSCALE 或者 YUV422
sensor.set_framesize(sensor.QVGA)                           # 设置图像大小为 QVGA (320 x 240) 像素个数 76800 K210最大支持格式为 VGA

sensor.set_auto_exposure(1)                                 # 设置自动曝光

sensor.set_auto_gain(0, gain_db = 12)                       # 设置画面增益 17 dB 影响实时画面亮度
sensor.set_auto_whitebal(0, rgb_gain_db = (0,0,0))          # 设置RGB增益 0 0 0 dB 影响画面色彩呈现效果 在 K210 上无法调节增益 初步判定是感光元件 ov2640 无法支持

sensor.set_vflip(1)                                         # 打开垂直翻转 如果是 01Studio 的 K210 不开启会导致画面方向与运动方向相反
sensor.set_hmirror(0)                                       # 打开水平镜像 如果是 01Studio 的 K210 不开启会导致画面方向与运动方向相反

sensor.skip_frames(time = 2000)                             # 延时跳过2s 等待感光元件稳定
sensor.set_jb_quality(100)

#__________________________________________________________________
# 创建时钟对象
clock = time.clock()                                        # 创建时钟对象 clock

#__________________________________________________________________
# 打印sensor参数
def print_sensor():
    print("Exposure: "+str(sensor.get_exposure_us()))       # 打印 曝光时间
    print("Gain: "+str(sensor.get_gain_db()))               # 打印 画面增益
    print("RGB: "+str(sensor.get_rgb_gain_db()))            # 打印 RGB 增益

#__________________________________________________________________
# LCD 按键信息及目标点信息显示函数
def lcd_key():
    lcd.draw_string(0, 0, "black_cx:"+str(black.cx), lcd.BLUE, lcd.WHITE)
    lcd.draw_string(0, 15, "motor1:"+str(motor.motor1)+"%", lcd.BLUE, lcd.WHITE)
    lcd.draw_string(0, 30,"motor2:"+str(motor.motor2)+"%", lcd.BLUE, lcd.WHITE)
    lcd.draw_string(0, 45,"Flag:"+str(Flag), lcd.BLUE, lcd.WHITE)
    lcd.draw_string(0, 60,"FPS:"+str(int(clock.fps())), lcd.BLUE, lcd.WHITE)

#__________________________________________________________________
# 定时器的使用
# 定义定时器属性类
class timer_property():
    cnt     = 0                                             # 定时器计数值
    cnt_max = 0                                             # 定时器计数值上限
    period  = 0                                             # 定时器周期
    freq    = 0                                             # 定时器频率

# 定时器0 配置_______________________________________________________
# 定时器0 实例化类
timer0 = timer_property()                                   # 实例化定时器属性类 timer_property() 为 timer0
timer0.cnt_max = 9                                          # 设定 定时器0 的计数值上限为 9
timer0.period = 100                                         # 设定 定时器0 的周期为 100

# 定时器0 定义回调函数
def timer0_back(tim0):
    if timer0.cnt < timer0.cnt_max:                         # 若 定时器0 的计数值小于 定时器0 的计数值上限
        timer0.cnt = timer0.cnt + 1                         # 计数值自增
    else:
        timer0.cnt = 0                                      # 超出计数值上限 则计数值重置为0

# 定时器0 初始化
tim0 = Timer(Timer.TIMER0,                                  # 定时器编号 定时器0
            Timer.CHANNEL0,                                 # 定时器通道 通道0
            mode = Timer.MODE_PERIODIC,                     # 定时器模式 周期性
            unit = Timer.UNIT_MS,                           # 定时器周期单位 ms
            period = timer0.period,                         # 定时器周期 timer0.period 若 unit 为 Timer.UNIT_MS 则周期为 timer0.period ms
            callback = timer0_back)                         # 定时器触发中断后执行的回调函数 timer0_back

# 定时器1 配置_______________________________________________________
# 电机类定义
class motor_property():
    motor1      = 0                                         # 电机1 占空比
    motor2      = 0                                         # 电机2 占空比
    control_x   = 0                                         # 被控坐标 x
    control_y   = 0                                         # 被控坐标 y

# 实例化电机类
motor = motor_property()                                    # 实例化电机类 motor_property() 为 motor
motor.motor1 = 50                                           # 电机1的占空比 初始设置为 50%
motor.motor2 = 50                                           # 电机2的占空比 初始设置为 50%
old_err = 0
Kp = 0.046
Kd = 0
Speed = 25 #期望速度

# 定义电机占空比控制函数
def motor_control(motor, x):
    err = x - motor.control_x
    motor.motor1 = Speed - (Kp*err+Kd*(err-old_err))
    motor.motor2 = Speed + (Kp*err+Kd*(err-old_err))
    # speed=25
    # example: x = 220
    # err = 220-160 = 60
    # speed_left = 25 - (0.08*60+kd*(err-old_err))
    # old_err = err
    # x = 280
    #err = 280 - 160 = 40
    # speed_left = 25 - (0.08*60+kd*(40-60))
    old_err = err
    motor.motor1 = int(motor.motor1)                        # 将 电机1占空比 转换为 整数
    motor.motor2 = int(motor.motor2)                        # 将 电机1占空比 转换为 整数

#__________________________________________________________________
# 寻找色块
# 定义类
class color_property():
    cx                      =  0                            # 色块 x轴 中心坐标
    cy                      =  0                            # 色块 y轴 中心坐标
    flag                    =  0                            # 色块标志位 1 找到 0 未找到
    color                   =  0                            # 色块颜色标志位 例如 你可以用 1 来表示 黑色
    density                 =  0                            # 色块密度比 反映色块锁定程度 值越大 锁定程度越好
    pixels_max              =  0                            # 色块像素最大值
    led_flag                =  0                            # LED标志位 方便调试用

    color_threshold         = (0, 0, 0, 0, 0, 0)            # 色块颜色阈值
    color_roi               = (0,0,320,240)                 # 色块寻找区域（感兴趣区域）
    color_x_stride          =  1                            # 色块 x轴 像素最小宽度 色块如果比较大可以调大此参数 提高寻找速度
    color_y_stride          =  1                            # 色块 y轴 像素最小宽度 色块如果比较大可以调大此参数 提高寻找速度
    color_pixels_threshold  =  100                          # 色块 像素个数阈值 例如调节此参数为100 则可以滤除色块像素小于100的色块
    color_area_threshold    =  100                          # 色块 被框面积阈值 例如调节此参数为100 则可以滤除色块被框面积小于100的色块
    color_merge             =  True                         # 是否合并寻找到的色块 True 则合并 False 则不合并
    color_margin            =  1                            # 色块合并间距 例如调节此参数为1 若上面选择True合并色块 且被找到的色块有多个 相距1像素 则会将这些色块合并

# 实例化类
# 黑色
black = color_property()
black.color_threshold         =  (0, 31, -8, 12, -9, 9)
black.color_roi               =  (0,100,320,30)
black.color_x_stride          =  1
black.color_y_stride          =  1
black.color_pixels_threshold  =  50
black.color_area_threshold    =  50
black.color_merge             =  True
black.color_margin            =  1


# 红色
red   = color_property()
red.color_threshold           = (0, 100, 20, 127, -10, 127)

#red.color_roi                = (0,0,320,240)
red.color_roi                 = (0,110,320,20)

red.color_x_stride            =  1
red.color_y_stride            =  1


red.color_pixels_threshold    =  10
red.color_area_threshold      =  10

red.color_merge               =  True
red.color_margin              =  1

# 绿色 预留
green = color_property()

# 蓝色 预留
blue  = color_property()

Flag = 0

# 定义寻找色块函数
def opv_find_blobs(color):
    color.pixels_max = 0                                    # 重置 色块 最大像素数量
    color.flag       = 0                                    # 重置 色块 标志位

    for blobs in img.find_blobs([color.color_threshold],    # 色块颜色阈值
    roi = color.color_roi,                                  # 色块寻找区域（感兴趣区域）
    x_stride = color.color_x_stride,                        # 色块 x轴 像素最小宽度 色块如果比较大可以调大此参数 提高寻找速度
    y_stride = color.color_y_stride,                        # 色块 y轴 像素最小宽度 色块如果比较大可以调大此参数 提高寻找速度
    pixels_threshold = color.color_pixels_threshold,        # 色块 像素个数阈值 例如调节此参数为100 则可以滤除色块像素小于100的色块
    area_threshold = color.color_area_threshold,            # 色块 被框面积阈值 例如调节此参数为100 则可以滤除色块被框面积小于100的色块
    merge = color.color_merge,                              # 是否合并寻找到的色块 True 则合并 False 则不合并
    margin = color.color_margin):                           # 色块合并间距 例如调节此参数为1 若上面选择True合并色块 且被找到的色块有多个 相距1像素 则会将这些色块合并
        img.draw_rectangle(blobs[0:4])                      # 圈出找到的色块
        if color.pixels_max < blobs.pixels():               # 找到面积最大的色块
            color.pixels_max = blobs.pixels()
            color.cx = blobs.cx()                           # 将面积最大的色块的 x轴 中心坐标值 赋值给 color
            color.cy = blobs.cy()                           # 将面积最大的色块的 y轴 中心坐标值 赋值给 color
            color.flag = 1                                  # 标志画面中有找到色块
            color.density = blobs.density()                # 将面积最大的色块的 色块密度比 赋值给 color

    if color.flag == 1:                                     # 标记画面中被找到的最大色块的中心坐标
        img.draw_cross(color.cx,color.cy, color=127, size = 15)
        img.draw_circle(color.cx,color.cy, 15, color = 127)

# 定义打印色块参数函数
def print_blobs_property(color,name):
    print(name,"cx:",color.cx,"cy:",color.cy,"flag:",color.flag,"color:",color.color,"density:",color.density)

#__________________________________________________________________
# 打包数据包
def sending_data(x,y,z):
    global uart;
    FH = bytearray([0x2C,0x12,x,y,z,0x5B])
    uart_A.write(FH)

#__________________________________________________________________
# 调试区
lcd.init(type=1)                       # lcd初始化
lcd.rotation(0)
lcd.clear(lcd.WHITE)

#__________________________________________________________________
# 主函数
while(True):

    clock.tick()                                            # 跟踪运行时间

    img = sensor.snapshot()                                 # 拍摄一张照片

    opv_find_blobs(black)                                 # 找黑色色块 led标志为1 表示黑色
    statistics =img.find_blobs([(0, 31, -8, 12, -9, 9)],roi=[0,180,320,8],area_threshold=120,merge=True,margin=120)
    if statistics:
            for b in statistics:
                tmp=img.draw_rectangle(b[0:4])
                tmp=img.draw_cross(b[5], b[6])
                c=img.get_pixel(b[5], b[6])
                if b[2] >100:
                    Flag = 1
                else:
                    Flag = 0
    lcd.display(img)                                        # LCD 显示图像
    lcd_key()                                               # LCD 显示信息

    motor.control_x = 160                                   # 控制目标处于 x轴中心点 160
    motor_control(motor,black.cx)                           # 电机占空比控制函数获取电机控制占空比
    sending_data(motor.motor1,motor.motor2,Flag)

    if timer0.cnt == 0:                                     # 如果 timer0.cnt 等于 0 此步骤的目的是控制打印周期 不要打印的太快
        print_sensor()                                      # 打印sensor参数
        print_blobs_property(black,"black")                # 打印黑色色块参数
        print(motor.motor1,motor.motor2,Flag)
#__________________________________________________________________
