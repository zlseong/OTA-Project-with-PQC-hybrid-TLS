#!/usr/bin/env python3
"""
TC375 Flash4 QSPI Test - Serial Monitor
UART 출력을 모니터링합니다.
"""

import serial
import serial.tools.list_ports
import time
import sys

def list_com_ports():
    """사용 가능한 COM 포트 목록 표시"""
    ports = serial.tools.list_ports.comports()
    if not ports:
        print("사용 가능한 COM 포트가 없습니다.")
        return None
    
    print("\n사용 가능한 COM 포트:")
    for i, port in enumerate(ports):
        print(f"  [{i}] {port.device} - {port.description}")
    
    return ports

def main():
    BAUDRATE = 115200
    
    # COM 포트 목록 표시
    ports = list_com_ports()
    if not ports:
        return
    
    # 사용자 입력
    if len(ports) == 1:
        selected_port = ports[0].device
        print(f"\n자동으로 {selected_port} 선택됨")
    else:
        try:
            idx = int(input(f"\n포트 번호 선택 (0-{len(ports)-1}): "))
            selected_port = ports[idx].device
        except (ValueError, IndexError):
            print("잘못된 선택입니다.")
            return
    
    print(f"\n{selected_port}에 연결 중... (115200 baud)")
    print("종료하려면 Ctrl+C를 누르세요.\n")
    print("=" * 60)
    
    try:
        # 시리얼 포트 열기
        ser = serial.Serial(
            port=selected_port,
            baudrate=BAUDRATE,
            bytesize=serial.EIGHTBITS,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            timeout=1
        )
        
        print(f"✓ 연결 성공! {selected_port} @ {BAUDRATE} baud\n")
        
        # 데이터 수신 루프
        while True:
            if ser.in_waiting > 0:
                # 데이터 읽기
                data = ser.read(ser.in_waiting)
                try:
                    # UTF-8로 디코딩
                    text = data.decode('utf-8', errors='ignore')
                    print(text, end='', flush=True)
                except UnicodeDecodeError:
                    # 디코딩 실패 시 hex로 표시
                    print(f"[HEX: {data.hex()}]", end='', flush=True)
            
            time.sleep(0.01)
    
    except serial.SerialException as e:
        print(f"\n✗ 시리얼 포트 오류: {e}")
        print(f"  - COM 포트가 다른 프로그램에서 사용 중인지 확인하세요.")
        print(f"  - TC375가 USB로 연결되어 있는지 확인하세요.")
    
    except KeyboardInterrupt:
        print("\n\n사용자에 의해 중지됨")
    
    except Exception as e:
        print(f"\n✗ 예상치 못한 오류: {e}")
    
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
            print(f"\n{selected_port} 연결 종료")

if __name__ == "__main__":
    print("=" * 60)
    print("  TC375 Flash4 QSPI Test - Serial Monitor")
    print("=" * 60)
    main()

