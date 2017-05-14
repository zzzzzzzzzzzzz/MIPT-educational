/*Определить для каждой группы количество экзаменов и зачетов.*/
SELECT TOP 1 WITH TIES gr1.groupname, isnull(zachete.amount, 0) as Зачёты, isnull(examene.amount, 0) as Экзамены, isnull(zachete.amount, 0) + isnull(examene.amount, 0) as Сумма
	FROM [GROUP] as gr1 
	LEFT OUTER JOIN (SELECT gr2.groupname ,COUNT(*) as amount 
		FROM [GROUP] as gr2, GROUP_ON_STUDY, STUDY, SUBJECT, INSPECTION_SUBJECT
		WHERE gr2.group_id = GROUP_ON_STUDY.group_id AND GROUP_ON_STUDY.study_id = STUDY.study_id 
			AND STUDY.subject_id = SUBJECT.subject_id AND INSPECTION_SUBJECT.subject_id = SUBJECT.subject_id 
			AND INSPECTION_SUBJECT.inspection_id = 1 
		GROUP BY gr2.groupname) as zachete 
	ON gr1.groupname = zachete.groupname 
	LEFT OUTER JOIN (SELECT gr3.groupname ,COUNT(*) as amount 
		FROM [GROUP] as gr3, GROUP_ON_STUDY, STUDY, SUBJECT, INSPECTION_SUBJECT
		WHERE gr3.group_id = GROUP_ON_STUDY.group_id AND GROUP_ON_STUDY.study_id = STUDY.study_id 
			AND STUDY.subject_id = SUBJECT.subject_id AND INSPECTION_SUBJECT.subject_id = SUBJECT.subject_id 
			AND INSPECTION_SUBJECT.inspection_id = 2
		GROUP BY gr3.groupname) as examene
	ON gr1.groupname = examene.groupname
	ORDER BY Сумма DESC;