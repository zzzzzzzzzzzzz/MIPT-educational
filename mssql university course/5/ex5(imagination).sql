/*Здание, аудитория, кол-во занятий в неделю*/
IF OBJECT_ID ('fifth_view', 'V') IS NOT NULL
DROP VIEW fifth_view ;
GO
CREATE VIEW fifth_view
AS
	SELECT BUILDING.name as building_name, AUDITORIUM.name as auditorium_name, COUNT(*) as num_studies 
	FROM BUILDING, 
		 AUDITORIUM LEFT OUTER JOIN GROUP_ON_STUDY ON AUDITORIUM.auditorium_id = GROUP_ON_STUDY.auditorium_id
	WHERE BUILDING.building_id = AUDITORIUM.building_id AND study_id IS NOT NULL
	GROUP BY BUILDING.name, AUDITORIUM.name
	UNION ALL
		SELECT BUILDING.name as building_name, AUDITORIUM.name as auditorium_name, 0 as num_studies 
		FROM BUILDING, 
			 AUDITORIUM LEFT OUTER JOIN GROUP_ON_STUDY ON AUDITORIUM.auditorium_id = GROUP_ON_STUDY.auditorium_id
		WHERE BUILDING.building_id = AUDITORIUM.building_id AND study_id IS NULL
		GROUP BY BUILDING.name, AUDITORIUM.name
GO
SELECT * FROM fifth_view;