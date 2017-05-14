/*****первый тестер*****/
CREATE SCHEMA X;

CREATE LOGIN tester_one WITH password = '123456';
CREATE USER tester_one FOR LOGIN tester_one WITH DEFAULT_SCHEMA= X;

REVOKE SELECT ON SCHEMA::X TO tester_one;
GRANT ALTER ON SCHEMA::X TO tester_one
GRANT CREATE TABLE TO tester_one

	CREATE TABLE X.TESTER_ONE (
		y int
	);

DROP TABLE TESTER_ONE;

/*****второй тестер*****/

CREATE SCHEMA Y;

--CREATE LOGIN tester_two WITH password = '123456';
CREATE USER tester_two WITHOUT LOGIN WITH DEFAULT_SCHEMA= Y;

GRANT ALTER ON SCHEMA::Y TO tester_two
GRANT CREATE TABLE TO tester_two

EXECUTE AS USER = 'tester_two';
	CREATE TABLE Y.TESTER_TWO (
		z int
	);

DROP TABLE TESTER_TWO;
	
