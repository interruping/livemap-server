
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

제한사항
===========
- *Linux & Unix 계열의 OS*를 대상으로 작성되었습니다.
- *CMake 3.0*이상을 필요로 합니다.
- *Boost 라이브러리 1.58* 이상을 필요로 합니다.
-*OpenSSL*라이브러리를 필요로 합니다.
- SSL 소켓 통신에 필요한 서명된 인증서와 개인키가 필요합니다.


라이센스
=======
이 프로젝트는 Apache License 2.0 라이센스를 따릅니다. 자세한 사항은 [LICENSE.txt](https://github.com/interruping/livemap-server/blob/Develop/LICENSE)파일을 참조하세요.
