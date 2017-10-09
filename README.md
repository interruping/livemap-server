
LiveMapServer
=======

LiveMapServer는 실시간 위치 공유 처리 미들웨어입니다. 클라이언트 API를
사용하여 여러 단말과 통신합니다. 여러 단말은 LiveMapServer를 매개로 서로
의 위치를 확인할 수 있을 뿐만 아니라 단말간 메시지도 전송가능합니다. 공유라
이브러리의 런타임 링크를 통해 LiveMapServer의 사용자는 자신이 원하는 서
비스를 위치정보와 연동할 수 있습니다.


기능
========

LiveMapServer는 SSL 소켓으로 클라이언트 API와 암호화된 통신을 합니다.

LiveMapServer는 별도의 DB없이 메모리에서 클라이언트 단말기간의 위치를 계산합니다.

LiveMapServer는 클라이언트 통신부분과 데이터 처리부분의 로직을 분리하였습니다. 데이터 처리
부분의 로직은 공유라이브러리 형태로 모듈화 되어있고 LiveMapServer의 런타임에 링크됩니다.
또한 이 서비스로직이 구현된 공유라이브러리는 런타임에 교체 다른 서비스 라이브러리로 교체가
가능합니다.


LiveMapServer 빌드
======================

LiveMapServer는 C++11 & C로 작성되었습니다. LiveMapServer는 CMake 빌드툴로 빌드하도록 설정하였습니다.
LiveMapServer는 'libssl-dev' 라이브러리와  'libboost-all-dev' 라이브러리에 종속되어있습니다. 따라서 openssl
라이브러리와 boost 라이브러리를 빌드 전에 시스템에 설치하여야 합니다.

LiveMapServer는 빌드 시 구동에 필요한 세팅 값을 livemapserver_config.h.in 파일에서 읽어옵니다. 따라서 세팅값에
변경이 필요할시에 다시 빌드하여야 합니다.

LiveMapServer 프로젝트는 release용 빌드와 debug용 빌드를 할 수 있습니다. debug용 빌드는 컴파일러 최적화 옵션이
빠져있고 소스 레벨 디버깅을 위한 옵션이 추가되어 있습니다.

release용으로 빌드하기 (아래)

    $ cmake . -DCMAKE_BUILD_TYPE=release
    $ make

debug용으로 빌드하기(아래)

    $ cmake . -DCMAKE_BUILD_TYPE=debug
    $ make

빌드에 관한 더 자세한 정보는
[BUILD.md](https://github.com/interruping/livemap-server/blob/Develop/BUILD.md) 파일을 참조.

LiveMapServer 구동 및 테스트
======================

## SSL 인증서 준비

LiveMapServer를 빌드 후 SSL 통신을 위한 인증서와 키를 준비해야 합니다. LiveMapServer는 별도의 공인된 SSL 인증서를
제공하지 않습니다. 사용자가 직접 LiveMapServer를 구동할 도메인에 맞는 공인된 인증서를 구해야 합니다.

LiveMapServer 구동에 필요한 SSL 인증서와 키 파일들은 아래와 같습니다.

 - ROOT CA 인증서
 - 개인키 파일
 - DH 파라미터 파일
 
인증서를 준비하였으면 LiveMapServer 설정파일 [livemapserver_config.h.in](https://github.com/interruping/livemap-server/blob/Develop/livemapserver_config.h.in)
에 해당 파일들의 경로를 설정해줍니다. 인증서의 경로는 LiveMapServer의 실행 바이너리 경로 기준 상대경로 혹은 절대경로를
입력하면 됩니다.

### 인증서 경로 입력 예시 ( file : livemapserver_config.h.in )
    #define LMS_CFG_SSL_SOCKET_SERVER_CERTIFICATE_CHAIN_FILE "../../server.crt"    //상대경로
    #define LMS_CFG_SSL_SOCKET_SERVER_PRIVATE_KEY_FILE "/home/root/tmp/server.key" //절대경로
    #define LMS_CFG_SSL_SOCKET_SERVER_TMP_DH_PARAM_FILE "dh2048.pem".              //상대경로

### 테스트를 위한 Self-Signed SSL 인증서 & 키파일 생성

LiveMapServer 프로젝트는 테스트를 위한 Self-Signed 인증서와 필요한 키파일을 자동으로 생성해주는 쉡스크립트를 제공합니다.
스크립트 파일 [self-signed-cert-generator.sh](https://github.com/interruping/livemap-server/blob/Develop/self-signed-cert-generator.sh)

####  [self-signed-cert-generator.sh](https://github.com/interruping/livemap-server/blob/Develop/self-signed-cert-generator.sh)스크립트 실행

    $ bash self-signed-cert-generator.sh
    Generate private key...
    Generating RSA private key, 1024 bit long modulus
    .....++++++
    ................++++++
    e is 65537 (0x10001)
    Enter pass phrase for server.key: [개인키 보호 비밀번호 입력]
    Verifying - Enter pass phrase for server.key: [앞서 입력한 비밀번호와 동일하게 입력]
    Generate certificate signing request...
    Enter pass phrase for server.key: [한 번 더 같은 비밀번호 입력]
    You are about to be asked to enter information that will be incorporated
    into your certificate request.
    What you are about to enter is what is called a Distinguished Name or a DN.
    There are quite a few fields but you can leave some blank
    For some fields there will be a default value,
    If you enter '.', the field will be left blank.
    -----
    Country Name (2 letter code) [AU]: [국가 코드 입력]
    State or Province Name (full name) [Some-State]: [지역 입력]
    Locality Name (eg, city) []: [시군구 입력]
    Organization Name (eg, company) [Internet Widgits Pty Ltd]: [단체명 입력]
    Organizational Unit Name (eg, section) []: [부서명 입력]
    Common Name (e.g. server FQDN or YOUR name) []: [서비스 도메인 명]
    Email Address []: [대표 이메일 입력]
    
    Please enter the following 'extra' attributes
    to be sent with your certificate request
    A challenge password []: (생략 그냥 Enter)
    An optional company name []: (생략 그냥 Enter)
    Sign certificate with private key...
    Signature ok
    subject=/C=[국가 코드]/ST=[지역]/L=[시군구]/O=[단체명]/OU=[부서명]/CN=[서비스 도메인]/emailAddress=[대표 이메일]
    Getting Private key
    Enter pass phrase for server.key: [처음에 입력했던 개인키 보호 비밀번호 다시 입력]
    Generate dhparam file...
    Generating DH parameters, 2048 bit long safe prime, generator 2
    This is going to take a long time
    ....................................................................
    ....................................................................
    ................................+...................................
    .....................................+...................
    Move all file.
    Done...
    
####  [self-signed-cert-generator.sh](https://github.com/interruping/livemap-server/blob/Develop/self-signed-cert-generator.sh) 실행 완료 후 결과물

- build/debug/server.crt
- testclient/debug/server.crt ( 위 파일과 동일 ; 테스트 클라이언트를 위한 인증서  )
- build/debug/server.key
- build/debug/dh2048.pem

*! 주의사항 !*
[self-signed-cert-generator.sh](https://github.com/interruping/livemap-server/blob/Develop/self-signed-cert-generator.sh) 을 실행하여 생성된 인증서와 키파일들의 경로는
 [livemapserver_config.h.in](https://github.com/interruping/livemap-server/blob/Develop/livemapserver_config.h.in) 파일에 디폴트 값으로 설정되어 있습니다.
 따라서  [livemapserver_config.h.in](https://github.com/interruping/livemap-server/blob/Develop/livemapserver_config.h.in) 파일에 별도의 설정을 하면 안됩니다.
 
 ## 구동 및 테스트
 
     $ cd build/debug
     $ ./server & (백그라운드 실행)
     $ cd ../../testclient
     $ cmake .
     $ make
     $ cd build/debug
     $ ./testclient localhost 1212 (테스트 클라이언트 실행)
     Verifying [인증서 정보]
     Start test.
     Test request_user_info service Ok.
     Test user_update_node service Ok.
     utf8_message_send service Ok.
     End Test.
     
 위와 같이 테스트 클라이언트를 실행했을 때 3개의 Ok가 나오면 테스트 성공입니다.
 Ok표시가 3개보다 적거나 다른 오류 메시지가 표시될 때, LiveMapServer를 재빌드하거나
 libcommandbinder 동적라이브러리 빌드 여부를 확인해보십시오.
 
 LiveMapClient API를 활용한 LiveMapServer 사용
 ======================
 
 LiveMapServer 프로젝트는 iOS와 Android 플랫폼에서 어플리케이션과 연동 가능한 API를 제공합니다.
 iOS, Android 플랫폼 각각 별도의 repository를 가지고 있으면 LiveMapServer 프로젝트의 서브모듈로 등록되어있습니다.
 자세한 사용법은 해당 repository의 README 혹은 API 문서를 확인하십시오.
 
 - [LiveMapClient iOS API Repository]()
 - [LiveMapClient Android API Repository]()
제한사항
===========
- *Linux & Unix 계열의 OS*를 대상으로 작성되었습니다.
- *CMake 3.0*이상을 필요로 합니다.
- *Boost 라이브러리 1.58* 이상을 필요로 합니다.
- *OpenSSL*라이브러리를 필요로 합니다.
- SSL 소켓 통신에 필요한 서명된 인증서와 개인키가 필요합니다.


라이센스
=======
이 프로젝트는 Apache License 2.0 라이센스를 따릅니다. 자세한 사항은 [LICENSE.txt](https://github.com/interruping/livemap-server/blob/Develop/LICENSE)파일을 참조하세요.
