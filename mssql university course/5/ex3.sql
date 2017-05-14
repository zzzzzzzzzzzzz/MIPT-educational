/*Название кафедры, название предмета, количество преподавателей, с ними связанных.*/
IF OBJECT_ID ('third_view', 'V') IS NOT NULL
DROP VIEW third_view ;
GO
CREATE VIEW third_view
AS
	SELECT temp1.depname, temp1.subjname, COUNT(temp2.teacher_id) as teachers_num
	FROM (SELECT DEPARTMENT.name AS depname, SUBJECT.name AS subjname 
		 FROM DEPARTMENT, SUBJECT_DEPARTMENT, SUBJECT
		 WHERE DEPARTMENT.department_id = SUBJECT_DEPARTMENT.department_id
		 AND SUBJECT.subject_id = SUBJECT_DEPARTMENT.subject_id) AS temp1
		LEFT OUTER JOIN
		(SELECT DISTINCT SUBJECT.name AS subjname, TEACHER.teacher_id
		 FROM GROUP_ON_STUDY, TEACHER, SUBJECT, STUDY 
		 WHERE GROUP_ON_STUDY.teacher_id = TEACHER.teacher_id
		 AND GROUP_ON_STUDY.study_id = STUDY.study_id
		 AND STUDY.subject_id = SUBJECT.subject_id) AS temp2
		ON temp1.subjname = temp2.subjname
	GROUP BY temp1.depname, temp1.subjname
GO
SELECT * FROM third_view;

/*SELECT DEPARTMENT.name AS depname, SUBJECT.name AS subjname 
FROM DEPARTMENT, SUBJECT_DEPARTMENT, SUBJECT
WHERE DEPARTMENT.department_id = SUBJECT_DEPARTMENT.department_id
AND SUBJECT.subject_id = SUBJECT_DEPARTMENT.subject_id;

SELECT DISTINCT SUBJECT.name AS subjname, TEACHER.teacher_id
FROM GROUP_ON_STUDY, TEACHER, SUBJECT, STUDY 
WHERE GROUP_ON_STUDY.teacher_id = TEACHER.teacher_id
AND GROUP_ON_STUDY.study_id = STUDY.study_id
AND STUDY.subject_id = SUBJECT.subject_id;*/