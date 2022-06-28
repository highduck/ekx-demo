
type u32 = number;
type i32 = number;
type f32 = number;
type f64 = number;
type cstring = number;

declare function canvas_fill_circle(x: f32, y: f32, radius: f32, color0: u32, color1: u32, segments: u32):void;

declare function canvas_line(x0: f32, y0: f32, x1: f32, y1: f32, color: u32, line_width: f32):void;

declare function canvas_quad_color(x: f32, y: f32, w: f32, h: f32, color: u32):void;

declare function canvas_scale(x: f32, y: f32):void;

declare function canvas_translate(x: f32, y: f32):void;

declare function canvas_save_transform():void;

declare function canvas_restore_transform():void;

declare function canvas_set_empty_image():void;

declare function time():f64;

declare function log_print(level: u32, text: cstring):void;

