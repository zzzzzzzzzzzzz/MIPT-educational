/*Вывести аудитории в которых проходят лекции*/
SELECT DISTINCT AUDITORIUM.auditorium_id, AUDITORIUM.name, BUILDING.name 
FROM AUDITORIUM, BUILDING, GROUP_ON_STUDY, STUDY
WHERE GROUP_ON_STUDY.study_id = STUDY.study_id
AND GROUP_ON_STUDY.auditorium_id = AUDITORIUM.auditorium_id
AND AUDITORIUM.building_id = BUILDING.building_id
AND STUDY.type_id = 1;