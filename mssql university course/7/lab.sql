DROP LOGIN test ;
DROP USER test;
DROP ROLE test_role;

/*�������� login � user � ����, ��� �������� ������� � ���� ������,
 ������� ������������ ��������� ������������ ���� public ������ ���� ������*/
CREATE LOGIN test WITH password = '123456';
CREATE USER test FOR LOGIN test;

/*������������� ����� �� �������*/
GRANT INSERT, SELECT, DELETE, UPDATE ON AUDITORIUM TO test;
GRANT INSERT, SELECT, DELETE, UPDATE ON BUILDING TO test;
GRANT INSERT, SELECT, DELETE, UPDATE ON STUDENT TO test;
GRANT SELECT, UPDATE ([name]) ON DEGREE TO test;
GRANT SELECT ON TEACHER TO test;

/*������������� SELECT � ������������� (�������������, �������, ���������� �������� � ����� ������������� ���������)*/
GRANT SELECT ON sixth_view TO test;


/* ������ ���� test_role � ������� SELECT � UPDATE �� ������� obligation
 �� ������������� (������, �������, ��������������),
 ��������� ��� ���� ���������� ������������ test*/
CREATE ROLE test_role;
GRANT SELECT, UPDATE (obligation) ON seventh_view TO test_role;
EXEC sp_addrolemember test_role, test;

/*��������� ������������ ������ ��������� ������� � ���� ������*/
EXEC sp_droprolemember db_ddladmin, test;--����� ����������� ���������, �������������� � ������� ������� ���� ������
/*����� ������*/
GRANT CREATE TABLE TO 'PaRRoT-��\test';
REVOKE CONNECT TO test;
GRANT CREATE TABLE TO guest;
GRANT CONNECT TO guest;
/*���������*/
SELECT * FROM fn_my_permissions(NULL, 'DATABASE');

ALTER DATABASE university SET TRUSTWORTHY ON;

/****************************/
/*��������� ����������� test*/
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
	SET name = '�������� ����'
WHERE degree_id = 1;

INSERT INTO AUDITORIUM (name, building_id) VALUES('109', 1);

/*�� �����*/
INSERT INTO DEGREE(name) VALUES('��������');

/*�� �����*/
INSERT INTO TEACHER(second_name, first_name, patronymic, degree_id) VALUES('���', '���', '�������', 1);

/*�� �����*/
UPDATE TEACHER
	SET first_name = '��������'
WHERE teacher_id = 1;

SELECT * FROM sixth_view;

SELECT * FROM seventh_view;

/*�� �����*/
UPDATE seventh_view
	SET subjname = '�����������'
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