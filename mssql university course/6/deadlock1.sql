/*111*/
SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED
BEGIN TRANSACTION
	UPDATE tmp SET tmp_value=2 WHERE id=2
	UPDATE tmp SET tmp_value=3 WHERE id=3
COMMIT
/*111*/
