# 用于自动截取并保存位于第二屏幕的 Excel 打印预览（A4 横向）图片

import mss    # pip install mss
import os
import sys

def screenshot_second_monitor(filename="second_screen.png", width=1080, height=720):
    """
    截取第二屏幕，并保存为指定文件名的 1080x720 图片
    """
    with mss.mss() as sct:
        # 获取所有显示器信息
        monitors = sct.monitors
        print("检测到显示器：")
        for i, m in enumerate(monitors):
            print(f"  Monitor {i}: {m}")

        # monitors[0] 是所有屏幕的合并虚拟桌面
        # monitors[1] 通常是主屏，monitors[2] 是第二屏，以此类推
        if len(monitors) < 3:
            raise RuntimeError("未检测到第二屏幕，请确认已连接扩展显示器。")

        second_monitor = monitors[2]  # 第二屏

        # 可选：在第二屏上居中截取 1080x720 区域
        mx = second_monitor["left"]
        my = second_monitor["top"]
        mw = second_monitor["width"]
        mh = second_monitor["height"]

        # 计算居中区域
        region_left = mx + (mw - width) // 2
        region_left += 228
        region_top = my + (mh - height) // 2
        region_top += 32

        region = {
            "left": region_left,
            "top": region_top,
            "width": width,
            "height": height,
        }

        # 截图
        img = sct.grab(region)

        # 保存（mss 返回的是 PIL Image 兼容对象，直接用 mss.tools 保存）
        mss.tools.to_png(img.rgb, img.size, output=filename)
        print(f"✅ 截图已保存: {os.path.abspath(filename)} ({width}x{height})")

if __name__ == "__main__":
    # 如果命令行有传参，用第一个参数作为文件名
    if len(sys.argv) > 1:
        output_filename = sys.argv[1]
    else:
        output_filename = input("Input the filename of the captured image: ")
    screenshot_second_monitor(filename=output_filename, width=1080, height=720)