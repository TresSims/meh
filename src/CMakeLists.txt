file(GLOB HEADER_LIST CONFIGURE_DEPENDS ${meh_SOURCE_DIR}/include/meh/*.hpp)

include_directories(${PROJECT_SOURCE_DIR}/extern/lib3mf/Bindings)

if (${BUILD_WIN})
        message("Setting build settings for windows, using windows lib")
        link_directories(${meh_SOURCE_DIR}/extern/lib3mf/Lib)
        find_library(3mf lib3mf )
else()
        message("Setting build settings fow linux, using linux lib")
        find_library(LIB3MF 3mf ${meh_SOURCE_DIR}/extern/lib3mf/Bin)
endif()

add_library(shims shims.cxx ${HEADER_LIST})
target_link_libraries(shims PRIVATE 3mf pmp)

add_library(logic logic.cxx ${HEADER_LIST})
target_link_libraries(logic PRIVATE pmp shims)
