IF OBJECT_ID ('CheckStudentsAmount', 'TR') IS NOT NULL
   DROP TRIGGER CheckStudentsAmount;

BEGIN TRAN
	ALTER TABLE STUDENT_GROUP
	DROP CONSTRAINT check_amount;

	CREATE TRIGGER CheckStudentsAmount
	ON STUDENT_GROUP
	AFTER INSERT, UPDATE
	AS 
	BEGIN
		WITH temp
		AS	(SELECT count(STUDENT_GROUP.student_id) AS num
			FROM inserted, [GROUP], STUDENT_GROUP
			WHERE inserted.group_id = [GROUP].group_id AND [GROUP].group_id = STUDENT_GROUP.group_id
			GROUP BY inserted.group_id)

		DECLARE @x INT
		SELECT @x = COUNT(8) FROM temp WHERE temp.num > 30;

		IF @x > 0 
		   BEGIN
			 RAISERROR( 'There are a lot of people in group',16, 1)
			 ROLLBACK TRANSACTION
		   END;	
	END;
ROLLBACK