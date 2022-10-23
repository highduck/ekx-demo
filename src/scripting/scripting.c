#include "scripting.h"
#include "ek/app.h"
#include <ek/gfx.h>
#include <ek/canvas.h>
#include <ek/rnd.h>
#include <ek/math.h>
#include <ek/time.h>
#include <ek/log.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define JS_API EMSCRIPTEN_KEEPALIVE
#else
#define JS_API
#endif

#ifdef __cplusplus
extern "C" {
#endif


JS_API void js_canvas_fill_circle(float x, float y, float r, uint32_t color0, uint32_t color1, uint32_t segments) {
    canvas_fill_circle(vec3(x, y, r), color_u32(color0), color_u32(color1), (int)segments);
}

JS_API void js_canvas_quad_color(float x, float y, float w, float h, uint32_t color) {
    canvas_quad_color(x, y, w, h, color_u32(color));
}

JS_API void js_canvas_line(float x0, float y0, float x1, float y1, uint32_t color, float line_width) {
    canvas_line(vec2(x0, y0), vec2(x1, y1), color_u32(color), line_width);
}

JS_API void js_canvas_scale(float x, float y) {
    canvas_scale(vec2(x, y));
}

JS_API void js_canvas_translate(float x, float y) {
    canvas_translate(vec2(x, y));
}

JS_API void js_canvas_save_transform(void) {
    canvas_save_transform();
}

JS_API void js_canvas_restore_transform(void) {
    canvas_restore_transform();
}

JS_API void js_canvas_set_empty_image(void) {
    canvas_set_empty_image();
}

JS_API double js_time(void) {
    return ek_time_now();
}

JS_API void js_log_print(uint32_t a0, const char* a1) {
    log_print(a0, 0, 0, "%s", a1);
}

#ifdef __cplusplus
}
#endif

#ifdef __EMSCRIPTEN__
#include <emscripten.h>

void js_call_global(const char* name) {
    EM_ASM({
        const id = UTF8ToString($0);
        if(window[id]) window[id]();
    }, name);
}

void js_initialize() {
    log_info("js initialized");
}

void load_script(const char* path) {
    //t.src = AsciiToString($0);
    EM_ASM({
        var t = document.createElement("script");
        t.src = UTF8ToString($0);
        t.onload = function() {
//            document.head.removeChild(t);
        };
        document.head.appendChild(t);
    }, path);
}

#else

#include <quickjs.h>
#include <quickjs-libc.h>
#include <ek/local_res.h>

JSRuntime* qjs_rt;
JSContext* qjs_ctx;

void js_initialize(void) {
    qjs_rt = JS_NewRuntime();
    EK_ASSERT(qjs_rt);
    qjs_ctx = JS_NewContext(qjs_rt);
    EK_ASSERT(qjs_ctx);

    js_std_init_handlers(qjs_rt);

    /* loader for ES6 modules */
    JS_SetModuleLoaderFunc(qjs_rt, NULL, js_module_loader, NULL);

    js_std_add_helpers(qjs_ctx, ek_app.argc - 1, ek_app.argv + 1);
/* system modules */
//    js_init_module_std(ctx, "std");
//    js_init_module_os(ctx, "os");
}

void js_call_global(const char* name) {
    JSValue global_obj = JS_GetGlobalObject(qjs_ctx);
    JSValue ff = JS_GetPropertyStr(qjs_ctx, global_obj, name);
    JS_Call(qjs_ctx, ff, global_obj, 0, NULL);
}

static void on_script_loaded(ek_local_res* lr) {
    if (ek_local_res_success(lr)) {
        JSValue global_obj = JS_GetGlobalObject(qjs_ctx);
        char* buffer = (char*) malloc(lr->length + 1);
        buffer[lr->length] = 0;
        memcpy(buffer, lr->buffer, lr->length);
        JSValue val = JS_EvalThis(qjs_ctx, global_obj, buffer, lr->length, "<eval>",
                                  JS_EVAL_TYPE_GLOBAL | JS_EVAL_FLAG_STRICT);
        if (JS_IsException(val)) {
            //log_warn("load exception: %d %d", JS_VALUE_GET_TAG(val), JS_VALUE_GET_INT(val));
            const char* str = JS_ToCString(qjs_ctx, val);
            log_error("js exception: %s", str);
            JS_FreeCString(qjs_ctx, str);
        }
        free(buffer);
    }
    ek_local_res_close(lr);
}

void load_script(const char* path) {
    ek_local_res_load(path, on_script_loaded, NULL);
}

#endif