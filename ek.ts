import * as path from "path";
import {Project} from "ekx/lib/cli/project.js";

export async function setup(project: Project) {
    project.addModule({
        name: "demo",
        cpp: "src"
    });

    project.name = "ekx-demo";
    project.title = "ekx";
    project.desc = "Engine Demo";
    project.appIcon = "assets/res#icon";

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

    project.web.firebaseToken = "../keys/_firebase/token.txt";
    project.web.applications = [{
        platform: "play",
        url: "https://play.google.com/store/apps/details?id=ilj.play.demo",
        id: "ilj.play.demo"
    }];
    project.web.firebaseConfig = {
        apiKey: "AIzaSyBPw5wA9U8CV6MN-65bBmxOsJOUzR_ZowM",
        authDomain: "play-ilj.firebaseapp.com",
        databaseURL: "https://play-ilj.firebaseio.com",
        projectId: "play-ilj",
        storageBucket: "play-ilj.appspot.com",
        messagingSenderId: "300613663654",
        appId: "1:300613663654:web:3e7a04085236af51a9ad67",
        measurementId: "G-CFW5S2M6KZ"
    };
    project.web.background_color = "black";
    project.web.text_color = "#73b3cb";
    project.web.og = {
        url: "https://play-ilj.web.app",
        image: "https://play-ilj.web.app/icons/icon192.png"
    };

    await project.importModule("ekx/packages/appbox/ek.ts");
    //await project.importModule("ekx/external/pocketmod/ek.ts");
    await project.importModule("ekx/external/quickjs/ek.ts");
}
