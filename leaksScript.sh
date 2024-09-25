#!/bin/bash

# 감시할 프로세스 이름 설정
PROCESS_NAME="ircserv"  # 실제 프로세스 이름으로 변경하세요

# 무한 루프 시작
while true; do
    clear
    PID=$(pgrep -x "$PROCESS_NAME")
    if [ -n "$PID" ]; then
        echo "----- Memory Leak Check -----"
        echo "Process Name: $PROCESS_NAME"
        echo "Process ID: $PID"
        echo "Timestamp: $(date)"
        echo "------------------------------"
        leaks -s $PID
    else
        echo "Process '$PROCESS_NAME' not found at $(date)"
    fi
    sleep 2
done

