const api_data = [
    {
        name: "canvas_fill_circle",
        params: ["x", "y", "radius", "color0", "color1", "segments"],
        sig: ["f32", "f32", "f32", "u32", "u32", "u32"]
    },
    {
        name: "canvas_line",
        params: ["x0", "y0", "x1", "y1", "color", "line_width"],
        sig: ["f32", "f32", "f32", "f32", "u32", "f32"]
    },
    {
        name: "canvas_quad_color",
        params: ["x", "y", "w", "h", "color"],
        sig: ["f32", "f32", "f32", "f32", "u32"]
    },
    {
        name: "canvas_scale",
        params: ["x", "y"],
        sig: ["f32", "f32"]
    },
    {
        name: "canvas_translate",
        params: ["x", "y"],
        sig: ["f32", "f32"]
    },
    {
        name: "canvas_save_transform"
    },
    {
        name: "canvas_restore_transform"
    },
    {
        name: "canvas_set_empty_image"
    },
    {
        name: "time",
        return_type: "f64"
    }
];

let functions = {};
for (let fn of api_data) {
    if (fn.name) {
        functions[fn.name] = fn;
    }
}

let ts_declarations = [];
let decl_emscripten = `void js_register(void) {
    EM_ASM({\n`;
for (let fn of Object.keys(functions)) {
    decl_emscripten += `        this["${fn}"] = _js_${fn};\n`;
}
decl_emscripten += `
   });
}`;

let proxy_qjs = ``;

const convert_arg_type_qjs = {
    void: "void",
    f32: "float",
    f64: "double",
    i32: "int32_t",
    u32: "uint32_t"
};

const convert_type_qjs = {
    void: "void",
    f32: "double",
    f64: "double",
    i32: "int32_t",
    u32: "int32_t"
};

const convert_function_qjs = {
    void: "void",
    f32: "JS_ToFloat64",
    f64: "JS_ToFloat64",
    u32: "JS_ToInt32"
};

const qjs_new_primitive = {
    void: "void",
    f32: "JS_NewFloat64",
    f64: "JS_NewFloat64",
    u32: "JS_NewUint32",
    i32: "JS_NewInt32"
};

function render_function_d_ts(fn) {
    const info = functions[fn];
    const sig = info.sig ?? [];
    const ret = info.return_type ?? "void";
    const params = info.params;
    const arg_list = [];
    for (let i = 0; i < sig.length; ++i) {
        const param = params ? params[i] : `arg${i}`;
        arg_list.push(`${param}: ${sig[i]}`);
    }
    return `declare function ${fn}(${arg_list.join(", ")}):${ret};`;
}

function render_function_wasm_extern(fn) {
    const info = functions[fn];
    const sig = info.sig ?? [];
    const ret = convert_arg_type_qjs[info.return_type ?? "void"];
    let args = [];
    for (let i = 0; i < sig.length; ++i) {
        args.push(convert_arg_type_qjs[sig[i]] + " a" + i);
    }
    if (args.length === 0) {
        args.push("void");
    }
    return `extern ${ret} js_${fn}(${args.join(", ")});`;
}

let fwd_js_functions = [];
for (let fn of Object.keys(functions)) {
    fwd_js_functions.push(render_function_wasm_extern(fn));
    ts_declarations.push(render_function_d_ts(fn));
}

for (let fn of Object.keys(functions)) {
    const info = functions[fn];
    const sig = info.sig ?? [];
    const ret = info.return_type ?? "void";

    proxy_qjs += `static JSValue qjs_${fn}(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {\n`;
    let values = [];
    for (let i = 0; i < sig.length; ++i) {
        proxy_qjs += `  ${convert_type_qjs[sig[i]]} a${i};\n`;
        proxy_qjs += `  ${convert_function_qjs[sig[i]]}(ctx, &a${i}, argv[${i}]);\n`;
        values.push("(" + convert_arg_type_qjs[sig[i]] + ")a" + i);
    }

    let val = `js_${fn}(${values.join(", ")})`;
    if (ret !== "void") {
        proxy_qjs += `  return ${qjs_new_primitive[ret]}(ctx, ${val});\n`;
    } else {
        proxy_qjs += val + ";\n";
        proxy_qjs += "  return JS_UNDEFINED;\n";
    }
    proxy_qjs += "}\n";
}

let decl_qjs = `void js_register(void) {
    JSValue global_object = JS_GetGlobalObject(qjs_ctx);
`;
for (let fn of Object.keys(functions)) {
    decl_qjs += `   JS_SetPropertyStr(qjs_ctx, global_object, "${fn}", JS_NewCFunction(qjs_ctx, qjs_${fn}, "${fn}", 1));\n`;
}
decl_qjs += `}\n\n`;

const header = `
#ifndef EK_JS_API_H
#define EK_JS_API_H

#ifdef __cplusplus
extern "C" {
#endif

void js_register(void);

#ifdef __cplusplus
}
#endif

#endif // EK_JS_API_H
`;

const impl = `
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

${fwd_js_functions.join("\n")}

#ifdef __EMSCRIPTEN__
${decl_emscripten}
#else
extern JSContext* qjs_ctx;

${proxy_qjs}

${decl_qjs}
#endif

#ifdef __cplusplus
}
#endif

`;


const ts_declarations_content = `
type u32 = number;
type i32 = number;
type f32 = number;
type f64 = number;

${ts_declarations.join("\n\n")}

`;

const fs = require("fs");
const path = require("path");
fs.writeFileSync(path.join(__dirname, "../src/scripting/js_api.h"), header, "utf8");
fs.writeFileSync(path.join(__dirname, "../src/scripting/js_api.c"), impl, "utf8");
fs.writeFileSync(path.join(__dirname, "../assets/scripts/index.d.ts"), ts_declarations_content, "utf8");
