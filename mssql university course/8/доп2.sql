/*Задача 2
	для всех представлений вывести все таблицы на которых эти представления основаны*/
SELECT VIEW_NAME, TABLE_NAME FROM 
	INFORMATION_SCHEMA.VIEW_TABLE_USAGE, sysobjects s1, sysusers
	WHERE s1.xtype='V'
	AND sysusers.[name] = CURRENT_USER AND s1.uid = sysusers.uid;