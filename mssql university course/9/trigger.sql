/*�������: ��� ���������� ������ ��������, �� �����������, ����������� ����� ������ �� ������� �������� */

IF OBJECT_ID ('regulate', 'TR') IS NOT NULL
   DROP TRIGGER regulate;

CREATE TRIGGER regulate
ON GROUP_ON_STUDY
AFTER INSERT
AS 
BEGIN
	/*������� ���������� ����������� ���������*/
	DECLARE @num INT
		SELECT  @num = count(8)
		FROM STUDY_TYPE, inserted, STUDY	
		WHERE inserted.study_id = STUDY.study_id AND STUDY.type_id = STUDY_TYPE.type_id AND STUDY_TYPE.typename = '�������';

	WITH t2
	AS (SELECT TOP (@num) GROUP_ON_STUDY.study_id, GROUP_ON_STUDY.group_id FROM GROUP_ON_STUDY, STUDY, STUDY_TYPE, 
				(SELECT inserted.group_id, inserted.teacher_id, inserted.study_id, inserted.auditorium_id 
					FROM inserted, STUDY, STUDY_TYPE 
					WHERE inserted.study_id = STUDY.study_id 
					AND STUDY.type_id = STUDY_TYPE.type_id 
					AND STUDY_TYPE.typename = '�������') AS temp
				WHERE
					temp.group_id = GROUP_ON_STUDY.group_id 
					AND GROUP_ON_STUDY.study_id = STUDY.study_id
					AND STUDY.type_id = STUDY_TYPE.type_id
					AND STUDY_TYPE.typename = '������')

	/*������� �� ����������� ����� �� ����� ������*/

	DELETE FROM GROUP_ON_STUDY
	WHERE study_id IN (SELECT study_id FROM t2) AND group_id IN (SELECT group_id FROM t2) AND study_id NOT IN (SELECT inserted.study_id 
																												FROM inserted, STUDY, STUDY_TYPE 
																												WHERE 
																													inserted.study_id = STUDY.study_id
																												AND STUDY.type_id = STUDY_TYPE.type_id
																												AND STUDY_TYPE.typename = '������')
END

/*��������:*/

/*���������� ������ ��������*/
BEGIN TRAN
	SELECT * FROM GROUP_ON_STUDY;
	INSERT INTO GROUP_ON_STUDY (group_id, teacher_id, study_id, auditorium_id) VALUES (3, 57, 17, 51);
	SELECT * FROM GROUP_ON_STUDY;
ROLLBACK

/*���������� ���������� ���������*/
BEGIN TRAN
	SELECT * FROM GROUP_ON_STUDY;
		INSERT INTO GROUP_ON_STUDY (group_id, teacher_id, study_id, auditorium_id)
		SELECT 3, 57, 17, 51 --�������
		UNION ALL
		SELECT 3, 87, 20, 15 --�������
		UNION ALL
		SELECT 3, 74, 34, 10 --������
	SELECT * FROM GROUP_ON_STUDY;
ROLLBACK


	