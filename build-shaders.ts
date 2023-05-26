import {getModuleDir} from "ekx/lib/utils/utils.js";
import {shdc} from "ekx/lib/sokol-shdc.js";
import {logger} from "ekx/lib/cli/logger.js";

await shdc({
    input: "src/render3d/render3d.glsl",
    output: "src/render3d/render3d_shader.h",
    cwd: getModuleDir(import.meta)
});

logger.info("render3d shader build completed");