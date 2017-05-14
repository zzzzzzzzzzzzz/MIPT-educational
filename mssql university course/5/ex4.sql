/*Группа, предмет, преподаватель, вид проверки.*/

IF OBJECT_ID ('fourth_view', 'V') IS NOT NULL
DROP VIEW fourth_view ;
GO
CREATE VIEW fourth_view
AS
SELECT groupname, subjname, teacher_second_name, teacher_first_name, teacher_patronymic, isnull(x.inspection_id, 0) as zach, isnull(y.inspection_id - 1, 0) as exam
FROM
	(SELECT DISTINCT [GROUP].groupname, SUBJECT.subject_id, SUBJECT.name as subjname, TEACHER.second_name as teacher_second_name, TEACHER.first_name as teacher_first_name, TEACHER.patronymic as teacher_patronymic
			FROM [GROUP], GROUP_ON_STUDY, STUDY, TEACHER, SUBJECT
			WHERE [GROUP].group_id = GROUP_ON_STUDY.group_id
			AND GROUP_ON_STUDY.teacher_id = TEACHER.teacher_id
			AND GROUP_ON_STUDY.study_id = STUDY.study_id
			AND STUDY.subject_id = SUBJECT.subject_id) as tmp1
			LEFT OUTER JOIN INSPECTION_SUBJECT as x
				ON (tmp1.subject_id = x.subject_id and x.inspection_id = 1)
				LEFT OUTER JOIN INSPECTION_SUBJECT as y
					ON tmp1.subject_id = y.subject_id AND y.inspection_id = 2
GO
SELECT * FROM fourth_view;