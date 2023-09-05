
#include "nlohmann.h"
struct kautil_json_nlohmann_prv{
    void * dl;
    void (*destruct)(kautil_json_nlohmann_extern*);
    int (*dlclose)(void*);
};

kautil_json_nlohmann_extern * kautil_json_nlohmann_extern_initialize(
        void *(*dlopen)(const char * ,int flags)
        ,void *(*dlsym)(void * ,const char *)
        ,int (*dlclose)(void*)
        ,int flags
        ){
    auto hdl = dlopen(KAUTIL_JSON_NLOHMANN_SO,flags);
    auto m = ((kautil_json_nlohmann_extern *(*)())dlsym(hdl,"extern_initialize"))();
    m->prv = new  kautil_json_nlohmann_prv;
    m->prv->destruct=(void (*)(kautil_json_nlohmann_extern*))dlsym(hdl,"extern_finalize");
    m->prv->dl =hdl;
    m->prv->dlclose = dlclose;
    return m;
}

void kautil_json_nlohmann_extern_finalize(kautil_json_nlohmann_extern *m){
    // order has mean
    auto dlclose = m->prv->dlclose;
    auto prv = m->prv;
    auto dl = m->prv->dl;
    auto destruct = m->prv->destruct;

    delete prv;
    destruct(m);
    dlclose(dl);
}


