# 2025/01/05 23:12:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
project (Earth2151)


#set(CMAKE_UNITY_BUILD OFF)


include_directories(
    .
    ../PiEngine/Engine
    ../PiEngine/TerathonCode
    ../PiEngine/SlugCode
    ../ThirdParty/angelscript/angelscript/include
    ../ThirdParty/angelscript/add_on
    ../ThirdParty/zlib
)


add_definitions(-DTERATHON_IMPORT)


if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /STACK:1048576000")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall")

    link_directories(
        ../ThirdParty/angelscript/angelscript/lib
    )

    add_compile_options(
#        /wd4623

        # zlib
        /wd4061
        /wd4265
        /wd5262
        /wd5264
    )

    add_definitions(
        -D_NOT_OVERLOAD_OPERATOR_NEW
#        -D_CORECRT_BUILD        # Без этого ошибка error C2485: '__restrict': unrecognized extended attribute
    )

    enable_language (ASM_MASM)

else()

    add_definitions(
        -Wno-cast-function-type
    )

endif()

file(GLOB RESOURCES                *.ico *.rc)
file(GLOB SRC                      *.h *.cpp)
file(GLOB ANGELSCRIPT              AngelScript/*.*)
file(GLOB ANGELSCRIPT_AS           ../ThirdParty/angelscript/angelscript/source/as_scriptengine.cpp
                                   ../ThirdParty/angelscript/angelscript/source/as_context.cpp
                                   ../ThirdParty/angelscript/angelscript/source/as_thread.cpp
                                   ../ThirdParty/angelscript/angelscript/source/as_memory.cpp
                                   ../ThirdParty/angelscript/angelscript/source/as_builder.cpp
                                   ../ThirdParty/angelscript/angelscript/source/as_string.cpp
                                   ../ThirdParty/angelscript/angelscript/source/as_datatype.cpp
                                   ../ThirdParty/angelscript/angelscript/source/as_atomic.cpp
                                   ../ThirdParty/angelscript/angelscript/source/as_objecttype.cpp
                                   ../ThirdParty/angelscript/angelscript/source/as_scriptcode.cpp
                                   ../ThirdParty/angelscript/angelscript/source/as_scriptfunction.cpp
                                   ../ThirdParty/angelscript/angelscript/source/as_typeinfo.cpp
                                   ../ThirdParty/angelscript/angelscript/source/as_globalproperty.cpp
                                   ../ThirdParty/angelscript/angelscript/source/as_tokenizer.cpp
                                   ../ThirdParty/angelscript/angelscript/source/as_string_util.cpp
                                   ../ThirdParty/angelscript/angelscript/source/as_module.cpp
                                   ../ThirdParty/angelscript/angelscript/source/as_scriptobject.cpp
                                   ../ThirdParty/angelscript/angelscript/source/as_scriptnode.cpp
                                   ../ThirdParty/angelscript/angelscript/source/as_outputbuffer.cpp
                                   ../ThirdParty/angelscript/angelscript/source/as_configgroup.cpp
                                   ../ThirdParty/angelscript/angelscript/source/as_callfunc.cpp
                                   ../ThirdParty/angelscript/angelscript/source/as_gc.cpp
                                   ../ThirdParty/angelscript/angelscript/source/as_generic.cpp
                                   ../ThirdParty/angelscript/angelscript/source/as_restore.cpp
                                   ../ThirdParty/angelscript/angelscript/source/as_compiler.cpp
                                   ../ThirdParty/angelscript/angelscript/source/as_bytecode.cpp
                                   ../ThirdParty/angelscript/angelscript/source/as_parser.cpp
                                   ../ThirdParty/angelscript/angelscript/source/as_variablescope.cpp
if(MSVC)
                                   ../ThirdParty/angelscript/angelscript/source/as_callfunc_x64_msvc.cpp
                                   ../ThirdParty/angelscript/angelscript/source/as_callfunc_x64_msvc_asm.asm
else()
                                   ../ThirdParty/angelscript/angelscript/source/as_callfunc_x64_gcc.cpp
endif()
                                   ../ThirdParty/angelscript/add_on/scriptstdstring/scriptstdstring.*
                                   ../ThirdParty/angelscript/add_on/scripthandle/scripthandle.*
                                   ../ThirdParty/angelscript/add_on/weakref/weakref.*)
file(GLOB EARTH2150                             Earth2150/*.*)
file(GLOB EARTH2150_FILES                       Earth2150/Files/*.*)
file(GLOB EARTH2150_PACKER                      Earth2150/Packer/*.*)
file(GLOB EARTH2150_PARAMETERS                  Earth2150/Parameters/*.*)
file(GLOB EARTH2150_PARAMETERS_MODELS           Earth2150/Parameters/Models/*.*)
file(GLOB EARTH2150_PARAMETERS_MODELS_ABSTRACTS Earth2150/Parameters/Models/Abstracts/*.*)
file(GLOB EDITOR                                Editor/*.*)
file(GLOB GRAPHICS                              Graphics/*.*)
file(GLOB INPUT                                 Input/*.*)
file(GLOB INTERFACE                             Interface/*.*)
file(GLOB INTERFACE_CONTROLS                    Interface/Controls/*.*)
file(GLOB INTERFACE_MENU                        Interface/Menu/*.*)
file(GLOB INTERFACE_MUTATORS                    Interface/Mutators/*.*)
file(GLOB INTERFACE_PANELS                      Interface/Panels/*.*)
file(GLOB INTERFACE_WIDGETS                     Interface/Widgets/*.*)
file(GLOB OBJECTS                               Objects/*.*)
file(GLOB OBJECTS_AMMOS                         Objects/Ammos/*.*)
file(GLOB OBJECTS_STRUCTURES                    Objects/Structures/*.*)
file(GLOB OBJECTS_UNITS                         Objects/Units/*.*)
file(GLOB OBJECTS_WORLD                         Objects/World/*.*)
file(GLOB SOUNDS                                Sounds/*.*)
file(GLOB UTILS                                 Utils/*.*)
file(GLOB THIRDPARTY_ZLIB        ../ThirdParty/zlib/uncompr.cpp
                                 ../ThirdParty/zlib/inflate.cpp
                                 ../ThirdParty/zlib/adler32.cpp
                                 ../ThirdParty/zlib/crc32.cpp
                                 ../ThirdParty/zlib/zutil.cpp
                                 ../ThirdParty/zlib/inftrees.cpp
                                 ../ThirdParty/zlib/inffast.cpp)


source_group(Resources                              FILES ${RESOURCES})
source_group(src                                    FILES ${SRC})
source_group(AngelScript                            FILES ${ANGELSCRIPT})
source_group(AngelScript/AS                         FILES ${ANGELSCRIPT_AS})
source_group(Earth2150                              FILES ${EARTH2150})
source_group(Earth2150/Files                        FILES ${EARTH2150_FILES})
source_group(Earth2150/Packer                       FILES ${EARTH2150_PACKER})
source_group(Earth2150/Parameters                   FILES ${EARTH2150_PARAMETERS})
source_group(Earth2150/Parameters/Models            FILES ${EARTH2150_PARAMETERS_MODELS})
source_group(Earth2150/Parameters/Models/Abstracts  FILES ${EARTH2150_PARAMETERS_MODELS_ABSTRACTS})
source_group(Editor                                 FILES ${EDITOR})
source_group(Graphics                               FILES ${GRAPHICS})
source_group(Input                                  FILES ${INPUT})
source_group(Interface                              FILES ${INTERFACE})
source_group(Interface/Controls                     FILES ${INTERFACE_CONTROLS})
source_group(Interface/Menu                         FILES ${INTERFACE_MENU})
source_group(Interface/Mutators                     FILES ${INTERFACE_MUTATORS})
source_group(Interface/Panels                       FILES ${INTERFACE_PANELS})
source_group(Interface/Widgets                      FILES ${INTERFACE_WIDGETS})
source_group(Objects                                FILES ${OBJECTS})
source_group(Objects/Ammos                          FILES ${OBJECTS_AMMOS})
source_group(Objects/Structures                     FILES ${OBJECTS_STRUCTURES})
source_group(Objects/Units                          FILES ${OBJECTS_UNITS})
source_group(Objects/World                          FILES ${OBJECTS_WORLD})
source_group(Sounds                                 FILES ${SOUNDS})
source_group(Utils                                  FILES ${UTILS})
source_group(ThirdPary/zlib                         FILES ${THIRDPARTY_ZLIB})

set_source_files_properties(
    ${THIRDPARTY_ZLIB}
    PROPERTIES SKIP_UNITY_BUILD_INCLUSION ON)

add_executable(
    ${PROJECT_NAME}
    ${RESOURCES}
    ${SRC}
    ${ANGELSCRIPT}
    ${ANGELSCRIPT_AS}
    ${EARTH2150}
    ${EARTH2150_FILES}
    ${EARTH2150_PACKER}
    ${EARTH2150_PARAMETERS}
    ${EARTH2150_PARAMETERS_MODELS}
    ${EARTH2150_PARAMETERS_MODELS_ABSTRACTS}
    ${EDITOR}
    ${GRAPHICS}
    ${INPUT}
    ${INTERFACE}
    ${INTERFACE_CONTROLS}
    ${INTERFACE_MENU}
    ${INTERFACE_MUTATORS}
    ${INTERFACE_PANELS}
    ${INTERFACE_WIDGETS}
    ${OBJECTS}
    ${OBJECTS_AMMOS}
    ${OBJECTS_STRUCTURES}
    ${OBJECTS_UNITS}
    ${OBJECTS_WORLD}
    ${SOUNDS}
    ${UTILS}
    ${THIRDPARTY_ZLIB}
)

add_dependencies(${PROJECT_NAME} PiEngine WorldEditor)

link_runtime()

target_link_libraries(${PROJECT_NAME} PiEngine)

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    if(MSVC)
        target_link_libraries(${PROJECT_NAME} odbc32 odbccp32)
    endif()
endif()

set(_INSTALL_DIR_ ${_ROOT_DIR_}\\${PROJECT_NAME}\\out\\${BUILD_TYPE})

copy_to_dir_exe("$(OutputPath)${PROJECT_NAME}" ${_INSTALL_DIR_})

message("INSTALL_DIR:         ${_INSTALL_DIR_}")
message("_ROOT_DIR_:          ${_ROOT_DIR_}")
message("TARGET_NAME:         ${PROJECT_NAME}")

set_work_directory()
