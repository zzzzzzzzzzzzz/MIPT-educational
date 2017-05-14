/*CREATE TABLE SUBJECT (
	subject_id int IDENTITY(1,1) NOT NULL PRIMARY KEY,
	name varchar(50),
	obligation bit NOT NULL DEFAULT(1),
	lections_hours smallint NOT NULL CONSTRAINT lections_hours_amount CHECK(lections_hours >= 0),
	seminars_hours smallint NOT NULL CONSTRAINT seminars_hours_amount CHECK(seminars_hours >= 0),
	selfwork_hours smallint NOT NULL CONSTRAINT selfwork_hours_amount CHECK(selfwork_hours >= 0),
	labs_hours smallint NOT NULL CONSTRAINT labs_hours_amount CHECK(labs_hours >= 0)
);*/


CREATE TABLE SUBJECT_STUDYTYPE (
	subject_id int FOREIGN KEY REFERENCES SUBJECT(subject_id),
	type_id int FOREIGN KEY REFERENCES STUDY_TYPE(type_id),
	hours smallint NOT NULL CONSTRAINT hours_amount CHECK(hours > 0),
	PRIMARY KEY (subject_id, type_id)
);

INSERT INTO SUBJECT_STUDYTYPE (subject_id, type_id, hours)
SELECT SUBJECT.subject_id, STUDY_TYPE.type_id, SUBJECT.lections_hours as hours 
FROM SUBJECT, STUDY_TYPE
WHERE SUBJECT.lections_hours > 0 AND STUDY_TYPE.type_id = 1;

INSERT INTO SUBJECT_STUDYTYPE (subject_id, type_id, hours)
SELECT SUBJECT.subject_id, STUDY_TYPE.type_id, SUBJECT.seminars_hours as hours 
FROM SUBJECT, STUDY_TYPE 
WHERE SUBJECT.seminars_hours > 0 AND STUDY_TYPE.type_id = 2;

INSERT INTO SUBJECT_STUDYTYPE (subject_id, type_id, hours)
SELECT SUBJECT.subject_id, STUDY_TYPE.type_id, SUBJECT.labs_hours as hours 
FROM SUBJECT, STUDY_TYPE
WHERE SUBJECT.labs_hours > 0 AND STUDY_TYPE.type_id = 3;

ALTER TABLE STUDY
DROP CONSTRAINT check_types_existence
ALTER TABLE SUBJECT
DROP CONSTRAINT lections_hours_amount, seminars_hours_amount, labs_hours_amount
DROP FUNCTION dbo.CheckTypesExistence

GO
CREATE FUNCTION CheckTypesExistence()
RETURNS int
AS 
BEGIN
   DECLARE @retval int
   SELECT @retval = COUNT(*)
FROM STUDY, SUBJECT_STUDYTYPE 
WHERE STUDY.subject_id = SUBJECT_STUDYTYPE.subject_id AND SUBJECT_STUDYTYPE.type_id = STUDY.type_id;
   RETURN @retval
END;
GO
ALTER TABLE STUDY
ADD CONSTRAINT check_types_existence CHECK(dbo.CheckTypesExistence() > 0);
GO

ALTER TABLE SUBJECT
DROP COLUMN lections_hours, seminars_hours, labs_hours;