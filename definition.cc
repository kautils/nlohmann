
#include "./nlohmann.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <vector>


extern "C"{

//#define __nlmj(obj) (&reinterpret_cast<nlomann_wrapper*>(obj)->nlmj)
#define __nlmj(obj) (reinterpret_cast<nlohmann::json*>(obj))
void nlmj_dump_to_fs(void *hdl, const char *outdir, const char * name,int const& indent);
void nlmj_load_string(void * hdl,const char * data, uint64_t const& size);
void nlmj_load_file(void * hdl,const char * path_to_json);
bool nlmj_is(void * hdl,int8_t const& t);

//struct nlomann_wrapper{
//    nlohmann::json nlmj;
//    std::string string_key_buffer;
//};
//  

    
    

void nlmj_clear(void *hdl){
    __nlmj(hdl)->clear();
}

uint64_t nlmj_erase_with_key(void *hdl,const char * key){
    return __nlmj(hdl)->erase(key);
}
void nlmj_erase_with_index(void *hdl,uint64_t index){
    __nlmj(hdl)->erase(index);
}
//void * nlmj_initialize(){ return new nlohmann::json; }
void * nlmj_initialize(){ return new nlohmann::json{}; }
void nlmj_free(void * hdl){
    delete reinterpret_cast<nlohmann::json*>(hdl);
}

void* nlmj_add_uint64_key(void * hdl,const char * key,uint64_t const& value){
    return &(__nlmj(hdl)->operator[](key) = value);
}
void* nlmj_add_int64_key(void * hdl,const char * key,int64_t const& value){
    return &(__nlmj(hdl)->operator[](key) = value);
}
void* nlmj_add_string_key(void * hdl,const char * key,const char * value){
    return &(__nlmj(hdl)->operator[](key) = value);
}
void* nlmj_add_integer_key(void * hdl,const char * key,uint64_t const& value){
    return &(__nlmj(hdl)->operator[](key) = value);
}
void* nlmj_add_float_key(void * hdl,const char * key,double const& value){
    return &(__nlmj(hdl)->operator[](key) = value);
}
void* nlmj_add_bool_key(void * hdl,const char * key,bool const& value){
    return &(__nlmj(hdl)->operator[](key) = value);
}
void* nlmj_add_object_key(void * hdl,const char * key){
    return &(__nlmj(hdl)->operator[](key) = nlohmann::json::object());
}
void* nlmj_add_array_key(void * hdl,const char * key){
    return &(__nlmj(hdl)->operator[](key) = nlohmann::json::array());
}
void* nlmj_add_string_index(void * hdl, int const& index,const char * value){
    return &(__nlmj(hdl)->operator[](index) = value);
}
void* nlmj_add_integer_index(void * hdl, int const& index,uint64_t const& value){
    return &(__nlmj(hdl)->operator[](index) = value);
}
void* nlmj_add_uint64_index(void * hdl, int const& index,uint64_t const& value){
    return &(__nlmj(hdl)->operator[](index) = value);
}
void* nlmj_add_int64_index(void * hdl, int const& index,int64_t const& value){
    return &(__nlmj(hdl)->operator[](index) = value);
}
void* nlmj_add_float_index(void * hdl, int const& index,double const& value){
    return &(__nlmj(hdl)->operator[](index) = value);
}
void* nlmj_add_bool_index(void * hdl, int const& index,bool const& value){
    return &(__nlmj(hdl)->operator[](index) = value);
}
void* nlmj_add_object_index(void * hdl, int const& index){
    return &(__nlmj(hdl)->operator[](index) = nlohmann::json::object());
}
void* nlmj_add_array_index(void * hdl, int const& index){
    return &(__nlmj(hdl)->operator[](index) = nlohmann::json::array());
}

void *nlmj_push_back_string(void *hdl,const char *value){
    return &(__nlmj(hdl)->operator[](__nlmj(hdl)->size()) = value);
}

void *nlmj_push_back_integer(void *hdl, const uint64_t &value) {
    return &(__nlmj(hdl)->operator[](__nlmj(hdl)->size()) = value);
}

void *nlmj_push_back_float(void *hdl, const double &value) {
    return &(__nlmj(hdl)->operator[](__nlmj(hdl)->size()) = value);
}

void *nlmj_push_back_bool(void *hdl, const bool &value) {
    return &(__nlmj(hdl)->operator[](__nlmj(hdl)->size()) = value);
}

void *nlmj_push_back_object(void *hdl) {
    return &(__nlmj(hdl)->operator[](__nlmj(hdl)->size()) = nlohmann::json::object());
}
void *nlmj_push_back_array(void *hdl) {
    return &(__nlmj(hdl)->operator[](__nlmj(hdl)->size()) = nlohmann::json::array());
}

void nlmj_merge_patch(void * src,void * patch){
    // (RFC 7386) : https://json.nlohmann.me/features/merge_patch/
    __nlmj(src)->merge_patch(*__nlmj(patch));
}

void nlmj_merge_files(void * hdl,char * outdir,char * name,int const& indent,char ** arr_files,uint64_t const& n_arr_files){
    auto res_hdl  = nlmj_initialize();
    for(auto i = 0; i < n_arr_files; ++i){
        auto buf_hdl = nlmj_initialize();
        nlmj_load_file(buf_hdl,arr_files[i]);
        if(nlmj_is(buf_hdl,NLMJ_T_NULL)) nlmj_load_string(buf_hdl,"{}",2);  /* if merge null, all is erased */
        nlmj_merge_patch(res_hdl,buf_hdl);
        nlmj_free(buf_hdl);
    }
    nlmj_dump_to_fs(res_hdl,outdir,name,indent);
    nlmj_free(res_hdl);
}





uint64_t nlmj_dump_size(void * hdl,const int& indent){
    return __nlmj(hdl)->dump(indent).size();
}
void nlmj_dump_to_string(void * hdl,void * result,uint64_t const& size,const int& indent){
    auto const& str = __nlmj(hdl)->dump(indent);
    if(str.size() > size){
        std::cerr << "[exception] : short of size" << std::endl;
        std::cerr << "              required size is " << str.size()  << std::endl;
        std::cerr << "              but specified size is " << size << "."  << std::endl;
        assert(false);
    }
    memcpy(result,str.data(),str.size());
}
void nlmj_dump_to_file(void * hdl,const char * file,const int& indent){
    auto const& str = __nlmj(hdl)->dump(indent);
    std::ofstream{file,std::ios::binary}.write(str.data(),str.size());
}




void * get_ref(nlohmann::json::reference const& res_obj) {
    switch (res_obj.type()) {
        case nlohmann::json::value_t::string : {
            return const_cast<void *>(reinterpret_cast<const void *>(res_obj.get_ref<const nlohmann::json::string_t &>().begin().base()));
        }
        case nlohmann::json::value_t::number_integer : {
            return const_cast<void *>(reinterpret_cast<const void *>(&res_obj.get_ref<const nlohmann::json::number_integer_t &>()));
        };
        case nlohmann::json::value_t::number_float : {
            return const_cast<void *>(reinterpret_cast<const void *>(&res_obj.get_ref<const nlohmann::json::number_float_t &>()));
        };
        case nlohmann::json::value_t::boolean : {
            return const_cast<void *>(reinterpret_cast<const void *>(&res_obj.get_ref<const nlohmann::json::boolean_t &>()));
        };
        case nlohmann::json::value_t::number_unsigned : {
            return const_cast<void *>(reinterpret_cast<const void *>(&res_obj.get_ref<const nlohmann::json::number_unsigned_t &>()));
        };
        case nlohmann::json::value_t::binary :{
            return const_cast<void *>(reinterpret_cast<const void *>(&res_obj.get_ref<const nlohmann::json::binary_t &>()));
        };
        case nlohmann::json::value_t::array :
        case nlohmann::json::value_t::object:
        {
            return const_cast<void *>(reinterpret_cast<const void *>(&res_obj));
        };
        case nlohmann::json::value_t::discarded:
        case nlohmann::json::value_t::null:
        default: {
            return nullptr;
        };
    }
}

getvt_result get_ref_and_type(nlohmann::json::reference const& res_obj) {
    switch (res_obj.type()) {
        case nlohmann::json::value_t::string : {
            return {.value=const_cast<void *>(reinterpret_cast<const void *>(res_obj.get_ref<const nlohmann::json::string_t &>().begin().base())),.type=NLMJ_T_STRING};
        }
        case nlohmann::json::value_t::number_integer : {
            return {.value=const_cast<void *>(reinterpret_cast<const void *>(&res_obj.get_ref<const nlohmann::json::number_integer_t &>())),.type=NLMJ_T_INTEGER};
        };
        case nlohmann::json::value_t::number_float : {
            return {.value=const_cast<void *>(reinterpret_cast<const void *>(&res_obj.get_ref<const nlohmann::json::number_float_t &>())),.type=NLMJ_T_FLOAT};
        };
        case nlohmann::json::value_t::boolean : {
            return {.value=const_cast<void *>(reinterpret_cast<const void *>(&res_obj.get_ref<const nlohmann::json::boolean_t &>())),.type=NLMJ_T_BOOL};
        };
        case nlohmann::json::value_t::number_unsigned : {
            return {.value=const_cast<void *>(reinterpret_cast<const void *>(&res_obj.get_ref<const nlohmann::json::number_unsigned_t &>())),.type=NLMJ_T_UNSINGED};
        };
        case nlohmann::json::value_t::binary :{
            return {.value=const_cast<void *>(reinterpret_cast<const void *>(&res_obj.get_ref<const nlohmann::json::binary_t &>())),.type=NLMJ_T_BINARY};
        };
        case nlohmann::json::value_t::array : {
            return {.value=const_cast<void *>(reinterpret_cast<const void *>(&res_obj)),.type=NLMJ_T_ARRAY};
        }
        case nlohmann::json::value_t::object :
        {
            return {.value=const_cast<void *>(reinterpret_cast<const void *>(&res_obj)),.type=NLMJ_T_OBJECT};
        };
        case nlohmann::json::value_t::discarded: {
            return {.value=nullptr,.type=NLMJ_T_DISCARDED};
        }
        case nlohmann::json::value_t::null:{
            return {.value=nullptr,.type=NLMJ_T_NULL};
        }
        default: {
            return {.value=nullptr,.type=NLMJ_T_UNKNOWN};
        };
    }
}


int get_type(nlohmann::json::reference const& res_obj) {
    switch (res_obj.type()) {
        case nlohmann::json::value_t::string : return NLMJ_T_STRING;
        case nlohmann::json::value_t::number_integer : return NLMJ_T_INTEGER;;
        case nlohmann::json::value_t::number_float : return NLMJ_T_FLOAT;;
        case nlohmann::json::value_t::boolean : return NLMJ_T_BOOL;;
        case nlohmann::json::value_t::number_unsigned : return NLMJ_T_UNSINGED;;
        case nlohmann::json::value_t::binary :return NLMJ_T_BINARY;;
        case nlohmann::json::value_t::array : return NLMJ_T_ARRAY;
        case nlohmann::json::value_t::object : return NLMJ_T_OBJECT;;
        case nlohmann::json::value_t::discarded: return NLMJ_T_DISCARDED;
        case nlohmann::json::value_t::null:return NLMJ_T_NULL;
        default:  return NLMJ_T_UNKNOWN;;
    }
}





void * nlmj_get_with_key(void * hdl,const char * key){
    auto obj = __nlmj(hdl);
    if(!obj->count(key)){
        return nullptr;
    }else{
        return get_ref(obj->operator[](key));
    }
    return nullptr;
}

void * nlmj_get_with_index(void * hdl,int const& index){
    auto obj = __nlmj(hdl);
    if(obj->size() > index){
        return get_ref(obj->operator[](index));
    }else{
        return nullptr;
    }
}

getvt_result nlmj_getvt_with_key(void * hdl,const char * key){
    auto obj = __nlmj(hdl);
    if(obj->count(key)){
        return get_ref_and_type(obj->operator[](key));
    }else{
        return getvt_result{.value=nullptr,.type=NLMJ_T_KEY_NOT_FOUND};
    }
}

getvt_result nlmj_getvt_with_index(void * hdl,int const& index){
    auto obj = __nlmj(hdl);
    if(obj->size() > index){
        return get_ref_and_type(obj->operator[](index));
    }else{
        return getvt_result{.value=nullptr,.type=NLMJ_T_INDEX_OUT_OF_RANGE};
    }
}

uint64_t nlmj_get_array_length(void * hdl){
    return __nlmj(hdl)->is_array()   ? __nlmj(hdl)->size() : 0;
}



const char* nlmj_key(void *hdl){
    return reinterpret_cast<nlohmann::json::const_iterator*>(hdl)->key().c_str();
}





bool nlmj_is(void * hdl,int8_t const& t){
    switch(t){
        case NLMJ_T_INTEGER: return __nlmj(hdl)->is_number_integer() ;
        case NLMJ_T_FLOAT: return __nlmj(hdl)->is_number_float();
        case NLMJ_T_STRING: return __nlmj(hdl)->is_string();
        case NLMJ_T_ARRAY: return __nlmj(hdl)->is_array();
        case NLMJ_T_OBJECT: return __nlmj(hdl)->is_object();
        case NLMJ_T_BOOL: return __nlmj(hdl)->is_boolean();
        case NLMJ_T_BINARY: return __nlmj(hdl)->is_binary();
        case NLMJ_T_NULL: return __nlmj(hdl)->is_null();
    }
    return false;
}


using iterator_t = nlohmann::json::const_iterator;

const int64_t * nlmj_iterator_value_int64(void *hdl){
    return &reinterpret_cast<iterator_t*>(hdl)->value().get_ref<const nlohmann::json::number_integer_t &>();
}
const double * nlmj_iterator_value_float(void *hdl){
    return &reinterpret_cast<iterator_t*>(hdl)->value().get_ref<const nlohmann::json::number_float_t &>();
}
const bool * nlmj_iterator_value_bool(void *hdl){
    return &reinterpret_cast<iterator_t*>(hdl)->value().get_ref<const nlohmann::json::boolean_t &>();
}
const char * nlmj_iterator_value_string(void *hdl){
    return reinterpret_cast<iterator_t*>(hdl)->value().get_ref<const nlohmann::json::string_t &>().c_str();
}

//https://json.nlohmann.me/features/comments/
// as default, exception is true , comment is true
void nlmj_load_file(void * hdl,const char * path_to_json){
    namespace fs = std::filesystem;
    auto o = __nlmj(hdl);
    if(fs::exists(path_to_json)){
        *o = o->parse(std::ifstream{path_to_json},nullptr,true,true);
    }else {
        std::cerr << "[exception] : no such path " << path_to_json << std::endl;
    }
}

void nlmj_load_from_fs(void * hdl,const char * dir,const char * name){
    nlmj_load_file(hdl,std::string{dir}.append("/").append(name).c_str());
}


void nlmj_load_string(void * hdl,const char * data, uint64_t const& size){
    if(size){
        auto buf = std::string(size,'\0');
        memcpy(buf.data(),data,size);
        *__nlmj(hdl) = __nlmj(hdl)->parse(buf,nullptr,true,true);
    }
}

void dev(void *hdl){
    auto o = __nlmj(hdl);
    *o = o->parse(R"(
      {
        "pi": 3.141,
        "happy": true
      }
    )");

    std::cout <<"AAAAA" << std::endl;
    std::cout << o->dump(4) << std::endl;

}


void *  nlmj_iterator_initialize(void *hdl,int8_t const& order){
    auto res = new iterator_t{};
    switch(order){
        case NLMJ_T_ITERATOR_BEGIN: *res = __nlmj(hdl)->begin() ; break;
        case NLMJ_T_ITERATOR_END: *res = __nlmj(hdl)->end(); break ;
        default : *res = __nlmj(hdl)->begin() ;
    }
    return res;
}



bool nlmj_iterator_is(void * itr_handler,int8_t const& order ){
    auto & itr = *reinterpret_cast<nlohmann::json::const_iterator*>(itr_handler);
    switch(order){
        case NLMJ_T_INTEGER: return itr->is_number_integer() ;
        case NLMJ_T_FLOAT: return itr->is_number_float();
        case NLMJ_T_STRING: return itr->is_string();
        case NLMJ_T_ARRAY: return itr->is_array();
        case NLMJ_T_OBJECT: return itr->is_object();
        case NLMJ_T_BOOL: return itr->is_boolean();
        case NLMJ_T_BINARY: return itr->is_binary();
        case NLMJ_T_NULL: return itr->is_null();
    }
    return false;
}



void * nlmj_iterator_to_object(void * itr){
    return (void*) &**reinterpret_cast<iterator_t*>(itr);
}

void nlmj_iterator_next(void * itr){
    ++(*reinterpret_cast<iterator_t*>(itr));
}
bool nlmj_iterator_equal(void * a,void * b){
    return *reinterpret_cast<iterator_t*>(a) == *reinterpret_cast<iterator_t*>(b);
}

void nlmj_iterator_free(void * itr){
    delete reinterpret_cast<nlohmann::json::const_iterator*>(itr);
}

//uint64_t const& nlmj_version() {
//    static auto const& version = static_cast<uint64_t>(KAUTIL_JSON_NLOHMANN_VERSION_CHECK_NUMBER);
//    return version;
//}


void nlmj_dump_to_fs(void *hdl, const char *outdir, const char *name, const int &indent) {
    namespace fs = std::filesystem;
    if (!fs::exists(outdir)) fs::create_directories(outdir);
    std::ofstream{fs::path(outdir) / name, std::ios::binary} << __nlmj(hdl)->dump(indent);

}
}





struct nlmj_node_hdl;
nlmj_node_hdl * nlmj_node_iterator(void * obj);
bool nlmj_node_is_array(nlmj_node_hdl* itr);
bool nlmj_node_is_object(nlmj_node_hdl* itr);
bool nlmj_node_next(nlmj_node_hdl* itr);
void nlmj_node_reset(nlmj_node_hdl* itr);
const char* nlmj_node_key(nlmj_node_hdl* itr);
void * nlmj_node_getv(nlmj_node_hdl* itr);
getvt_result nlmj_node_getvt(nlmj_node_hdl* itr);
void nlmj_node_release(nlmj_node_hdl* itr);






#define assign(m,name) m->name =nlmj_##name;


struct nlmj_node_hdl{
    iterator_t beg;
    iterator_t cur;
    iterator_t end;
    bool fst = true;
    std::string buffer_for_string_key;
    
//    int type = NLMJ_T_NULL;
};

extern "C" kautil_json_nlohmann_extern* extern_initialize(){
    auto m = new kautil_json_nlohmann_extern;
    assign(m,key);
    
    m->node_iterator=nlmj_node_iterator;
    assign(m,node_iterator);
    assign(m,node_is_array);
    assign(m,node_is_object);
    assign(m,node_key);
    assign(m,node_next);
    assign(m,node_release);
    assign(m,node_reset);
    assign(m,node_getv);
    assign(m,node_getvt);
    assign(m,iterator_value_int64);
    assign(m,iterator_value_float);
    assign(m,iterator_value_bool);
    assign(m,iterator_value_string);
    assign(m,iterator_initialize);
    assign(m,iterator_to_object);
    assign(m,iterator_next);
    assign(m,iterator_is);
    assign(m,iterator_equal);
    assign(m,iterator_free);
    assign(m,initialize);
    assign(m,free);
    assign(m,clear);
    assign(m,erase_with_key);
    assign(m,erase_with_index);
    assign(m,add_string_key);
    assign(m,add_integer_key);
    assign(m,add_int64_key);
    assign(m,add_uint64_key);
    assign(m,add_float_key);
    assign(m,add_bool_key);
    assign(m,add_object_key);
    assign(m,add_array_key);
    assign(m,add_string_index);
    assign(m,add_integer_index);
    assign(m,add_int64_index);
    assign(m,add_uint64_index);
    assign(m,add_float_index);
    assign(m,add_bool_index);
    assign(m,add_object_index);
    assign(m,add_array_index);
    assign(m,push_back_string);
    assign(m,push_back_integer);
    assign(m,push_back_float);
    assign(m,push_back_bool);
    assign(m,push_back_array);
    assign(m,push_back_object);
    assign(m,merge_patch);
    assign(m,merge_files);
    assign(m,dump_size);
    assign(m,dump_to_string);
    assign(m,dump_to_file);
    assign(m,dump_to_fs);
    assign(m,load_string);
    assign(m,load_file);
    assign(m,load_from_fs);
    assign(m,get_array_length);
    assign(m,get_with_key);
    assign(m,get_with_index);
    assign(m,getvt_with_key);
    assign(m,getvt_with_index);
    assign(m,is);
    return m;
}


extern "C" void extern_finalize(kautil_json_nlohmann_extern* m){
    delete m;
}


#include <stdarg.h>

struct kautil_json_nlmj_exception : std::exception{
    std::string msg;
    kautil_json_nlmj_exception(int bytes,const char* fmt,...){
        va_list lst;
        va_start(lst,fmt);
        msg.resize(bytes);
        __mingw_vsprintf(msg.data(),fmt,lst);
        va_end(lst);
    }
    const char * what() const noexcept override{ return msg.data(); }
};


nlmj_node_hdl * nlmj_node_iterator(void * obj){
    int type = -1;
    if(nlmj_is(obj,NLMJ_T_OBJECT))type= NLMJ_T_OBJECT;
    else if(nlmj_is(obj,NLMJ_T_ARRAY)) type = NLMJ_T_ARRAY;
        if((NLMJ_T_OBJECT==type) || (NLMJ_T_ARRAY==type)){
        auto res = (nlmj_node_hdl*)0;
        res = new nlmj_node_hdl{/*.type=type*/};
        res->beg = __nlmj(obj)->begin();
        res->end = __nlmj(obj)->end();
        res->cur = res->beg;
        return res;
    }
    throw kautil_json_nlmj_exception{1024,"argument passed is neither object nor array [%s(%d)]",__FILE__,__LINE__};
}

bool nlmj_node_is_array(nlmj_node_hdl* itr){ return nlohmann::json::value_t::array == itr->cur->type(); }
bool nlmj_node_is_object(nlmj_node_hdl* itr){ return nlohmann::json::value_t::object == itr->cur->type(); }
bool nlmj_node_next(nlmj_node_hdl* itr){
    if(itr->fst)itr->fst= false;
    else ++itr->cur;
    return itr->cur != itr->end;
}
void nlmj_node_reset(nlmj_node_hdl* itr){ itr->cur = itr->beg;itr->fst = true; }


const char* nlmj_node_key(nlmj_node_hdl* itr){
    return itr->cur.key().data();
}



// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// nlohmann::json::reference & nlohmann::json::iterator::reference should not be treated in same way 
// problem was caused when treating string of nlohmann::json::iterator::reference.
// i think the life cycle of iterator::reference is the same as reference, but they are different.
// "get_ref<const nlohmann::json::string_t &>().data()" works well on reference, but dose not work on iterator::reference.
// the reference of string exists inside reference, but not exists on iterator::reference. 

void * get_ref_iterator(nlmj_node_hdl * itr) {
    auto const& res_obj = itr->cur.value();
    switch (res_obj.type()) {
        case nlohmann::json::value_t::string : {
            itr->buffer_for_string_key=res_obj.get_ref<const nlohmann::json::string_t &>().data();
            return reinterpret_cast<void *>(itr->buffer_for_string_key.data());
        }
        case nlohmann::json::value_t::number_integer : {
            return const_cast<void *>(reinterpret_cast<const void *>(&res_obj.get_ref<const nlohmann::json::number_integer_t &>()));
        };
        case nlohmann::json::value_t::number_float : {
            return const_cast<void *>(reinterpret_cast<const void *>(&res_obj.get_ref<const nlohmann::json::number_float_t &>()));
        };
        case nlohmann::json::value_t::boolean : {
            return const_cast<void *>(reinterpret_cast<const void *>(&res_obj.get_ref<const nlohmann::json::boolean_t &>()));
        };
        case nlohmann::json::value_t::number_unsigned : {
            return const_cast<void *>(reinterpret_cast<const void *>(&res_obj.get_ref<const nlohmann::json::number_unsigned_t &>()));
        };
        case nlohmann::json::value_t::binary :{
            return const_cast<void *>(reinterpret_cast<const void *>(&res_obj.get_ref<const nlohmann::json::binary_t &>()));
        };
        case nlohmann::json::value_t::array :
        case nlohmann::json::value_t::object:
        {
            return (void*)&*itr->cur;
        };
        case nlohmann::json::value_t::discarded:
        case nlohmann::json::value_t::null:
        default: {
            return nullptr;
        };
    }
}

getvt_result get_ref_and_type_iterator(nlmj_node_hdl * itr) {
    auto const& res_obj = itr->cur.value();
    switch (res_obj.type()) {
        case nlohmann::json::value_t::string : {
            itr->buffer_for_string_key=res_obj.get_ref<const nlohmann::json::string_t &>().data();
            return {.value=reinterpret_cast<void *>(itr->buffer_for_string_key.data()),.type=NLMJ_T_STRING};
        }
        case nlohmann::json::value_t::number_integer : {
            return {.value=const_cast<void *>(reinterpret_cast<const void *>(&res_obj.get_ref<const nlohmann::json::number_integer_t &>())),.type=NLMJ_T_INTEGER};
        };
        case nlohmann::json::value_t::number_float : {
            return {.value=const_cast<void *>(reinterpret_cast<const void *>(&res_obj.get_ref<const nlohmann::json::number_float_t &>())),.type=NLMJ_T_FLOAT};
        };
        case nlohmann::json::value_t::boolean : {
            return {.value=const_cast<void *>(reinterpret_cast<const void *>(&res_obj.get_ref<const nlohmann::json::boolean_t &>())),.type=NLMJ_T_BOOL};
        };
        case nlohmann::json::value_t::number_unsigned : {
            return {.value=const_cast<void *>(reinterpret_cast<const void *>(&res_obj.get_ref<const nlohmann::json::number_unsigned_t &>())),.type=NLMJ_T_UNSINGED};
        };
        case nlohmann::json::value_t::binary :{
            return {.value=const_cast<void *>(reinterpret_cast<const void *>(&res_obj.get_ref<const nlohmann::json::binary_t &>())),.type=NLMJ_T_BINARY};
        };
        case nlohmann::json::value_t::array : 
        case nlohmann::json::value_t::object :
        {
            return {.value=(void*)(&*itr->cur),.type=NLMJ_T_OBJECT};
        };
        case nlohmann::json::value_t::discarded: {
            return {.value=nullptr,.type=NLMJ_T_DISCARDED};
        }
        case nlohmann::json::value_t::null:{
            return {.value=nullptr,.type=NLMJ_T_NULL};
        }
        default: {
            return {.value=nullptr,.type=NLMJ_T_UNKNOWN};
        };
    }
}




getvt_result nlmj_node_getvt(nlmj_node_hdl* itr){
    auto v = itr->cur.value();
    auto type = get_type(v);
    return get_ref_and_type_iterator(itr);
}


void * nlmj_node_getv(nlmj_node_hdl* itr){ return reinterpret_cast<void*>(const_cast<nlohmann::json*>(&*itr->cur)); }
void nlmj_node_release(nlmj_node_hdl* itr){ delete itr; }








void dev_iterate(void *hdl){
    auto cnt = 0;
    auto o = __nlmj(hdl);
    for(auto el = o->begin(); el != o->end(); ++el){
        if(el->is_object()){
            std::cout << "t : " << el.key() << std::endl;
            if(!el->is_null()) dev(&*el);
        }else if(el->is_array()){
            std::cout << "a : " << el.key() << std::endl;
            if(!el->is_null()) dev(&*el);
        }
        else{
            if(!el->is_null()){
                if(!o->is_array()){
                    if(el->is_number_integer()){
                        auto p = &el->get_ref<const nlohmann::json::number_integer_t &>();
                    }else if(el->is_string()){
                        auto p = el->get_ref<const nlohmann::json::string_t &>().c_str();
                    }
                    std::cout << "b : " << el.key() << " : " << el.value() << std::endl;
                }else{
                    std::cout << "b : " << cnt++  << " : " << el.value() << std::endl;
                }
            }else{
                std::cout << "b : " << std::endl;
            }
        }
    }

}


