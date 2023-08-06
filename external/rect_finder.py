from machine import UART
from fpioa_manager import fm
from machine import Timer
from Maix import GPIO
from board import board_info
import image, sensor, lcd
from math import *

# 打开串口
fm.register(18, fm.fpioa.UART1_TX, force=True)
fm.register(19, fm.fpioa.UART1_RX, force=True)
uart1 = UART(UART.UART1, 115200, 8, 0, 1, timeout=1000, read_buf_len=128)

# 打开 LCD 和摄像头
lcd.init()
sensor.reset()
sensor.set_pixformat(sensor.RGB565) # 设置图像色彩格式为RGB565格式
sensor.set_framesize(sensor.QQVGA)  # 设置图像大小为160*120
sensor.set_auto_whitebal(True)      # 设置自动白平衡
sensor.set_brightness(3000)         # 设置亮度为3000
sensor.set_hmirror(False)            #水平翻转
sensor.set_vflip(True)              #垂直翻转
sensor.skip_frames(time = 20)       # 跳过帧

# 红色色块阈值
red_threshold = (0, 100, 8, 127, 8, 127)

# 记录色块中心点
red_centre = [0, 0]

# 记录中心和正方形边界
origin = [80, 60]
boundary_dot1 = [40, 20]
boundary_dot2 = [120, 20]
boundary_dot3 = [120, 100]
boundary_dot4 = [40, 100]

# 校准按键所用变量
cnt_OK = 0
is_last_OK = 0;
is_adjusting = 1;

# 按键输入引脚为 20
fm.register(21, fm.fpioa.GPIOHS21)
key_OK = GPIO(GPIO.GPIOHS21, GPIO.IN, GPIO.PULL_UP)

# 校准按键定时器回调任务
def tim_callback(timer):
    global cnt_OK
    global is_last_OK
    global is_adjusting
    global key_OK
    global red_centre
    global origin
    global boundary_dot1
    global boundary_dot2
    global boundary_dot3
    global boundary_dot4
    if key_OK.value() == 0:
        if is_last_OK == 0:
            cnt_OK = cnt_OK + 1;
            is_last_OK = 1
            # 校准
            if cnt_OK == 1:
                origin = red_centre[:]
            elif cnt_OK == 2:
                boundary_dot1 = red_centre[:]
            elif cnt_OK == 3:
                boundary_dot2 = red_centre[:]
            elif cnt_OK == 4:
                boundary_dot3 = red_centre[:]
            elif cnt_OK == 5:
                boundary_dot4 = red_centre[:]
            else:
                # OK -> 进入工作模式
                is_adjusting = 0
    else:
        is_last_OK = 0

# 启用定时器
tim = Timer(Timer.TIMER0, Timer.CHANNEL0, mode=Timer.MODE_PERIODIC, period=20, unit=Timer.UNIT_MS, callback=tim_callback, arg=None, start=True, priority=1, div=0)
tim.start()


# 进行校准所用的色块识别
while is_adjusting:
    img=sensor.snapshot()
    blobs = img.find_blobs([red_threshold], x_stride=2, y_stride=2, merge=True)
    if blobs:
        for b in blobs:
            tmp=img.draw_rectangle(b[0:4])
            tmp=img.draw_cross(b[5], b[6])
            red_centre = [b[5], b[6]]
            # c=img.get_pixel(b[5], b[6]) # 获取中心处像素颜色
    img.draw_circle(origin[0], origin[1], 2, color = (255, 0, 0), thickness = 1, fill = True)
    img.draw_circle(boundary_dot1[0], boundary_dot1[1], 2, color = (255, 0, 0), thickness = 1, fill = True)
    img.draw_circle(boundary_dot2[0], boundary_dot2[1], 2, color = (255, 0, 0), thickness = 1, fill = True)
    img.draw_circle(boundary_dot3[0], boundary_dot3[1], 2, color = (255, 0, 0), thickness = 1, fill = True)
    img.draw_circle(boundary_dot4[0], boundary_dot4[1], 2, color = (255, 0, 0), thickness = 1, fill = True)
    lcd.display(img)

# 关闭定时器
tim.stop()
# 清屏 LCD
lcd.clear()

bm_dot1 = [(boundary_dot1[0] - origin[0]), (origin[1] - boundary_dot1[1])]
bm_dot2 = [(boundary_dot2[0] - origin[0]), (origin[1] - boundary_dot2[1])]
bm_dot3 = [(boundary_dot3[0] - origin[0]), (origin[1] - boundary_dot3[1])]
bm_dot4 = [(boundary_dot4[0] - origin[0]), (origin[1] - boundary_dot4[1])]

# uart1.write("<!" + str(rect_dot1[0]) + ", " + str(rect_dot1[1]) + ">!")

def shift2pos(dot_shift):
    global bm_dot1
    global bm_dot2
    global bm_dot3
    global bm_dot4
    dot = [0, 0]
    if dot_shift[1] != 0 or dot_shift[0] != 0:
        dot_pos = 1
        alpha_shift = atan2(dot_shift[1], dot_shift[0])
        if alpha_shift > atan2(bm_dot1[1], bm_dot1[0]):
            dot_pos = 4
        elif alpha_shift > atan2(bm_dot2[1], bm_dot2[0]):
            dot_pos = 1
        elif alpha_shift > atan2(bm_dot3[1], bm_dot3[0]):
            dot_pos = 2
        elif alpha_shift > atan2(bm_dot4[1], bm_dot4[0]):
            dot_pos = 3
        else:
            dot_pos = 4

        if dot_pos == 1:
            # up
            k = (bm_dot2[1] - bm_dot1[1]) / (bm_dot2[0] - bm_dot1[0])
            meet_x = (bm_dot2[1] - k * bm_dot2[0]) / ((dot_shift[1] / dot_shift[0]) - k)
            meet_y = meet_x * (dot_shift[1] / dot_shift[0])
            ratio_c = dot_shift[1] / meet_y;
            ratio_b = (meet_x - bm_dot1[0]) / (bm_dot2[0] - bm_dot1[0]);
            x = ratio_b * 25 + (1 - ratio_b) * (-25)
            dot[0] = x * ratio_c
            dot[1] = 25 * ratio_c
        elif dot_pos == 2:
            # right
            k = (bm_dot2[0] - bm_dot3[0]) / (bm_dot2[1] - bm_dot3[1])
            meet_y = (bm_dot2[0] - k * bm_dot2[1]) / ((dot_shift[0] / dot_shift[1]) - k)
            meet_x = meet_y * (dot_shift[0] / dot_shift[1])
            ratio_c = dot_shift[0] / meet_x;
            ratio_b = (meet_y - bm_dot3[1]) / (bm_dot2[1] - bm_dot3[1]);
            y = ratio_b * 25 + (1 - ratio_b) * (-25)
            dot[0] = 25 * ratio_c
            dot[1] = y * ratio_c
        elif dot_pos == 3:
            # down
            k = (bm_dot3[1] - bm_dot4[1]) / (bm_dot3[0] - bm_dot4[0])
            meet_x = (bm_dot3[1] - k * bm_dot3[0]) / ((dot_shift[1] / dot_shift[0]) - k)
            meet_y = meet_x * (dot_shift[1] / dot_shift[0])
            ratio_c = dot_shift[1] / meet_y;
            ratio_b = (meet_x - bm_dot4[0]) / (bm_dot3[0] - bm_dot4[0]);
            x = ratio_b * 25 + (1 - ratio_b) * (-25)
            dot[0] = x * ratio_c
            dot[1] = -25 * ratio_c
        else:
            #left
            k = (bm_dot1[0] - bm_dot4[0]) / (bm_dot1[1] - bm_dot4[1])
            meet_y = (bm_dot1[0] - k * bm_dot1[1]) / ((dot_shift[0] / dot_shift[1]) - k)
            meet_x = meet_y * (dot_shift[0] / dot_shift[1])
            ratio_c = dot_shift[0] / meet_x;
            ratio_b = (meet_y - bm_dot3[1]) / (bm_dot2[1] - bm_dot3[1]);
            y = ratio_b * 25 + (1 - ratio_b) * (-25)
            dot[0] = -25 * ratio_c
            dot[1] = y * ratio_c

    return dot


while True:
    img = sensor.snapshot()
    #寻找矩形
    for r in img.find_rects(threshold = 10000):
        # 判断矩形边长是否符合要求
        if r.w() > 20 and r.w() < 70 and r.h() > 20 and r.h() < 70:
            # 在屏幕上框出矩形
            #img.draw_rectangle(r.rect(), color = (255, 0, 0), scale = 4)
            # 获取矩形角点位置
            corner = r.corners()
            # 在屏幕上圈出矩形角点
            img.draw_circle(corner[0][0], corner[0][1], 2, color = (0, 255, 0), thickness = 1, fill = True)
            img.draw_circle(corner[1][0], corner[1][1], 2, color = (0, 255, 0), thickness = 1, fill = True)
            img.draw_circle(corner[2][0], corner[2][1], 2, color = (0, 255, 0), thickness = 1, fill = True)
            img.draw_circle(corner[3][0], corner[3][1], 2, color = (0, 255, 0), thickness = 1, fill = True)
            # 打印四个角点坐标, 角点1的数组是corner[0], 坐标就是(corner[0][0],corner[0][1])
            # 角点检测输出的角点排序每次不一定一致，矩形左上的角点有可能是corner0,1,2,3其中一个
            # TODO 多次取平均值
            rect_dot1_shift = [(corner[0][0] - origin[0]), (origin[1] - corner[0][1])];
            rect_dot2_shift = [(corner[1][0] - origin[0]), (origin[1] - corner[1][1])];
            rect_dot3_shift = [(corner[2][0] - origin[0]), (origin[1] - corner[2][1])];
            rect_dot4_shift = [(corner[3][0] - origin[0]), (origin[1] - corner[3][1])];

            rect_dot1 = shift2pos(rect_dot1_shift)
            rect_dot2 = shift2pos(rect_dot2_shift)
            rect_dot3 = shift2pos(rect_dot3_shift)
            rect_dot4 = shift2pos(rect_dot4_shift)

            uart1.write("<!" + str(rect_dot4[0]) + ", " + str(rect_dot4[1]) + ", " + str(rect_dot3[0]) + ", " + str(rect_dot3[1]) + ", " + str(rect_dot2[0]) + ", " + str(rect_dot2[1]) + ", " + str(rect_dot1[0]) + ", " + str(rect_dot1[1]) + ">!\r\n")
    #img.draw_circle(origin[0], origin[1], 2, color = (255, 0, 0), thickness = 1, fill = True)
    #img.draw_circle(boundary_dot1[0], boundary_dot1[1], 2, color = (255, 0, 0), thickness = 1, fill = True)
    #img.draw_circle(boundary_dot2[0], boundary_dot2[1], 2, color = (255, 0, 0), thickness = 1, fill = True)
    #img.draw_circle(boundary_dot3[0], boundary_dot3[1], 2, color = (255, 0, 0), thickness = 1, fill = True)
    #img.draw_circle(boundary_dot4[0], boundary_dot4[1], 2, color = (255, 0, 0), thickness = 1, fill = True)
    lcd.display(img)
