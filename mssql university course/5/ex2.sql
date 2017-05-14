/*Группа, дата, время, название предмета, преподаватель, аудитория. (вид занятия - лекция)*/

IF OBJECT_ID ('second_view', 'V') IS NOT NULL
DROP VIEW second_view ;
GO
CREATE VIEW second_view
AS
	WITH SCND_VIEW_RECURSIVE(groupname, date, end_date, begin_time, end_time, subjname, second_name, first_name, patronymic, auditorium, periodicity_id, level)
	AS
		(SELECT [GROUP].groupname, STUDY.begin_date as date, STUDY.end_date, STUDY.begin_time as begin_time, STUDY.end_time, SUBJECT.name as subjname, TEACHER.second_name, TEACHER.first_name, TEACHER.patronymic, AUDITORIUM.name as auditorium, STUDY.periodicity_id, 0
		FROM [GROUP], GROUP_ON_STUDY, AUDITORIUM, SUBJECT, STUDY, TEACHER
		WHERE GROUP_ON_STUDY.group_id = [GROUP].group_id
		AND GROUP_ON_STUDY.study_id = STUDY.study_id
		AND STUDY.subject_id = SUBJECT.subject_id
		AND STUDY.type_id = 1
		AND AUDITORIUM.auditorium_id = GROUP_ON_STUDY.auditorium_id
		AND GROUP_ON_STUDY.teacher_id = TEACHER.teacher_id
		UNION ALL
			SELECT SCND_VIEW_RECURSIVE.groupname, SCND_VIEW_RECURSIVE.date + in_days as date, SCND_VIEW_RECURSIVE.end_date, SCND_VIEW_RECURSIVE.begin_time as begin_time, SCND_VIEW_RECURSIVE.end_time as end_time, SCND_VIEW_RECURSIVE.subjname, SCND_VIEW_RECURSIVE.second_name, SCND_VIEW_RECURSIVE.first_name, SCND_VIEW_RECURSIVE.patronymic, SCND_VIEW_RECURSIVE.auditorium, SCND_VIEW_RECURSIVE.periodicity_id, level+1
			FROM SCND_VIEW_RECURSIVE, PERIODICITY 
			WHERE SCND_VIEW_RECURSIVE.periodicity_id = PERIODICITY.periodicity_id AND level < 15)
	SELECT DISTINCT groupname, date, CONVERT(char(5), begin_time, 108) as begin_time, CONVERT(char(5), end_time, 108) as end_time, second_name, first_name, patronymic, auditorium
	FROM SCND_VIEW_RECURSIVE
	WHERE date <= end_date
GO
SELECT * FROM second_view ORDER BY date ASC;