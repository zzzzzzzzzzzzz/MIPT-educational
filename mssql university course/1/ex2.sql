SELECT name AS department_name, regional_group, COUNT(*) AS number_of_analysts
FROM DEPARTMENT, LOCATION, EMPLOYEE, JOB
WHERE DEPARTMENT.location_id = LOCATION.location_id 
	AND (JOB.[function] = 'ANALYST' AND JOB.job_id = EMPLOYEE.job_id AND DEPARTMENT.department_id = EMPLOYEE.department_id)
GROUP BY name, regional_group;