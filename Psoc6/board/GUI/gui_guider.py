# Copyright 2025 NXP
# NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
# accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
# activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
# comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
# terms, then you may not retain, install, activate or otherwise use the software.

import SDL
import utime as time
import usys as sys
import lvgl as lv
import lodepng as png
import ustruct
import fs_driver

lv.init()
SDL.init(w=320,h=240)

# Register SDL display driver.
disp_buf1 = lv.disp_draw_buf_t()
buf1_1 = bytearray(320*240*4)
disp_buf1.init(buf1_1, None, len(buf1_1)//4)
disp_drv = lv.disp_drv_t()
disp_drv.init()
disp_drv.draw_buf = disp_buf1
disp_drv.flush_cb = SDL.monitor_flush
disp_drv.hor_res = 320
disp_drv.ver_res = 240
disp_drv.register()

# Regsiter SDL mouse driver
indev_drv = lv.indev_drv_t()
indev_drv.init()
indev_drv.type = lv.INDEV_TYPE.POINTER
indev_drv.read_cb = SDL.mouse_read
indev_drv.register()

fs_drv = lv.fs_drv_t()
fs_driver.fs_register(fs_drv, 'Z')

# Below: Taken from https://github.com/lvgl/lv_binding_micropython/blob/master/driver/js/imagetools.py#L22-L94

COLOR_SIZE = lv.color_t.__SIZE__
COLOR_IS_SWAPPED = hasattr(lv.color_t().ch,'green_h')

class lodepng_error(RuntimeError):
    def __init__(self, err):
        if type(err) is int:
            super().__init__(png.error_text(err))
        else:
            super().__init__(err)

# Parse PNG file header
# Taken from https://github.com/shibukawa/imagesize_py/blob/ffef30c1a4715c5acf90e8945ceb77f4a2ed2d45/imagesize.py#L63-L85

def get_png_info(decoder, src, header):
    # Only handle variable image types

    if lv.img.src_get_type(src) != lv.img.SRC.VARIABLE:
        return lv.RES.INV

    data = lv.img_dsc_t.__cast__(src).data
    if data == None:
        return lv.RES.INV

    png_header = bytes(data.__dereference__(24))

    if png_header.startswith(b'\211PNG\r\n\032\n'):
        if png_header[12:16] == b'IHDR':
            start = 16
        # Maybe this is for an older PNG version.
        else:
            start = 8
        try:
            width, height = ustruct.unpack(">LL", png_header[start:start+8])
        except ustruct.error:
            return lv.RES.INV
    else:
        return lv.RES.INV

    header.always_zero = 0
    header.w = width
    header.h = height
    header.cf = lv.img.CF.TRUE_COLOR_ALPHA

    return lv.RES.OK

def convert_rgba8888_to_bgra8888(img_view):
    for i in range(0, len(img_view), lv.color_t.__SIZE__):
        ch = lv.color_t.__cast__(img_view[i:i]).ch
        ch.red, ch.blue = ch.blue, ch.red

# Read and parse PNG file

def open_png(decoder, dsc):
    img_dsc = lv.img_dsc_t.__cast__(dsc.src)
    png_data = img_dsc.data
    png_size = img_dsc.data_size
    png_decoded = png.C_Pointer()
    png_width = png.C_Pointer()
    png_height = png.C_Pointer()
    error = png.decode32(png_decoded, png_width, png_height, png_data, png_size)
    if error:
        raise lodepng_error(error)
    img_size = png_width.int_val * png_height.int_val * 4
    img_data = png_decoded.ptr_val
    img_view = img_data.__dereference__(img_size)

    if COLOR_SIZE == 4:
        convert_rgba8888_to_bgra8888(img_view)
    else:
        raise lodepng_error("Error: Color mode not supported yet!")

    dsc.img_data = img_data
    return lv.RES.OK

# Above: Taken from https://github.com/lvgl/lv_binding_micropython/blob/master/driver/js/imagetools.py#L22-L94

decoder = lv.img.decoder_create()
decoder.info_cb = get_png_info
decoder.open_cb = open_png

def anim_x_cb(obj, v):
    obj.set_x(v)

def anim_y_cb(obj, v):
    obj.set_y(v)

def anim_width_cb(obj, v):
    obj.set_width(v)

def anim_height_cb(obj, v):
    obj.set_height(v)

def anim_img_zoom_cb(obj, v):
    obj.set_zoom(v)

def anim_img_rotate_cb(obj, v):
    obj.set_angle(v)

global_font_cache = {}
def test_font(font_family, font_size):
    global global_font_cache
    if font_family + str(font_size) in global_font_cache:
        return global_font_cache[font_family + str(font_size)]
    if font_size % 2:
        candidates = [
            (font_family, font_size),
            (font_family, font_size-font_size%2),
            (font_family, font_size+font_size%2),
            ("montserrat", font_size-font_size%2),
            ("montserrat", font_size+font_size%2),
            ("montserrat", 16)
        ]
    else:
        candidates = [
            (font_family, font_size),
            ("montserrat", font_size),
            ("montserrat", 16)
        ]
    for (family, size) in candidates:
        try:
            if eval(f'lv.font_{family}_{size}'):
                global_font_cache[font_family + str(font_size)] = eval(f'lv.font_{family}_{size}')
                if family != font_family or size != font_size:
                    print(f'WARNING: lv.font_{family}_{size} is used!')
                return eval(f'lv.font_{family}_{size}')
        except AttributeError:
            try:
                load_font = lv.font_load(f"Z:MicroPython/lv_font_{family}_{size}.fnt")
                global_font_cache[font_family + str(font_size)] = load_font
                return load_font
            except:
                if family == font_family and size == font_size:
                    print(f'WARNING: lv.font_{family}_{size} is NOT supported!')

global_image_cache = {}
def load_image(file):
    global global_image_cache
    if file in global_image_cache:
        return global_image_cache[file]
    try:
        with open(file,'rb') as f:
            data = f.read()
    except:
        print(f'Could not open {file}')
        sys.exit()

    img = lv.img_dsc_t({
        'data_size': len(data),
        'data': data
    })
    global_image_cache[file] = img
    return img

def calendar_event_handler(e,obj):
    code = e.get_code()

    if code == lv.EVENT.VALUE_CHANGED:
        source = e.get_current_target()
        date = lv.calendar_date_t()
        if source.get_pressed_date(date) == lv.RES.OK:
            source.set_highlighted_dates([date], 1)

def spinbox_increment_event_cb(e, obj):
    code = e.get_code()
    if code == lv.EVENT.SHORT_CLICKED or code == lv.EVENT.LONG_PRESSED_REPEAT:
        obj.increment()
def spinbox_decrement_event_cb(e, obj):
    code = e.get_code()
    if code == lv.EVENT.SHORT_CLICKED or code == lv.EVENT.LONG_PRESSED_REPEAT:
        obj.decrement()

def digital_clock_cb(timer, obj, current_time, show_second, use_ampm):
    hour = int(current_time[0])
    minute = int(current_time[1])
    second = int(current_time[2])
    ampm = current_time[3]
    second = second + 1
    if second == 60:
        second = 0
        minute = minute + 1
        if minute == 60:
            minute = 0
            hour = hour + 1
            if use_ampm:
                if hour == 12:
                    if ampm == 'AM':
                        ampm = 'PM'
                    elif ampm == 'PM':
                        ampm = 'AM'
                if hour > 12:
                    hour = hour % 12
    hour = hour % 24
    if use_ampm:
        if show_second:
            obj.set_text("%d:%02d:%02d %s" %(hour, minute, second, ampm))
        else:
            obj.set_text("%d:%02d %s" %(hour, minute, ampm))
    else:
        if show_second:
            obj.set_text("%d:%02d:%02d" %(hour, minute, second))
        else:
            obj.set_text("%d:%02d" %(hour, minute))
    current_time[0] = hour
    current_time[1] = minute
    current_time[2] = second
    current_time[3] = ampm

def analog_clock_cb(timer, obj):
    datetime = time.localtime()
    hour = datetime[3]
    if hour >= 12: hour = hour - 12
    obj.set_time(hour, datetime[4], datetime[5])

def datetext_event_handler(e, obj):
    code = e.get_code()
    target = e.get_target()
    if code == lv.EVENT.FOCUSED:
        if obj is None:
            bg = lv.layer_top()
            bg.add_flag(lv.obj.FLAG.CLICKABLE)
            obj = lv.calendar(bg)
            scr = target.get_screen()
            scr_height = scr.get_height()
            scr_width = scr.get_width()
            obj.set_size(int(scr_width * 0.8), int(scr_height * 0.8))
            datestring = target.get_text()
            year = int(datestring.split('/')[0])
            month = int(datestring.split('/')[1])
            day = int(datestring.split('/')[2])
            obj.set_showed_date(year, month)
            highlighted_days=[lv.calendar_date_t({'year':year, 'month':month, 'day':day})]
            obj.set_highlighted_dates(highlighted_days, 1)
            obj.align(lv.ALIGN.CENTER, 0, 0)
            lv.calendar_header_arrow(obj)
            obj.add_event_cb(lambda e: datetext_calendar_event_handler(e, target), lv.EVENT.ALL, None)
            scr.update_layout()

def datetext_calendar_event_handler(e, obj):
    code = e.get_code()
    target = e.get_current_target()
    if code == lv.EVENT.VALUE_CHANGED:
        date = lv.calendar_date_t()
        if target.get_pressed_date(date) == lv.RES.OK:
            obj.set_text(f"{date.year}/{date.month}/{date.day}")
            bg = lv.layer_top()
            bg.clear_flag(lv.obj.FLAG.CLICKABLE)
            bg.set_style_bg_opa(lv.OPA.TRANSP, 0)
            target.delete()

# Create home
home = lv.obj()
home.set_size(320, 240)
home.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for home, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
home.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create home_cont_1
home_cont_1 = lv.obj(home)
home_cont_1.set_pos(0, 0)
home_cont_1.set_size(320, 240)
home_cont_1.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for home_cont_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
home_cont_1.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
home_cont_1.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
home_cont_1.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
home_cont_1.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
home_cont_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_cont_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
home_cont_1.set_style_bg_color(lv.color_hex(0xe7fafd), lv.PART.MAIN|lv.STATE.DEFAULT)
home_cont_1.set_style_bg_grad_dir(lv.GRAD_DIR.HOR, lv.PART.MAIN|lv.STATE.DEFAULT)
home_cont_1.set_style_bg_grad_color(lv.color_hex(0x4dffdf), lv.PART.MAIN|lv.STATE.DEFAULT)
home_cont_1.set_style_bg_main_stop(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_cont_1.set_style_bg_grad_stop(255, lv.PART.MAIN|lv.STATE.DEFAULT)
home_cont_1.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_cont_1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_cont_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_cont_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_cont_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create home_digital_clock_1
home_digital_clock_1_time = [int(11), int(25), int(50), "AM"]
home_digital_clock_1 = lv.dclock(home, "11:25:50 AM")
home_digital_clock_1_timer = lv.timer_create_basic()
home_digital_clock_1_timer.set_period(1000)
home_digital_clock_1_timer.set_cb(lambda src: digital_clock_cb(home_digital_clock_1_timer, home_digital_clock_1, home_digital_clock_1_time, True, True ))
home_digital_clock_1.set_pos(70, 41)
home_digital_clock_1.set_size(180, 50)
# Set style for home_digital_clock_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
home_digital_clock_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_digital_clock_1.set_style_text_color(lv.color_hex(0x2F35DA), lv.PART.MAIN|lv.STATE.DEFAULT)
home_digital_clock_1.set_style_text_font(test_font("Alatsi_Regular", 35), lv.PART.MAIN|lv.STATE.DEFAULT)
home_digital_clock_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
home_digital_clock_1.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_digital_clock_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
home_digital_clock_1.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_digital_clock_1.set_style_pad_top(7, lv.PART.MAIN|lv.STATE.DEFAULT)
home_digital_clock_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_digital_clock_1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_digital_clock_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_digital_clock_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create home_datetext_1
home_datetext_1 = lv.label(home)
home_datetext_1.set_text("2023/07/31")
home_datetext_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, 0)
home_datetext_1.add_flag(lv.obj.FLAG.CLICKABLE)
home_datetext_1_calendar = None
home_datetext_1.add_event_cb(lambda e: datetext_event_handler(e, home_datetext_1_calendar), lv.EVENT.ALL, None)
home_datetext_1.set_pos(95, 15)
home_datetext_1.set_size(130, 36)
# Set style for home_datetext_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
home_datetext_1.set_style_text_color(lv.color_hex(0x2F35DA), lv.PART.MAIN|lv.STATE.DEFAULT)
home_datetext_1.set_style_text_font(test_font("Alatsi_Regular", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
home_datetext_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
home_datetext_1.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
home_datetext_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
home_datetext_1.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_datetext_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_datetext_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_datetext_1.set_style_pad_top(7, lv.PART.MAIN|lv.STATE.DEFAULT)
home_datetext_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_datetext_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_datetext_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create home_btn_led
home_btn_led = lv.btn(home)
home_btn_led_label = lv.label(home_btn_led)
home_btn_led_label.set_text("Off\n")
home_btn_led_label.set_long_mode(lv.label.LONG.WRAP)
home_btn_led_label.set_width(lv.pct(100))
home_btn_led_label.align(lv.ALIGN.CENTER, 0, 0)
home_btn_led.set_style_pad_all(0, lv.STATE.DEFAULT)
home_btn_led.set_pos(34, 151)
home_btn_led.set_size(80, 50)
# Set style for home_btn_led, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
home_btn_led.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_led.set_style_bg_color(lv.color_hex(0x8c8d8e), lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_led.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_led.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_led.set_style_radius(30, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_led.set_style_shadow_width(6, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_led.set_style_shadow_color(lv.color_hex(0x4f606b), lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_led.set_style_shadow_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_led.set_style_shadow_spread(3, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_led.set_style_shadow_ofs_x(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_led.set_style_shadow_ofs_y(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_led.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_led.set_style_text_font(test_font("SourceHanSerifSC_Regular", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_led.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_led.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create home_bright_slider
home_bright_slider = lv.slider(home)
home_bright_slider.set_range(0, 100)
home_bright_slider.set_mode(lv.slider.MODE.NORMAL)
home_bright_slider.set_value(50, lv.ANIM.OFF)
home_bright_slider.set_pos(137, 98)
home_bright_slider.set_size(160, 8)
# Set style for home_bright_slider, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
home_bright_slider.set_style_bg_opa(80, lv.PART.MAIN|lv.STATE.DEFAULT)
home_bright_slider.set_style_bg_color(lv.color_hex(0x0c6aff), lv.PART.MAIN|lv.STATE.DEFAULT)
home_bright_slider.set_style_bg_grad_dir(lv.GRAD_DIR.HOR, lv.PART.MAIN|lv.STATE.DEFAULT)
home_bright_slider.set_style_bg_grad_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
home_bright_slider.set_style_bg_main_stop(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_bright_slider.set_style_bg_grad_stop(255, lv.PART.MAIN|lv.STATE.DEFAULT)
home_bright_slider.set_style_radius(8, lv.PART.MAIN|lv.STATE.DEFAULT)
home_bright_slider.set_style_outline_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_bright_slider.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for home_bright_slider, Part: lv.PART.MAIN, State: lv.STATE.FOCUSED.
home_bright_slider.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.FOCUSED)

# Set style for home_bright_slider, Part: lv.PART.INDICATOR, State: lv.STATE.DEFAULT.
home_bright_slider.set_style_bg_opa(255, lv.PART.INDICATOR|lv.STATE.DEFAULT)
home_bright_slider.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.INDICATOR|lv.STATE.DEFAULT)
home_bright_slider.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.INDICATOR|lv.STATE.DEFAULT)
home_bright_slider.set_style_radius(50, lv.PART.INDICATOR|lv.STATE.DEFAULT)

# Set style for home_bright_slider, Part: lv.PART.KNOB, State: lv.STATE.DEFAULT.
home_bright_slider.set_style_bg_opa(255, lv.PART.KNOB|lv.STATE.DEFAULT)
home_bright_slider.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.KNOB|lv.STATE.DEFAULT)
home_bright_slider.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.KNOB|lv.STATE.DEFAULT)
home_bright_slider.set_style_radius(50, lv.PART.KNOB|lv.STATE.DEFAULT)

# Create home_sw_2
home_sw_2 = lv.switch(home)
home_sw_2.set_pos(78, 91)
home_sw_2.set_size(40, 20)
# Set style for home_sw_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
home_sw_2.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
home_sw_2.set_style_bg_color(lv.color_hex(0xe6e2e6), lv.PART.MAIN|lv.STATE.DEFAULT)
home_sw_2.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
home_sw_2.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_sw_2.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
home_sw_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for home_sw_2, Part: lv.PART.MAIN, State: lv.STATE.FOCUSED.
home_sw_2.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.FOCUSED)
home_sw_2.set_style_bg_color(lv.color_hex(0xe6e2e6), lv.PART.MAIN|lv.STATE.FOCUSED)
home_sw_2.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.FOCUSED)
home_sw_2.set_style_border_width(0, lv.PART.MAIN|lv.STATE.FOCUSED)
home_sw_2.set_style_radius(10, lv.PART.MAIN|lv.STATE.FOCUSED)
home_sw_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.FOCUSED)
# Set style for home_sw_2, Part: lv.PART.MAIN, State: lv.STATE.DISABLED.
home_sw_2.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DISABLED)
home_sw_2.set_style_bg_color(lv.color_hex(0xe6e2e6), lv.PART.MAIN|lv.STATE.DISABLED)
home_sw_2.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DISABLED)
home_sw_2.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DISABLED)
home_sw_2.set_style_radius(10, lv.PART.MAIN|lv.STATE.DISABLED)
home_sw_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DISABLED)
# Set style for home_sw_2, Part: lv.PART.INDICATOR, State: lv.STATE.CHECKED.
home_sw_2.set_style_bg_opa(255, lv.PART.INDICATOR|lv.STATE.CHECKED)
home_sw_2.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.INDICATOR|lv.STATE.CHECKED)
home_sw_2.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.INDICATOR|lv.STATE.CHECKED)
home_sw_2.set_style_border_width(0, lv.PART.INDICATOR|lv.STATE.CHECKED)

# Set style for home_sw_2, Part: lv.PART.KNOB, State: lv.STATE.DEFAULT.
home_sw_2.set_style_bg_opa(255, lv.PART.KNOB|lv.STATE.DEFAULT)
home_sw_2.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.KNOB|lv.STATE.DEFAULT)
home_sw_2.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.KNOB|lv.STATE.DEFAULT)
home_sw_2.set_style_border_width(0, lv.PART.KNOB|lv.STATE.DEFAULT)
home_sw_2.set_style_radius(10, lv.PART.KNOB|lv.STATE.DEFAULT)

# Create home_cont_3
home_cont_3 = lv.obj(home)
home_cont_3.set_pos(144, 127)
home_cont_3.set_size(150, 100)
home_cont_3.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for home_cont_3, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
home_cont_3.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
home_cont_3.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
home_cont_3.set_style_border_color(lv.color_hex(0xbdfffc), lv.PART.MAIN|lv.STATE.DEFAULT)
home_cont_3.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
home_cont_3.set_style_radius(15, lv.PART.MAIN|lv.STATE.DEFAULT)
home_cont_3.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
home_cont_3.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
home_cont_3.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
home_cont_3.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_cont_3.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_cont_3.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_cont_3.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_cont_3.set_style_shadow_width(1, lv.PART.MAIN|lv.STATE.DEFAULT)
home_cont_3.set_style_shadow_color(lv.color_hex(0x6e8385), lv.PART.MAIN|lv.STATE.DEFAULT)
home_cont_3.set_style_shadow_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
home_cont_3.set_style_shadow_spread(2, lv.PART.MAIN|lv.STATE.DEFAULT)
home_cont_3.set_style_shadow_ofs_x(3, lv.PART.MAIN|lv.STATE.DEFAULT)
home_cont_3.set_style_shadow_ofs_y(2, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create home_btn_2
home_btn_2 = lv.btn(home_cont_3)
home_btn_2_label = lv.label(home_btn_2)
home_btn_2_label.set_text("监测模式")
home_btn_2_label.set_long_mode(lv.label.LONG.WRAP)
home_btn_2_label.set_width(lv.pct(100))
home_btn_2_label.align(lv.ALIGN.CENTER, 0, 0)
home_btn_2.set_style_pad_all(0, lv.STATE.DEFAULT)
home_btn_2.set_pos(48, 5)
home_btn_2.set_size(92, 26)
# Set style for home_btn_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
home_btn_2.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_2.set_style_bg_color(lv.color_hex(0x46e6bc), lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_2.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_2.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_2.set_style_radius(12, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_2.set_style_shadow_width(1, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_2.set_style_shadow_color(lv.color_hex(0xc4c4c4), lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_2.set_style_shadow_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_2.set_style_shadow_spread(1, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_2.set_style_shadow_ofs_x(1, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_2.set_style_shadow_ofs_y(1, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_2.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_2.set_style_text_font(test_font("SourceHanSerifSC_Regular", 13), lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_2.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_2.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create home_btn_4
home_btn_4 = lv.btn(home_cont_3)
home_btn_4_label = lv.label(home_btn_4)
home_btn_4_label.set_text("警戒模式")
home_btn_4_label.set_long_mode(lv.label.LONG.WRAP)
home_btn_4_label.set_width(lv.pct(100))
home_btn_4_label.align(lv.ALIGN.CENTER, 0, 0)
home_btn_4.set_style_pad_all(0, lv.STATE.DEFAULT)
home_btn_4.set_pos(48, 67)
home_btn_4.set_size(92, 26)
# Set style for home_btn_4, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
home_btn_4.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_4.set_style_bg_color(lv.color_hex(0xbedc3f), lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_4.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_4.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_4.set_style_radius(12, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_4.set_style_shadow_width(1, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_4.set_style_shadow_color(lv.color_hex(0xc4c4c4), lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_4.set_style_shadow_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_4.set_style_shadow_spread(1, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_4.set_style_shadow_ofs_x(1, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_4.set_style_shadow_ofs_y(1, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_4.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_4.set_style_text_font(test_font("SourceHanSerifSC_Regular", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_4.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_4.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create home_btn_3
home_btn_3 = lv.btn(home_cont_3)
home_btn_3_label = lv.label(home_btn_3)
home_btn_3_label.set_text("设置")
home_btn_3_label.set_long_mode(lv.label.LONG.WRAP)
home_btn_3_label.set_width(lv.pct(100))
home_btn_3_label.align(lv.ALIGN.CENTER, 0, 0)
home_btn_3.set_style_pad_all(0, lv.STATE.DEFAULT)
home_btn_3.set_pos(48, 36)
home_btn_3.set_size(92, 26)
# Set style for home_btn_3, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
home_btn_3.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_3.set_style_bg_color(lv.color_hex(0x82e249), lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_3.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_3.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_3.set_style_radius(12, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_3.set_style_shadow_width(1, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_3.set_style_shadow_color(lv.color_hex(0xc4c4c4), lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_3.set_style_shadow_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_3.set_style_shadow_spread(1, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_3.set_style_shadow_ofs_x(1, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_3.set_style_shadow_ofs_y(1, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_3.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_3.set_style_text_font(test_font("SourceHanSerifSC_Regular", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_3.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
home_btn_3.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create home_label_2
home_label_2 = lv.label(home)
home_label_2.set_text("自动亮度")
home_label_2.set_long_mode(lv.label.LONG.WRAP)
home_label_2.set_width(lv.pct(100))
home_label_2.set_pos(8, 91)
home_label_2.set_size(70, 18)
# Set style for home_label_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
home_label_2.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_2.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_2.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_2.set_style_text_font(test_font("SourceHanSerifSC_Regular", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_2.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_2.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_2.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_2.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_2.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_2.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_2.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_2.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_2.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create home_label_3
home_label_3 = lv.label(home)
home_label_3.set_text("")
home_label_3.set_long_mode(lv.label.LONG.WRAP)
home_label_3.set_width(lv.pct(100))
home_label_3.set_pos(152, 140)
home_label_3.set_size(36, 15)
# Set style for home_label_3, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
home_label_3.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_3.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_3.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_3.set_style_text_font(test_font("iconfont", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_3.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_3.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_3.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_3.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_3.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_3.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_3.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_3.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_3.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_3.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create home_label_4
home_label_4 = lv.label(home)
home_label_4.set_text("")
home_label_4.set_long_mode(lv.label.LONG.WRAP)
home_label_4.set_width(lv.pct(100))
home_label_4.set_pos(152, 171)
home_label_4.set_size(36, 15)
# Set style for home_label_4, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
home_label_4.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_4.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_4.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_4.set_style_text_font(test_font("iconfont", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_4.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_4.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_4.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_4.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_4.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_4.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_4.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_4.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_4.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_4.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create home_label_5
home_label_5 = lv.label(home)
home_label_5.set_text("")
home_label_5.set_long_mode(lv.label.LONG.WRAP)
home_label_5.set_width(lv.pct(100))
home_label_5.set_pos(152, 201)
home_label_5.set_size(36, 15)
# Set style for home_label_5, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
home_label_5.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_5.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_5.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_5.set_style_text_font(test_font("iconfont", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_5.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_5.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_5.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_5.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_5.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_5.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_5.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_5.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_5.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
home_label_5.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

home.update_layout()
# Create monitor
monitor = lv.obj()
monitor.set_size(320, 240)
monitor.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for monitor, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create monitor_cont_1
monitor_cont_1 = lv.obj(monitor)
monitor_cont_1.set_pos(0, 0)
monitor_cont_1.set_size(320, 240)
monitor_cont_1.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for monitor_cont_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_cont_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_1.set_style_bg_color(lv.color_hex(0xccffe0), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_1.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create monitor_tabview_1
monitor_tabview_1 = lv.tabview(monitor, lv.DIR.TOP, 25)
monitor_tabview_1.set_pos(0, 15)
monitor_tabview_1.set_size(320, 225)
monitor_tabview_1.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for monitor_tabview_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_tabview_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tabview_1.set_style_bg_color(lv.color_hex(0x4dfff3), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tabview_1.set_style_bg_grad_dir(lv.GRAD_DIR.HOR, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tabview_1.set_style_bg_grad_color(lv.color_hex(0x4d9cff), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tabview_1.set_style_bg_main_stop(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tabview_1.set_style_bg_grad_stop(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tabview_1.set_style_text_color(lv.color_hex(0x4d4d4d), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tabview_1.set_style_text_font(test_font("SourceHanSerifSC_Regular", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tabview_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tabview_1.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tabview_1.set_style_text_line_space(16, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tabview_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tabview_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tabview_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for monitor_tabview_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
style_monitor_tabview_1_extra_btnm_main_default = lv.style_t()
style_monitor_tabview_1_extra_btnm_main_default.init()
style_monitor_tabview_1_extra_btnm_main_default.set_bg_opa(255)
style_monitor_tabview_1_extra_btnm_main_default.set_bg_color(lv.color_hex(0xffffff))
style_monitor_tabview_1_extra_btnm_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_monitor_tabview_1_extra_btnm_main_default.set_border_width(0)
style_monitor_tabview_1_extra_btnm_main_default.set_radius(0)
monitor_tabview_1.get_tab_btns().add_style(style_monitor_tabview_1_extra_btnm_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for monitor_tabview_1, Part: lv.PART.ITEMS, State: lv.STATE.DEFAULT.
style_monitor_tabview_1_extra_btnm_items_default = lv.style_t()
style_monitor_tabview_1_extra_btnm_items_default.init()
style_monitor_tabview_1_extra_btnm_items_default.set_text_color(lv.color_hex(0x4d4d4d))
style_monitor_tabview_1_extra_btnm_items_default.set_text_font(test_font("SourceHanSerifSC_Regular", 12))
style_monitor_tabview_1_extra_btnm_items_default.set_text_opa(255)
monitor_tabview_1.get_tab_btns().add_style(style_monitor_tabview_1_extra_btnm_items_default, lv.PART.ITEMS|lv.STATE.DEFAULT)
# Set style for monitor_tabview_1, Part: lv.PART.ITEMS, State: lv.STATE.CHECKED.
style_monitor_tabview_1_extra_btnm_items_checked = lv.style_t()
style_monitor_tabview_1_extra_btnm_items_checked.init()
style_monitor_tabview_1_extra_btnm_items_checked.set_text_color(lv.color_hex(0x2195f6))
style_monitor_tabview_1_extra_btnm_items_checked.set_text_font(test_font("SourceHanSerifSC_Regular", 12))
style_monitor_tabview_1_extra_btnm_items_checked.set_text_opa(255)
style_monitor_tabview_1_extra_btnm_items_checked.set_border_width(4)
style_monitor_tabview_1_extra_btnm_items_checked.set_border_opa(255)
style_monitor_tabview_1_extra_btnm_items_checked.set_border_color(lv.color_hex(0x2195f6))
style_monitor_tabview_1_extra_btnm_items_checked.set_border_side(lv.BORDER_SIDE.BOTTOM)
style_monitor_tabview_1_extra_btnm_items_checked.set_radius(0)
style_monitor_tabview_1_extra_btnm_items_checked.set_bg_opa(60)
style_monitor_tabview_1_extra_btnm_items_checked.set_bg_color(lv.color_hex(0x2195f6))
style_monitor_tabview_1_extra_btnm_items_checked.set_bg_grad_dir(lv.GRAD_DIR.NONE)
monitor_tabview_1.get_tab_btns().add_style(style_monitor_tabview_1_extra_btnm_items_checked, lv.PART.ITEMS|lv.STATE.CHECKED)
# Create 数据
monitor_tabview_1_tab_1 = monitor_tabview_1.add_tab("数据")
monitor_tabview_1_tab_1_label = lv.label(monitor_tabview_1_tab_1)
monitor_tabview_1_tab_1_label.set_text("")

# Create monitor_cont_3
monitor_cont_3 = lv.obj(monitor_tabview_1_tab_1)
monitor_cont_3.set_pos(24, -11)
monitor_cont_3.set_size(92, 175)
monitor_cont_3.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for monitor_cont_3, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_cont_3.set_style_border_width(1, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_3.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_3.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_3.set_style_border_side(lv.BORDER_SIDE.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_3.set_style_radius(15, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_3.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_3.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_3.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_3.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_3.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_3.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_3.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_3.set_style_shadow_width(1, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_3.set_style_shadow_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_3.set_style_shadow_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_3.set_style_shadow_spread(2, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_3.set_style_shadow_ofs_x(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_3.set_style_shadow_ofs_y(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create monitor_breath_l
monitor_breath_l = lv.label(monitor_cont_3)
monitor_breath_l.set_text("Label")
monitor_breath_l.set_long_mode(lv.label.LONG.WRAP)
monitor_breath_l.set_width(lv.pct(100))
monitor_breath_l.set_pos(27, 125)
monitor_breath_l.set_size(52, 16)
# Set style for monitor_breath_l, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_breath_l.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_breath_l.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_breath_l.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_breath_l.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_breath_l.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_breath_l.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_breath_l.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_breath_l.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_breath_l.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_breath_l.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_breath_l.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_breath_l.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_breath_l.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_breath_l.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create monitor_temperature
monitor_temperature = lv.label(monitor_cont_3)
monitor_temperature.set_text("")
monitor_temperature.set_long_mode(lv.label.LONG.WRAP)
monitor_temperature.set_width(lv.pct(100))
monitor_temperature.set_pos(3, 26)
monitor_temperature.set_size(26, 15)
# Set style for monitor_temperature, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_temperature.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_temperature.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_temperature.set_style_text_color(lv.color_hex(0xff6500), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_temperature.set_style_text_font(test_font("iconfont", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_temperature.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_temperature.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_temperature.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_temperature.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_temperature.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_temperature.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_temperature.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_temperature.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_temperature.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_temperature.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create monitor_co2
monitor_co2 = lv.label(monitor_cont_3)
monitor_co2.set_text("")
monitor_co2.set_long_mode(lv.label.LONG.WRAP)
monitor_co2.set_width(lv.pct(100))
monitor_co2.set_pos(3, 84)
monitor_co2.set_size(26, 15)
# Set style for monitor_co2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_co2.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_co2.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_co2.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_co2.set_style_text_font(test_font("iconfont", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_co2.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_co2.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_co2.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_co2.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_co2.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_co2.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_co2.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_co2.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_co2.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_co2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create monitor_label_4
monitor_label_4 = lv.label(monitor_cont_3)
monitor_label_4.set_text("当前")
monitor_label_4.set_long_mode(lv.label.LONG.WRAP)
monitor_label_4.set_width(lv.pct(100))
monitor_label_4.set_pos(6, 5)
monitor_label_4.set_size(80, 16)
# Set style for monitor_label_4, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_label_4.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_4.set_style_radius(15, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_4.set_style_text_color(lv.color_hex(0x2900ff), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_4.set_style_text_font(test_font("SourceHanSerifSC_Regular", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_4.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_4.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_4.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_4.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_4.set_style_bg_opa(64, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_4.set_style_bg_color(lv.color_hex(0xb400ff), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_4.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_4.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_4.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_4.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_4.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_4.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create monitor_humidity
monitor_humidity = lv.label(monitor_cont_3)
monitor_humidity.set_text("")
monitor_humidity.set_long_mode(lv.label.LONG.WRAP)
monitor_humidity.set_width(lv.pct(100))
monitor_humidity.set_pos(3, 44)
monitor_humidity.set_size(26, 15)
# Set style for monitor_humidity, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_humidity.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_humidity.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_humidity.set_style_text_color(lv.color_hex(0x0093ff), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_humidity.set_style_text_font(test_font("iconfont", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_humidity.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_humidity.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_humidity.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_humidity.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_humidity.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_humidity.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_humidity.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_humidity.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_humidity.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_humidity.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create monitor_pressure
monitor_pressure = lv.label(monitor_cont_3)
monitor_pressure.set_text("")
monitor_pressure.set_long_mode(lv.label.LONG.WRAP)
monitor_pressure.set_width(lv.pct(100))
monitor_pressure.set_pos(3, 63)
monitor_pressure.set_size(26, 15)
# Set style for monitor_pressure, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_pressure.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_pressure.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_pressure.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_pressure.set_style_text_font(test_font("iconfont", 15), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_pressure.set_style_text_opa(203, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_pressure.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_pressure.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_pressure.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_pressure.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_pressure.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_pressure.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_pressure.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_pressure.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_pressure.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create monitor_heart
monitor_heart = lv.label(monitor_cont_3)
monitor_heart.set_text("")
monitor_heart.set_long_mode(lv.label.LONG.WRAP)
monitor_heart.set_width(lv.pct(100))
monitor_heart.set_pos(3, 145)
monitor_heart.set_size(26, 15)
# Set style for monitor_heart, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_heart.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_heart.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_heart.set_style_text_color(lv.color_hex(0xf00000), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_heart.set_style_text_font(test_font("iconfont", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_heart.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_heart.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_heart.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_heart.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_heart.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_heart.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_heart.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_heart.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_heart.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_heart.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create monitor_tvoc
monitor_tvoc = lv.label(monitor_cont_3)
monitor_tvoc.set_text("")
monitor_tvoc.set_long_mode(lv.label.LONG.WRAP)
monitor_tvoc.set_width(lv.pct(100))
monitor_tvoc.set_pos(3, 105)
monitor_tvoc.set_size(26, 15)
# Set style for monitor_tvoc, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_tvoc.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tvoc.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tvoc.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tvoc.set_style_text_font(test_font("iconfont", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tvoc.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tvoc.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tvoc.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tvoc.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tvoc.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tvoc.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tvoc.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tvoc.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tvoc.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tvoc.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create monitor_breath
monitor_breath = lv.label(monitor_cont_3)
monitor_breath.set_text("")
monitor_breath.set_long_mode(lv.label.LONG.WRAP)
monitor_breath.set_width(lv.pct(100))
monitor_breath.set_pos(3, 123)
monitor_breath.set_size(26, 16)
# Set style for monitor_breath, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_breath.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_breath.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_breath.set_style_text_color(lv.color_hex(0xff3c69), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_breath.set_style_text_font(test_font("iconfont", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_breath.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_breath.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_breath.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_breath.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_breath.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_breath.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_breath.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_breath.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_breath.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_breath.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create monitor_humidity_l
monitor_humidity_l = lv.label(monitor_cont_3)
monitor_humidity_l.set_text("Label")
monitor_humidity_l.set_long_mode(lv.label.LONG.WRAP)
monitor_humidity_l.set_width(lv.pct(100))
monitor_humidity_l.set_pos(26, 45)
monitor_humidity_l.set_size(52, 16)
# Set style for monitor_humidity_l, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_humidity_l.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_humidity_l.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_humidity_l.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_humidity_l.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_humidity_l.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_humidity_l.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_humidity_l.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_humidity_l.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_humidity_l.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_humidity_l.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_humidity_l.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_humidity_l.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_humidity_l.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_humidity_l.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create monitor_temperature_l
monitor_temperature_l = lv.label(monitor_cont_3)
monitor_temperature_l.set_text("Label")
monitor_temperature_l.set_long_mode(lv.label.LONG.WRAP)
monitor_temperature_l.set_width(lv.pct(100))
monitor_temperature_l.set_pos(26, 25)
monitor_temperature_l.set_size(52, 16)
# Set style for monitor_temperature_l, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_temperature_l.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_temperature_l.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_temperature_l.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_temperature_l.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_temperature_l.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_temperature_l.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_temperature_l.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_temperature_l.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_temperature_l.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_temperature_l.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_temperature_l.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_temperature_l.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_temperature_l.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_temperature_l.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create monitor_pressure_l
monitor_pressure_l = lv.label(monitor_cont_3)
monitor_pressure_l.set_text("Label")
monitor_pressure_l.set_long_mode(lv.label.LONG.WRAP)
monitor_pressure_l.set_width(lv.pct(100))
monitor_pressure_l.set_pos(26, 65)
monitor_pressure_l.set_size(52, 16)
# Set style for monitor_pressure_l, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_pressure_l.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_pressure_l.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_pressure_l.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_pressure_l.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_pressure_l.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_pressure_l.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_pressure_l.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_pressure_l.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_pressure_l.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_pressure_l.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_pressure_l.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_pressure_l.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_pressure_l.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_pressure_l.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create monitor_co2_l
monitor_co2_l = lv.label(monitor_cont_3)
monitor_co2_l.set_text("Label")
monitor_co2_l.set_long_mode(lv.label.LONG.WRAP)
monitor_co2_l.set_width(lv.pct(100))
monitor_co2_l.set_pos(26, 85)
monitor_co2_l.set_size(52, 16)
# Set style for monitor_co2_l, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_co2_l.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_co2_l.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_co2_l.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_co2_l.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_co2_l.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_co2_l.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_co2_l.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_co2_l.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_co2_l.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_co2_l.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_co2_l.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_co2_l.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_co2_l.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_co2_l.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create monitor_tvoc_l
monitor_tvoc_l = lv.label(monitor_cont_3)
monitor_tvoc_l.set_text("Label")
monitor_tvoc_l.set_long_mode(lv.label.LONG.WRAP)
monitor_tvoc_l.set_width(lv.pct(100))
monitor_tvoc_l.set_pos(26, 105)
monitor_tvoc_l.set_size(52, 16)
# Set style for monitor_tvoc_l, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_tvoc_l.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tvoc_l.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tvoc_l.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tvoc_l.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tvoc_l.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tvoc_l.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tvoc_l.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tvoc_l.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tvoc_l.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tvoc_l.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tvoc_l.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tvoc_l.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tvoc_l.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_tvoc_l.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create monitor_heart_l
monitor_heart_l = lv.label(monitor_cont_3)
monitor_heart_l.set_text("Label")
monitor_heart_l.set_long_mode(lv.label.LONG.WRAP)
monitor_heart_l.set_width(lv.pct(100))
monitor_heart_l.set_pos(26, 145)
monitor_heart_l.set_size(52, 16)
# Set style for monitor_heart_l, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_heart_l.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_heart_l.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_heart_l.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_heart_l.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_heart_l.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_heart_l.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_heart_l.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_heart_l.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_heart_l.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_heart_l.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_heart_l.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_heart_l.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_heart_l.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_heart_l.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create 工作状态
monitor_tabview_1_tab_2 = monitor_tabview_1.add_tab("工作状态")
monitor_tabview_1_tab_2_label = lv.label(monitor_tabview_1_tab_2)
monitor_tabview_1_tab_2_label.set_text("")

# Create monitor_bar_energy
monitor_bar_energy = lv.bar(monitor_tabview_1_tab_2)
monitor_bar_energy.set_style_anim_time(1000, 0)
monitor_bar_energy.set_mode(lv.bar.MODE.NORMAL)
monitor_bar_energy.set_range(0, 100)
monitor_bar_energy.set_value(50, lv.ANIM.OFF)
monitor_bar_energy.set_pos(94, 10)
monitor_bar_energy.set_size(181, 15)
# Set style for monitor_bar_energy, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_bar_energy.set_style_bg_opa(180, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_bar_energy.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_bar_energy.set_style_bg_grad_dir(lv.GRAD_DIR.HOR, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_bar_energy.set_style_bg_grad_color(lv.color_hex(0x920ee3), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_bar_energy.set_style_bg_main_stop(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_bar_energy.set_style_bg_grad_stop(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_bar_energy.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_bar_energy.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for monitor_bar_energy, Part: lv.PART.INDICATOR, State: lv.STATE.DEFAULT.
monitor_bar_energy.set_style_bg_opa(255, lv.PART.INDICATOR|lv.STATE.DEFAULT)
monitor_bar_energy.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.INDICATOR|lv.STATE.DEFAULT)
monitor_bar_energy.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.INDICATOR|lv.STATE.DEFAULT)
monitor_bar_energy.set_style_radius(10, lv.PART.INDICATOR|lv.STATE.DEFAULT)

# Create monitor_bar_carefulness
monitor_bar_carefulness = lv.bar(monitor_tabview_1_tab_2)
monitor_bar_carefulness.set_style_anim_time(1000, 0)
monitor_bar_carefulness.set_mode(lv.bar.MODE.NORMAL)
monitor_bar_carefulness.set_range(0, 100)
monitor_bar_carefulness.set_value(50, lv.ANIM.OFF)
monitor_bar_carefulness.set_pos(95, 39)
monitor_bar_carefulness.set_size(181, 15)
# Set style for monitor_bar_carefulness, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_bar_carefulness.set_style_bg_opa(180, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_bar_carefulness.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_bar_carefulness.set_style_bg_grad_dir(lv.GRAD_DIR.HOR, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_bar_carefulness.set_style_bg_grad_color(lv.color_hex(0x920ee3), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_bar_carefulness.set_style_bg_main_stop(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_bar_carefulness.set_style_bg_grad_stop(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_bar_carefulness.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_bar_carefulness.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for monitor_bar_carefulness, Part: lv.PART.INDICATOR, State: lv.STATE.DEFAULT.
monitor_bar_carefulness.set_style_bg_opa(255, lv.PART.INDICATOR|lv.STATE.DEFAULT)
monitor_bar_carefulness.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.INDICATOR|lv.STATE.DEFAULT)
monitor_bar_carefulness.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.INDICATOR|lv.STATE.DEFAULT)
monitor_bar_carefulness.set_style_radius(10, lv.PART.INDICATOR|lv.STATE.DEFAULT)

# Create monitor_label_8
monitor_label_8 = lv.label(monitor_tabview_1_tab_2)
monitor_label_8.set_text("专注度")
monitor_label_8.set_long_mode(lv.label.LONG.WRAP)
monitor_label_8.set_width(lv.pct(100))
monitor_label_8.set_pos(14, 37)
monitor_label_8.set_size(67, 16)
# Set style for monitor_label_8, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_label_8.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_8.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_8.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_8.set_style_text_font(test_font("SourceHanSerifSC_Regular", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_8.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_8.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_8.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_8.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_8.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_8.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_8.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_8.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_8.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_8.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create monitor_label_7
monitor_label_7 = lv.label(monitor_tabview_1_tab_2)
monitor_label_7.set_text("能量值")
monitor_label_7.set_long_mode(lv.label.LONG.WRAP)
monitor_label_7.set_width(lv.pct(100))
monitor_label_7.set_pos(15, 9)
monitor_label_7.set_size(67, 16)
# Set style for monitor_label_7, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_label_7.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_7.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_7.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_7.set_style_text_font(test_font("SourceHanSerifSC_Regular", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_7.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_7.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_7.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_7.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_7.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_7.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_7.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_7.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_7.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_7.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create 设置
monitor_tabview_1_tab_3 = monitor_tabview_1.add_tab("设置")
monitor_tabview_1_tab_3_label = lv.label(monitor_tabview_1_tab_3)
monitor_tabview_1_tab_3_label.set_text("")

# Create monitor_work_btn
monitor_work_btn = lv.btn(monitor_tabview_1_tab_3)
monitor_work_btn_label = lv.label(monitor_work_btn)
monitor_work_btn_label.set_text("Button")
monitor_work_btn_label.set_long_mode(lv.label.LONG.WRAP)
monitor_work_btn_label.set_width(lv.pct(100))
monitor_work_btn_label.align(lv.ALIGN.CENTER, 0, 0)
monitor_work_btn.set_style_pad_all(0, lv.STATE.DEFAULT)
monitor_work_btn.set_pos(50, 108)
monitor_work_btn.set_size(72, 50)
# Set style for monitor_work_btn, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_work_btn.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_work_btn.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_work_btn.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_work_btn.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_work_btn.set_style_radius(20, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_work_btn.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_work_btn.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_work_btn.set_style_text_font(test_font("SourceHanSerifSC_Regular", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_work_btn.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_work_btn.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create monitor_ddlist_1
monitor_ddlist_1 = lv.dropdown(monitor_tabview_1_tab_3)
monitor_ddlist_1.set_options("30min\n1h\n1.5h\n2h")
monitor_ddlist_1.set_pos(112, -12)
monitor_ddlist_1.set_size(82, 30)
# Set style for monitor_ddlist_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_ddlist_1.set_style_text_color(lv.color_hex(0x0D3055), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_1.set_style_text_font(test_font("SourceHanSerifSC_Regular", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_1.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_1.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_1.set_style_border_color(lv.color_hex(0x4da0ff), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_1.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_1.set_style_pad_top(7, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_1.set_style_pad_left(6, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_1.set_style_pad_right(6, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_1.set_style_radius(7, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_1.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for monitor_ddlist_1, Part: lv.PART.SELECTED, State: lv.STATE.CHECKED.
style_monitor_ddlist_1_extra_list_selected_checked = lv.style_t()
style_monitor_ddlist_1_extra_list_selected_checked.init()
style_monitor_ddlist_1_extra_list_selected_checked.set_border_width(1)
style_monitor_ddlist_1_extra_list_selected_checked.set_border_opa(255)
style_monitor_ddlist_1_extra_list_selected_checked.set_border_color(lv.color_hex(0xe1e6ee))
style_monitor_ddlist_1_extra_list_selected_checked.set_border_side(lv.BORDER_SIDE.FULL)
style_monitor_ddlist_1_extra_list_selected_checked.set_radius(3)
style_monitor_ddlist_1_extra_list_selected_checked.set_bg_opa(255)
style_monitor_ddlist_1_extra_list_selected_checked.set_bg_color(lv.color_hex(0x00a1b5))
style_monitor_ddlist_1_extra_list_selected_checked.set_bg_grad_dir(lv.GRAD_DIR.NONE)
monitor_ddlist_1.get_list().add_style(style_monitor_ddlist_1_extra_list_selected_checked, lv.PART.SELECTED|lv.STATE.CHECKED)
# Set style for monitor_ddlist_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
style_monitor_ddlist_1_extra_list_main_default = lv.style_t()
style_monitor_ddlist_1_extra_list_main_default.init()
style_monitor_ddlist_1_extra_list_main_default.set_max_height(90)
style_monitor_ddlist_1_extra_list_main_default.set_text_color(lv.color_hex(0x0D3055))
style_monitor_ddlist_1_extra_list_main_default.set_text_font(test_font("SourceHanSerifSC_Regular", 12))
style_monitor_ddlist_1_extra_list_main_default.set_text_opa(255)
style_monitor_ddlist_1_extra_list_main_default.set_border_width(1)
style_monitor_ddlist_1_extra_list_main_default.set_border_opa(255)
style_monitor_ddlist_1_extra_list_main_default.set_border_color(lv.color_hex(0xe1e6ee))
style_monitor_ddlist_1_extra_list_main_default.set_border_side(lv.BORDER_SIDE.FULL)
style_monitor_ddlist_1_extra_list_main_default.set_radius(3)
style_monitor_ddlist_1_extra_list_main_default.set_bg_opa(255)
style_monitor_ddlist_1_extra_list_main_default.set_bg_color(lv.color_hex(0xffffff))
style_monitor_ddlist_1_extra_list_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
monitor_ddlist_1.get_list().add_style(style_monitor_ddlist_1_extra_list_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for monitor_ddlist_1, Part: lv.PART.SCROLLBAR, State: lv.STATE.DEFAULT.
style_monitor_ddlist_1_extra_list_scrollbar_default = lv.style_t()
style_monitor_ddlist_1_extra_list_scrollbar_default.init()
style_monitor_ddlist_1_extra_list_scrollbar_default.set_radius(3)
style_monitor_ddlist_1_extra_list_scrollbar_default.set_bg_opa(255)
style_monitor_ddlist_1_extra_list_scrollbar_default.set_bg_color(lv.color_hex(0x00ff00))
style_monitor_ddlist_1_extra_list_scrollbar_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
monitor_ddlist_1.get_list().add_style(style_monitor_ddlist_1_extra_list_scrollbar_default, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)

# Create monitor_label_5
monitor_label_5 = lv.label(monitor_tabview_1_tab_3)
monitor_label_5.set_text("工作时间：")
monitor_label_5.set_long_mode(lv.label.LONG.WRAP)
monitor_label_5.set_width(lv.pct(100))
monitor_label_5.set_pos(24, -7)
monitor_label_5.set_size(88, 21)
# Set style for monitor_label_5, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_label_5.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_5.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_5.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_5.set_style_text_font(test_font("SourceHanSerifSC_Regular", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_5.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_5.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_5.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_5.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_5.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_5.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_5.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_5.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_5.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_5.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create monitor_spinner_1
monitor_spinner_1 = lv.spinner(monitor_tabview_1_tab_3, 3000, 60)
monitor_spinner_1.set_pos(150, 96)
monitor_spinner_1.set_size(78, 80)
# Set style for monitor_spinner_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_spinner_1.set_style_pad_top(5, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_spinner_1.set_style_pad_bottom(5, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_spinner_1.set_style_pad_left(5, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_spinner_1.set_style_pad_right(5, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_spinner_1.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_spinner_1.set_style_arc_width(12, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_spinner_1.set_style_arc_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_spinner_1.set_style_arc_color(lv.color_hex(0xd5d6de), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_spinner_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for monitor_spinner_1, Part: lv.PART.INDICATOR, State: lv.STATE.DEFAULT.
monitor_spinner_1.set_style_arc_width(12, lv.PART.INDICATOR|lv.STATE.DEFAULT)
monitor_spinner_1.set_style_arc_opa(255, lv.PART.INDICATOR|lv.STATE.DEFAULT)
monitor_spinner_1.set_style_arc_color(lv.color_hex(0x2195f6), lv.PART.INDICATOR|lv.STATE.DEFAULT)

# Create monitor_label_6
monitor_label_6 = lv.label(monitor_tabview_1_tab_3)
monitor_label_6.set_text("休息时间：")
monitor_label_6.set_long_mode(lv.label.LONG.WRAP)
monitor_label_6.set_width(lv.pct(100))
monitor_label_6.set_pos(24, 28)
monitor_label_6.set_size(88, 21)
# Set style for monitor_label_6, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_label_6.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_6.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_6.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_6.set_style_text_font(test_font("SourceHanSerifSC_Regular", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_6.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_6.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_6.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_6.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_6.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_6.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_6.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_6.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_6.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_6.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create monitor_ddlist_2
monitor_ddlist_2 = lv.dropdown(monitor_tabview_1_tab_3)
monitor_ddlist_2.set_options("5min\n10h\n15min\n30min\n1h")
monitor_ddlist_2.set_pos(112, 24)
monitor_ddlist_2.set_size(82, 30)
# Set style for monitor_ddlist_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_ddlist_2.set_style_text_color(lv.color_hex(0x0D3055), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_2.set_style_text_font(test_font("SourceHanSerifSC_Regular", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_2.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_2.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_2.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_2.set_style_border_color(lv.color_hex(0x4da0ff), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_2.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_2.set_style_pad_top(7, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_2.set_style_pad_left(6, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_2.set_style_pad_right(6, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_2.set_style_radius(7, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_2.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_2.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_2.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for monitor_ddlist_2, Part: lv.PART.SELECTED, State: lv.STATE.CHECKED.
style_monitor_ddlist_2_extra_list_selected_checked = lv.style_t()
style_monitor_ddlist_2_extra_list_selected_checked.init()
style_monitor_ddlist_2_extra_list_selected_checked.set_border_width(1)
style_monitor_ddlist_2_extra_list_selected_checked.set_border_opa(255)
style_monitor_ddlist_2_extra_list_selected_checked.set_border_color(lv.color_hex(0xe1e6ee))
style_monitor_ddlist_2_extra_list_selected_checked.set_border_side(lv.BORDER_SIDE.FULL)
style_monitor_ddlist_2_extra_list_selected_checked.set_radius(3)
style_monitor_ddlist_2_extra_list_selected_checked.set_bg_opa(255)
style_monitor_ddlist_2_extra_list_selected_checked.set_bg_color(lv.color_hex(0x00a1b5))
style_monitor_ddlist_2_extra_list_selected_checked.set_bg_grad_dir(lv.GRAD_DIR.NONE)
monitor_ddlist_2.get_list().add_style(style_monitor_ddlist_2_extra_list_selected_checked, lv.PART.SELECTED|lv.STATE.CHECKED)
# Set style for monitor_ddlist_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
style_monitor_ddlist_2_extra_list_main_default = lv.style_t()
style_monitor_ddlist_2_extra_list_main_default.init()
style_monitor_ddlist_2_extra_list_main_default.set_max_height(90)
style_monitor_ddlist_2_extra_list_main_default.set_text_color(lv.color_hex(0x0D3055))
style_monitor_ddlist_2_extra_list_main_default.set_text_font(test_font("SourceHanSerifSC_Regular", 12))
style_monitor_ddlist_2_extra_list_main_default.set_text_opa(255)
style_monitor_ddlist_2_extra_list_main_default.set_border_width(1)
style_monitor_ddlist_2_extra_list_main_default.set_border_opa(255)
style_monitor_ddlist_2_extra_list_main_default.set_border_color(lv.color_hex(0xe1e6ee))
style_monitor_ddlist_2_extra_list_main_default.set_border_side(lv.BORDER_SIDE.FULL)
style_monitor_ddlist_2_extra_list_main_default.set_radius(3)
style_monitor_ddlist_2_extra_list_main_default.set_bg_opa(255)
style_monitor_ddlist_2_extra_list_main_default.set_bg_color(lv.color_hex(0xffffff))
style_monitor_ddlist_2_extra_list_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
monitor_ddlist_2.get_list().add_style(style_monitor_ddlist_2_extra_list_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for monitor_ddlist_2, Part: lv.PART.SCROLLBAR, State: lv.STATE.DEFAULT.
style_monitor_ddlist_2_extra_list_scrollbar_default = lv.style_t()
style_monitor_ddlist_2_extra_list_scrollbar_default.init()
style_monitor_ddlist_2_extra_list_scrollbar_default.set_radius(3)
style_monitor_ddlist_2_extra_list_scrollbar_default.set_bg_opa(255)
style_monitor_ddlist_2_extra_list_scrollbar_default.set_bg_color(lv.color_hex(0x00ff00))
style_monitor_ddlist_2_extra_list_scrollbar_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
monitor_ddlist_2.get_list().add_style(style_monitor_ddlist_2_extra_list_scrollbar_default, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)

# Create monitor_label_9
monitor_label_9 = lv.label(monitor_tabview_1_tab_3)
monitor_label_9.set_text("循环次数：")
monitor_label_9.set_long_mode(lv.label.LONG.WRAP)
monitor_label_9.set_width(lv.pct(100))
monitor_label_9.set_pos(25, 66)
monitor_label_9.set_size(88, 21)
# Set style for monitor_label_9, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_label_9.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_9.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_9.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_9.set_style_text_font(test_font("SourceHanSerifSC_Regular", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_9.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_9.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_9.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_9.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_9.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_9.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_9.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_9.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_9.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_9.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create monitor_ddlist_3
monitor_ddlist_3 = lv.dropdown(monitor_tabview_1_tab_3)
monitor_ddlist_3.set_options("1\n2\n3\n4\n5")
monitor_ddlist_3.set_pos(112, 60)
monitor_ddlist_3.set_size(82, 30)
# Set style for monitor_ddlist_3, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_ddlist_3.set_style_text_color(lv.color_hex(0x0D3055), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_3.set_style_text_font(test_font("SourceHanSerifSC_Regular", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_3.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_3.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_3.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_3.set_style_border_color(lv.color_hex(0x4da0ff), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_3.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_3.set_style_pad_top(7, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_3.set_style_pad_left(6, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_3.set_style_pad_right(6, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_3.set_style_radius(7, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_3.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_3.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_3.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_ddlist_3.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for monitor_ddlist_3, Part: lv.PART.SELECTED, State: lv.STATE.CHECKED.
style_monitor_ddlist_3_extra_list_selected_checked = lv.style_t()
style_monitor_ddlist_3_extra_list_selected_checked.init()
style_monitor_ddlist_3_extra_list_selected_checked.set_border_width(1)
style_monitor_ddlist_3_extra_list_selected_checked.set_border_opa(255)
style_monitor_ddlist_3_extra_list_selected_checked.set_border_color(lv.color_hex(0xe1e6ee))
style_monitor_ddlist_3_extra_list_selected_checked.set_border_side(lv.BORDER_SIDE.FULL)
style_monitor_ddlist_3_extra_list_selected_checked.set_radius(3)
style_monitor_ddlist_3_extra_list_selected_checked.set_bg_opa(255)
style_monitor_ddlist_3_extra_list_selected_checked.set_bg_color(lv.color_hex(0x00a1b5))
style_monitor_ddlist_3_extra_list_selected_checked.set_bg_grad_dir(lv.GRAD_DIR.NONE)
monitor_ddlist_3.get_list().add_style(style_monitor_ddlist_3_extra_list_selected_checked, lv.PART.SELECTED|lv.STATE.CHECKED)
# Set style for monitor_ddlist_3, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
style_monitor_ddlist_3_extra_list_main_default = lv.style_t()
style_monitor_ddlist_3_extra_list_main_default.init()
style_monitor_ddlist_3_extra_list_main_default.set_max_height(90)
style_monitor_ddlist_3_extra_list_main_default.set_text_color(lv.color_hex(0x0D3055))
style_monitor_ddlist_3_extra_list_main_default.set_text_font(test_font("SourceHanSerifSC_Regular", 12))
style_monitor_ddlist_3_extra_list_main_default.set_text_opa(255)
style_monitor_ddlist_3_extra_list_main_default.set_border_width(1)
style_monitor_ddlist_3_extra_list_main_default.set_border_opa(255)
style_monitor_ddlist_3_extra_list_main_default.set_border_color(lv.color_hex(0xe1e6ee))
style_monitor_ddlist_3_extra_list_main_default.set_border_side(lv.BORDER_SIDE.FULL)
style_monitor_ddlist_3_extra_list_main_default.set_radius(3)
style_monitor_ddlist_3_extra_list_main_default.set_bg_opa(255)
style_monitor_ddlist_3_extra_list_main_default.set_bg_color(lv.color_hex(0xffffff))
style_monitor_ddlist_3_extra_list_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
monitor_ddlist_3.get_list().add_style(style_monitor_ddlist_3_extra_list_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for monitor_ddlist_3, Part: lv.PART.SCROLLBAR, State: lv.STATE.DEFAULT.
style_monitor_ddlist_3_extra_list_scrollbar_default = lv.style_t()
style_monitor_ddlist_3_extra_list_scrollbar_default.init()
style_monitor_ddlist_3_extra_list_scrollbar_default.set_radius(3)
style_monitor_ddlist_3_extra_list_scrollbar_default.set_bg_opa(255)
style_monitor_ddlist_3_extra_list_scrollbar_default.set_bg_color(lv.color_hex(0x00ff00))
style_monitor_ddlist_3_extra_list_scrollbar_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
monitor_ddlist_3.get_list().add_style(style_monitor_ddlist_3_extra_list_scrollbar_default, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)

# Create monitor_cont_2
monitor_cont_2 = lv.obj(monitor)
monitor_cont_2.set_pos(0, 0)
monitor_cont_2.set_size(320, 15)
monitor_cont_2.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for monitor_cont_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_cont_2.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_2.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_2.set_style_border_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_2.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_2.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_2.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_2.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_2.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_2.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_2.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_2.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_2.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_cont_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create monitor_datetext_1
monitor_datetext_1 = lv.label(monitor)
monitor_datetext_1.set_text("2023/07/31")
monitor_datetext_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, 0)
monitor_datetext_1.add_flag(lv.obj.FLAG.CLICKABLE)
monitor_datetext_1_calendar = None
monitor_datetext_1.add_event_cb(lambda e: datetext_event_handler(e, monitor_datetext_1_calendar), lv.EVENT.ALL, None)
monitor_datetext_1.set_pos(0, 0)
monitor_datetext_1.set_size(100, 15)
# Set style for monitor_datetext_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_datetext_1.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_datetext_1.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_datetext_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_datetext_1.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_datetext_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_datetext_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_datetext_1.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_datetext_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_datetext_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_datetext_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_datetext_1.set_style_pad_top(1, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_datetext_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_datetext_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_datetext_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create monitor_digital_clock_1
monitor_digital_clock_1_time = [int(11), int(25), int(50), "AM"]
monitor_digital_clock_1 = lv.dclock(monitor, "11:25:50 AM")
monitor_digital_clock_1_timer = lv.timer_create_basic()
monitor_digital_clock_1_timer.set_period(1000)
monitor_digital_clock_1_timer.set_cb(lambda src: digital_clock_cb(monitor_digital_clock_1_timer, monitor_digital_clock_1, monitor_digital_clock_1_time, True, True ))
monitor_digital_clock_1.set_pos(220, 0)
monitor_digital_clock_1.set_size(100, 15)
# Set style for monitor_digital_clock_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_digital_clock_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_digital_clock_1.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_digital_clock_1.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_digital_clock_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_digital_clock_1.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_digital_clock_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_digital_clock_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_digital_clock_1.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_digital_clock_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_digital_clock_1.set_style_pad_top(1, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_digital_clock_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_digital_clock_1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_digital_clock_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_digital_clock_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create monitor_btn_1
monitor_btn_1 = lv.btn(monitor)
monitor_btn_1_label = lv.label(monitor_btn_1)
monitor_btn_1_label.set_text("")
monitor_btn_1_label.set_long_mode(lv.label.LONG.WRAP)
monitor_btn_1_label.set_width(lv.pct(100))
monitor_btn_1_label.align(lv.ALIGN.CENTER, 0, 0)
monitor_btn_1.set_style_pad_all(0, lv.STATE.DEFAULT)
monitor_btn_1.set_pos(7, 213)
monitor_btn_1.set_size(23, 23)
# Set style for monitor_btn_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_btn_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_btn_1.set_style_bg_color(lv.color_hex(0xf9dfff), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_btn_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_btn_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_btn_1.set_style_radius(25, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_btn_1.set_style_shadow_width(3, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_btn_1.set_style_shadow_color(lv.color_hex(0xb8a0bd), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_btn_1.set_style_shadow_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_btn_1.set_style_shadow_spread(2, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_btn_1.set_style_shadow_ofs_x(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_btn_1.set_style_shadow_ofs_y(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_btn_1.set_style_text_color(lv.color_hex(0x575757), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_btn_1.set_style_text_font(test_font("iconfont", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_btn_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_btn_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create monitor_label_3
monitor_label_3 = lv.label(monitor)
monitor_label_3.set_text("")
monitor_label_3.set_long_mode(lv.label.LONG.WRAP)
monitor_label_3.set_width(lv.pct(100))
monitor_label_3.set_pos(95, 0)
monitor_label_3.set_size(20, 15)
# Set style for monitor_label_3, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
monitor_label_3.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_3.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_3.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_3.set_style_text_font(test_font("iconfont", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_3.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_3.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_3.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_3.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_3.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_3.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_3.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_3.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_3.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
monitor_label_3.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

monitor.update_layout()
# Create settings
settings = lv.obj()
settings.set_size(320, 240)
settings.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for settings, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
settings.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create settings_cont_1
settings_cont_1 = lv.obj(settings)
settings_cont_1.set_pos(0, 0)
settings_cont_1.set_size(320, 240)
settings_cont_1.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for settings_cont_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
settings_cont_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_cont_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_cont_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_cont_1.set_style_bg_color(lv.color_hex(0xccffe0), lv.PART.MAIN|lv.STATE.DEFAULT)
settings_cont_1.set_style_bg_grad_dir(lv.GRAD_DIR.HOR, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_cont_1.set_style_bg_grad_color(lv.color_hex(0x4dffb8), lv.PART.MAIN|lv.STATE.DEFAULT)
settings_cont_1.set_style_bg_main_stop(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_cont_1.set_style_bg_grad_stop(255, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_cont_1.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_cont_1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_cont_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_cont_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_cont_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create settings_cont_2
settings_cont_2 = lv.obj(settings)
settings_cont_2.set_pos(0, 0)
settings_cont_2.set_size(320, 15)
settings_cont_2.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for settings_cont_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
settings_cont_2.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_cont_2.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_cont_2.set_style_border_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
settings_cont_2.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_cont_2.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_cont_2.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_cont_2.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
settings_cont_2.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_cont_2.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_cont_2.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_cont_2.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_cont_2.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_cont_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create settings_datetext_1
settings_datetext_1 = lv.label(settings)
settings_datetext_1.set_text("2023/07/31")
settings_datetext_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, 0)
settings_datetext_1.add_flag(lv.obj.FLAG.CLICKABLE)
settings_datetext_1_calendar = None
settings_datetext_1.add_event_cb(lambda e: datetext_event_handler(e, settings_datetext_1_calendar), lv.EVENT.ALL, None)
settings_datetext_1.set_pos(0, 0)
settings_datetext_1.set_size(100, 15)
# Set style for settings_datetext_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
settings_datetext_1.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
settings_datetext_1.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
settings_datetext_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_datetext_1.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_datetext_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_datetext_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_datetext_1.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
settings_datetext_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_datetext_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_datetext_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_datetext_1.set_style_pad_top(1, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_datetext_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_datetext_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_datetext_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create settings_digital_clock_1
settings_digital_clock_1_time = [int(11), int(25), int(50), "AM"]
settings_digital_clock_1 = lv.dclock(settings, "11:25:50 AM")
settings_digital_clock_1_timer = lv.timer_create_basic()
settings_digital_clock_1_timer.set_period(1000)
settings_digital_clock_1_timer.set_cb(lambda src: digital_clock_cb(settings_digital_clock_1_timer, settings_digital_clock_1, settings_digital_clock_1_time, True, True ))
settings_digital_clock_1.set_pos(220, 0)
settings_digital_clock_1.set_size(100, 15)
# Set style for settings_digital_clock_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
settings_digital_clock_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_digital_clock_1.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
settings_digital_clock_1.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
settings_digital_clock_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_digital_clock_1.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_digital_clock_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_digital_clock_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_digital_clock_1.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
settings_digital_clock_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_digital_clock_1.set_style_pad_top(1, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_digital_clock_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_digital_clock_1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_digital_clock_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_digital_clock_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create settings_mode_l
settings_mode_l = lv.label(settings)
settings_mode_l.set_text("模式")
settings_mode_l.set_long_mode(lv.label.LONG.WRAP)
settings_mode_l.set_width(lv.pct(100))
settings_mode_l.set_pos(-3, 29)
settings_mode_l.set_size(50, 18)
# Set style for settings_mode_l, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
settings_mode_l.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_mode_l.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_mode_l.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
settings_mode_l.set_style_text_font(test_font("SourceHanSerifSC_Regular", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
settings_mode_l.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_mode_l.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_mode_l.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_mode_l.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_mode_l.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_mode_l.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_mode_l.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_mode_l.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_mode_l.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_mode_l.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create settings_label_3
settings_label_3 = lv.label(settings)
settings_label_3.set_text("")
settings_label_3.set_long_mode(lv.label.LONG.WRAP)
settings_label_3.set_width(lv.pct(100))
settings_label_3.set_pos(95, 0)
settings_label_3.set_size(20, 15)
# Set style for settings_label_3, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
settings_label_3.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_label_3.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_label_3.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
settings_label_3.set_style_text_font(test_font("iconfont", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
settings_label_3.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_label_3.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_label_3.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_label_3.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_label_3.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_label_3.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_label_3.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_label_3.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_label_3.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_label_3.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create settings_btn_1
settings_btn_1 = lv.btn(settings)
settings_btn_1_label = lv.label(settings_btn_1)
settings_btn_1_label.set_text("")
settings_btn_1_label.set_long_mode(lv.label.LONG.WRAP)
settings_btn_1_label.set_width(lv.pct(100))
settings_btn_1_label.align(lv.ALIGN.CENTER, 0, 0)
settings_btn_1.set_style_pad_all(0, lv.STATE.DEFAULT)
settings_btn_1.set_pos(291, 107)
settings_btn_1.set_size(23, 23)
# Set style for settings_btn_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
settings_btn_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_btn_1.set_style_bg_color(lv.color_hex(0xf9dfff), lv.PART.MAIN|lv.STATE.DEFAULT)
settings_btn_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_btn_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_btn_1.set_style_radius(25, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_btn_1.set_style_shadow_width(3, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_btn_1.set_style_shadow_color(lv.color_hex(0xb8a0bd), lv.PART.MAIN|lv.STATE.DEFAULT)
settings_btn_1.set_style_shadow_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_btn_1.set_style_shadow_spread(2, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_btn_1.set_style_shadow_ofs_x(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_btn_1.set_style_shadow_ofs_y(0, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_btn_1.set_style_text_color(lv.color_hex(0x575757), lv.PART.MAIN|lv.STATE.DEFAULT)
settings_btn_1.set_style_text_font(test_font("iconfont", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
settings_btn_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_btn_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create settings_ddlist_1
settings_ddlist_1 = lv.dropdown(settings)
settings_ddlist_1.set_options("自动开关模式\n手动模式")
settings_ddlist_1.set_pos(64, 23)
settings_ddlist_1.set_size(130, 30)
# Set style for settings_ddlist_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
settings_ddlist_1.set_style_text_color(lv.color_hex(0x0D3055), lv.PART.MAIN|lv.STATE.DEFAULT)
settings_ddlist_1.set_style_text_font(test_font("SourceHanSerifSC_Regular", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
settings_ddlist_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_ddlist_1.set_style_border_width(1, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_ddlist_1.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_ddlist_1.set_style_border_color(lv.color_hex(0xe1e6ee), lv.PART.MAIN|lv.STATE.DEFAULT)
settings_ddlist_1.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_ddlist_1.set_style_pad_top(8, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_ddlist_1.set_style_pad_left(6, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_ddlist_1.set_style_pad_right(6, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_ddlist_1.set_style_radius(3, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_ddlist_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_ddlist_1.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
settings_ddlist_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
settings_ddlist_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for settings_ddlist_1, Part: lv.PART.SELECTED, State: lv.STATE.CHECKED.
style_settings_ddlist_1_extra_list_selected_checked = lv.style_t()
style_settings_ddlist_1_extra_list_selected_checked.init()
style_settings_ddlist_1_extra_list_selected_checked.set_border_width(1)
style_settings_ddlist_1_extra_list_selected_checked.set_border_opa(255)
style_settings_ddlist_1_extra_list_selected_checked.set_border_color(lv.color_hex(0xe1e6ee))
style_settings_ddlist_1_extra_list_selected_checked.set_border_side(lv.BORDER_SIDE.FULL)
style_settings_ddlist_1_extra_list_selected_checked.set_radius(3)
style_settings_ddlist_1_extra_list_selected_checked.set_bg_opa(255)
style_settings_ddlist_1_extra_list_selected_checked.set_bg_color(lv.color_hex(0x00a1b5))
style_settings_ddlist_1_extra_list_selected_checked.set_bg_grad_dir(lv.GRAD_DIR.NONE)
settings_ddlist_1.get_list().add_style(style_settings_ddlist_1_extra_list_selected_checked, lv.PART.SELECTED|lv.STATE.CHECKED)
# Set style for settings_ddlist_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
style_settings_ddlist_1_extra_list_main_default = lv.style_t()
style_settings_ddlist_1_extra_list_main_default.init()
style_settings_ddlist_1_extra_list_main_default.set_max_height(90)
style_settings_ddlist_1_extra_list_main_default.set_text_color(lv.color_hex(0x0D3055))
style_settings_ddlist_1_extra_list_main_default.set_text_font(test_font("SourceHanSerifSC_Regular", 12))
style_settings_ddlist_1_extra_list_main_default.set_text_opa(255)
style_settings_ddlist_1_extra_list_main_default.set_border_width(1)
style_settings_ddlist_1_extra_list_main_default.set_border_opa(255)
style_settings_ddlist_1_extra_list_main_default.set_border_color(lv.color_hex(0xe1e6ee))
style_settings_ddlist_1_extra_list_main_default.set_border_side(lv.BORDER_SIDE.FULL)
style_settings_ddlist_1_extra_list_main_default.set_radius(3)
style_settings_ddlist_1_extra_list_main_default.set_bg_opa(255)
style_settings_ddlist_1_extra_list_main_default.set_bg_color(lv.color_hex(0xffffff))
style_settings_ddlist_1_extra_list_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
settings_ddlist_1.get_list().add_style(style_settings_ddlist_1_extra_list_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for settings_ddlist_1, Part: lv.PART.SCROLLBAR, State: lv.STATE.DEFAULT.
style_settings_ddlist_1_extra_list_scrollbar_default = lv.style_t()
style_settings_ddlist_1_extra_list_scrollbar_default.init()
style_settings_ddlist_1_extra_list_scrollbar_default.set_radius(3)
style_settings_ddlist_1_extra_list_scrollbar_default.set_bg_opa(255)
style_settings_ddlist_1_extra_list_scrollbar_default.set_bg_color(lv.color_hex(0x00ff00))
style_settings_ddlist_1_extra_list_scrollbar_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
settings_ddlist_1.get_list().add_style(style_settings_ddlist_1_extra_list_scrollbar_default, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)

settings.update_layout()
# Create guader
guader = lv.obj()
guader.set_size(320, 240)
guader.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for guader, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
guader.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create guader_cont_1
guader_cont_1 = lv.obj(guader)
guader_cont_1.set_pos(0, 0)
guader_cont_1.set_size(320, 240)
guader_cont_1.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for guader_cont_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
guader_cont_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_cont_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_cont_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_cont_1.set_style_bg_color(lv.color_hex(0xbb57ff), lv.PART.MAIN|lv.STATE.DEFAULT)
guader_cont_1.set_style_bg_grad_dir(lv.GRAD_DIR.HOR, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_cont_1.set_style_bg_grad_color(lv.color_hex(0x7b57ff), lv.PART.MAIN|lv.STATE.DEFAULT)
guader_cont_1.set_style_bg_main_stop(87, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_cont_1.set_style_bg_grad_stop(153, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_cont_1.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_cont_1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_cont_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_cont_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_cont_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create guader_spinner_1
guader_spinner_1 = lv.spinner(guader, 3000, 50)
guader_spinner_1.set_pos(100, 24)
guader_spinner_1.set_size(120, 120)
# Set style for guader_spinner_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
guader_spinner_1.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_spinner_1.set_style_pad_bottom(5, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_spinner_1.set_style_pad_left(5, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_spinner_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_spinner_1.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_spinner_1.set_style_arc_width(20, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_spinner_1.set_style_arc_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_spinner_1.set_style_arc_color(lv.color_hex(0xd5d6de), lv.PART.MAIN|lv.STATE.DEFAULT)
guader_spinner_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for guader_spinner_1, Part: lv.PART.INDICATOR, State: lv.STATE.DEFAULT.
guader_spinner_1.set_style_arc_width(20, lv.PART.INDICATOR|lv.STATE.DEFAULT)
guader_spinner_1.set_style_arc_opa(255, lv.PART.INDICATOR|lv.STATE.DEFAULT)
guader_spinner_1.set_style_arc_color(lv.color_hex(0x2195f6), lv.PART.INDICATOR|lv.STATE.DEFAULT)

# Create guader_label_1
guader_label_1 = lv.label(guader)
guader_label_1.set_text("")
guader_label_1.set_long_mode(lv.label.LONG.WRAP)
guader_label_1.set_width(lv.pct(100))
guader_label_1.set_pos(142, 62)
guader_label_1.set_size(40, 40)
# Set style for guader_label_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
guader_label_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_1.set_style_text_color(lv.color_hex(0xaffe07), lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_1.set_style_text_font(test_font("iconfont", 40), lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_1.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_1.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_1.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_1.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create guader_btn_1
guader_btn_1 = lv.btn(guader)
guader_btn_1_label = lv.label(guader_btn_1)
guader_btn_1_label.set_text("")
guader_btn_1_label.set_long_mode(lv.label.LONG.WRAP)
guader_btn_1_label.set_width(lv.pct(100))
guader_btn_1_label.align(lv.ALIGN.CENTER, 0, 0)
guader_btn_1.set_style_pad_all(0, lv.STATE.DEFAULT)
guader_btn_1.set_pos(5, 91)
guader_btn_1.set_size(23, 23)
# Set style for guader_btn_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
guader_btn_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_btn_1.set_style_bg_color(lv.color_hex(0xf9dfff), lv.PART.MAIN|lv.STATE.DEFAULT)
guader_btn_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_btn_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_btn_1.set_style_radius(25, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_btn_1.set_style_shadow_width(3, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_btn_1.set_style_shadow_color(lv.color_hex(0xb8a0bd), lv.PART.MAIN|lv.STATE.DEFAULT)
guader_btn_1.set_style_shadow_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_btn_1.set_style_shadow_spread(2, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_btn_1.set_style_shadow_ofs_x(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_btn_1.set_style_shadow_ofs_y(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_btn_1.set_style_text_color(lv.color_hex(0x575757), lv.PART.MAIN|lv.STATE.DEFAULT)
guader_btn_1.set_style_text_font(test_font("iconfont", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
guader_btn_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_btn_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create guader_cont_2
guader_cont_2 = lv.obj(guader)
guader_cont_2.set_pos(100, 182)
guader_cont_2.set_size(120, 30)
guader_cont_2.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for guader_cont_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
guader_cont_2.set_style_border_width(3, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_cont_2.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_cont_2.set_style_border_color(lv.color_hex(0x7fc2fa), lv.PART.MAIN|lv.STATE.DEFAULT)
guader_cont_2.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_cont_2.set_style_radius(14, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_cont_2.set_style_bg_opa(123, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_cont_2.set_style_bg_color(lv.color_hex(0x52e0fe), lv.PART.MAIN|lv.STATE.DEFAULT)
guader_cont_2.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_cont_2.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_cont_2.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_cont_2.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_cont_2.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_cont_2.set_style_shadow_width(3, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_cont_2.set_style_shadow_color(lv.color_hex(0x7fc2fa), lv.PART.MAIN|lv.STATE.DEFAULT)
guader_cont_2.set_style_shadow_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_cont_2.set_style_shadow_spread(1, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_cont_2.set_style_shadow_ofs_x(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_cont_2.set_style_shadow_ofs_y(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create guader_label_2
guader_label_2 = lv.label(guader_cont_2)
guader_label_2.set_text("记录次数：")
guader_label_2.set_long_mode(lv.label.LONG.WRAP)
guader_label_2.set_width(lv.pct(100))
guader_label_2.set_pos(4, 5)
guader_label_2.set_size(70, 17)
# Set style for guader_label_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
guader_label_2.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_2.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_2.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_2.set_style_text_font(test_font("SourceHanSerifSC_Regular", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_2.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_2.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_2.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_2.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_2.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_2.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_2.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_2.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_2.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create guader_label_3
guader_label_3 = lv.label(guader_cont_2)
guader_label_3.set_text("La")
guader_label_3.set_long_mode(lv.label.LONG.WRAP)
guader_label_3.set_width(lv.pct(100))
guader_label_3.set_pos(82, 7)
guader_label_3.set_size(25, 17)
# Set style for guader_label_3, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
guader_label_3.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_3.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_3.set_style_text_color(lv.color_hex(0xfffe34), lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_3.set_style_text_font(test_font("montserratMedium", 15), lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_3.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_3.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_3.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_3.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_3.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_3.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_3.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_3.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_3.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_3.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create guader_label_4
guader_label_4 = lv.label(guader)
guader_label_4.set_text("")
guader_label_4.set_long_mode(lv.label.LONG.WRAP)
guader_label_4.set_width(lv.pct(100))
guader_label_4.set_pos(31, 144)
guader_label_4.set_size(46, 27)
# Set style for guader_label_4, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
guader_label_4.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_4.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_4.set_style_text_color(lv.color_hex(0xf44d4d), lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_4.set_style_text_font(test_font("iconfont", 25), lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_4.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_4.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_4.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_4.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_4.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_4.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_4.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_4.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_4.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_4.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create guader_label_5
guader_label_5 = lv.label(guader)
guader_label_5.set_text("")
guader_label_5.set_long_mode(lv.label.LONG.WRAP)
guader_label_5.set_width(lv.pct(100))
guader_label_5.set_pos(245, 144)
guader_label_5.set_size(46, 28)
# Set style for guader_label_5, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
guader_label_5.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_5.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_5.set_style_text_color(lv.color_hex(0xf44d4d), lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_5.set_style_text_font(test_font("iconfont", 25), lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_5.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_5.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_5.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_5.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_5.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_5.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_5.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_5.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_5.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_5.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create guader_label_6
guader_label_6 = lv.label(guader)
guader_label_6.set_text("您在监控范围内的行为都将被记录")
guader_label_6.set_long_mode(lv.label.LONG.SCROLL)
guader_label_6.set_width(lv.pct(100))
guader_label_6.set_pos(89, 150)
guader_label_6.set_size(147, 16)
# Set style for guader_label_6, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
guader_label_6.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_6.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_6.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_6.set_style_text_font(test_font("SourceHanSerifSC_Regular", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_6.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_6.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_6.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_6.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_6.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_6.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_6.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_6.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_6.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
guader_label_6.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

guader.update_layout()

def home_btn_led_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        

home_btn_led.add_event_cb(lambda e: home_btn_led_event_handler(e), lv.EVENT.ALL, None)

def home_sw_2_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.VALUE_CHANGED):
        pass
        

home_sw_2.add_event_cb(lambda e: home_sw_2_event_handler(e), lv.EVENT.ALL, None)

def home_btn_2_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        lv.scr_load_anim(monitor, lv.SCR_LOAD_ANIM.NONE, 20, 20, False)
home_btn_2.add_event_cb(lambda e: home_btn_2_event_handler(e), lv.EVENT.ALL, None)

def home_btn_4_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        lv.scr_load_anim(guader, lv.SCR_LOAD_ANIM.NONE, 20, 20, False)
home_btn_4.add_event_cb(lambda e: home_btn_4_event_handler(e), lv.EVENT.ALL, None)

def home_btn_3_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        lv.scr_load_anim(settings, lv.SCR_LOAD_ANIM.NONE, 20, 20, False)
home_btn_3.add_event_cb(lambda e: home_btn_3_event_handler(e), lv.EVENT.ALL, None)

def monitor_work_btn_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        

monitor_work_btn.add_event_cb(lambda e: monitor_work_btn_event_handler(e), lv.EVENT.ALL, None)

def monitor_btn_1_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        lv.scr_load_anim(home, lv.SCR_LOAD_ANIM.NONE, 20, 20, False)
monitor_btn_1.add_event_cb(lambda e: monitor_btn_1_event_handler(e), lv.EVENT.ALL, None)

def settings_btn_1_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        lv.scr_load_anim(home, lv.SCR_LOAD_ANIM.NONE, 20, 20, False)
settings_btn_1.add_event_cb(lambda e: settings_btn_1_event_handler(e), lv.EVENT.ALL, None)

def guader_btn_1_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        lv.scr_load_anim(home, lv.SCR_LOAD_ANIM.NONE, 20, 20, False)
guader_btn_1.add_event_cb(lambda e: guader_btn_1_event_handler(e), lv.EVENT.ALL, None)

# content from custom.py

# Load the default screen
lv.scr_load(monitor)

while SDL.check():
    time.sleep_ms(5)

