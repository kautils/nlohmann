
#ifndef KAUTIL_JSON_NLOHMANN_C_INTERFACE_H
#define KAUTIL_JSON_NLOHMANN_C_INTERFACE_H


#include "stdint.h"

#define NLMJ_T_INTEGER 0
#define NLMJ_T_UNSINGED 1
#define NLMJ_T_FLOAT 2
#define NLMJ_T_STRING 3
#define NLMJ_T_BOOL 4
#define NLMJ_T_BINARY 5
#define NLMJ_T_ARRAY  6
#define NLMJ_T_OBJECT 7
#define NLMJ_T_NULL 8
#define NLMJ_T_DISCARDED 9
#define NLMJ_T_UNKNOWN 10
#define NLMJ_T_KEY_NOT_FOUND 11
#define NLMJ_T_INDEX_OUT_OF_RANGE 12
#define NLMJ_T_ITERATOR_BEGIN  100
#define NLMJ_T_ITERATOR_END 101

struct getvt_result{ void * value=0;int type=-1; };

struct nlmj_node_hdl;
struct kautil_json_nlohmann_prv;
struct kautil_json_nlohmann_extern{

    void *(*initialize)();
    void (*finalize)(void *hdl);
    void (*clear)(void *hdl);
    uint64_t (*erase_with_key)(void *hdl,const char * key);
    void (*erase_with_index)(void *hdl,uint64_t index);
    void * (*add_string_key)(void *hdl, const char *key, const char *value);
    void * (*add_integer_key)(void *hdl, const char *key, uint64_t const &value);
    void * (*add_int64_key)(void *hdl, const char *key, int64_t const &value);
    void * (*add_uint64_key)(void *hdl, const char *key, uint64_t const &value);
    void * (*add_float_key)(void *hdl, const char *key, double const &value);
    void * (*add_bool_key)(void *hdl, const char *key, bool const &value);
    void * (*add_object_key)(void *hdl, const char *key);
    void * (*add_array_key)(void *hdl, const char *key);

    void * (*add_string_index)(void *hdl, int const& index, const char *value);
    void * (*add_integer_index)(void *hdl, int const& index, uint64_t const &value);
    void * (*add_int64_index)(void *hdl, int const& index, int64_t const &value);
    void * (*add_uint64_index)(void *hdl, int const& index, uint64_t const &value);
    void * (*add_float_index)(void *hdl, int const& index, double const &value);
    void * (*add_bool_index)(void *hdl, int const& index, bool const &value);
    void * (*add_object_index)(void *hdl, int const& index);
    void * (*add_array_index)(void *hdl, int const& index);

    void * (*push_back_string)(void *hdl,const char *value);
    void * (*push_back_integer)(void *hdl, uint64_t const &value);
    void * (*push_back_float)(void *hdl, double const &value);
    void * (*push_back_bool)(void *hdl, bool const &value);
    void * (*push_back_array)(void *hdl);
    void * (*push_back_object)(void *hdl);

    // merge
    void (*merge_patch)(void * src_hdl,void * patch_hdl);
    void (*merge_files)(void * hdl,char * outdir,char * name,int const& indent,char ** arr_files,uint64_t const& n_arr_files);

    uint64_t (*dump_size)(void *hdl, const int& indent);
    void (*dump_to_string)(void *hdl, void *result, uint64_t const &size, const int& indent);
    void (*dump_to_file)(void *hdl, const char *file, const int &indent);
    void (*dump_to_fs)(void *hdl, const char *outdir, const char * name,int const& indent);

    void (*load_string)(void * hdl,const char * data, uint64_t const& size);
    void (*load_file)(void * hdl,const char * path_to_json);
    void (*load_from_fs)(void * hdl,const char * dir,const char * name);

    uint64_t (*get_array_length)(void *hdl);
    void * (*get_with_key)(void *hdl, const char *key);
    void * (*get_with_index)(void *hdl, int const& index);
    getvt_result (*getvt_with_key)(void *hdl, const char *key);
    getvt_result (*getvt_with_index)(void *hdl, int const& index);
    bool (*is)(void *hdl, int8_t const &t);


    const char* (*key)(void *hdl);
    const int64_t * (*iterator_value_int64)(void *hdl);
    const double * (*iterator_value_float)(void *hdl);
    const bool * (*iterator_value_bool)(void *hdl);
    const char *  (*iterator_value_string)(void *hdl);
    void * (*iterator_initialize)(void *hdl,int8_t const& order);
    void * (*iterator_to_object)(void * itr);
    void (*iterator_next)(void * itr);
    bool (*iterator_is)(void * itr,int8_t const& order );
    bool (*iterator_equal)(void * a,void * b);
    void (*iterator_finalize)(void * itr);
    

    nlmj_node_hdl* (*node_iterator)(void * obj)=0;
    bool (*node_is_array)(nlmj_node_hdl* itr)=0;
    bool (*node_is_object)(nlmj_node_hdl* itr)=0;
    bool (*node_next)(nlmj_node_hdl* itr)=0;
    void (*node_reset)(nlmj_node_hdl* itr)=0;
    const char* (*node_key)(nlmj_node_hdl* itr)=0;
    void * (*node_getv)(nlmj_node_hdl* itr)=0;
    getvt_result (*node_getvt)(nlmj_node_hdl* itr)=0;
    void (*node_release)(nlmj_node_hdl* itr)=0;
    
    kautil_json_nlohmann_prv* prv =0;
};


kautil_json_nlohmann_extern * kautil_json_nlohmann_extern_auto();
kautil_json_nlohmann_extern * kautil_json_nlohmann_extern_initialize(
        void *(*dlopen)(const char * ,int flags)
        ,void *(*dlsym)(void * ,const char *)
        ,int (*dlclose)(void*)
        ,int flags
        );
void kautil_json_nlohmann_extern_free(kautil_json_nlohmann_extern *);




#endif // KAUTIL_JSON_NLOHMANN_H
