
if(NOT DEFINED ${KAUTIL_THIRD_PARTY_DIR})
    set(KAUTIL_THIRD_PARTY_DIR ${CMAKE_BINARY_DIR})
    file(MAKE_DIRECTORY "${KAUTIL_THIRD_PARTY_DIR}")
endif()

macro(git_clone url)
    get_filename_component(file_name ${url} NAME)
    if(NOT EXISTS ${KAUTIL_THIRD_PARTY_DIR}/kautil_cmake/${file_name})
        file(DOWNLOAD ${url} "${KAUTIL_THIRD_PARTY_DIR}/kautil_cmake/${file_name}")
    endif()
    include("${KAUTIL_THIRD_PARTY_DIR}/kautil_cmake/${file_name}")
    unset(file_name)
endmacro()

git_clone(https://raw.githubusercontent.com/kautils/CMakeLibrarytemplate/v0.0.1/CMakeLibrarytemplate.cmake)



set(__t kautil_json_nlohmann_definition_${PROJECT_VERSION})
add_library(${__t} SHARED)
target_sources(${__t} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/definition.cc) 
set(__target_file_name ${CMAKE_SHARED_LIBRARY_PREFIX}${__t}${CMAKE_SHARED_LIBRARY_SUFFIX})
install(TARGETS ${__t} EXPORT ${__t} DESTINATION lib) 


set(module_name nlohmann)
get_filename_component(__include_dir "${CMAKE_CURRENT_LIST_DIR}" DIRECTORY)
unset(srcs)
file(GLOB srcs ${CMAKE_CURRENT_LIST_DIR}/extern.cc)
set(${module_name}_common_pref
    #DEBUG_VERBOSE
    MODULE_PREFIX kautil json
    MODULE_NAME ${module_name}
    INCLUDES $<BUILD_INTERFACE:${__nlohamnn_dir}> $<BUILD_INTERFACE:${__include_dir}>  $<INSTALL_INTERFACE:include> 
    SOURCES ${srcs}
    EXPORT_NAME_PREFIX ${PROJECT_NAME}
    EXPORT_VERSION ${PROJECT_VERSION}
    EXPORT_VERSION_COMPATIBILITY AnyNewerVersion

    DESTINATION_INCLUDE_DIR include/kautil/json
    DESTINATION_CMAKE_DIR cmake
    DESTINATION_LIB_DIR lib
)


CMakeLibraryTemplate(${module_name} EXPORT_LIB_TYPE static ${${module_name}_common_pref} )
get_filename_component(output_file $<TARGET_FILE:${__json_definition_target}> NAME)
target_compile_definitions(${${module_name}_static} INTERFACE KAUTIL_JSON_NLOHMANN_SO= $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/lib/${__target_file_name}>)
target_compile_definitions(${${module_name}_static} PRIVATE KAUTIL_JSON_NLOHMANN_SO="$<TARGET_FILE:kautil_json_nlohmann_definition_${PROJECT_VERSION}>")


set(__t ${${module_name}_static_tmain})
add_executable(${__t})
target_sources(${__t} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/unit_test.cc)
target_link_libraries(${__t} PRIVATE ${${module_name}_static})
target_compile_definitions(${__t} PRIVATE ${${module_name}_static_tmain_ppcs})
