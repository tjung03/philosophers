#include "philosophers.h"

/*
 *		< 그 외 기능 >
 *		1. 몰라 아직 생각안함 ㅅㄱ
 *		
 *		
 *		
 *		
 */

int	dining_philo(t_common *cmn)
{
	t_philo	*po;
	int		i;

	/*
	 *		1. po(철학자) 포인터 할당
	 *		2. po 관련 모든 데이터 초기화(malloc 2 번 해야 함) 기능 추가
	 *		3. 철학자 스레드 생성
	 *		4. 초기 시작 시간, 굶주리는 시간 초기화
	 *		5. 모두 착석 -> 각 스레드 동작 start
	 *		6. 외부에서 스레드 검사하도록 하는 기능 추가 (상태 출력, must_eat 및 death 검사)
	 *		7. 모든 자원 회수하는 기능 추가
	 *		8. 현재 함수 종료
	 */ 
	return (0);
}

int	main(int ac, char **av)
{
	t_common	cmn;

	memset(&cmn, 0, sizeof(cmn));
	if (ac == 5 || ac == 6)
	{
		if (get_options(&cmn, ac, av))
			return (print_error(1, "Parsing ERROR!(options)"));
		if (dining_philo(&cmn))
			return (print_error(1, "dining_philo() ERROR!"));
	}
	else
		return (print_error(1, "Parsing ERROR!"));
	return (0);
}
