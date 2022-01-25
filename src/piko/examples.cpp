#include "examples.h"
#include "piko.h"
#include <ek/canvas.h>
#include <ek/rnd.h>
#include <ek/time.h>
#include <ek/scenex/app/basic_application.hpp>

namespace ek::piko {

void book::draw() {

    canvas_set_empty_image();
    canvas_quad_color(0, 0, 128, 128, COLOR_BLACK);

    canvas_save_transform();
    canvas_scale(vec2(0.5f, 0.5f));
    canvas_translate(vec2(64, 64 + 32));
    for (int k = -1; k <= 1; k += 2) {
        for (int j = 8 - 8 * k; j >= 0 && j <= 16; j += k) {
            float x = 59.5f;
            float q = 1.0f - sqr(mid(2.0f - fmod(time() / 4.0f, 2.0f) - j / 16.0f, 1.0f));
            float p = 1.0f - q;
            float y = 84.5f + (16.0f - j) / 2.0f * p + q * j / 2.0f;
            float w = q / 2;
            for (float i = 0.0f; i <= 1.0f; i += 0.01f) {
                int c = 6 + j % 2;
                if (fabsf((float) j * i) < MATH_F32_EPSILON || j > 15) {
                    c = 1;
                }
                if (sgn(x - 60.0f) == k) {
                    canvas_line(vec2(x, y), vec2(x + 10, y - 41), colorf(c), 2.0f);
                }
                x += cosu(w);
                y += sinu(w);
                w -= 0.5f * p * 0.035f * q * (1.0f - i);
            }
        }
    }

    canvas_restore_transform();
}

void dna::draw() {
    float t = time();

    /*
     *
    /// https://twitter.com/2DArray/status/1047971355268972545

    cls()
    for i=0,288 do
        x=i%17
        y=i/17
        circfill(x*8,y*8,5+sin(t()+i*.618)*2,1+i%3)
    end
    for j=5,3,-.5 do
        for i=0,150 do
            o=2
            if(i%20<1)o=.1
            for k=-1,1,o do
                a=i*.02+t()/8
                z=4+k*sin(a)
                y=i/150-.5
                if(abs(z-j)<.3)
                    circ(64+cos(a)*k*50/z,64+y*999/z,11/z,18.6-z)
            end
        end
    end
    flip()
    goto _
     */

    canvas_set_empty_image();
    canvas_quad_color(0, 0, 128, 128, COLOR_BLACK);

    for (int i = 0; i <= 288; ++i) {
        int x = i % 17;
        int y = i / 17;
        auto color = colorf(1 + i % 3);
        //circfill(x*8,y*8,5+sin(t()+i*.618)*2,1+i%3)
        vec3_t circ = vec3(
                (float) x * 8.0f,
                (float) y * 8.0f,
                5.0f + sinu(t + (float) i * 0.618f) * 2.0f
        );
        canvas_fill_circle(circ, color, color, 10);
    }
    for (float j = 5.0f; j >= 3.0f; j -= 0.5f) {
        for (int i = 0; i <= 150; ++i) {
            float o = 2.0f;
            if (i % 20 < 1) o = 0.1f;
            for (float k = -1.0f; k <= 1.0f; k += o) {
                // a = i * .02 + t()/8
                float a = i * 0.02f + t / 8.0f;
                // z = 4 + k*sin(a)
                float z = 4 + k * sinu(a);
                float y = i / 150.0f - 0.5f;
                if (fabs(z - j) < 0.3f) {
                    const vec3_t circ = vec3(
                            64 + cosu(a) * k * 50.0f / z,
                            64 + y * 999 / z,
                            11 / z
                    );
                    float ci = 18.6f - z; // color index
                    color_t color1 = colorf(ci);
                    const color_t color2 = color1;
                    color1.a = 0;
                    canvas_fill_circle(circ, color1, color2, 10);
                }
            }
        }
    }
}

// https://twitter.com/lucatron_/status/1144337102399651840

//e={3,11,5,8,14,2,9,10,4,13,7,6}
//::_::
//color(1)
//for i=0,80 do line(rnd(128),rnd(128)) end
//for n=1,10,3 do
//a=n/4-t()/4
//x=64+cos(a)*42
//y=64+sin(a)*42
//for j=-1,1,.02 do
//i=flr(j+t()*3)
//line(x,y+20,x+j*20,y,e[n+i%3])
//color(e[n+(i+1)%3])
//line(x+j*10,y-10)
//end
//        end
//flip()goto _

void diamonds::draw() {
    auto info = sg_query_image_info(rt);
    canvas_set_image(rt);
    canvas_set_image_rect(rect_01());
    canvas_quad(0, 0, (float) info.width, (float) info.height);
//    recorder.render();
}

diamonds::diamonds() {
//        recorder{"result", {0, 0, 512 * 2 / 2, 512 * 2 / 2}}
    rt = ek_gfx_make_render_target(128, 128, nullptr);
    g_game_app->dispatcher.listeners.push_back(this);

    sg_pass_desc passDesc{};
    passDesc.color_attachments[0].image = rt;
    passDesc.label = "diamonds-rt-pass";
    pass = sg_make_pass(passDesc);
}

diamonds::~diamonds() {
    sg_destroy_pass(pass);
    sg_destroy_image(rt);
    g_game_app->dispatcher.listeners.remove(this);
}

void diamonds::onPreRender() {
    time += g_time_layers->dt;
    float t = time;
    auto info = sg_query_image_info(rt);
    int w = info.width;
    int h = info.height;

    sg_pass_action clear{};
    if (first_frame) {
        const vec4_t clear_color = vec4_color(colorf(2));
        clear.colors[0].action = SG_ACTION_CLEAR;
        clear.colors[0].value.r = clear_color.x;
        clear.colors[0].value.g = clear_color.y;
        clear.colors[0].value.b = clear_color.z;
        clear.colors[0].value.a = clear_color.w;
        first_frame = false;
    } else {
        clear.colors[0].action = SG_ACTION_DONTCARE;
    }
    sg_begin_pass(pass, clear);
    canvas_begin_ex(rect_wh((float) w, (float) h), mat3x2_identity(), rt, {0});
    float sc = w / 128.0f;
    vec2_t center = vec2(w * 0.5f, h * 0.5f);
    int e[] = {0, 3, 11, 5, 8, 14, 2, 9, 10, 4, 13, 7, 6};
    auto c = colorf(1);
    c.a = (uint8_t)(255.0f * 0.3f);
    for (int i = 0; i < 80; ++i) {
        canvas_line(vec2(w * random_f(), h * random_f()),
                    vec2(w * random_f(), h * random_f()),
                    c, sc * 4.0f);
    }

    for (int n = 1; n <= 10; n += 3) {
        float a = n / 4.0f - t / 4.0f;
        vec2_t p = center + sc * 42.0f * vec2(cosu(a), sinu(a));
        for (float j = -1.0f; j <= 1.0f; j += 0.02f) {
            int i = static_cast<int>(floorf(j + t * 3.0f));
            canvas_line(p + vec2(0.0f, sc * 20.0f),
                        p + vec2(j * sc * 20, 0),
                        colorf(e[n + i % 3]),
                        sc * 1.0f);
            canvas_line(p + vec2(j * sc * 20, 0),
                        p + vec2(j * sc * 10, -sc * 10),
                        colorf(e[n + (i + 1) % 3]),
                        sc * 1.0f);
        }
    }
    canvas_end();
    sg_end_pass();
}

}