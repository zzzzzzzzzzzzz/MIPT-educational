/*Преподаватель, предмет, количество учащихся у этого преподавателя студентов*/
IF OBJECT_ID ('sixth_view', 'V') IS NOT NULL
DROP VIEW sixth_view ;
GO
CREATE VIEW sixth_view
AS
			SELECT TEACHER.second_name as teacher_second_name, TEACHER.first_name as teacher_name, TEACHER.patronymic as teacher_patronymic, SUBJECT.name as subjname, COUNT(DISTINCT STUDENT_GROUP.student_id) as num_of_students
			FROM TEACHER, SUBJECT, GROUP_ON_STUDY, STUDY, STUDENT_GROUP
			WHERE TEACHER.teacher_id = GROUP_ON_STUDY.teacher_id
			AND STUDY.study_id = GROUP_ON_STUDY.study_id
			AND STUDY.subject_id = SUBJECT.subject_id
			AND STUDENT_GROUP.group_id = GROUP_ON_STUDY.group_id 
			GROUP BY TEACHER.second_name, TEACHER.first_name, TEACHER.patronymic, SUBJECT.name
GO
SELECT * FROM sixth_view;
