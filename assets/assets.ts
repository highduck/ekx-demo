import {AssetBuilder} from "ekx/lib/assets/AssetBuilder.js";
import {TextureDataType} from "ekx/lib/cli/assets/Texture.js";

export function on_populate(builder: AssetBuilder) {
    for (const mod of builder.glob("mods/*.mod")) {
        builder.copy({filepath: mod});
    }

    const audio_files = builder.glob("sfx/*.mp3");
    for (const audio_file of audio_files) {
        builder.audio({
            filepath: audio_file,
            streaming: audio_file.indexOf("music") > 0
        });
    }

    for (const file of builder.glob("scripts/*.js")) {
        builder.copy({filepath: file});
    }

    builder.default_glyph_cache();

    builder.ttf({
        name: "Comfortaa-Regular",
        filepath: "Comfortaa-Bold.ttf",
        base_font_size: 48
    });
    builder.ttf({
        filepath: "Cousine-Regular.ttf",
        base_font_size: 16,
        dev: true
    });
    builder.ttf({
        filepath: "mini.ttf",
        base_font_size: 16
    });


    builder.texture({
        name: "skybox",
        images: [
            "skybox/right.jpg",
            "skybox/left.jpg",
            "skybox/top.jpg",
            "skybox/bottom.jpg",
            "skybox/front.jpg",
            "skybox/back.jpg",
        ],
        type: TextureDataType.CubeMap,
        webp: {
            lossless: false
        }
    });

    builder.obj({
        filepath: "sphere.obj"
    });
    builder.obj({
        filepath: "torus.obj"
    });
    builder.obj({
        filepath: "monkey.obj"
    });

    builder.atlas({name: "main"});

    builder.bmfont({
        name: "TickingTimebombBB",
        filepath: "TickingTimebombBB.ttf",
        atlas: "main",
        font_size: 24,
        mirror_case: true,
    });

    builder.fla({
        filepath: "tests"
    });
}
