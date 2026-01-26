# philosophers
42seoul project

# Notion Page
[tjung's projects](https://tjung.notion.site/philosophers-2f47a6d3620980179855e34ad3b21d4e?source=copy_link "philosophers")

# philosophers (philo) 코드 구조 요약

## 0) 전체 흐름도(텍스트)

```
main()
|
|-- memset(&cmn, 0, sizeof(cmn))
|-- get_options(&cmn, ac, av)          // 입력 검증 + 옵션 파싱
|
`-> simulation(&cmn)
    |
    |-- malloc: po[nop], mnt_id[2], forkm[nop]
    |-- init(&cmn, po)                 // mutex/인덱스/초기 상태 세팅
    |-- create_thread(&cmn, po, mnt_id)
    |     |
    |     |-- cmn.start_time = get_time()
    |     |-- for i in [0..nop)
    |     |     |-- po[i].hunger_time = start_time
    |     |     `-- pthread_create(po[i].tid, start_dining, &po[i])
    |     |
    |     `-- (nop > 1) 일 때 모니터 스레드 생성 + detach
    |           |-- monitoring(po)                 // 죽음 감시
    |           `-- monitoring_must_eat(cmn)       // 전원 식사 완료 감시(옵션일 때)
    |
    `-- recall_resources(&cmn, po, mnt_id)
          |-- pthread_join(모든 철학자 스레드)
          |-- mutex destroy(stdout + 각 포크)
          `-- free(forkm, mnt_id, po)
```

---

## 1) 파일별 역할과 구조적 의도

### main.c
프로그램 전체 흐름을 '**입력 파싱 → 시뮬레이션 실행 → 자원 회수**'로 나눴다.
동시성 문제와 직접 관련 없는 초기 설정과 종료 처리를 한 눈에 볼 수 있게 정리했다.

### parsing.c
입력값을 숫자로 제한하고, 과제 조건에 맞는 최소 범위를 강제한다.  
동시성 버그처럼 보일 수 있는 “잘못된 입력으로 인한 비정상 상태”를 사전에 차단하기 위한 방어 계층이다.

### init.c
- 포크 mutex 배열 생성
- stdout mutex 초기화
- 각 철학자에게 포크 인덱스(lf/rf) 할당

스레드가 생성되기 전에 **공유 자원과 인덱스 관계를 확정**시키는 단계로,  
실행 중에 구조가 바뀌지 않도록 의도된 분리다.

### thread.c
- 철학자 스레드(start_dining)
- 모니터링 스레드(monitoring / monitoring_must_eat)

행동 수행과 종료 조건 판정을 분리함으로써  
상태 판단이 여러 스레드에 흩어지지 않도록 설계했다.

### action_dining.c
철학자의 행동을
- pick_up
- eat
- sleep
- think

로 나누되, 단순 기능 분리가 아니라  
**공유 자원 접근(포크), 상태 갱신(hunger_time), 출력 동기화(stdout)**가  
각 단계에서 어떻게 엮이는지를 드러내는 구조다.

### recall.c
모든 스레드 join 후 mutex destroy 및 메모리 해제.  
정상 종료/비정상 종료와 무관하게 자원이 회수되도록 구성했다.

### tools.c
시간 계산(get_time)과 단순 파싱(my_atoi)을 분리해  
시뮬레이션 로직에서 시간/입력 처리 의존을 최소화했다.

---

## 2) 문제 인식: 동시성에서 드러난 불안정성

이 프로젝트에서 핵심적으로 마주한 문제는 다음의 세 가지.

1. **공유 자원 경쟁**
   - 포크 mutex를 여러 스레드가 동시에 획득하려는 상황
   - 획득 순서와 타이밍에 따라 교착이나 기아 가능성 발생

2. **공유 상태 경쟁**
   - is_surv, hunger_time, full_cnt 등은 여러 스레드가 읽고/씀
   - 동기화가 느슨하면 “사후 출력”, “이미 끝났는데 계속 동작” 같은 현상 발생

3. **시간 기반 시뮬레이션의 비결정성**
   - 스레드 스케줄링과 컨텍스트 스위칭에 따라 결과가 달라짐
   - 같은 코드라도 실행할 때마다 다른 결과를 낼 수 있음

---

## 3) 실행 안정성을 위한 선택

### 출력 동기화(stdout mutex)
모든 출력은 stdout mutex로 보호.  
이는 단순 로그 정리가 아니라,
- 출력 순서 꼬임 방지
- 죽음 이후 출력 차단
- 일부 상태 갱신의 일관성 확보

를 동시에 노린 선택.

### 종료 조건의 중앙화(모니터 스레드)
철학자 스레드는 행동만 수행하고,
- 죽음 판정
- 전원 식사 완료 판정

은 별도의 모니터 스레드에서 수행.  
종료 조건을 한 곳에서 관리함으로써 상태 판단 기준을 단순화.

### 초기 충돌 완화(짝수 철학자 지연)
짝수 번호 철학자는 시작 시 tte만큼 대기.  
모든 철학자가 동일한 포크 획득 순서를 사용하므로 교착 가능성은 원리적으로 남아 있음.  

다만 초기 시점에서 짝수 철학자를 지연시켜 포크 경쟁을 완화함으로써  
교착을 해결하기보다는 과제 제약 안에서 실행 안정성을 확보하는 방향을 선택.

### 시간 처리 방식
eat/sleep는 목표 시간까지
- while 루프
- usleep(1000)

으로 대기.  
정확도를 어느 정도 확보하는 대신 CPU 사용과 스케줄링 영향이라는 트레이드오프를 감수한 선택.

usleep 값을 1000으로 둔 이유는,
  너무 작으면 → CPU 낭비 + 스케줄링 압박
  너무 크면 → 죽음 판정/상태 전이가 늦어짐
때문임.

---

## 4) 공유 자원과 상태 관리 정리

### 공유 자원(명시적 mutex)
- forkm[] : 포크 mutex
- stdout  : 출력 및 일부 상태 보호용 mutex

### 공유 상태
- is_surv        : 전체 시뮬레이션 종료 플래그
- hunger_time    : 죽음 판정 기준 시각
- full_cnt / eat_cnt : must_eat 옵션 관련 상태

현재 구현은 stdout mutex에 일부 의존해 상태를 보호하지만,
모든 접근이 일관되게 동기화되어 있지는 않음.

---

## 5) 비결정성에 대한 대응 전략

컨텍스트 스위칭으로 인해,
- 어떤 스레드가 먼저 실행되는지
- 포크 mutex를 누가 먼저 획득하는지
- hunger_time 갱신이 모니터보다 앞서는지

가 매 실행마다 달라질 수 있음.

이 코드는 이러한 비결정성을 제거하기보다,
- 출력 동기화
- 종료 조건 분리
- 초기 지연

을 통해 **실행 결과를 최대한 안정적으로 보이게 만드는 방향**을 선택.

---

## 6) 현재 코드의 한계와 개선점

### (1) init()의 초기화 범위 문제
- 철학자 한 명만 초기화되며 나머지는 초기화되지 않음.  
- must_eat 판정 등에서 버그로 이어질 가능성이 있음.

**개선**
- 전체 배열 크기로 memset
- 또는 루프를 통한 명시적 필드 초기화
```
/* ===========================
 * philo/init.c
 * =========================== */
void    init(t_common *cmn, t_philo *po)
{
    int idx;

    pthread_mutex_init(&cmn->stdout, NULL);
    cmn->is_surv = 1;
    // po[0]만 초기화되는 실수 방지: 전체 철학자 배열을 0으로 초기화
    // memset(po, 0, sizeof(*po));
    memset(po, 0, sizeof(*po) * cmn->nop); 
    idx = -1;
    while (++idx < cmn->nop)
    {
        pthread_mutex_init(&cmn->forkm[idx], NULL);
        po[idx].cmn = cmn;
        po[idx].p_num = idx + 1;
        po[idx].rf = idx;
        po[idx].lf = idx - 1;
        if (!idx)
            po[idx].lf = cmn->nop - 1;
    }
}
```

---

### (2) 공유 상태 접근의 데이터 레이스 가능성
- hunger_time, is_surv, full_cnt는 잠금 없이 읽히는 구간이 존재
- stdout mutex에 간접적으로 기대고 있어 근본적 해결은 아님

**개선**
- 상태 전용 mutex 분리
- 또는 atomic 변수 사용(허용 시)
```
/* ===========================
 * philo/philo.h
 * =========================== */
typedef struct s_common {
    pthread_mutex_t stdout;
    pthread_mutex_t state;     // 시뮬레이션 상태 전용 mutex 추가
    pthread_mutex_t *forkm;
    long long       start_time;
    int             nop;
    int             ttd;
    int             tte;
    int             tts;
    int             pme;
    int             is_surv;
    int             full_cnt;
}   t_common;
```
```
/* ===========================
 * philo/init.c
 * =========================== */
void    init(t_common *cmn, t_philo *po)
{
    pthread_mutex_init(&cmn->stdout, NULL);
    pthread_mutex_init(&cmn->state, NULL); // state mutex 초기화

    pthread_mutex_lock(&cmn->state);
    cmn->is_surv = 1;
    cmn->full_cnt = 0;
    pthread_mutex_unlock(&cmn->state);

    memset(po, 0, sizeof(*po) * cmn->nop);
    /* ... */
}
```
```
/* ===========================
 * philo/recall.c
 * =========================== */
void    recall_resources(t_common *cmn, t_philo *po, pthread_t *mnt)
{
    int i;

    i = -1;
    while (++i < cmn->nop)
        pthread_join(po[i].tid, NULL);

    pthread_mutex_destroy(&cmn->stdout);
    pthread_mutex_destroy(&cmn->state); // state mutex 해제

    i = -1;
    while (++i < cmn->nop)
        pthread_mutex_destroy(&cmn->forkm[i]);
    /* ... */
}
```
```
/* ===========================
 * philo/action_dining.c
 * =========================== */
void    eat(t_philo *po)
{
    long long eat_start;
    int       alive;

    // 상태 변경은 state mutex로 보호
    pthread_mutex_lock(&po->cmn->state);

    // pthread_mutex_lock(&po->cmn->stdout);
    po->eat_cnt++;
    if (po->cmn->pme != -1 && po->eat_cnt == po->cmn->pme)
        po->cmn->full_cnt += ++po->full;

    po->hunger_time = get_time();
    // if (po->cmn->is_surv)
		//    printf("%lldms\t[%d]\t%s\n", \
		//    po->hunger_time - po->cmn->start_time, po->p_num, "is eating");
    // pthread_mutex_unlock(&po->cmn->stdout);
    eat_start = po->hunger_time;
    alive = po->cmn->is_surv; // 출력 여부 판단을 위해 잠깐 읽어둠

    pthread_mutex_unlock(&po->cmn->state);

    // 출력은 stdout mutex로 보호
    pthread_mutex_lock(&po->cmn->stdout);
    if (alive)
        printf("%lldms\t[%d]\t%s\n",
            eat_start - po->cmn->start_time, po->p_num, "is eating");
    pthread_mutex_unlock(&po->cmn->stdout);

    /* 기존 코드 이어서 */
    while (po->cmn->tte > get_time() - po->hunger_time)
      usleep(1000);
    pthread_mutex_unlock(&po->cmn->forkm[po->lf]);
    pthread_mutex_unlock(&po->cmn->forkm[po->rf]);
}
```
```
/* ===========================
 * philo/thread.c
 * is_surv, hunger_time를 잠금 없이 읽기/쓰기
 * =========================== */
static void *monitoring(void *info)
{
    t_philo     *mnt = (t_philo *)info;
    long long   ms_time;
    long long   last_hunger;  // hunger_time 스냅샷(임시 지역 변수)
    int         alive;        // is_surv 스냅샷
    int         i = -1;

    // while (mnt[++i].cmn->is_surv)
    // { ... }
    while (1)
    {
        i++;
        if (mnt[i].cmn->nop == i)
            i = 0;

        ms_time = get_time();

        // 상태 읽기: state mutex로 보호
        pthread_mutex_lock(&mnt[i].cmn->state);
        alive = mnt[i].cmn->is_surv;
        last_hunger = mnt[i].hunger_time;
        pthread_mutex_unlock(&mnt[i].cmn->state);

        if (!alive)
            break;

        if (mnt[i].cmn->ttd < ms_time - last_hunger)
        {
            // 상태 쓰기: state mutex로 보호
            pthread_mutex_lock(&mnt[i].cmn->state);
            mnt[i].cmn->is_surv = 0;
            pthread_mutex_unlock(&mnt[i].cmn->state);

            pthread_mutex_lock(&mnt[i].cmn->stdout);
            printf("%lldms\t[%d]\t%s\n",
                ms_time - mnt[i].cmn->start_time, mnt[i].p_num, "died");
            pthread_mutex_unlock(&mnt[i].cmn->stdout);
            break;
        }
        usleep(1000);
    }
    return NULL;
}
```

---

### (3) 포크 lock 순서 고정
모든 철학자가 동일한 순서로 포크를 잡음.  
짝수 지연으로 완화는 했지만 교착 가능성은 남아 있음.

**개선**
- 홀/짝 철학자에 따라 lock 순서 반전
- 또는 전역 포크 순서 규칙 도입
```
/* ===========================
 * philo/action_dining.c
 * =========================== */
void    pick_up(t_philo *po)
{
    int first;    // 먼저 잠글 포크 인덱스
    int second;

    // pthread_mutex_lock(&po->cmn->forkm[po->rf]);
    // ... 출력 부분
    // pthread_mutex_lock(&po->cmn->forkm[po->lf]);
    // ... 출력 부분

    // 홀/짝에 따라 포크 잡는 순서 반전 -> 교착 가능성 크게 감소
    if (po->p_num % 2)        // 홀수 철학자: left -> right
    {
        first = po->lf;
        second = po->rf;
    }
    else                      // 짝수 철학자: right -> left
    {
        first = po->rf;
        second = po->lf;
    }

    pthread_mutex_lock(&po->cmn->forkm[first]);
    pthread_mutex_lock(&po->cmn->stdout);
    // 앞서 개선한 state mutex 있는 경우, 여기서 is_surv 읽기도 state로 보호하는 편이 더 안전
    if (po->cmn->is_surv)
        printf("%lldms\t[%d]\t%s\n",
            get_time() - po->cmn->start_time, po->p_num, "has taken a fork");
    pthread_mutex_unlock(&po->cmn->stdout);

    pthread_mutex_lock(&po->cmn->forkm[second]);
    pthread_mutex_lock(&po->cmn->stdout);
    if (po->cmn->is_surv)
        printf("%lldms\t[%d]\t%s\n",
            get_time() - po->cmn->start_time, po->p_num, "has taken a fork");
    pthread_mutex_unlock(&po->cmn->stdout);
}

```

---

### (4) 시간 대기 방식의 비효율
while + usleep(1000)은 정확도는 확보하지만 CPU 사용이 증가.

**개선**
- 남은 시간 기반 sleep 간격 조정
- 불필요한 폴링 감소
```
/* ===========================
 * philo/philo.h
 * =========================== */
long long    get_time(void);
void         smart_sleep_ms(int duration_ms); // 신규 함수 추가

```
```
/* ===========================
 * philo/tools.c (신규 함수 구현)
 * =========================== */
void    smart_sleep_ms(int duration_ms)
{
    long long start;
    long long now;
    long long remain;

    start = get_time();
    while (1)
    {
        now = get_time();
        remain = (long long)duration_ms - (now - start);
        if (remain <= 0)
            break;

        // 남은 시간에 따라 폴링 빈도 조절 (CPU 사용 감소)
        if (remain > 20)
            usleep(5000);     // 5ms 단위로 크게 잠
        else if (remain > 5)
            usleep(1000);     // 1ms
        else
            usleep(200);      // 마지막은 조금 더 촘촘히
    }
}
```
```
/* ===========================
 * philo/action_dining.c
 * =========================== */
void    do_sleep(t_philo *po)
{
    long long stime;

    pthread_mutex_lock(&po->cmn->stdout);
    stime = get_time();
    if (po->cmn->is_surv)
        printf("%lldms\t[%d]\t%s\n",
            stime - po->cmn->start_time, po->p_num, "is sleeping");
    pthread_mutex_unlock(&po->cmn->stdout);

    // 폴링 루프 제거(신규 함수 이용)
    // while (po->cmn->tts > get_time() - stime)
		//   usleep(1000);
    smart_sleep_ms(po->cmn->tts);
}
```
```
/* ===========================
 * philo/action_dining.c (추가 개선 버전)
 * =========================== */
void    eat(t_philo *po)
{
    /*
     * (앞 부분 코드 생략)
     * long long eat_start;
     * int       alive;
     * ...
     * pthread_mutex_unlock(&po->cmn->stdout);
     */

    // 식사 시간 대기: 폴링 감소
    // while (po->cmn->tte > get_time() - po->hunger_time)
    //   usleep(1000);
    smart_sleep_ms(po->cmn->tte);

    pthread_mutex_unlock(&po->cmn->forkm[po->lf]);
    pthread_mutex_unlock(&po->cmn->forkm[po->rf]);
}

```

---

