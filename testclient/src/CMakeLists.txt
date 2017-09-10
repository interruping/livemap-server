#always debug build

set ( CMAKE_BUILD_TYPE debug )

#OpenSSL Setting

if ( APPLE )
    set (OPENSSL_ROOT_DIR /usr/local/opt/openssl )
    set (OPENSSL_INCLUDE_DIR /usr/local/opt/openssl/include )
    set (OPENSSL_LIBRARY_DIR /usr/local/opt/openssl/lib )
    set (OPENSSL_LIBRARIES "")
    link_libraries( ssl crypto )
endif( APPLE )

find_package (openssl REQUIRED)
find_package (OpenSSL REQUIRED)

if ( OpenSSL_FOUND )

    include_directories ( ${OPENSSL_INCLUDE_DIR} )
    link_directories( ${OPENSSL_LIBRARY_DIR} )


else ()
    #Error;with REQUIRED, pkg_search_module() will throw an error by it's own
endif (OpenSSL_FOUND)


# 공통 헤더 파일 Include 디렉토리 (-I)
include_directories ( header )

# 빌드 대상 바이너리 파일명 및 소스 파일 목록
set ( OUTPUT_ELF
        "test_client"
        )
file ( GLOB LIVEMAP_TEST_CLIENT_CXX_SOURCES *.cpp )
 
# 공통 컴파일 옵션, 링크 옵션
add_compile_options ( -W -Wall -std=c++14 )
#set ( CMAKE_EXE_LINKER_FLAGS "<링크_옵션> <링크_옵션> ..." )
 
# 공통 링크 라이브러리 (-l)
#link_libraries ( dl )

#Boost Library Setting

find_package ( Boost 1.58 COMPONENTS system thread REQUIRED )
include_directories( ${Boost_INCLUDE_DIR} )

# "debug" 형상 한정 컴파일 옵션, 링크 옵션
set ( CMAKE_CXX_FLAGS_DEBUG "-o0 -g -D_DEBUG_" )
set ( CMAKE_EXE_LINKER_FLAGS_DEBUG "-g" )
 
    
# 출력 디렉토리
set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${livemap_server_test_client_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/ )

# 빌드 대상 바이너리 추가
add_executable( ${OUTPUT_ELF} ${LIVEMAP_TEST_CLIENT_CXX_SOURCES} )
target_link_libraries ( ${OUTPUT_ELF} LINK_PUBLIC ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} )
