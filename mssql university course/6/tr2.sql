/*���������� 2*/

/* SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED
* (���������� ���������, ������� ������, ��������������� ������, ������)
* (          -         ,        +      ,           +           ,    +  )
*/
/*���������� ���������*/
SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED
BEGIN TRANSACTION
  UPDATE SUBJECT SET selfwork_hours = (SELECT tmp_value FROM tmp WHERE id = 1) + 2 WHERE subject_id = 1;
  COMMIT
SELECT selfwork_hours FROM SUBJECT WHERE subject_id = 1;

/*�� ����������� �� �����*/
SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED
BEGIN TRANSACTION
	 	UPDATE DEGREE SET [name]='������ ����' WHERE degree_id=2
COMMIT

/*������� ������*/
SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED
BEGIN TRANSACTION
SELECT * FROM DEGREE WHERE degree_id = 2
SELECT * FROM DEGREE WHERE degree_id = 2
SELECT * FROM DEGREE WHERE degree_id = 2
COMMIT


/* SET TRANSACTION ISOLATION LEVEL READ COMMITTED
* (���������� ���������, ������� ������, ��������������� ������, ������)
* (          -         ,        -      ,           +           ,    +  )
*/
/*������� ������*/
SET TRANSACTION ISOLATION LEVEL READ COMMITTED
BEGIN TRANSACTION
SELECT * FROM DEGREE WHERE degree_id = 2
SELECT * FROM DEGREE WHERE degree_id = 2
SELECT * FROM DEGREE WHERE degree_id = 2
COMMIT
/*��������������� ������*/
BEGIN TRANSACTION
SET TRANSACTION ISOLATION LEVEL READ COMMITTED
SELECT * FROM DEGREE WHERE degree_id = 2
SELECT * FROM DEGREE WHERE degree_id = 2
COMMIT
/* SET TRANSACTION ISOLATION LEVEL REPEATABLE READ
* (���������� ���������, ������� ������, ��������������� ������, ������)
* (          -         ,        -      ,           -           ,    +  )
*/
/*��������������� ������*/
SET TRANSACTION ISOLATION LEVEL REPEATABLE READ
BEGIN TRANSACTION
 UPDATE DEGREE SET [name] = 'DELETED' WHERE degree_id = 2
	COMMIT
/*������*/
SET TRANSACTION ISOLATION LEVEL REPEATABLE READ
BEGIN TRANSACTION
INSERT INTO DEGREE ([name]) VALUES ('Achtung');
	COMMIT

/* SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
* (���������� ���������, ������� ������, ��������������� ������, ������)
* (          -         ,        -      ,           -           ,    -  )
*/
/*������*/
SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
BEGIN TRANSACTION
INSERT INTO DEGREE ([name]) VALUES ('Cookie');
	COMMIT

DELETE FROM DEGREE WHERE ( [name] = 'Cookie');
DELETE FROM DEGREE WHERE ( [name] = 'Achtung');