SELECT DISTINCT name, regional_group
FROM DEPARTMENT, LOCATION, EMPLOYEE, JOB
WHERE EMPLOYEE.department_id = DEPARTMENT.department_id 
	AND EMPLOYEE.job_id = JOB.job_id 
	AND commission > 0.0
	AND commission IS NOT NULL
	AND NOT JOB.[function] = 'SALESPERSON'