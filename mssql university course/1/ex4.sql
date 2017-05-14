SELECT TOP 1 SUM(total) AS min_sales
FROM EMPLOYEE, CUSTOMER, SALES_ORDER, DEPARTMENT, LOCATION
WHERE SALES_ORDER.customer_id = CUSTOMER.customer_id 
	AND CUSTOMER.salesperson_id = employee_id
	AND EMPLOYEE.department_id = DEPARTMENT.department_id
	AND DEPARTMENT.location_id = LOCATION.location_id
	AND regional_group = 'NEW YORK' 
GROUP BY employee_id, regional_group
ORDER BY min_sales
