file(GLOB HEADER_LIST CONFIGURE_DEPENDS ${meh_SOURCE_DIR}/include/meh/*.hpp)

add_library(shims shims.cxx ${HEADER_LIST})
add_library(logic logic.cxx ${HEADER_LIST})

# target_include_directories(shims PUBLIC ../include)
# target_include_directories(logic PUBLIC ../include)

target_link_libraries(shims PRIVATE pmp)
target_link_libraries(logic PRIVATE pmp shims)
