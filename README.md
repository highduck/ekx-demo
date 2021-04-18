# ekx demo

[![Build Android](https://github.com/highduck/ekx-demo/actions/workflows/build.yml/badge.svg)](https://github.com/highduck/ekx-demo/actions/workflows/build.yml)


ekx demo project

### API configuration files

To make Android, iOS, PWA builds all your public API configuration files should be downloaded from your own services:

- google-services.json
- GoogleService-Info.plist

### iOS

```bash
ekx ios
```

### Android

```bash
ekx android
```

### Web

Install `emsdk`: [instructions](https://emscripten.org/docs/getting_started/downloads.html)

Select Emscripten toolchain for CMake:
```
-DCMAKE_TOOLCHAIN_FILE=/Users/PATH_TO_emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake
```

Build cmake project executable, then copy `wasm` and `js` output to `export/web` directory.

```bash
ekx web
```
