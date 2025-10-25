# OTA-Project

차량용 OTA (Over-The-Air) 업데이트 풀스택 프로젝트

## 개요

차량용 OTA 업데이트 시스템의 풀스택 구현 프로젝트입니다. 현재는 VMG (Vehicle Management Gateway)와 Server 간의 통신을 위한 JSON 메시지 포맷 명세서를 먼저 구현했습니다.

## 현재 구현 상태

### ✅ 완료된 부분
- **메시지 포맷 명세서**: VMG ↔ Server 간 통신 메시지 정의
- **보안 프로토콜**: TLS 1.3, mTLS, ML-KEM 768 등 보안 요구사항 정의
- **통신 흐름**: 전체 OTA 업데이트 프로세스의 메시지 흐름 설계

### 🚧 향후 구현 예정
- **VMG 구현**: 차량 내 게이트웨이 소프트웨어 (C++)
- **Server 구현**: 백엔드 서버 및 관리 시스템
- **MQTT 클라이언트**: 메시지 송수신 구현
- **HTTPS 파일 전송**: 대용량 펌웨어 파일 전송
- **인증서 관리**: PKI 기반 인증서 발급 및 관리
- **모니터링 시스템**: 업데이트 진행률 및 상태 모니터링

## 메시지 흐름

1. **Wake up message** (VMG → Server) - 차량 부팅 시 서버에 알림
2. **Request VCI** (Server → VMG) - 차량 구성 정보 요청
3. **Send VCI** (VMG → Server) - 차량 구성 정보 전송
4. **Request readiness** (Server → VMG) - 다운로드 준비 상태 확인 요청
5. **Send ready response** (VMG → Server) - 준비 상태 응답
6. **Send success results** (VMG → Server) - 업데이트 성공 결과 보고

## 주요 특징

- **MQTT over TLS**: 제어 메시지는 MQTT로 전송
- **HTTPS**: 대용량 파일은 HTTPS로 전송
- **Post-quantum cryptography**: ML-KEM 768 키 교환
- **차량 중심 설계**: 차량 전체 상태를 고려한 업데이트 관리
- **지역별 Calibration 처리**: 지역별 패키지 선택 지원
- **완벽한 추적**: correlation_id를 통한 요청-응답 매칭

## 파일 구조

```
message_format/
├── 1. Wake up message (VMG - Server).md
├── 2. Request VCI (Server -VMG).md
├── 3. Send VCI (VMG - Server).md
├── 4. Request readiness to download software update packages (Server → VMG).md
├── 5. Send ready response (VMG → Server).md
└── 6. Send results of software update operation (VMG → Server).md
```

## 보안

- **TLS 1.3**: 모든 통신에 TLS 암호화 적용
- **mTLS**: 상호 인증서 기반 인증
- **ML-KEM 768**: 양자 컴퓨터 대응 키 교환
- **ECDSA P-256**: 셀룰러 네트워크 최적화 인증서

## 사용법

각 메시지 파일을 참조하여 VMG와 Server 구현 시 사용하세요. 모든 메시지는 JSON 형식이며, 성공 케이스 기준으로 설계되었습니다.

## 라이선스

이 프로젝트는 MIT 라이선스 하에 배포됩니다.