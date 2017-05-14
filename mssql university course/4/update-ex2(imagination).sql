/*Перенос лекции в другой лекционный зал, если он свободен*/
/*UPDATE GROUP_ON_STUDY 
SET auditorium_id = 28
WHERE study_id = 1;*/
SELECT * FROM GROUP_ON_STUDY WHERE auditorium_id IN (28,29);

UPDATE GROUP_ON_STUDY
SET auditorium_id = (SELECT AUDITORIUM.auditorium_id
						FROM AUDITORIUM
						WHERE AUDITORIUM.name = 'Лекционный зал' 
						AND auditorium_id NOT IN (SELECT AUDITORIUM.auditorium_id 
													FROM AUDITORIUM, GROUP_ON_STUDY, STUDY, (SELECT begin_date, begin_time FROM STUDY WHERE study_id = 1) as st1 
													WHERE GROUP_ON_STUDY.study_id = STUDY.study_id
															AND AUDITORIUM.auditorium_id = GROUP_ON_STUDY.auditorium_id
															AND st1.begin_date = STUDY.begin_date
															AND st1.begin_time BETWEEN STUDY.begin_time AND STUDY.end_time))
WHERE study_id = 1;
SELECT * FROM GROUP_ON_STUDY WHERE auditorium_id IN (28,29);

															