# philosophers

C와 POSIX Threads로 구현한 식사하는 철학자 시뮬레이션입니다. 철학자마다 스레드를 생성하고, 이웃과 공유하는 포크를 뮤텍스로 표현합니다. 각 스레드의 식사·수면·생각 상태를 출력하며, 별도 모니터가 생존 시간과 식사 횟수를 확인합니다.

42 Seoul 과제로 2021년에 작성했습니다. 핵심은 **포크의 상호 배제, 공유 상태의 갱신, 시간에 따른 종료 조건**을 여러 스레드 안에서 다루는 것입니다.

## 코드 구조

| 파일 | 역할 |
| --- | --- |
| [main.c](philo/main.c) · [parsing.c](philo/parsing.c) | 실행 인자 검증, 메모리 할당, 시뮬레이션 시작 |
| [philo.h](philo/philo.h) · [init.c](philo/init.c) | 공통 상태와 철학자 상태, 포크 인덱스·뮤텍스 설정 |
| [thread.c](philo/thread.c) | 철학자 스레드와 생존·식사 횟수 모니터 |
| [action_dining.c](philo/action_dining.c) | 포크 획득, 식사, 수면, 생각 |
| [tools.c](philo/tools.c) · [recall.c](philo/recall.c) | 밀리초 시간 계산, 철학자 스레드 합류와 자원 해제 |

## 빌드와 실행

GCC, Make, POSIX Threads를 사용합니다. 저장소 루트에서 실행합니다.

```bash
make -C philo
./philo/philo 4 800 200 200 3
```

인자는 `철학자 수 사망까지의 시간 식사 시간 수면 시간 [최소 식사 횟수]` 순서입니다.

| 인자 | 코드에서 허용하는 값 |
| --- | --- |
| 철학자 수 | 1–199 |
| 시간 3개 | 각각 60 이상, 밀리초 |
| 최소 식사 횟수 | 생략하거나 0 이상의 정수. 0이면 식사 없이 종료 |

양의 식사 횟수를 지정하면 전원이 해당 횟수에 도달했는지 모니터가 확인합니다. 생략하면 사망 판정 또는 사용자 중단까지 반복합니다. 실행 중단은 `Ctrl+C`, 빌드 산출물 정리는 `make -C philo fclean`입니다.

출력은 경과 시간, 철학자 번호, 행동 순서입니다. 아래는 철학자가 한 명인 경우의 실행 결과입니다.

```bash
./philo/philo 1 100 60 60
```

```text
0ms [1] has taken a fork
100ms [1] died
```

## 스레드와 자원 처리

- **포크 공유:** 각 철학자는 오른쪽 포크와 왼쪽 포크의 뮤텍스를 차례로 잠그고, 식사가 끝나면 해제합니다.
- **시작 시점 분산:** 짝수 번호는 식사 시간만큼 기다린 뒤 행동을 시작합니다.
- **출력 직렬화:** 여러 철학자와 모니터의 출력 구간에서 공통 `stdout` 뮤텍스를 사용합니다.
- **종료 조건 분리:** 생존 모니터는 마지막 식사 시각을 순회 확인하고, 식사 횟수 모니터는 목표에 도달한 인원수를 확인합니다.
- **시간 계산:** `gettimeofday()`로 밀리초를 계산하고, 식사·수면 대기 루프에서 `usleep(1000)`을 호출합니다.

철학자 스레드는 종료 시 `pthread_join()`으로 합류하고, 모니터는 생성 시 detach합니다. 초기화 범위와 공유 상태 동기화에 남아 있는 문제는 [구현 메모](docs/implementation.md)에 정리했습니다.

## 실행 확인

Linux/GCC에서 빌드, 단일 철학자의 사망 출력, `4 800 200 200 3`의 종료를 확인했습니다. 철학자 수 0·200과 60ms 미만 시간 입력은 오류로 종료했습니다.

## 개발 기록

공통 상태를 구조체로 분리하고, 모니터와 식사 횟수 종료 처리를 단계적으로 추가한 기록이 있습니다. 개발 중 참고 코드 도입과 후속 수정도 커밋에 남아 있습니다.

- [공통 상태 구조체 분리](https://github.com/tjung03/philosophers/commit/46c81231d2283900ef0af2b48442ae4f1d917a10)
- [참고 코드 도입](https://github.com/tjung03/philosophers/commit/c35ca77b4fc11fd853493bdf2482300447d4ac0d) · [식사 횟수 모니터 추가](https://github.com/tjung03/philosophers/commit/ed6e1689f69d68e58609b77ddbad47fd6e77c04e) · [후속 수정](https://github.com/tjung03/philosophers/commit/119082c991936a34fd40149097b73f7d18cd8e92)
- [Notion 학습 기록](https://tjung.notion.site/philosophers-2f47a6d3620980179855e34ad3b21d4e?source=copy_link)
