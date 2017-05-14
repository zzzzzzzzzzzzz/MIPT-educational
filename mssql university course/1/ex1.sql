SELECT list_price, description 
	FROM PRICE, PRODUCT
	WHERE GETDATE() 
	BETWEEN start_date AND end_date OR end_date IS NULL 
	AND CHARINDEX('WIFF SOFTBALL', description) > 0    