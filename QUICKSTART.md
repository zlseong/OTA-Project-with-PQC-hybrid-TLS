# Quick Start Guide - QSPI External Flash

## 빠른 시작 (5분 안에 시작하기)

### 1단계: 하드웨어 연결 (1분)
1. TC375 Lite Kit의 전원을 끕니다
2. Flash 4 Click 모듈을 MikroBUS 소켓에 삽입합니다
3. USB 케이블을 연결합니다

### 2단계: 프로젝트 빌드 (2분)
1. AURIX Development Studio를 엽니다
2. 프로젝트를 빌드합니다: `Ctrl+B`
3. 빌드 성공을 확인합니다

### 3단계: 플래시 및 실행 (2분)
1. `Run → Debug As → AURIX C/C++ Application`
2. 프로그램이 자동으로 다운로드됩니다
3. `F8` 키를 눌러 실행합니다

### 4단계: 결과 확인
- **LED1이 켜짐** ✅ = 테스트 성공!
- **LED1이 꺼짐** ❌ = 하드웨어 연결 확인 필요

---

## Quick Start (Get Running in 5 Minutes)

### Step 1: Hardware Setup (1 min)
1. Power off TC375 Lite Kit
2. Insert Flash 4 Click into MikroBUS socket
3. Connect USB cable

### Step 2: Build Project (2 min)
1. Open AURIX Development Studio
2. Build project: `Ctrl+B`
3. Verify build success

### Step 3: Flash and Run (2 min)
1. `Run → Debug As → AURIX C/C++ Application`
2. Program downloads automatically
3. Press `F8` to run

### Step 4: Check Result
- **LED1 ON** ✅ = Test Passed!
- **LED1 OFF** ❌ = Check hardware connection

---

## 프로젝트 파일 구조

```
QSPI_Externalmemory/
│
├── 📄 Flash4_Driver.h         # 드라이버 헤더
├── 📄 Flash4_Driver.c         # 드라이버 구현
├── 📄 Flash4_Config.h         # 설정 파일
├── 📄 Flash4_Examples.c       # 사용 예제 모음
├── 📄 Cpu0_Main.c             # 메인 테스트 코드
│
├── 📘 README.md               # 완전한 문서 (영문)
├── 📘 PROJECT_SUMMARY_KR.md   # 프로젝트 요약 (한글)
├── 📘 HARDWARE_SETUP.md       # 하드웨어 설정 가이드
├── 📘 VISUAL_DIAGRAMS.md      # 연결 다이어그램
└── 📘 QUICKSTART.md           # 이 파일
```

## 기본 사용법

### 초기화
```c
Flash4_Init();
```

### 데이터 쓰기
```c
uint8 data[16] = "Hello World!";

// 쓰기 활성화
Flash4_WriteCommand(FLASH4_CMD_WRITE_ENABLE_WREN);

// 섹터 지우기
Flash4_SectorErase4(0x00001234);
Flash4_WaitReady(5000);

// 쓰기 활성화
Flash4_WriteCommand(FLASH4_CMD_WRITE_ENABLE_WREN);

// 데이터 쓰기
Flash4_PageProgram4(data, 0x00001234, 16);
Flash4_WaitReady(1000);
```

### 데이터 읽기
```c
uint8 readData[16];

// 플래시에서 읽기
Flash4_ReadFlash4(readData, 0x00001234, 16);
```

## 문제 해결

### ❌ LED1이 켜지지 않음

**원인 1: 하드웨어 미연결**
- Flash 4 Click 모듈을 다시 삽입하세요
- 핀이 올바르게 연결되었는지 확인하세요

**원인 2: 통신 오류**
- `Flash4_Config.h`를 열고
- `FLASH4_QSPI_BAUDRATE`를 `100000`으로 변경 (100kHz)
- 다시 빌드하고 실행

**원인 3: 전원 문제**
- 멀티미터로 3.3V 전원 확인
- USB 케이블을 다시 연결

### 🔧 디버깅

브레이크포인트를 설정하여 변수 확인:

```c
// testFlash4() 함수에서
g_deviceId[0]     // 0x01이어야 함 (제조사 ID)
g_deviceId[1]     // 0x19이어야 함 (장치 ID)
g_testPassed      // TRUE이어야 함
```

## 다음 단계

### 1. 예제 코드 실행
`Flash4_Examples.c`에 있는 예제들을 시도해보세요:
- Example1: 기본 읽기/쓰기
- Example2: 멀티 페이지 쓰기
- Example5: 설정 저장
- Example6: 데이터 로깅

### 2. 자신만의 애플리케이션 작성
드라이버를 사용하여 원하는 기능을 구현하세요:
- 센서 데이터 저장
- 로그 파일 관리
- 펌웨어 업데이트
- 설정 관리

### 3. 성능 최적화
통신 속도를 높이세요:
```c
// Flash4_Config.h에서
#define FLASH4_QSPI_BAUDRATE  10000000  // 10 MHz
```

## 주요 API 함수

| 함수 | 설명 |
|------|------|
| `Flash4_Init()` | 초기화 |
| `Flash4_ReadManufacturerId()` | 장치 ID 읽기 |
| `Flash4_SectorErase4()` | 섹터 지우기 |
| `Flash4_PageProgram4()` | 데이터 쓰기 |
| `Flash4_ReadFlash4()` | 데이터 읽기 |
| `Flash4_WaitReady()` | 완료 대기 |
| `Flash4_WriteCommand()` | 명령 전송 |

## 핀 연결 (MikroBUS)

| 핀 | TC375 | 기능 |
|----|-------|------|
| SCK | P15.8 | 클럭 |
| MOSI | P15.6 | 데이터 출력 |
| MISO | P15.7 | 데이터 입력 |
| CS | P15.1 | 칩 선택 |
| 3.3V | VCC | 전원 |
| GND | GND | 접지 |

## 메모리 정보

- **용량**: 64 MB (512 Mbit)
- **섹터**: 256개 (각 256 KB)
- **페이지**: 섹터당 1024개 (각 256 bytes)
- **주소 범위**: 0x00000000 ~ 0x03FFFFFF

## 타이밍

- **섹터 지우기**: 최대 2.6초
- **페이지 쓰기**: 최대 700µs
- **읽기**: SPI 클럭 속도에 따라

## 전력 소비

- **대기**: ~10µA
- **읽기**: ~15mA
- **쓰기**: ~20mA
- **지우기**: ~25mA

## 지원 및 문의

### 문서
- 📘 `README.md` - 전체 문서
- 📘 `PROJECT_SUMMARY_KR.md` - 한글 요약
- 📘 `HARDWARE_SETUP.md` - 상세 설정
- 📘 `VISUAL_DIAGRAMS.md` - 다이어그램

### 온라인 지원
- [Infineon Support](https://www.infineon.com/support)
- [MikroE Support](https://www.mikroe.com/support)

### 데이터시트
- TC375 User Manual
- S25FL512S Datasheet
- MIKROE-3191 Datasheet

## 체크리스트

프로젝트 시작 전 확인사항:

- [ ] TC375 Lite Kit 있음
- [ ] Flash 4 Click 모듈 있음
- [ ] USB 케이블 있음
- [ ] AURIX Development Studio 설치됨
- [ ] 프로젝트 빌드 성공
- [ ] 하드웨어 연결 완료
- [ ] 테스트 실행 (LED1 확인)

모두 완료되었다면 준비 완료! 🎉

---

## 참고 사항

### ⚠️ 중요
- 플래시에 쓰기 전에 항상 섹터를 지워야 합니다
- 지우기 후 플래시는 0xFF 상태입니다
- 한 페이지는 256바이트입니다 (초과하면 안됨)
- 타임아웃을 충분히 설정하세요 (특히 지우기)

### 💡 팁
- 처음에는 낮은 SPI 클럭으로 시작 (1 MHz)
- 안정적으로 동작하면 클럭 속도 증가 (최대 50 MHz)
- 디버거를 사용하여 변수 값 확인
- 로직 애널라이저로 SPI 신호 확인

### 🚀 성능 향상
1. SPI 클럭 속도 증가
2. DMA 사용 (구현 필요)
3. 여러 페이지를 연속으로 쓰기
4. 읽기 작업은 빠름 (지우기/쓰기가 느림)

---

**성공적인 프로젝트를 기원합니다! 🎯**

질문이나 문제가 있으면 문서를 참조하거나 지원팀에 문의하세요.

