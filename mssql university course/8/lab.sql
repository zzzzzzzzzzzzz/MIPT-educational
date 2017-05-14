/*Задача 1
	Создаем пользвоателя, создаем схему, назначаем пользователю эту схему, даем права пользователю
	в этой схеме создавать таблицы, создаем таблицу, выполняем первый запрос, O_o, исправляем, почему.*/
/*Задача 2
	для всех представлений вывести все таблицы на которых эти представления основаны*/
/*Выбрать имена всех таблиц, созданных назначенным пользователем базы данных*/
SELECT s1.name AS 'Имя', sysusers.[name]
FROM sysobjects s1, sysusers
WHERE s1.xtype='U' AND sysusers.[name] = CURRENT_USER AND s1.uid = sysusers.uid
	AND s1.id NOT IN 
	(SELECT major_id FROM sys.extended_properties) 

SELECT * FROM sysusers;
/*Выбрать имя таблицы, имя столбца таблицы, признак того, 
допускает ли данный столбец null-значения, название типа данных столбца таблицы, 
размер этого типа данных - для всех таблиц, 
созданных назначенным пользователем базы данных и всех их столбцов*/
SELECT s2.name as 'Имя таблицы', s1.name as 'Имя столбца', s1.is_nullable as 'Допуск NULL' , type_name(s1.user_type_id) AS 'Тип данных', s1.max_length AS 'Размер типа данных'
	FROM sys.all_columns as s1, sys.sysobjects as s2, sysusers
	WHERE s2.id=s1.object_id 
		  AND s2.xtype='U'
		  AND sysusers.[name] = CURRENT_USER AND s2.uid = sysusers.uid



/*Выбрать название ограничения целостности (первичные и внешние ключи), 
имя таблицы, в которой оно находится, признак того, что это за ограничение 
('PK' для первичного ключа и 'F' для внешнего) - для всех ограничений целостности, 
созданных назначенным пользователем базы данных*/
SELECT s1.name AS 'Имя ограничения',s2.name AS 'Имя таблицы',s1.xtype AS 'Тип ограничения'
FROM sysobjects s1, sysobjects s2, sysusers
WHERE s1.parent_obj=s2.id AND (s1.xtype='F' OR s1.xtype='PK') AND
	sysusers.[name] = CURRENT_USER AND s1.uid = sysusers.uid

/*Выбрать название внешнего ключа, имя таблицы, содержащей внешний ключ, 
имя таблицы, содержащей его родительский ключ - для всех внешних ключей, 
созданных назначенным пользователем базы данных*/
SELECT s1.name AS 'Имя внешнего ключа',
	s2.name AS 'Имя внешней таблицы',
	s3.name AS 'Имя родительской таблицы'
FROM sysobjects s1, sysobjects s2, sysobjects s3, sysreferences s4, sysusers
WHERE s2.id=s4.fkeyid AND s4.constid=s1.id AND s4.rkeyid=s3.id AND s1.xtype='F' AND
	sysusers.[name] = CURRENT_USER AND s1.uid = sysusers.uid


/*Выбрать название представления, SQL-запрос, создающий это представление - 
для всех представлений, созданных назначенным пользователем базы данных*/
SELECT s1.name AS 'Имя', s2.text AS 'Скрипт'
FROM sysobjects s1, syscomments s2, sysusers
WHERE s1.id=s2.id AND s1.xtype='V' AND 
	sysusers.[name] = CURRENT_USER AND s1.uid = sysusers.uid

/*выбрать название триггера, имя таблицы,
для которой определен триггер - для всех триггеров, созданных назначенным пользователем базы данных*/
SELECT s1.name AS 'Название триггера', s2.name AS 'Имя таблицы'
	FROM sysobjects s1, sysobjects s2, sysusers
		WHERE s1.xtype = 'TR'
		AND s1.parent_obj=s2.id
		AND sysusers.[name] = CURRENT_USER AND s1.uid = sysusers.uid
