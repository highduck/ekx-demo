const path = require("path");
const fs = require("fs");

module.exports = function (project) {
    project.addModule({
        name: "demo",
        cpp: [path.join(__dirname, "src")],
        assets: [path.join(__dirname, "export/contents")]
    });

    const shortname = "ekx-demo";

    project.name = shortname;
    project.title = "ekx";
    project.desc = "Engine Demo";
    project.binary_name = shortname;
    project.pwa_url = "";
    project.cmake_target = shortname;
    project.version_name = "1.1.4";
    project.version_code = "14";
    project.orientation = "portrait";
    project.assets = {
        output: "export/contents/assets"
    };

    const secretPath = '/Users/ilyak/Dropbox/dev_keys/demo';

    project.android = {
        application_id: "ilj.play.demo",
        package_id: "com.eliasku.iljdemo",
        admob_app_id: "ca-app-pub-3931267664278058~6275600638",
        game_services_id: "300613663654",
        googleServicesConfigDir: secretPath,
        signingConfigPath: path.join(secretPath, "google-play-signing.json")
    };

    project.ios = {
        application_id: "ilj.play.demo",
        admob_app_id: "ca-app-pub-3931267664278058~8636974437",
        googleServicesConfigDir: secretPath,
    };

    project.html = {
        google_analytics_property_id: "UA-3755607-9",
        background_color: "black",
        text_color: "#73b3cb",
        deploy_dir: "/Users/ilyak/ek/eliasku.github.io/picos/book",
        url: "https://eliasku.github.io/picos/book",
        image_url: "https://eliasku.github.io/picos/preview.png"
    };

    project.includeProject(path.join(project.path.EKX_ROOT, "appbox"));
};