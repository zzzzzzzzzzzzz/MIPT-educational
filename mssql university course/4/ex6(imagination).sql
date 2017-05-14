/*Вывести 5 кафедр с наибольшим числом преподавателей*/
SELECT TOP 5 DEPARTMENT.department_id, DEPARTMENT.name, COUNT(*) as 'Число преподавателей' 
FROM TEACHER_SEAT, DEPARTMENT
WHERE TEACHER_SEAT.department_id = DEPARTMENT.department_id
GROUP BY DEPARTMENT.department_id, DEPARTMENT.name
ORDER BY 'Число преподавателей' DESC;