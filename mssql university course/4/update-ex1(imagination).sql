/*Попытка смены типа занятия на лабораторную по предмету, по которому нет лабораторных*/

/*SELECT * FROM STUDY;
SELECT * FROM STUDY_TYPE;*/

UPDATE STUDY 
SET type_id = 3
FROM STUDY WHERE study_id = 1; 
/*сработает ограничение check_types_existence*/