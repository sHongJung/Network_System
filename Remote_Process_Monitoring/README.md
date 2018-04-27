프로젝트 개요 
서버 클라이언트 구조로 클라이언트에서는 자기 ID 로 실행되고 있는 프로세스의 리스트 (  PID / PPID / COMMAND / STIME ) 
를 주기적으로 서버에게 보내주고 서버의 프로세스은 그정보를 공유메모리로 관리. 

프로세스별 기능 요구 사항 
SERVERD ( 클라이언트 관리 블록 ) 
- 지정된 IP 이외의 CLIENT 의 접근을 불허 한다. ( config )
    기술 : file I/O / string 파싱 
- 정보를 받으면 Msg Queue 를 이용하여 PSMANAGER 블럭으로 정보를 전달한다. 
    기술 : Message Queue  
- 여러대의 CLIENT 가 연동 될수 있으며 SELECT 를 이용하여 정보를 수신한다. 
    기술 : Socket 통신 /  select 함수 / FD_ZERO / FD_SET 등등 의 함수  

PSMANAGER ( 최종적으로 프로세스 정보를 받는 블록 ) 
- SERVERD 로 부터 받은 정보를 공유메모리로 관리 한다. 
    기술 : 공유 메모리  
- 블럭이 재기동 되어도 공유메모리에 정보를 읽어와 정보의 유실을 방지 한다.
    기술 : 공유 메모리  
- 정보가 변경되면 변경사항을  모니터에 출력 한다. 정보의 변경 유무는 THERAD 로 따로 분리하여 주기적으로 체크 한다. 
    기술 : Main Thread로 체크 

- 특정 Signal 을 받으면 공유메모리에 있는 내용을 화면에 출력한다. 
    기술 : Signal



l 핸들 
 
- 주기적으로 공유메모리에 있는 내용을 화면에 출력한다. 
    기술 : 별도 Thread로 출력 

GETPSD ( 서버로 프로세스 정보를 보내는 클라이언트 블록 ) 
- 지정된 IP 로 접속을 시도 한다. ( config )
    기술 : file I/O / string 파싱 / Socket 통신 
- 자신의 ID 로 실행된 프로세스의 리스트를 주기적으로 서버로 전송한다. 
    기술 : Socket 통신 / 리눅스 명령어 / file I/O / string 파싱