

#ifdef TMAIN_KAUTIL_JSON_NLOHMANN_STATIC 

#include <stdio.h>
#include "nlohmann.h"

// kautil_sharedlib
#include <libloaderapi.h>
#include <errhandlingapi.h>
int rtld_lazy = 0;
int rtld_now = 1;
int rtld_global = 2;
int rtld_local = 4;
int rtld_nodelete = 8;
int rtld_noload = 16;
int rtld_deepbind = 32;

void* kautil_dlopen(const char * path_to_shared_lib, int flags) { return LoadLibraryA((char*)path_to_shared_lib); }
void* kautil_dlsym(void * handle, const char *symbol) { return (void *) GetProcAddress(reinterpret_cast<HMODULE>(handle),(char *)symbol); }
int kautil_dlclose(void * handle) { return FreeLibrary(reinterpret_cast<HMODULE>(handle));}


int main(){
    
    auto json = kautil_json_nlohmann_extern_initialize(kautil_dlopen,kautil_dlsym,kautil_dlclose,rtld_lazy|rtld_nodelete);
    auto j = json->initialize();
    
        auto arr_0 = json->add_array_key(j,"arr_0");
            json->add_string_index(arr_0,0,"a");
            json->add_string_index(arr_0,1,"b");
            json->add_string_index(arr_0,2,"c");

        auto obj_0 = json->add_object_key(j,"obj_0");
            json->add_integer_key(obj_0,"i",123);
            json->add_string_key(obj_0,"s","abc");
    
    auto indent = 4;
    auto size = json->dump_size(j,indent);
    if(size > 0){
        char * dump = new char[size];
        json->dump_to_string(j,dump,size,indent);
        printf("%.*s",size,dump);
        delete [] dump;
    }
    json->finalize(j);
    
    return 0;
}







#endif