/*CREATE FUNCTION NumbersTable (
  @fromNumber int,
  @toNumber int,
  @byStep int
)
RETURNS @NumbersTable TABLE (i int)
AS
BEGIN

  WITH CTE_NumbersTable AS (

    SELECT @fromNumber AS i

    UNION ALL

    SELECT i + @byStep
    FROM CTE_NumbersTable
    WHERE
      (i + @byStep) <= @toNumber
  )
  INSERT INTO @NumbersTable
  SELECT i FROM CTE_NumbersTable OPTION (MAXRECURSION 0)

  RETURN;
END */
/*SELECT temp.teacher_id, temp.second_name, temp.first_name, temp.patronymic 
FROM
	(SELECT DISTINCT TEACHER.teacher_id, second_name, first_name, patronymic, STUDY.study_id, STUDY.begin_date, STUDY.end_date
		FROM GROUP_ON_STUDY, STUDY, TEACHER
		WHERE STUDY.study_id = GROUP_ON_STUDY.study_id AND GROUP_ON_STUDY.teacher_id = TEACHER.teacher_id) as temp		
CROSS APPLY NumbersTable(1, DATEDIFF(day ,temp.begin_date, temp.end_date), 1) --n*/

/*Выбрать преподавателей, у которых наибольшее количество пар весной 2015г. */
SELECT TOP 1 WITH TIES teacher_id, second_name, first_name, patronymic, COUNT(*) as studies
FROM
	(SELECT temp.teacher_id, temp.second_name, temp.first_name, temp.patronymic 
		FROM
		(SELECT DISTINCT TEACHER.teacher_id, second_name, first_name, patronymic, STUDY.study_id, STUDY.begin_date, STUDY.end_date, in_days
			FROM GROUP_ON_STUDY, STUDY, TEACHER, PERIODICITY
			WHERE STUDY.study_id = GROUP_ON_STUDY.study_id AND GROUP_ON_STUDY.teacher_id = TEACHER.teacher_id AND STUDY.periodicity_id = PERIODICITY.periodicity_id) as temp		
		CROSS APPLY NumbersTable(1, DATEDIFF(day ,temp.begin_date, temp.end_date)/(temp.in_days), 1)) as table_of_studies
GROUP BY teacher_id, second_name, first_name, patronymic
ORDER BY studies DESC