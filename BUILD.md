
LiveMapServer 빌드하기
=======

LiveMapServer은  Unix-like OS에서 빌드할 수 있습니다. 빌드툴은 CMake 버전 3.0 이상이 필요합니다.
또한 시스템에 따라 필요한 요구 사항이 상이할 수 있습니다.

BUILD.md에 명시된 방법으로 빌드를 실패하거나 오류가 발생한다면
[LiveMapServer issue tracker](https://github.com/interruping/livemap-server/issues) 에서 검색
또는 새로운 이슈를 등록하십시오.

## 컴파일러 별 요구사항

- gcc 5.2 이상
- Apple LLVM Compiler Version 6.1 이상
- Clang 3.4 이상

## 빌드 하기

### OpenSSL 라이브러리 설치

### MacOS

#### brew 설치 후 설치

    $ /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
    $ brew update
    $ brew upgrade
    $ brew install openssl
    
### Linux & Unix

#### yum

    $ yum install openssl-devel
    
#### apt-get

    $ apt-get install
    
## Boost 라이브러리 설치

### MacOS

### brew

    $ brew install boost

### Linux & Unix

#### yum

    $ yum install boost-devel

#### apt-get

    $ sudo apt-get install libboost-all-dev
    
## CMake 설치

### MacOS

#### brew

    $ brew install cmake

### Linux & Unix

#### yum

    $ yum install cmake
    

#### apt-get

    $ sudo apt-get install cmake

## LiveMapServer repository 복제

    $ git clonehttps://github.com/interruping/livemap-server.git
    $ cd livemap-server
    
## Makefile 생성

### 디버그 용

    $ cmake . -DCMAKE_BUILD_TYPE=debug (선택사항) -G[Xcode|"Eclipse CDT4 - Unix Makefile"]

### 릴리스 용

    $ cmake . -DCMAKE_BUIL_TYPE=release
    
## 빌드 결과물 확인

### 디버그 용

    $ cd ./build/debug/
    $ ls server
    server
    $ cd ./dylib
    $ ls libcommandbinder.*
    libcommandbinder.so 또는 libcommandbinder.dylib
    
### 릴리즈 용

    $ cd ./build/release/
    $ls server
    server
    $ cd ./dylib
    $ ls libcommandbinder.*
    libcommandbinder.so 또는 libcommandbinder.dylib
    
## 테스트용 클라이언트 빌드 & 테스트

    


    
