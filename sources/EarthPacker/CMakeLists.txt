# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by

cmake_minimum_required(VERSION 3.14)

project (EarthPacker)

set(CMAKE_UNITY_BUILD ON)

include_directories(.
                    ../ThirdParty/wxWidgets/include
                    ../ThirdParty
                    ../generated/wxWidgets/lib/vc_dll/mswud)

link_directories(../generated/wxWidgets/lib/vc_dll)

add_definitions(-DWXUSINGDLL -DGUI)

if(WIN32)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall /std:c++20")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
    
    # Older cmake versions don't support string(APPEND ...) provide a workaround
    macro(wx_string_append var str)
        set(${var} ${${var}}${str})
    endmacro()
    
    if(BUILD_USE_STATIC_RUNTIME)
        set(MSVC_LIB_USE "/MT")
        set(MSVC_LIB_REPLACE "/MD")
        
        # Set MSVC runtime flags for all configurations
        foreach(cfg "" ${CMAKE_CONFIGURATION_TYPES})
            set(c_flag_var CMAKE_C_FLAGS)
            set(cxx_flag_var CMAKE_CXX_FLAGS)
            if(cfg)
                string(TOUPPER ${cfg} cfg_upper)
                wx_string_append(c_flag_var "_${cfg_upper}")
                wx_string_append(cxx_flag_var "_${cfg_upper}")
            endif()
            if(${c_flag_var} MATCHES ${MSVC_LIB_REPLACE})
                string(REPLACE ${MSVC_LIB_REPLACE} ${MSVC_LIB_USE} ${c_flag_var} "${${c_flag_var}}")
                set(${c_flag_var} ${${c_flag_var}} CACHE STRING
                "Flags used by the C compiler during ${cfg_upper} builds." FORCE)
            endif()
            if(${cxx_flag_var} MATCHES ${MSVC_LIB_REPLACE})
                string(REPLACE ${MSVC_LIB_REPLACE} ${MSVC_LIB_USE} ${cxx_flag_var} "${${cxx_flag_var}}")
                set(${cxx_flag_var} ${${cxx_flag_var}} CACHE STRING
                "Flags used by the CXX compiler durin ${cfg_upper} builds." FORCE)
            endif()
        endforeach()
    endif()

    add_compile_options(/wd4355)    # 'this': used in base member initializer list
    add_compile_options(/wd4365)    # 'action' : conversion from 'type_1' to 'type_2', signed/unsigned mismatch
    add_compile_options(/wd4464)
    add_compile_options(/wd4514)    # 'function' : unreferenced inline function has been removed
    add_compile_options(/wd4625)    # 'derived class' : copy constructor was implicitly defined as deleted because a base class copy constructor is inaccessible or deleted
    add_compile_options(/wd4626)    # 'derived class' : assignment operator was implicitly defined as deleted because a base class assignment operator is inaccessible or deleted
    add_compile_options(/wd4820)    # 'bytes' bytes padding added after construct 'member_name'
    add_compile_options(/wd4996)    # Your code uses a function, class member, variable, or typedef that's marked deprecated
    add_compile_options(/wd5026)    # move constructor was implicitly defined as deleted
    add_compile_options(/wd5027)    # move assignment operator was implicitly defined as deleted
    add_compile_options(/wd5039)    # 'xxx': pointer or reference to potentially throwing function passed to extern C
                                    # function under -EHc.
    add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
endif()

file(GLOB SRC                       *.h *.cpp)
file(GLOB CONTROLS                  Controls/*.*)
file(GLOB CONTROLS_NB               Controls/Notebook/*.*)
file(GLOB CONTROLS_NB_LEFT          Controls/Notebook/NotebookLeft/*.*)
file(GLOB CONTROLS_NB_RIGHT         Controls/Notebook/NotebookRight/*.*)
file(GLOB MODELS                    Models/*.*)
file(GLOB MODELS_COLLECTIONS        Models/Collections/*.*)
file(GLOB MODELS_ELEMENTS           Models/Elements/*.*)
file(GLOB RESOURCES                 Resources/*.*)
file(GLOB TEXTURES                  Textures/*.*)
file(GLOB UTILS                     Utils/*.*)

source_group(src                             FILES ${SRC})
source_group(Controls                        FILES ${CONTROLS})
source_group(Controls/Notebook               FILES ${CONTROLS_NB})
source_group(Controls/Notebook/NotebookLeft  FILES ${CONTROLS_NB_LEFT})
source_group(Controls/Notebook/NotebookRight FILES ${CONTROLS_NB_RIGHT})
source_group(Models                          FILES ${MODELS})
source_group(Models/Collections              FILES ${MODELS_COLLECTIONS})
source_group(Models/Elements                 FILES ${MODELS_ELEMENTS})
source_group(Resources                       FILES ${RESOURCES})
source_group(Textures                        FILES ${TEXTURES})
source_group(Utils                           FILES ${UTILS})

add_executable( ${PROJECT_NAME}
                ${SRC}
                ${CONTROLS}
                ${CONTROLS_NB}
                ${CONTROLS_NB_LEFT}
                ${CONTROLS_NB_RIGHT}
                ${MODELS}
                ${MODELS_COLLECTIONS}
                ${MODELS_ELEMENTS}
                ${RESOURCES}
                ${TEXTURES}
                ${UTILS}
                )

target_link_libraries(${PROJECT_NAME} wxbase32ud wxmsw32ud_core Ws2_32)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND copy ..\\wxWidgets\\lib\\vc_dll\\wxbase32ud_vc_custom.dll $(OutputPath) /Y)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND copy ..\\wxWidgets\\lib\\vc_dll\\wxmsw32ud_core_vc_custom.dll $(OutputPath) /Y)
