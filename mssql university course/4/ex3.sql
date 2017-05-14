/*Вывести расписание занятий преподавателя 'Половинкин Е.С.' на Апрель 2015г. */
WITH TIMETABLE_RECURSIVE(teacher_id, second_name, first_name, patronymic, study_id, begin_date, begin_time, end_time, periodicity_id, level) 
AS (SELECT DISTINCT TEACHER.teacher_id, second_name, first_name, patronymic, STUDY.study_id, STUDY.begin_date, STUDY.begin_time, STUDY.end_time, STUDY.periodicity_id, 0
	FROM TEACHER, GROUP_ON_STUDY, STUDY
	WHERE STUDY.study_id = GROUP_ON_STUDY.study_id AND GROUP_ON_STUDY.teacher_id = TEACHER.teacher_id AND TEACHER.teacher_id = 101
	UNION ALL
		SELECT TIMETABLE_RECURSIVE.teacher_id, TIMETABLE_RECURSIVE.second_name, TIMETABLE_RECURSIVE.first_name, TIMETABLE_RECURSIVE.patronymic, TIMETABLE_RECURSIVE.study_id, TIMETABLE_RECURSIVE.begin_date + in_days, TIMETABLE_RECURSIVE.begin_time, TIMETABLE_RECURSIVE.end_time, TIMETABLE_RECURSIVE.periodicity_id, level+1
		FROM TIMETABLE_RECURSIVE, PERIODICITY
		WHERE TIMETABLE_RECURSIVE.periodicity_id = PERIODICITY.periodicity_id AND level < 13)
SELECT DISTINCT teacher_id, second_name, first_name, patronymic, study_id, CONVERT(char(10), begin_date, 103) as begin_date, CONVERT(char(5), begin_time, 108) as begin_time, CONVERT(char(5), end_time, 108) as end_time
FROM TIMETABLE_RECURSIVE
WHERE begin_date BETWEEN '01/04/2015' AND '30/04/2015'