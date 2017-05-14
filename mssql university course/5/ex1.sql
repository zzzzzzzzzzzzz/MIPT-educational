/*Название предмета, количество лекций, семинаров, лабораторных занятий по нему.*/

IF OBJECT_ID ('first_view', 'V') IS NOT NULL
DROP VIEW first_view ;
GO
CREATE VIEW first_view
AS
	SELECT tmp1.subjname, lections, seminars, labs FROM 
	(SELECT DISTINCT SUBJECT.name as subjname, 0 as lections 
		FROM STUDY as s, SUBJECT 
		WHERE s.subject_id = SUBJECT.subject_id 
		AND NOT EXISTS(SELECT * FROM STUDY as ss WHERE s.subject_id = ss.subject_id AND ss.type_id = 1)
	UNION ALL 
	SELECT subj.name as subjname, COUNT(std.study_id) as lections
		FROM SUBJECT as subj LEFT OUTER JOIN STUDY as std
		ON subj.subject_id = std.subject_id
		WHERE std.type_id = 1
		GROUP BY subj.name) as tmp1 
	LEFT OUTER JOIN
	(SELECT DISTINCT SUBJECT.name as subjname, 0 as seminars 
		FROM STUDY as s, SUBJECT 
		WHERE s.subject_id = SUBJECT.subject_id 
		AND NOT EXISTS(SELECT * FROM STUDY as ss WHERE s.subject_id = ss.subject_id AND ss.type_id = 2)
	UNION ALL 
	SELECT subj.name as subjname, COUNT(std.study_id) as seminars
		FROM SUBJECT as subj LEFT OUTER JOIN STUDY as std
		ON subj.subject_id = std.subject_id
		WHERE std.type_id = 2
		GROUP BY subj.name) as tmp2
	ON tmp2.subjname = tmp1.subjname
	LEFT OUTER JOIN
	(SELECT DISTINCT SUBJECT.name as subjname, 0 as labs 
		FROM STUDY as s, SUBJECT 
		WHERE s.subject_id = SUBJECT.subject_id 
		AND NOT EXISTS(SELECT * FROM STUDY as ss WHERE s.subject_id = ss.subject_id AND ss.type_id = 3)
	UNION ALL 
	SELECT subj.name as subjname, COUNT(std.study_id) as seminars
		FROM SUBJECT as subj LEFT OUTER JOIN STUDY as std
		ON subj.subject_id = std.subject_id
		WHERE std.type_id = 3
		GROUP BY subj.name) as tmp3
	ON tmp3.subjname = tmp1.subjname;
GO
SELECT * FROM first_view;