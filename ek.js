const path = require("path");

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
    project.version_name = "1.1.22";
    project.version_code = "42";
    project.orientation = "portrait";
    project.assets = {
        output: "export/contents/assets"
    };

    const secretPath = '../keys/demo';

    project.android = {
        application_id: "ilj.play.demo",
        package_id: "com.eliasku.iljdemo",
        admob_app_id: "ca-app-pub-3931267664278058~6275600638",
        game_services_id: "300613663654",
        googleServicesConfigDir: secretPath,
        signingConfigPath: path.join(secretPath, "google-play-signing.json"),
        serviceAccountKey: '../keys/_fastlane/api-5415425812860184091-52535-0303d398e1e6.json',
    };

    project.ios = {
        application_id: "ilj.play.demo",
        admob_app_id: "ca-app-pub-3931267664278058~8636974437",
        googleServicesConfigDir: secretPath,
        appStoreCredentials: '../keys/_fastlane/appstore.json',
    };

    project.web = {
        firebaseToken: "../keys/_firebase/token.txt",
        applications: [
            {
                platform: "play",
                url: "https://play.google.com/store/apps/details?id=ilj.play.demo",
                id: "ilj.play.demo"
            }
        ]
    };

    project.html = {
        firebaseAutoSetup: true,
        background_color: "black",
        text_color: "#73b3cb",
        og: {
            url: "https://play-ilj.web.app",
            image: "https://play-ilj.web.app/icons/icon192.png"
        }
    };

    project.importModule("@ekx/appbox");
};