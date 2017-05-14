/****************************************************************************************************************/
/*Построить SQL-запрос к таблице Customers с фильтрациями по идентификатору заказчика и нескольким другим полям.*/ 
/****************************************************************************************************************/

SELECT DISTINCT *
	FROM customers
	WHERE Country = 'Germany' 
	and PostalCode > '1001'
	and CustomerID >= 'BLAUS';

/*Добавим индексы*/

DROP INDEX CustomerIDindex ON Customers
CREATE CLUSTERED INDEX CustomerIDindex ON Customers
(
	CustomerID
)

DROP INDEX CountryIndex ON Customers
CREATE NONCLUSTERED INDEX CountryIndex ON Customers
(
	Country
)

DROP INDEX PostalCodeIndex ON Customers
CREATE NONCLUSTERED INDEX PostalCodeIndex ON Customers
(
	PostalCode
)



/***************************************************************************************************************/
/* Построить SQL-запрос к связанным таблицам Customers и Orders с фильтрациями по нескольким полям этих таблиц.*/
/***************************************************************************************************************/
SELECT DISTINCT *
	FROM Customers, Orders
	WHERE Customers.CustomerID = Orders.CustomerID 
 	AND Country = 'Germany'
	AND PostalCode > '1001'
	AND shipaddress = 'Mehrheimerstr. 369'

/*Добавим индексы*/

DROP INDEX CustomerIDindex ON Customers
CREATE CLUSTERED INDEX CustomerIDindex ON Customers
(
	CustomerID
)

DROP INDEX OrderIDindex ON Orders
CREATE CLUSTERED INDEX OrderIDindex ON Orders
(
	OrderID
)

DROP INDEX CustomerIDonOrdersIndex ON Orders
CREATE NONCLUSTERED INDEX CustomerIDonOrdersIndex ON Orders
(
	CustomerID
)

/***********************************************************************************************************************************************/
/*Построить SQL-запрос ко всем 4-м связанным таблицам Customers, Orders, OrderDetails и Products с фильтрациями по нескольким полям этих таблиц*/
/***********************************************************************************************************************************************/
SELECT DISTINCT *
	FROM customers
	JOIN orders ON customers.customerid = orders.customerid
	JOIN OrderDetails ON orders.orderid = [OrderDetails].orderid
	JOIN Products ON products.productid = [OrderDetails].productid
	AND Country = 'Germany' 
	AND PostalCode > '1001'
	AND shipaddress = 'Mehrheimerstr. 369'
	AND OrderDetails.quantity > 10
	AND Products.unitsinstock > 0

DROP INDEX country ON Customers
CREATE CLUSTERED INDEX country ON Customers
(
	country
)


DROP INDEX [add] ON Orders
CREATE CLUSTERED INDEX [add] ON Orders
(
	ShipAddress
)

DROP INDEX qu ON OrderDetails
CREATE CLUSTERED INDEX qu ON OrderDetails
(
	Quantity
)

DROP INDEX uis ON Products
CREATE CLUSTERED INDEX uis ON Products
(
	UnitsInStock
)