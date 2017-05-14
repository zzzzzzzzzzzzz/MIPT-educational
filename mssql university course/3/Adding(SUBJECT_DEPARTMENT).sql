/*Добавление таблицы предметы-кафедры*/

CREATE TABLE SUBJECT_DEPARTMENT (
	subject_id int FOREIGN KEY REFERENCES SUBJECT(subject_id),
	department_id int FOREIGN KEY REFERENCES DEPARTMENT(department_id),
	PRIMARY KEY (subject_id, department_id)
);

INSERT INTO SUBJECT_DEPARTMENT(subject_id, department_id) VALUES(1,2);
INSERT INTO SUBJECT_DEPARTMENT(subject_id, department_id) VALUES(2,4);
INSERT INTO SUBJECT_DEPARTMENT(subject_id, department_id) VALUES(3,5);
INSERT INTO SUBJECT_DEPARTMENT(subject_id, department_id) VALUES(4,7);
INSERT INTO SUBJECT_DEPARTMENT(subject_id, department_id) VALUES(5,4);
INSERT INTO SUBJECT_DEPARTMENT(subject_id, department_id) VALUES(6,6);
INSERT INTO SUBJECT_DEPARTMENT(subject_id, department_id) VALUES(7,1);
INSERT INTO SUBJECT_DEPARTMENT(subject_id, department_id) VALUES(8,4);
INSERT INTO SUBJECT_DEPARTMENT(subject_id, department_id) VALUES(9,2);
INSERT INTO SUBJECT_DEPARTMENT(subject_id, department_id) VALUES(10,3);

GO
CREATE FUNCTION CheckTeachersSubjects()
RETURNS int
AS 
BEGIN
   DECLARE @retval int
   SELECT @retval = COUNT(*)
FROM GROUP_ON_STUDY, TEACHER_SEAT, STUDY, SUBJECT_DEPARTMENT 
WHERE GROUP_ON_STUDY.teacher_id = TEACHER_SEAT.teacher_id 
	AND GROUP_ON_STUDY.study_id = STUDY.study_id 
	AND STUDY.subject_id = SUBJECT_DEPARTMENT.subject_id
	AND TEACHER_SEAT.department_id = SUBJECT_DEPARTMENT.department_id;
   RETURN @retval
END;
GO
ALTER TABLE GROUP_ON_STUDY
ADD CONSTRAINT check_teachers_subjects CHECK(dbo.CheckTypesExistence() > 0);
GO