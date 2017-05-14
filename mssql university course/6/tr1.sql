SELECT * FROM DEGREE;
/*���������� 1*/

/* SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED
* (���������� ���������, ������� ������, ��������������� ������, ������)
* (          -         ,        +      ,           +           ,    +  )
*/
/*���������� ���������*/
DROP TABLE tmp;

            CREATE TABLE tmp (id int, tmp_value int)
            INSERT INTO tmp VALUES (1,1)
			INSERT INTO tmp VALUES (2,2)
			INSERT INTO tmp VALUES (3,3)
			INSERT INTO tmp VALUES (4,4)
			INSERT INTO tmp VALUES (5,5)
            go
            SELECT * FROM tmp

SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED
BEGIN TRANSACTION
  UPDATE tmp SET tmp_value = (SELECT selfwork_hours FROM  SUBJECT WHERE subject_id = 1)
            WHERE id = 1;
  UPDATE SUBJECT SET selfwork_hours = (SELECT tmp_value FROM tmp WHERE id = 1) + 1 WHERE subject_id = 1;
  COMMIT
SELECT selfwork_hours FROM SUBJECT WHERE subject_id = 1;

/*�� ����������� �� �����*/
SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED
BEGIN TRANSACTION
	 	UPDATE DEGREE SET [name]='GOD' WHERE degree_id=2
COMMIT
SELECT [name] FROM DEGREE WHERE degree_id = 2;

/*������� ������*/
SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
BEGIN TRANSACTION
  UPDATE DEGREE SET [name] = '���' WHERE degree_id = 2
	ROLLBACK

/* SET TRANSACTION ISOLATION LEVEL READ COMMITTED
* (���������� ���������, ������� ������, ��������������� ������, ������)
* (          -         ,        -      ,           +           ,    +  )
*/
/*������� ������*/
SET TRANSACTION ISOLATION LEVEL READ COMMITTED
BEGIN TRANSACTION
  UPDATE DEGREE SET [name] = 'GOD' WHERE degree_id = 2
  ROLLBACK
/*��������������� ������*/
SET TRANSACTION ISOLATION LEVEL READ COMMITTED
BEGIN TRANSACTION
 UPDATE DEGREE SET [name] = 'GOD' WHERE degree_id = 2
	COMMIT

/* SET TRANSACTION ISOLATION LEVEL REPEATABLE READ
* (���������� ���������, ������� ������, ��������������� ������, ������)
* (          -         ,        -      ,           -           ,    +  )
*/
/*��������������� ������*/
SET TRANSACTION ISOLATION LEVEL REPEATABLE READ
BEGIN TRANSACTION
SELECT * FROM DEGREE WHERE degree_id = 2
SELECT * FROM DEGREE WHERE degree_id = 2
	COMMIT
/*������*/
SET TRANSACTION ISOLATION LEVEL REPEATABLE READ
BEGIN TRANSACTION
SELECT * FROM DEGREE WHERE degree_id > 1
SELECT * FROM DEGREE WHERE degree_id > 1
	COMMIT

/* SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
* (���������� ���������, ������� ������, ��������������� ������, ������)
* (          -         ,        -      ,           -           ,    -  )
*/
/*������*/
SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
BEGIN TRANSACTION
SELECT * FROM DEGREE WHERE degree_id > 1
SELECT * FROM DEGREE WHERE degree_id > 1
	COMMIT