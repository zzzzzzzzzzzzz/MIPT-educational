/*Группа, предмет, обязательность*/
IF OBJECT_ID ('seventh_view', 'V') IS NOT NULL
DROP VIEW seventh_view ;
GO
CREATE VIEW seventh_view
AS
	SELECT [GROUP].groupname, SUBJECT.name as subjname, obligation 
	FROM [GROUP], GROUP_ON_STUDY, STUDY, SUBJECT 
	WHERE [GROUP].group_id = GROUP_ON_STUDY.group_id
	AND GROUP_ON_STUDY.study_id = STUDY.study_id
	AND STUDY.subject_id = SUBJECT.subject_id
GO
SELECT * FROM seventh_view;