### kautil_json_nlohmann
* wrapper of nlohmann json. prepared this to shorten copilation time

### note 
* kautil_json_nlohmann_extern_initialize : use when want to make detailed settings for dl options
* kautil_json_nlohmann_extern_auto       : this is handire than kautil_json_nlohmann_extern_initialize, and perhaps compilation time is short, but dl options are fixed.     

### example 

```c++

#include <stdio.h>
#include "nlohmann.h"
int main(){
    
    auto json = kautil_json_nlohmann_extern_auto();
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
    json->free(j);
    return 0;
}


```