/*Триггер: при добавлении нового семинара, по возможности, уменьшается число лекций по данному предмету */

IF OBJECT_ID ('regulate', 'TR') IS NOT NULL
   DROP TRIGGER regulate;

CREATE TRIGGER regulate
ON GROUP_ON_STUDY
AFTER INSERT
AS 
BEGIN
	/*Считаем количество добавленных семинаров*/
	DECLARE @num INT
		SELECT  @num = count(8)
		FROM STUDY_TYPE, inserted, STUDY	
		WHERE inserted.study_id = STUDY.study_id AND STUDY.type_id = STUDY_TYPE.type_id AND STUDY_TYPE.typename = 'Семинар';

	WITH t2
	AS (SELECT TOP (@num) GROUP_ON_STUDY.study_id, GROUP_ON_STUDY.group_id FROM GROUP_ON_STUDY, STUDY, STUDY_TYPE, 
				(SELECT inserted.group_id, inserted.teacher_id, inserted.study_id, inserted.auditorium_id 
					FROM inserted, STUDY, STUDY_TYPE 
					WHERE inserted.study_id = STUDY.study_id 
					AND STUDY.type_id = STUDY_TYPE.type_id 
					AND STUDY_TYPE.typename = 'Семинар') AS temp
				WHERE
					temp.group_id = GROUP_ON_STUDY.group_id 
					AND GROUP_ON_STUDY.study_id = STUDY.study_id
					AND STUDY.type_id = STUDY_TYPE.type_id
					AND STUDY_TYPE.typename = 'Лекция')

	/*Убираем по возможности такое же число лекций*/

	DELETE FROM GROUP_ON_STUDY
	WHERE study_id IN (SELECT study_id FROM t2) AND group_id IN (SELECT group_id FROM t2) AND study_id NOT IN (SELECT inserted.study_id 
																												FROM inserted, STUDY, STUDY_TYPE 
																												WHERE 
																													inserted.study_id = STUDY.study_id
																												AND STUDY.type_id = STUDY_TYPE.type_id
																												AND STUDY_TYPE.typename = 'Лекция')
END

/*Потестим:*/

/*Добавление одного семинара*/
BEGIN TRAN
	SELECT * FROM GROUP_ON_STUDY;
	INSERT INTO GROUP_ON_STUDY (group_id, teacher_id, study_id, auditorium_id) VALUES (3, 57, 17, 51);
	SELECT * FROM GROUP_ON_STUDY;
ROLLBACK

/*Добавление нескольких семинаров*/
BEGIN TRAN
	SELECT * FROM GROUP_ON_STUDY;
		INSERT INTO GROUP_ON_STUDY (group_id, teacher_id, study_id, auditorium_id)
		SELECT 3, 57, 17, 51 --семинар
		UNION ALL
		SELECT 3, 87, 20, 15 --семинар
		UNION ALL
		SELECT 3, 74, 34, 10 --лекция
	SELECT * FROM GROUP_ON_STUDY;
ROLLBACK


	