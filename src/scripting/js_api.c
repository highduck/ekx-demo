
#include "js_api.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#include <quickjs.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern void js_canvas_fill_circle(float a0, float a1, float a2, uint32_t a3, uint32_t a4, uint32_t a5);
extern void js_canvas_line(float a0, float a1, float a2, float a3, uint32_t a4, float a5);
extern void js_canvas_quad_color(float a0, float a1, float a2, float a3, uint32_t a4);
extern void js_canvas_scale(float a0, float a1);
extern void js_canvas_translate(float a0, float a1);
extern void js_canvas_save_transform(void);
extern void js_canvas_restore_transform(void);
extern void js_canvas_set_empty_image(void);
extern double js_time(void);
extern void js_log_print(uint32_t a0, const char* a1);

#ifdef __EMSCRIPTEN__
void js_register(void) {
    EM_ASM({
        window["canvas_fill_circle"] = _js_canvas_fill_circle;
        window["canvas_line"] = _js_canvas_line;
        window["canvas_quad_color"] = _js_canvas_quad_color;
        window["canvas_scale"] = _js_canvas_scale;
        window["canvas_translate"] = _js_canvas_translate;
        window["canvas_save_transform"] = _js_canvas_save_transform;
        window["canvas_restore_transform"] = _js_canvas_restore_transform;
        window["canvas_set_empty_image"] = _js_canvas_set_empty_image;
        window["time"] = _js_time;
        window["log_print"] = _js_log_print;
   });
}
#else
extern JSContext* qjs_ctx;

static JSValue qjs_canvas_fill_circle(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
  double a0;
  JS_ToFloat64(ctx, &a0, argv[0]);
  double a1;
  JS_ToFloat64(ctx, &a1, argv[1]);
  double a2;
  JS_ToFloat64(ctx, &a2, argv[2]);
  uint32_t a3;
  JS_ToUint32(ctx, &a3, argv[3]);
  uint32_t a4;
  JS_ToUint32(ctx, &a4, argv[4]);
  uint32_t a5;
  JS_ToUint32(ctx, &a5, argv[5]);
js_canvas_fill_circle((float)a0, (float)a1, (float)a2, a3, a4, a5);
  return JS_UNDEFINED;
}
static JSValue qjs_canvas_line(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
  double a0;
  JS_ToFloat64(ctx, &a0, argv[0]);
  double a1;
  JS_ToFloat64(ctx, &a1, argv[1]);
  double a2;
  JS_ToFloat64(ctx, &a2, argv[2]);
  double a3;
  JS_ToFloat64(ctx, &a3, argv[3]);
  uint32_t a4;
  JS_ToUint32(ctx, &a4, argv[4]);
  double a5;
  JS_ToFloat64(ctx, &a5, argv[5]);
js_canvas_line((float)a0, (float)a1, (float)a2, (float)a3, a4, (float)a5);
  return JS_UNDEFINED;
}
static JSValue qjs_canvas_quad_color(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
  double a0;
  JS_ToFloat64(ctx, &a0, argv[0]);
  double a1;
  JS_ToFloat64(ctx, &a1, argv[1]);
  double a2;
  JS_ToFloat64(ctx, &a2, argv[2]);
  double a3;
  JS_ToFloat64(ctx, &a3, argv[3]);
  uint32_t a4;
  JS_ToUint32(ctx, &a4, argv[4]);
js_canvas_quad_color((float)a0, (float)a1, (float)a2, (float)a3, a4);
  return JS_UNDEFINED;
}
static JSValue qjs_canvas_scale(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
  double a0;
  JS_ToFloat64(ctx, &a0, argv[0]);
  double a1;
  JS_ToFloat64(ctx, &a1, argv[1]);
js_canvas_scale((float)a0, (float)a1);
  return JS_UNDEFINED;
}
static JSValue qjs_canvas_translate(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
  double a0;
  JS_ToFloat64(ctx, &a0, argv[0]);
  double a1;
  JS_ToFloat64(ctx, &a1, argv[1]);
js_canvas_translate((float)a0, (float)a1);
  return JS_UNDEFINED;
}
static JSValue qjs_canvas_save_transform(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
js_canvas_save_transform();
  return JS_UNDEFINED;
}
static JSValue qjs_canvas_restore_transform(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
js_canvas_restore_transform();
  return JS_UNDEFINED;
}
static JSValue qjs_canvas_set_empty_image(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
js_canvas_set_empty_image();
  return JS_UNDEFINED;
}
static JSValue qjs_time(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
  return JS_NewFloat64(ctx, js_time());
}
static JSValue qjs_log_print(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
  uint32_t a0;
  JS_ToUint32(ctx, &a0, argv[0]);
  const char* a1;
  a1 = JS_ToCString(ctx, argv[1]);
js_log_print(a0, a1);
  JS_FreeCString(ctx, a1);
  return JS_UNDEFINED;
}


void js_register(void) {
    JSValue global_object = JS_GetGlobalObject(qjs_ctx);
   JS_SetPropertyStr(qjs_ctx, global_object, "canvas_fill_circle", JS_NewCFunction(qjs_ctx, qjs_canvas_fill_circle, "canvas_fill_circle", 1));
   JS_SetPropertyStr(qjs_ctx, global_object, "canvas_line", JS_NewCFunction(qjs_ctx, qjs_canvas_line, "canvas_line", 1));
   JS_SetPropertyStr(qjs_ctx, global_object, "canvas_quad_color", JS_NewCFunction(qjs_ctx, qjs_canvas_quad_color, "canvas_quad_color", 1));
   JS_SetPropertyStr(qjs_ctx, global_object, "canvas_scale", JS_NewCFunction(qjs_ctx, qjs_canvas_scale, "canvas_scale", 1));
   JS_SetPropertyStr(qjs_ctx, global_object, "canvas_translate", JS_NewCFunction(qjs_ctx, qjs_canvas_translate, "canvas_translate", 1));
   JS_SetPropertyStr(qjs_ctx, global_object, "canvas_save_transform", JS_NewCFunction(qjs_ctx, qjs_canvas_save_transform, "canvas_save_transform", 1));
   JS_SetPropertyStr(qjs_ctx, global_object, "canvas_restore_transform", JS_NewCFunction(qjs_ctx, qjs_canvas_restore_transform, "canvas_restore_transform", 1));
   JS_SetPropertyStr(qjs_ctx, global_object, "canvas_set_empty_image", JS_NewCFunction(qjs_ctx, qjs_canvas_set_empty_image, "canvas_set_empty_image", 1));
   JS_SetPropertyStr(qjs_ctx, global_object, "time", JS_NewCFunction(qjs_ctx, qjs_time, "time", 1));
   JS_SetPropertyStr(qjs_ctx, global_object, "log_print", JS_NewCFunction(qjs_ctx, qjs_log_print, "log_print", 1));
}


#endif

#ifdef __cplusplus
}
#endif

