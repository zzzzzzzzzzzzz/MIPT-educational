/*������� ����� ���� ������� �� ������������ �� ��������, �� �������� ��� ������������*/

/*SELECT * FROM STUDY;
SELECT * FROM STUDY_TYPE;*/

UPDATE STUDY 
SET type_id = 3
FROM STUDY WHERE study_id = 1; 
/*��������� ����������� check_types_existence*/