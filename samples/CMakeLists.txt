cmake_minimum_required(VERSION 3.10)

file(GLOB SUBDIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *)
foreach(SUBDIR ${SUBDIRS})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR})
        file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR}/*.c)
        foreach(SRC ${SOURCES})
            get_filename_component(BASE_NAME ${SRC} NAME_WE)
            # 使用源文件名作为目标名（如 pair_ser、pair_cli）
            set(TARGET_NAME "${BASE_NAME}")
            add_executable(${TARGET_NAME} ${SRC})
            target_link_libraries(${TARGET_NAME} PRIVATE nng)
            # 设置输出目录：build/samples/子目录名/
            set_target_properties(${TARGET_NAME} PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/samples/${SUBDIR}"
            )
            message(STATUS "Added sample: ${TARGET_NAME} -> samples/${SUBDIR}/")
        endforeach()
    endif()
endforeach()

