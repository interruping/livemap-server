cmake_minimum_required (VERSION 3.0)

#프로젝트 이름 및 버전
project ( livemap_server )

set ( PROJECT_VERSION_MAJOR 0 )
set ( PROJECT_VERSION_MINOR 0 )
set ( PROJECT_VERSION_PATCH 0 )
set ( PROJECT_VERSION_TWEAK 0 )

configure_file( ${CMAKE_SOURCE_DIR}/livemapserver_config.h.in ${CMAKE_SOURCE_DIR}/src/header/livemapserver_config.h )

add_subdirectory( src ) 

