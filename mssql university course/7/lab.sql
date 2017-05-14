DROP LOGIN test ;
DROP USER test;
DROP ROLE test_role;

/*Создания login и user к нему, при открытии доступа к базе данных,
 каждому пользователю автоматом выставляется роль public уровня базы данных*/
CREATE LOGIN test WITH password = '123456';
CREATE USER test FOR LOGIN test;

/*Предоставляем права по заданию*/
GRANT INSERT, SELECT, DELETE, UPDATE ON AUDITORIUM TO test;
GRANT INSERT, SELECT, DELETE, UPDATE ON BUILDING TO test;
GRANT INSERT, SELECT, DELETE, UPDATE ON STUDENT TO test;
GRANT SELECT, UPDATE ([name]) ON DEGREE TO test;
GRANT SELECT ON TEACHER TO test;

/*Предоставляем SELECT к представлению (Преподаватель, предмет, количество учащихся у этого преподавателя студентов)*/
GRANT SELECT ON sixth_view TO test;


/* создаём роль test_role с правами SELECT и UPDATE на столбец obligation
 на представление (Группа, предмет, обязательность),
 назначаем эту роль созданному пользователю test*/
CREATE ROLE test_role;
GRANT SELECT, UPDATE (obligation) ON seventh_view TO test_role;
EXEC sp_addrolemember test_role, test;

/*Позволяем пользователю ТОЛЬКО СОЗДАВАТЬ таблицы в базе данных*/
EXEC sp_droprolemember db_ddladmin, test;--имеет возможность создавать, модифицировать и удалять объекты базы данных
/*уберём лишнее*/
GRANT CREATE TABLE TO 'PaRRoT-ПК\test';
REVOKE CONNECT TO test;
GRANT CREATE TABLE TO guest;
GRANT CONNECT TO guest;
/*посмотрим*/
SELECT * FROM fn_my_permissions(NULL, 'DATABASE');

ALTER DATABASE university SET TRUSTWORTHY ON;

/****************************/
/*Проверяем возможности test*/
/****************************/
SELECT * FROM AUDITORIUM;
SELECT * FROM BUILDING;
SELECT * FROM STUDENT;
SELECT * FROM DEGREE;
SELECT * FROM TEACHER; 

UPDATE AUDITORIUM
	SET name = '111'
WHERE auditorium_id = 1;

UPDATE DEGREE
	SET name = 'Кандидат наук'
WHERE degree_id = 1;

INSERT INTO AUDITORIUM (name, building_id) VALUES('109', 1);

/*не можно*/
INSERT INTO DEGREE(name) VALUES('динозавр');

/*не можно*/
INSERT INTO TEACHER(second_name, first_name, patronymic, degree_id) VALUES('Хоп', 'Хэй', 'Лалалей', 1);

/*не можно*/
UPDATE TEACHER
	SET first_name = 'Владимир'
WHERE teacher_id = 1;

SELECT * FROM sixth_view;

SELECT * FROM seventh_view;

/*не можно*/
UPDATE seventh_view
	SET subjname = 'Физкультура'
WHERE groupname = '112';

BEGIN TRANSACTION
	UPDATE seventh_view
		SET obligation = 0
	WHERE obligation = 1
	ROLLBACK


SELECT * FROM fn_my_permissions('fifth_view', 'OBJECT') 
    ORDER BY subentity_name, permission_name ;  
REVERT;
GO

REVOKE SELECT ON fifth_view TO test;

GRANT SELECT ON fifth_view TO test;

DENY SELECT ON fifth_view TO test;

SELECT * FROM fifth_view;
GO

SELECT * FROM sysusers WHERE uid=5;