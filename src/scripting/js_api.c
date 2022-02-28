
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

#ifdef __EMSCRIPTEN__
void js_register(void) {
    EM_ASM({
        this["canvas_fill_circle"] = _js_canvas_fill_circle;
        this["canvas_line"] = _js_canvas_line;
        this["canvas_quad_color"] = _js_canvas_quad_color;
        this["canvas_scale"] = _js_canvas_scale;
        this["canvas_translate"] = _js_canvas_translate;
        this["canvas_save_transform"] = _js_canvas_save_transform;
        this["canvas_restore_transform"] = _js_canvas_restore_transform;
        this["canvas_set_empty_image"] = _js_canvas_set_empty_image;
        this["time"] = _js_time;

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
  int32_t a3;
  JS_ToInt32(ctx, &a3, argv[3]);
  int32_t a4;
  JS_ToInt32(ctx, &a4, argv[4]);
  int32_t a5;
  JS_ToInt32(ctx, &a5, argv[5]);
js_canvas_fill_circle((float)a0, (float)a1, (float)a2, (uint32_t)a3, (uint32_t)a4, (uint32_t)a5);
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
  int32_t a4;
  JS_ToInt32(ctx, &a4, argv[4]);
  double a5;
  JS_ToFloat64(ctx, &a5, argv[5]);
js_canvas_line((float)a0, (float)a1, (float)a2, (float)a3, (uint32_t)a4, (float)a5);
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
  int32_t a4;
  JS_ToInt32(ctx, &a4, argv[4]);
js_canvas_quad_color((float)a0, (float)a1, (float)a2, (float)a3, (uint32_t)a4);
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
}


#endif

#ifdef __cplusplus
}
#endif

