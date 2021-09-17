const path = require("path");

module.exports = function (project) {
    project.addModule({
        name: "demo",
        path: __dirname,
        cpp: "src"
    });

    project.name = "ekx-demo";
    project.title = "ekx";
    project.desc = "Engine Demo";
    project.version_name = "1.5.0";
    project.version_code = "69";
    project.orientation = "portrait";

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