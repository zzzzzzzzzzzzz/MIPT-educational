IF OBJECT_ID ('CheckTypesExistence', 'TR') IS NOT NULL
   DROP TRIGGER CheckTypesExistence;

BEGIN TRAN
	ALTER TABLE STUDY
	DROP CONSTRAINT check_types_existence;

	CREATE TRIGGER CheckTypesExistence
	ON STUDY
	AFTER INSERT, UPDATE
	AS 
	BEGIN
		   DECLARE @num INT
			SELECT @num = COUNT(*) - COUNT(t2.type_id)
			FROM (SELECT DISTINCT subject_id, type_id FROM inserted) AS t1 
			LEFT OUTER JOIN 
			(SELECT subject_id, type_id FROM SUBJECT_STUDYTYPE) AS t2 
			ON t1.type_id = t2.type_id AND t1.subject_id = t2.subject_id;
			
		   IF @num <> 0
		   BEGIN
			 RAISERROR( 'You try to add study with type which not exists for this subject', 16, 1)
			 ROLLBACK TRANSACTION
		   END;		
	END;
ROLLBACK