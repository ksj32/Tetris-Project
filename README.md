# Tetris Project
C언어를 이용하여 만든 테트리스 게임.

![맵생성 및 블럭 낙하 화면](https://github.com/user-attachments/assets/8484111c-5b79-429f-a99e-c727d0bcb379){: width="100" height="100"} 

## 프로젝트 설명

한컴MDS 아카데미에서 진행한 ARM 디바이스 펌웨어 팀프로젝트.

## 게임 설명

- 게임 요약 : LCD 화면상에서 무작위로 떨어지는 노트들을 맞추는 리듬게임

- 게임 장르 :  음악 게임

- 게임 조작 : 터치 버튼 4개

- 게임 목표 : 터치 버튼을 이용하여 무작위로 떨어지는 노트들을 맞춰 최대한 높은 점수를 얻기

## 구현 내용

### 프로젝트 완성도

- 필수 구현 요소  <br>
[배경화면, 시작화면, 2가지 노트, 소리, 낙하효과, 시간 및 점수 출력] -> 100% 구현 완료.


### 프로젝트 분담 역할

- 김상준 : Touch(점수 증가, Buzzer 울림), Timer(노트 생성 및 낙하, 노트 증가 및 게임 종료)
- 홍성현 : Key(게임 시작 및 초기화), LCD(초기화면, 인게임화면, 경과 시간, 상태 ,점수 등), Buzzer(소리 출력)

## 실행 방법

- Windows 환경에서 소스코드 작성(호스트 개발환경)
- Ubuntu Linux 실행
- 터미널에서 디렉토리 생성
- Windows 환경에서 개발한 소스코드 파일들을 Ubuntu Linux 내 디렉토리에 복사
- 명령어 make clean 입력 후 make 입력하여 컴파일
- TeraTerm 실행 후 S3C2450(타겟보드)에서 정상적으로 구현되는지 확인

## 개발 환경

DEV-C++, Ubuntu Linux

## 정보

프로젝트에 대한 더 자세한 정보를 원하신다면, _펌웨어 포트폴리오.pptx_ 를 참조해주세요.
