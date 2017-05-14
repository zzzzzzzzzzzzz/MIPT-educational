/*для каждого сотрудника посчитать количество подчиненных (рекурсивно)*/
WITH EMPLOYEE_RECURSIVE(employee_id, manager_id) 
AS (SELECT employee_id, manager_id
	FROM EMPLOYEE
	UNION ALL
		SELECT e.employee_id, e.manager_id
		FROM EMPLOYEE AS e
			JOIN EMPLOYEE_RECURSIVE AS er ON e.employee_id = er.manager_id)
SELECT manager_id, COUNT(*)
FROM EMPLOYEE_RECURSIVE
WHERE manager_id IS NOT NULL
GROUP BY manager_id
ORDER BY manager_id


