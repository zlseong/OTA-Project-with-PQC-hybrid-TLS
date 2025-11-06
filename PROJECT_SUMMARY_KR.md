# QSPI External Memory Integration - Project Summary

## 프로젝트 개요

TC375 Lite Kit에 MIKROE-3191 Flash 4 Click 외장 플래시 메모리를 QSPI 인터페이스로 연결하여 데이터 읽기/쓰기 기능을 구현했습니다.

## 완성된 파일 목록

### 주요 드라이버 파일
1. **Flash4_Driver.h** - 플래시 드라이버 헤더 파일
   - 모든 Flash 명령어 정의
   - API 함수 프로토타입
   - 상수 및 구조체 정의

2. **Flash4_Driver.c** - 플래시 드라이버 구현 파일
   - QSPI 초기화 및 설정
   - 읽기/쓰기/삭제 기능 구현
   - 인터럽트 핸들러 구현

3. **Flash4_Config.h** - 설정 파일
   - 하드웨어 핀 설정
   - 타이밍 파라미터 설정
   - 인터럽트 우선순위 설정

### 응용 프로그램 파일
4. **Cpu0_Main.c** - 메인 애플리케이션
   - Flash4 초기화
   - 테스트 코드 (읽기/쓰기/검증)
   - LED 표시 기능

5. **Flash4_Examples.c** - 다양한 사용 예제
   - 기본 읽기/쓰기
   - 멀티 페이지 쓰기
   - 설정 데이터 저장
   - 데이터 로깅
   - 펌웨어 업데이트 예제

### 문서 파일
6. **README.md** - 프로젝트 설명서
   - 전체 프로젝트 개요
   - API 레퍼런스
   - 사용 방법
   - 문제 해결 가이드

7. **HARDWARE_SETUP.md** - 하드웨어 설정 가이드
   - 하드웨어 연결 방법
   - 핀 배치도
   - 전원 요구사항
   - 검증 절차

8. **PROJECT_SUMMARY_KR.md** - 이 파일 (한국어 요약)

## 주요 기능

### 1. QSPI 인터페이스 설정
- **모듈**: QSPI2 사용
- **핀 연결**:
  - P15.8 - SCLK (클럭)
  - P15.6 - MOSI (데이터 출력)
  - P15.7 - MISO (데이터 입력)
  - P15.1 - CS (칩 선택)
- **통신 속도**: 1 MHz (최대 50 MHz 지원)
- **모드**: SPI Mode 0 (CPOL=0, CPHA=0)

### 2. 플래시 메모리 작동
- **용량**: 512 Mbit (64 MB)
- **주소 공간**: 0x00000000 ~ 0x03FFFFFF
- **섹터 크기**: 256 KB (256개 섹터)
- **페이지 크기**: 256 바이트

### 3. 지원 기능
- ✅ 장치 ID 읽기 (제조사 및 장치 확인)
- ✅ 섹터 지우기 (4바이트 주소)
- ✅ 페이지 프로그램 (최대 256바이트)
- ✅ 데이터 읽기 (4바이트 주소)
- ✅ 상태 레지스터 확인
- ✅ 쓰기 진행 상태 확인 (WIP)
- ✅ 타임아웃 처리
- ✅ 인터럽트 기반 통신

## 사용 방법

### 기본 초기화
```c
#include "Flash4_Driver.h"

// 플래시 드라이버 초기화
Flash4_Init();

// 초기화 완료 대기
waitMs(100);
```

### 데이터 쓰기
```c
uint8 data[16] = "MikroElektronika";
uint32 address = 0x00001234;

// 1. 쓰기 활성화
Flash4_WriteCommand(FLASH4_CMD_WRITE_ENABLE_WREN);

// 2. 섹터 지우기
Flash4_SectorErase4(address);
Flash4_WaitReady(5000);

// 3. 다시 쓰기 활성화
Flash4_WriteCommand(FLASH4_CMD_WRITE_ENABLE_WREN);

// 4. 데이터 쓰기
Flash4_PageProgram4(data, address, 16);
Flash4_WaitReady(1000);
```

### 데이터 읽기
```c
uint8 readData[16];
uint32 address = 0x00001234;

// 플래시에서 데이터 읽기
Flash4_ReadFlash4(readData, address, 16);
```

## 테스트 프로그램

`Cpu0_Main.c`에 포함된 테스트 프로그램은 다음을 수행합니다:

1. **초기화**: QSPI 인터페이스 및 LED 설정
2. **장치 ID 확인**: 제조사 ID 읽기 (0x01이어야 함)
3. **섹터 지우기**: 주소 0x00001234의 섹터 지우기
4. **데이터 쓰기**: "MikroElektronika" 문자열 쓰기
5. **데이터 읽기**: 쓴 데이터 읽어오기
6. **검증**: 쓴 데이터와 읽은 데이터 비교
7. **결과 표시**: 
   - LED1 ON = 테스트 성공
   - LED1 OFF = 테스트 실패 또는 연결 안됨

## 하드웨어 연결

### MikroBUS 핀맵

| Flash 4 Click | TC375 핀 | 기능 |
|--------------|---------|------|
| SCK | P15.8 | QSPI2_SCLK |
| MOSI | P15.6 | QSPI2_MTSR |
| MISO | P15.7 | QSPI2_MRST |
| CS | P15.1 | QSPI2_SLSO8 |
| 3.3V | VCC | 전원 |
| GND | GND | 접지 |

### 연결 방법
1. TC375 Lite Kit의 전원을 끕니다
2. Flash 4 Click 모듈을 MikroBUS 소켓에 삽입합니다
3. 모든 핀이 올바르게 연결되었는지 확인합니다
4. 전원을 켭니다

## 빌드 및 실행

### AURIX Development Studio 사용

1. **프로젝트 열기**
   - ADS에서 File → Import → Existing Projects
   - 이 프로젝트 폴더 선택

2. **빌드**
   - Project → Build Project (Ctrl+B)
   - 에러 없이 빌드 완료 확인

3. **플래시 및 디버그**
   - TC375 Lite Kit를 USB로 연결
   - Run → Debug As → AURIX C/C++ Application
   - 프로그램이 자동으로 다운로드되고 실행됩니다

4. **결과 확인**
   - 프로그램 실행 후 LED1 확인
   - LED1이 켜지면 테스트 성공
   - 디버거를 사용하여 변수 값 확인 가능

## API 함수 목록

### 초기화
- `Flash4_Init()` - QSPI 인터페이스 초기화

### 장치 정보
- `Flash4_ReadManufacturerId()` - 제조사 및 장치 ID 읽기
- `Flash4_ReadElectronicId()` - 전자 서명 읽기

### 메모리 작업
- `Flash4_ReadFlash4()` - 플래시에서 데이터 읽기
- `Flash4_PageProgram4()` - 플래시에 데이터 쓰기
- `Flash4_SectorErase4()` - 섹터 지우기

### 상태 확인
- `Flash4_CheckWIP()` - 쓰기 진행 중인지 확인
- `Flash4_CheckWEL()` - 쓰기 활성화 여부 확인
- `Flash4_WaitReady()` - 작업 완료 대기

### 명령
- `Flash4_WriteCommand()` - 명령 전송

## 문제 해결

### LED가 켜지지 않을 때

1. **하드웨어 연결 확인**
   - Flash 4 Click이 MikroBUS에 제대로 삽입되었는지 확인
   - 핀이 구부러지지 않았는지 확인

2. **전원 확인**
   - 3.3V 전원이 공급되는지 멀티미터로 확인
   - VCC와 GND 사이에 단락이 없는지 확인

3. **통신 속도 조정**
   - `Flash4_Config.h`에서 `FLASH4_QSPI_BAUDRATE`를 100000 (100kHz)으로 낮춤
   - 다시 빌드하고 테스트

4. **디버거 사용**
   - 브레이크포인트를 설정하여 어디서 실패하는지 확인
   - `g_deviceId[0]` 값 확인 (0x01이어야 함)
   - `g_testPassed` 값 확인

### 통신 오류가 발생할 때

1. **SPI 신호 확인**
   - 로직 애널라이저로 SCK, MOSI, MISO, CS 신호 확인
   - 신호 무결성 문제가 있으면 통신 속도를 낮춤

2. **타임아웃 증가**
   - `Flash4_Config.h`에서 타임아웃 값 증가
   - 특히 `FLASH4_TIMEOUT_ERASE`를 10000ms로 증가

3. **소프트웨어 리셋**
   - `Flash4_Reset()` 함수 호출하여 플래시 칩 리셋

## 예제 사용

`Flash4_Examples.c` 파일에는 다양한 사용 예제가 포함되어 있습니다:

- **Example 1**: 기본 읽기/쓰기
- **Example 2**: 멀티 페이지 쓰기
- **Example 3**: 장치 식별
- **Example 4**: 섹터 관리
- **Example 5**: 설정 데이터 저장
- **Example 6**: 데이터 로깅
- **Example 7**: 펌웨어 업데이트

각 예제는 독립적으로 실행 가능하며, 실제 응용 프로그램에 참고할 수 있습니다.

## 성능 특성

### 타이밍
- **섹터 지우기**: 일반적으로 650ms, 최대 2.6초
- **페이지 프로그램**: 일반적으로 170µs, 최대 700µs
- **읽기 작업**: SPI 클럭 속도에 따라 다름

### 전력 소비
- **대기 모드**: ~10µA
- **읽기 작업**: ~15mA
- **쓰기 작업**: ~20mA
- **지우기 작업**: ~25mA

## 참고 자료

### 데이터시트
- [TC375 Lite Kit 사용 설명서](https://www.infineon.com/cms/en/product/evaluation-boards/kit_a2g_tc375_lite/)
- [AURIX TC3xx 사용자 매뉴얼](https://www.infineon.com/aurix)
- [S25FL512S 플래시 데이터시트](https://www.infineon.com/cms/en/product/memories/nor-flash/)
- [MIKROE-3191 제품 페이지](https://www.mikroe.com/flash-4-click)

### 온라인 리소스
- Infineon 개발자 커뮤니티
- AURIX 포럼
- iLLD 문서

## 라이선스

이 소프트웨어는 Boost Software License - Version 1.0 하에 제공됩니다.

## 버전 정보

- **v1.0.0** (2024) - 초기 릴리스
  - QSPI2 인터페이스 구현
  - 읽기/쓰기/지우기 기능
  - 테스트 애플리케이션
  - iLLD를 사용한 하드웨어 추상화

## 기술 지원

문제가 발생하거나 질문이 있으면:
- Infineon 지원: www.infineon.com/support
- MikroElektronika 지원: www.mikroe.com/support

---

## 프로젝트 체크리스트

### 하드웨어
- [x] Flash 4 Click 모듈 연결
- [x] 전원 연결 확인
- [x] 핀 매핑 확인

### 소프트웨어
- [x] 드라이버 구현 완료
- [x] 테스트 코드 작성
- [x] 예제 코드 작성
- [x] 문서화 완료

### 테스트
- [ ] 기본 통신 테스트
- [ ] 읽기/쓰기 테스트
- [ ] 장기 안정성 테스트

### 문서
- [x] README 작성
- [x] 하드웨어 설정 가이드
- [x] API 문서
- [x] 예제 코드

---

**프로젝트 완료!** 

이제 TC375 Lite Kit에서 MIKROE-3191 외장 플래시 메모리를 사용할 수 있습니다. 빌드하고 실행하여 LED1이 켜지는지 확인하세요!

