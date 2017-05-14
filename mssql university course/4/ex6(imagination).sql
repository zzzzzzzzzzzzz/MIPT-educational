/*������� 5 ������ � ���������� ������ ��������������*/
SELECT TOP 5 DEPARTMENT.department_id, DEPARTMENT.name, COUNT(*) as '����� ��������������' 
FROM TEACHER_SEAT, DEPARTMENT
WHERE TEACHER_SEAT.department_id = DEPARTMENT.department_id
GROUP BY DEPARTMENT.department_id, DEPARTMENT.name
ORDER BY '����� ��������������' DESC;