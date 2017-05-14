SELECT * FROM DEGREE;
/*Транзакция 1*/

/* SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED
* (Потерянные изменения, Грязное чтение, Неповторяющееся чтение, Фантом)
* (          -         ,        +      ,           +           ,    +  )
*/
/*Потерянные изменения*/
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

/*По определению на сайте*/
SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED
BEGIN TRANSACTION
	 	UPDATE DEGREE SET [name]='GOD' WHERE degree_id=2
COMMIT
SELECT [name] FROM DEGREE WHERE degree_id = 2;

/*Грязное чтение*/
SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
BEGIN TRANSACTION
  UPDATE DEGREE SET [name] = 'АБВ' WHERE degree_id = 2
	ROLLBACK

/* SET TRANSACTION ISOLATION LEVEL READ COMMITTED
* (Потерянные изменения, Грязное чтение, Неповторяющееся чтение, Фантом)
* (          -         ,        -      ,           +           ,    +  )
*/
/*Грязное чтение*/
SET TRANSACTION ISOLATION LEVEL READ COMMITTED
BEGIN TRANSACTION
  UPDATE DEGREE SET [name] = 'GOD' WHERE degree_id = 2
  ROLLBACK
/*Неповторяющееся чтение*/
SET TRANSACTION ISOLATION LEVEL READ COMMITTED
BEGIN TRANSACTION
 UPDATE DEGREE SET [name] = 'GOD' WHERE degree_id = 2
	COMMIT

/* SET TRANSACTION ISOLATION LEVEL REPEATABLE READ
* (Потерянные изменения, Грязное чтение, Неповторяющееся чтение, Фантом)
* (          -         ,        -      ,           -           ,    +  )
*/
/*Неповторяющееся чтение*/
SET TRANSACTION ISOLATION LEVEL REPEATABLE READ
BEGIN TRANSACTION
SELECT * FROM DEGREE WHERE degree_id = 2
SELECT * FROM DEGREE WHERE degree_id = 2
	COMMIT
/*Фантом*/
SET TRANSACTION ISOLATION LEVEL REPEATABLE READ
BEGIN TRANSACTION
SELECT * FROM DEGREE WHERE degree_id > 1
SELECT * FROM DEGREE WHERE degree_id > 1
	COMMIT

/* SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
* (Потерянные изменения, Грязное чтение, Неповторяющееся чтение, Фантом)
* (          -         ,        -      ,           -           ,    -  )
*/
/*Фантом*/
SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
BEGIN TRANSACTION
SELECT * FROM DEGREE WHERE degree_id > 1
SELECT * FROM DEGREE WHERE degree_id > 1
	COMMIT