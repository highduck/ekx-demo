(() => {
    log_print(0, ' [QUICKJS] => Script loaded. Ok. \n');
    for (let n = 1; n <= 5; n++) {
        log_print(0, ` [QUICKJS-TRACE] n = ${n}/5 `);
    }
    const sqr = (x) => x * x;
    const RGB = (x) => (0xFF << 24) | (x & 0xFF00) | ((0xFF0000 & x) >>> 16) | ((x & 0xFF) << 16);
    const palette = [
        RGB(0x000000), //black
        RGB(0x1D2B53), // dark-blue
        RGB(0x7E2553), // dark-purple
        RGB(0x008751), // dark-green
        RGB(0xAB5236), // brown
        RGB(0x5F574F), // dark-gray
        RGB(0xC2C3C7), // light-gray
        RGB(0xFFF1E8), // white
        RGB(0xFF004D), // red
        RGB(0xFFA300), // orange
        RGB(0xFFEC27), // yellow
        RGB(0x00E436), // green
        RGB(0x29ADFF), // blue
        RGB(0x83769C), // indigo
        RGB(0xFF77A8), // pink
        RGB(0xFFCCAA), // peach
        0x000000, // reset cycle
    ];

    const colorf = (index) => {
        if (index < 0) index = 0;
        const i = index | 0;
        let e = i + 1;
        const count = 16;
        if (e >= count) e = count - 1;
        //return lerp_color(colors[i], colors[e], index - i);
        return palette[i];
    };

    const mid = (x, y, z = 0) => {
        if (x > y) {
            const tmp = x;
            x = y;
            y = tmp;
        }
        const yz = Math.min(y, z);
        return Math.max(x, yz);
    }

    const sgn = (a) => a >= 0 ? 1 : -1;

    const sinu = (unit) => -Math.sin(unit * Math.PI * 2);
    const cosu = (unit) => Math.cos(unit * Math.PI * 2);

    this.draw_book = () => {
        canvas_set_empty_image();

        canvas_quad_color(0, 0, 128, 128, 0xFF114477);
        canvas_quad_color(10, 10, 128 - 20, 128 - 20, 0xFF115599);
        canvas_quad_color(20, 20, 128 - 40, 128 - 40, 0xFF2277BB);

        if(1) {
            const random_color = () => (0xFF << 24) | (Math.random() * 0xFFFFFF);
            let pad = 0;
            for (let i = 0; i < 7; ++i) {
                canvas_quad_color(pad, pad, 128 - pad * 2, 128 - pad * 2, random_color());
                pad += 10;
            }
        }

        const t = time();

        canvas_save_transform();
        canvas_scale(0.5, 0.5);
        canvas_translate(64, 64 + 32);

        for (let k = -1; k <= 1; k += 2) {
            for (let j = 8 - 8 * k; j >= 0 && j <= 16; j += k) {
                let x = 59.5;
                const q = 1.0 - sqr(mid(2 - (t / 4) % 2 - j / 16, 1));
                const p = 1.0 - q;
                let y = 84.5 + (16.0 - j) / 2 * p + q * j / 2;
                let w = q / 2;
                for (let i = 0.0; i <= 1.0; i += 0.01) {
                    let c = 6 + j % 2;
                    if (Math.abs(j * i) < 0.00001 || j > 15) {
                        c = 1;
                    }
                    if (sgn(x - 60.0) === k) {
                        canvas_line(x, y, x + 10, y - 41, colorf(c), 2);
                    }
                    x += cosu(w);
                    y += sinu(w);
                    w -= 0.5 * p * 0.035 * q * (1.0 - i);
                }
            }
        }

        canvas_restore_transform();
    };

    this.draw_dna = () => {
        /* original: https://twitter.com/2DArray/status/1047971355268972545

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
        canvas_quad_color(0, 0, 128, 128, (0xFF << 24));
        const t = time();
        for (let i = 0; i <= 288; ++i) {
            const x = i % 17;
            const y = (i / 17) | 0;
            const color = colorf(1 + i % 3);
            canvas_fill_circle(x * 8, y * 8, 5 + 2 * sinu(t + i * 0.618), color, color, 10);
        }
        for (let j = 5.0; j >= 3.0; j -= 0.5) {
            for (let i = 0; i <= 150; ++i) {
                let o = 2.0;
                if (i % 20 < 1) o = 0.1;
                for (let k = -1; k <= 1; k += o) {
                    // a = i * .02 + t()/8
                    const a = i * 0.02 + t / 8;
                    // z = 4 + k*sin(a)
                    const z = 4 + k * sinu(a);
                    const y = i / 150 - 0.5;
                    if (Math.abs(z - j) < 0.3) {
                        const cx = 64 + cosu(a) * k * 50 / z;
                        const cy = 64 + y * 999 / z;
                        const cr = 11 / z;
                        const ci = 18.6 - z; // color index
                        const color1 = colorf(ci);
                        const color2 = color1;
                        canvas_fill_circle(cx, cy, cr, color1, color2, 10);
                    }
                }
            }
        }
    };

    this.draw_diamonds = () => {

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

        canvas_set_empty_image();

        const t = time();
        const w = 128.0;
        const h = 128.0;
        const sc = w / 128.0;
        const center_x = w * 0.5;
        const center_y = h * 0.5;
        const e = [0, 3, 11, 5, 8, 14, 2, 9, 10, 4, 13, 7, 6];
        const c = colorf(1) & 0x55FFFFFF;
        for (let i = 0; i < 80; ++i) {
            canvas_line(w * Math.random(), h * Math.random(),
                w * Math.random(), h * Math.random(),
                c, sc * 4);
        }

        for (let n = 1; n <= 10; n += 3) {
            const a = n / 4 - t / 4;
            const px = center_x + sc * 42 * cosu(a);
            const py = center_y + sc * 42 * sinu(a);
            for (let j = -1.0; j <= 1.0; j += 0.02) {
                let i = Math.floor(j + t * 3);
                canvas_line(px, py + sc * 20,
                    px + j * sc * 20, py,
                    colorf(e[n + i % 3]), sc);
                canvas_line(px + j * sc * 20, py,
                    px + j * sc * 10, py - sc * 10,
                    colorf(e[n + (i + 1) % 3]),
                    sc);
            }
        }
    };
})();