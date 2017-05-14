CREATE TABLE PERIODICITY (
	periodicity_id int IDENTITY(1,1) NOT NULL PRIMARY KEY,
	name varchar(30) NOT NULL UNIQUE,
	in_days int NOT NULL CONSTRAINT periodicity_days UNIQUE CHECK(in_days > 0) 
);
GO
CREATE FUNCTION trimCheckPeriodicity()
RETURNS int
AS 
BEGIN
   DECLARE @retval int
   SELECT @retval = COUNT(*)
FROM
PERIODICITY as res1, PERIODICITY as res2
WHERE res1.periodicity_id <> res2.periodicity_id AND LTRIM(RTRIM(res1.name)) = LTRIM(RTRIM(res2.name)) 
   RETURN @retval
END;
GO
ALTER TABLE PERIODICITY
ADD CONSTRAINT trim_check CHECK(dbo.trimCheckPeriodicity() = 0);
GO

CREATE TABLE STUDY_TYPE (
	type_id int IDENTITY(1,1) NOT NULL PRIMARY KEY,
	typename varchar(30) NOT NULL UNIQUE
);

CREATE TABLE INSPECTION (
	inspection_id int IDENTITY(1,1) NOT NULL PRIMARY KEY,
	typename varchar(30) NOT NULL UNIQUE
);

CREATE TABLE SUBJECT (
	subject_id int IDENTITY(1,1) NOT NULL PRIMARY KEY,
	name varchar(50),
	obligation bit NOT NULL DEFAULT(1),
	lections_hours smallint NOT NULL CONSTRAINT lections_hours_amount CHECK(lections_hours >= 0),
	seminars_hours smallint NOT NULL CONSTRAINT seminars_hours_amount CHECK(seminars_hours >= 0),
	selfwork_hours smallint NOT NULL CONSTRAINT selfwork_hours_amount CHECK(selfwork_hours >= 0),
	labs_hours smallint NOT NULL CONSTRAINT labs_hours_amount CHECK(labs_hours >= 0)
);

CREATE TABLE INSPECTION_SUBJECT (
	inspection_id int FOREIGN KEY REFERENCES INSPECTION(inspection_id),
	subject_id int FOREIGN KEY REFERENCES SUBJECT(subject_id),
	PRIMARY KEY (inspection_id, subject_id)
);

CREATE TABLE STUDY (
	study_id int IDENTITY(1,1) NOT NULL PRIMARY KEY,
	begin_date datetime NOT NULL,
	end_date datetime NOT NULL,
	begin_time datetime NOT NULL,
	end_time datetime NOT NULL,
	periodicity_id int FOREIGN KEY REFERENCES PERIODICITY(periodicity_id),
	type_id int FOREIGN KEY REFERENCES STUDY_TYPE(type_id),
	subject_id int FOREIGN KEY REFERENCES SUBJECT(subject_id)
);
GO
CREATE FUNCTION CheckTypesExistence()
RETURNS int
AS 
BEGIN
   DECLARE @retval int
   SELECT @retval = COUNT(*)
FROM STUDY, SUBJECT 
WHERE (type_id = 1 AND STUDY.subject_id = SUBJECT.subject_id AND lections_hours = 0) 
OR (type_id = 2 AND STUDY.subject_id = SUBJECT.subject_id AND seminars_hours = 0)
OR (type_id = 3 AND STUDY.subject_id = SUBJECT.subject_id AND labs_hours = 0);
   RETURN @retval
END;
GO
ALTER TABLE STUDY
ADD CONSTRAINT check_types_existence CHECK(dbo.CheckTypesExistence() = 0);
GO

CREATE TABLE BUILDING (
	building_id int IDENTITY(1,1) NOT NULL PRIMARY KEY,
	name varchar(40) NOT NULL UNIQUE
);

CREATE TABLE AUDITORIUM (
	auditorium_id int IDENTITY(1,1) NOT NULL PRIMARY KEY,
	name varchar(20) NOT NULL,
	building_id int FOREIGN KEY REFERENCES BUILDING(building_id)
);

CREATE TABLE DEGREE (
	degree_id int IDENTITY(1,1) NOT NULL PRIMARY KEY,
	name varchar(50) NOT NULL UNIQUE
);

CREATE TABLE DEPARTMENT (
	department_id int IDENTITY(1,1) NOT NULL PRIMARY KEY,
	name varchar(50) NOT NULL UNIQUE
);

CREATE TABLE SUBJECT_DEPARTMENT (
	subject_id int FOREIGN KEY REFERENCES SUBJECT(subject_id),
	department_id int FOREIGN KEY REFERENCES DEPARTMENT(department_id),
	PRIMARY KEY (subject_id, department_id)
);

CREATE TABLE SEAT (
	seat_id int IDENTITY(1,1) NOT NULL PRIMARY KEY,
	name varchar(50) NOT NULL UNIQUE
);

CREATE TABLE TEACHER (
	teacher_id int IDENTITY(1,1) NOT NULL PRIMARY KEY,
	second_name varchar(50) NOT NULL,
	first_name varchar(50) NOT NULL,
	patronymic varchar(50) NULL,
	degree_id int FOREIGN KEY REFERENCES DEGREE(degree_id)
);

CREATE TABLE TEACHER_SEAT (
	seat_id int FOREIGN KEY REFERENCES SEAT(seat_id),
	teacher_id int FOREIGN KEY REFERENCES TEACHER(teacher_id),
	department_id int FOREIGN KEY REFERENCES DEPARTMENT(department_id),
	PRIMARY KEY (teacher_id, department_id)
);

CREATE TABLE GROUP_TYPE(
	group_type_id int IDENTITY(1,1) NOT NULL PRIMARY KEY,
	typename varchar(40) NOT NULL UNIQUE
);

CREATE TABLE [GROUP] (
	group_id int IDENTITY(1,1) NOT NULL PRIMARY KEY,
	groupname varchar(30) UNIQUE NOT NULL,
	group_type_id int FOREIGN KEY REFERENCES GROUP_TYPE(group_type_id)
);

CREATE TABLE STUDENT (
	student_id int IDENTITY(1,1) NOT NULL PRIMARY KEY,
	second_name varchar(50) NOT NULL,
	first_name varchar(50) NOT NULL,
	patronymic varchar(50) NULL
);

CREATE TABLE STUDENT_GROUP (
	student_id int FOREIGN KEY REFERENCES STUDENT(student_id),
	group_id int FOREIGN KEY REFERENCES [GROUP](group_id),
	PRIMARY KEY (student_id, group_id),
);

GO
CREATE FUNCTION CheckStudentsAmount()
RETURNS int
AS 
BEGIN
   DECLARE @retval int
   SELECT @retval = COUNT(*)
FROM 
(SELECT COUNT(*) as amount_of_students, group_id FROM STUDENT_GROUP GROUP BY group_id) as res
WHERE res.amount_of_students > 30
   RETURN @retval
END;
GO
ALTER TABLE STUDENT_GROUP
ADD CONSTRAINT check_amount CHECK(dbo.CheckStudentsAmount() = 0);
GO 

CREATE TABLE GROUP_ON_STUDY (
	group_id int FOREIGN KEY REFERENCES [GROUP](group_id),
	teacher_id int FOREIGN KEY REFERENCES TEACHER(teacher_id),
	study_id int FOREIGN KEY REFERENCES STUDY(study_id),
	auditorium_id int FOREIGN KEY REFERENCES AUDITORIUM(auditorium_id),
	PRIMARY KEY (group_id, study_id)
);
GO
CREATE FUNCTION CheckRightGroupsOnPares()
RETURNS int
AS 
BEGIN
   DECLARE @retval int
   SELECT @retval = COUNT(*)
FROM GROUP_ON_STUDY, [GROUP], STUDY, SUBJECT
WHERE ([GROUP].group_type_id = 1 AND [GROUP].group_id = GROUP_ON_STUDY.group_id AND GROUP_ON_STUDY.study_id = STUDY.study_id AND (STUDY.subject_id IN (3,4,7))) 
OR ([GROUP].group_type_id = 2 AND [GROUP].group_id = GROUP_ON_STUDY.group_id AND GROUP_ON_STUDY.study_id = STUDY.study_id AND (STUDY.subject_id != 4))
OR ([GROUP].group_type_id = 3 AND [GROUP].group_id = GROUP_ON_STUDY.group_id AND GROUP_ON_STUDY.study_id = STUDY.study_id AND (STUDY.subject_id != 7))
OR ([GROUP].group_type_id = 4 AND [GROUP].group_id = GROUP_ON_STUDY.group_id AND GROUP_ON_STUDY.study_id = STUDY.study_id AND (STUDY.subject_id != 3));
   RETURN @retval
END;
GO
ALTER TABLE GROUP_ON_STUDY
ADD CONSTRAINT check_right_groups_on_pares CHECK(dbo.CheckRightGroupsOnPares() = 0);
GO