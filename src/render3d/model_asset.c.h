#include <ek/local_res.h>
#include <ek/log.h>

// model

static void model_on_file_loaded(ek_local_res* lr) {
    model_asset_t* this_ = (model_asset_t*)lr->userdata;
    if (ek_local_res_success(lr)) {
        calo_reader_t reader = INIT_ZERO;
        reader.p = lr->buffer;
        read_calo(&reader);
        model3d_t model = read_stream_model3d(&reader);
        REF_RESOLVE(res_mesh3d, this_->res) = static_mesh(model);
        ek_local_res_close(lr);
    } else {
        log_error("MODEL resource not found: %s", this_->name);
        this_->base.error = 1;
    }
    this_->base.state = ASSET_STATE_READY;
}

static void model_do_load(asset_t* base) {
    model_asset_t* asset = (model_asset_t*)base;
    char full_path[1024];
    ek_snprintf(full_path, sizeof full_path, "%s/%s.model", asset_manager.base_path, asset->name);
    ek_local_res_load(full_path, model_on_file_loaded, asset);
}

static void model_do_unload(asset_t* base) {
    model_asset_t* asset = (model_asset_t*)base;
    static_mesh_t* mesh = &REF_RESOLVE(res_mesh3d, asset->res);
    static_mesh_destroy(mesh);
}

static void model_read(asset_t* base, calo_reader_t* r) {
    log_trace("read model header");
    model_asset_t* asset = (model_asset_t*)base;
    asset->name = read_stream_string(r);
    asset->res = R_MESH3D(read_u32(r));
}

static void setup_model_asset_(void) {
    asset_type type = NEW_ASSET_TYPE(model);
    type.read = model_read;
    type.do_load = model_do_load;
    type.do_unload = model_do_unload;
    add_asset_type(type);
}
