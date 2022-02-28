//#include <emscripten.h>
#include <ek/nep.h>
#include <ek/log.h>

static void handle_call_result(call_t* call){
    log_info("call completed %u", call->status);
    log_info("call result type %u", call->result.type);

    for(uint32_t i = 0; i < call->result.via.map.size; ++i) {
        log_info("#%u => %s: %s", i, call->result.via.map.ptr[i].key.via.str.ptr, call->result.via.map.ptr[i].val.via.str.ptr);
    }
}

void temp_test(void) {
//    EM_ASM({
//       window["nep_handler"] = (call) =>{
//           console.info(JSON.stringify(call.args));
//           call.set_result({user:"User Name", user_id: "12345"});
//       };
//    });

    call_t* call = create_call(handle_call_result, 0);
    msgpack_object obj;
    obj.type = MSGPACK_OBJECT_MAP;
    obj.via.map.size = 1;
    static msgpack_object_kv pp;
    obj.via.map.ptr = &pp;
    pp.key.type = MSGPACK_OBJECT_STR;
    pp.key.via.str.ptr = "test_key";
    pp.key.via.str.size = strlen("test_key");
    pp.val.type = MSGPACK_OBJECT_STR;
    pp.val.via.str.ptr = "test value";
    pp.val.via.str.size = strlen("test value");
    send_call(call, obj);
}