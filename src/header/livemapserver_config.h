#ifndef LMS_CFG_HEADER
#define LMS_CFG_HEADER

/*
 워커스레드 갯수 설정
 디폴트 설정 값 1
 */
#define LMS_CFG_WORKER_THREAD_NUM 1

/*
 클리이언트와 통신할 때 소켓의 읽고 쓰는 작업간 시간 간격
 디폴트 설정 값 10
 */
#define LMS_CFG_RW_IO_INTERVAL_MILISECOND 10

#define LMS_CFG_BOOST_IMPL 0
#define LMS_CFG_POSIX_IMPL 1 //미지원

/*
 서버 구현 설정
 디폴트 설정 값 LMS_CFG_BOOST_IMPL
*/
#define LMS_CFG_SERVER_IMPL LMS_CFG_BOOST_IMPL

#define LMS_CFG_SSL_SOCKET 0
#define LMS_CFG_TCP_SOCKET 1 //미지원 (지원 예정)
#define LMS_CFG_UDP_SOCKET 2 //미지원 (지원 예정)

/*
 서버 타입 설정
 디폴트 설정 값 "ssl-socket"
 */
#define LMS_CFG_SERVER_TYPE LMS_CFG_SSL_SOCKET

/*
 ssl socket 서버
 인증서, 개인키 파일 path
 설정 경로는 server 실행파일이 있는 폴더 기준
 디폴트 설정 값  self signed cert path.
 */
#if LMS_SERVER_TYPE == LMS_CFG_SSL_SOCKET

#define LMS_CFG_SSL_SOCKET_SERVER_CERTIFICATE_CHAIN_FILE "server.crt"
#define LMS_CFG_SSL_SOCKET_SERVER_PRIVATE_KEY_FILE "server.key"
#define LMS_CFG_SSL_SOCKET_SERVER_TMP_DH_PARAM_FILE "dh2048.pem"

#endif // end if LMS_SERVER_TYPE

/*
 클라이언트 노드 간 식별할 수 있는 최대거리 (단위: km)
 디폴트 설정 값 1
*/
#define LMS_CFG_DETECTABLE_DISTANCE_KILOMETER 1

#endif // end if LMS_CFG_HEADER
