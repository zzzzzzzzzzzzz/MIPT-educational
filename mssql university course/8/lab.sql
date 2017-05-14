/*������ 1
	������� ������������, ������� �����, ��������� ������������ ��� �����, ���� ����� ������������
	� ���� ����� ��������� �������, ������� �������, ��������� ������ ������, O_o, ����������, ������.*/
/*������ 2
	��� ���� ������������� ������� ��� ������� �� ������� ��� ������������� ��������*/
/*������� ����� ���� ������, ��������� ����������� ������������� ���� ������*/
SELECT s1.name AS '���', sysusers.[name]
FROM sysobjects s1, sysusers
WHERE s1.xtype='U' AND sysusers.[name] = CURRENT_USER AND s1.uid = sysusers.uid
	AND s1.id NOT IN 
	(SELECT major_id FROM sys.extended_properties) 

SELECT * FROM sysusers;
/*������� ��� �������, ��� ������� �������, ������� ����, 
��������� �� ������ ������� null-��������, �������� ���� ������ ������� �������, 
������ ����� ���� ������ - ��� ���� ������, 
��������� ����������� ������������� ���� ������ � ���� �� ��������*/
SELECT s2.name as '��� �������', s1.name as '��� �������', s1.is_nullable as '������ NULL' , type_name(s1.user_type_id) AS '��� ������', s1.max_length AS '������ ���� ������'
	FROM sys.all_columns as s1, sys.sysobjects as s2, sysusers
	WHERE s2.id=s1.object_id 
		  AND s2.xtype='U'
		  AND sysusers.[name] = CURRENT_USER AND s2.uid = sysusers.uid



/*������� �������� ����������� ����������� (��������� � ������� �����), 
��� �������, � ������� ��� ���������, ������� ����, ��� ��� �� ����������� 
('PK' ��� ���������� ����� � 'F' ��� ��������) - ��� ���� ����������� �����������, 
��������� ����������� ������������� ���� ������*/
SELECT s1.name AS '��� �����������',s2.name AS '��� �������',s1.xtype AS '��� �����������'
FROM sysobjects s1, sysobjects s2, sysusers
WHERE s1.parent_obj=s2.id AND (s1.xtype='F' OR s1.xtype='PK') AND
	sysusers.[name] = CURRENT_USER AND s1.uid = sysusers.uid

/*������� �������� �������� �����, ��� �������, ���������� ������� ����, 
��� �������, ���������� ��� ������������ ���� - ��� ���� ������� ������, 
��������� ����������� ������������� ���� ������*/
SELECT s1.name AS '��� �������� �����',
	s2.name AS '��� ������� �������',
	s3.name AS '��� ������������ �������'
FROM sysobjects s1, sysobjects s2, sysobjects s3, sysreferences s4, sysusers
WHERE s2.id=s4.fkeyid AND s4.constid=s1.id AND s4.rkeyid=s3.id AND s1.xtype='F' AND
	sysusers.[name] = CURRENT_USER AND s1.uid = sysusers.uid


/*������� �������� �������������, SQL-������, ��������� ��� ������������� - 
��� ���� �������������, ��������� ����������� ������������� ���� ������*/
SELECT s1.name AS '���', s2.text AS '������'
FROM sysobjects s1, syscomments s2, sysusers
WHERE s1.id=s2.id AND s1.xtype='V' AND 
	sysusers.[name] = CURRENT_USER AND s1.uid = sysusers.uid

/*������� �������� ��������, ��� �������,
��� ������� ��������� ������� - ��� ���� ���������, ��������� ����������� ������������� ���� ������*/
SELECT s1.name AS '�������� ��������', s2.name AS '��� �������'
	FROM sysobjects s1, sysobjects s2, sysusers
		WHERE s1.xtype = 'TR'
		AND s1.parent_obj=s2.id
		AND sysusers.[name] = CURRENT_USER AND s1.uid = sysusers.uid
