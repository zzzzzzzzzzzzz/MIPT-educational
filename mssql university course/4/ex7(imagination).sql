/*Вывести расписание уроков сальсы на Март 2015*/
WITH SALSA_RECURSIVE(begin_date, begin_time, end_time, periodicity_id, level) 
AS (SELECT STUDY.begin_date, STUDY.begin_time, STUDY.end_time, STUDY.periodicity_id, 0
	FROM STUDY, SUBJECT
	WHERE STUDY.subject_id = SUBJECT.subject_id AND SUBJECT.name = 'Уроки сальсы'
	UNION ALL
		SELECT SALSA_RECURSIVE.begin_date + in_days, SALSA_RECURSIVE.begin_time, SALSA_RECURSIVE.end_time, SALSA_RECURSIVE.periodicity_id, level+1
		FROM SALSA_RECURSIVE, PERIODICITY
		WHERE SALSA_RECURSIVE.periodicity_id = PERIODICITY.periodicity_id AND level < 7)
SELECT DISTINCT CONVERT(char(10), begin_date, 103) as begin_date, CONVERT(char(5), begin_time, 108) as begin_time, CONVERT(char(5), end_time, 108) as end_time
FROM SALSA_RECURSIVE
WHERE begin_date BETWEEN '01/03/2015' AND '30/03/2015';