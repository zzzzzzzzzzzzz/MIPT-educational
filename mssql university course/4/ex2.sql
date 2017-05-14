/*Определить возможные "накладки" у преподавателей в расписании.*/
SELECT timetable1.teacher_id, timetable1.second_name, timetable1.first_name, timetable1.patronymic, timetable1.study_id, timetable2.study_id FROM
	(SELECT DISTINCT TEACHER.teacher_id, second_name, first_name, patronymic, STUDY.study_id, STUDY.begin_date, STUDY.begin_time, STUDY.end_time
		FROM GROUP_ON_STUDY, STUDY, TEACHER
		WHERE STUDY.study_id = GROUP_ON_STUDY.study_id AND GROUP_ON_STUDY.teacher_id = TEACHER.teacher_id) as timetable1 FULL OUTER JOIN
	(SELECT DISTINCT TEACHER.teacher_id, second_name, first_name, patronymic, STUDY.study_id, STUDY.begin_date, STUDY.begin_time, STUDY.end_time
		FROM GROUP_ON_STUDY, STUDY, TEACHER
		WHERE STUDY.study_id = GROUP_ON_STUDY.study_id AND GROUP_ON_STUDY.teacher_id = TEACHER.teacher_id) as timetable2
		ON timetable1.study_id >= timetable2.study_id
WHERE timetable1.begin_date = timetable2.begin_date 
AND timetable1.teacher_id = timetable2.teacher_id
AND (timetable1.study_id != timetable2.study_id)
AND ((timetable1.begin_time BETWEEN timetable2.begin_time AND timetable2.end_time) 
OR (timetable1.end_time BETWEEN timetable2.begin_time AND timetable2.end_time))