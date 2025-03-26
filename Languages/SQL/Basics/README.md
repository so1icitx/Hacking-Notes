# Database and SQL 

## What Are Databases?

- **Definition**: Databases are structured collections of data that allow easy access, storage, and manipulation—like digital filing cabinets.
- **Why They Matter**: Used in nearly every system—TryHackMe stores your progress, Instagram saves posts, Netflix tracks your watch history. Even small businesses rely on them.
- **How They Work**: They store everything from user credentials to analytics, making them essential for apps and a prime target for hackers.

## Types of Databases

1. **Relational Databases (SQL-based)**
   - **Structure**: Data is stored in tables with rows and columns.
   - **Relationships**: Tables are linked using keys (Primary & Foreign Keys).
   - **Example Use Case**: E-commerce sites (products, orders, customers).
   - **Example**:
     ```sql
     CREATE TABLE Users (
         user_id INT PRIMARY KEY,
         username VARCHAR(50),
         email VARCHAR(100)
     );
     ```

2. **Non-Relational Databases (NoSQL-based)**
   - **Structure**: Data is stored in flexible formats like JSON, key-value pairs, or graphs.
   - **Use Case**: Social media posts, big data analytics.
   - **Example (JSON Document)**:
     ```json
     {
         "name": "John Doe",
         "age": 30,
         "interests": ["coding", "hacking"]
     }
     ```

## SQL (Structured Query Language)

SQL is used to create, manipulate, and query relational databases. It allows users to insert, retrieve, update, and delete data efficiently.

## SQL Database Commands

- **CREATE DATABASE**: Creates a new database.
  ```sql
  CREATE DATABASE bookstore;
  ```
- **SHOW DATABASES**: Lists available databases.
  ```sql
  SHOW DATABASES;
  ```
- **USE**: Selects a database for queries.
  ```sql
  USE bookstore;
  ```
- **DROP DATABASE**: Deletes a database permanently.
  ```sql
  DROP DATABASE bookstore;
  ```

## SQL Table Commands

- **CREATE TABLE**: Defines a new table.
  ```sql
  CREATE TABLE books (
      book_id INT AUTO_INCREMENT PRIMARY KEY,
      title VARCHAR(255) NOT NULL,
      author VARCHAR(100),
      price DECIMAL(6,2)
  );
  ```
- **SHOW TABLES**: Lists all tables in the active database.
  ```sql
  SHOW TABLES;
  ```
- **DESCRIBE**: Shows table structure.
  ```sql
  DESCRIBE books;
  ```
- **ALTER TABLE**: Modifies a table (e.g., adding a column).
  ```sql
  ALTER TABLE books ADD published_date DATE;
  ```
- **DROP TABLE**: Deletes a table.
  ```sql
  DROP TABLE books;
  ```

## CRUD Operations (Create, Read, Update, Delete)

- **INSERT INTO** (Adding Data)
  ```sql
  INSERT INTO books (title, author, price)
  VALUES ('Hacking 101', 'Alice Cyber', 19.99);
  ```

- **SELECT** (Retrieving Data)
  ```sql
  SELECT title, author FROM books;
  ```

- **UPDATE** (Modifying Data)
  ```sql
  UPDATE books SET price = 24.99 WHERE title = 'Hacking 101';
  ```

- **DELETE** (Removing Data)
  ```sql
  DELETE FROM books WHERE title = 'Hacking 101';
  ```

## SQL Clauses and Operators

- **DISTINCT**: Removes duplicates.
  ```sql
  SELECT DISTINCT author FROM books;
  ```
- **ORDER BY**: Sorts results.
  ```sql
  SELECT * FROM books ORDER BY price DESC;
  ```
- **LIKE**: Pattern matching.
  ```sql
  SELECT * FROM books WHERE title LIKE '%Hacking%';
  ```
- **BETWEEN**: Selects a range.
  ```sql
  SELECT * FROM books WHERE price BETWEEN 10 AND 30;
  ```
- **GROUP BY & HAVING**: Groups and filters data.
  ```sql
  SELECT author, COUNT(*) FROM books GROUP BY author HAVING COUNT(*) > 1;
  ```

## SQL Functions

- **COUNT()**: Counts rows.
  ```sql
  SELECT COUNT(*) FROM books;
  ```
- **SUM()**: Adds values.
  ```sql
  SELECT SUM(price) FROM books;
  ```
- **MAX() / MIN()**: Finds highest and lowest.
  ```sql
  SELECT MAX(price), MIN(price) FROM books;
  ```
- **LENGTH()**: Finds string length.
  ```sql
  SELECT title, LENGTH(title) FROM books;
  ```

## Big Examples Combining Multiple SQL Concepts

### Example 1: Retrieving All Books by a Specific Author, Sorted by Price
```sql
SELECT title, price
FROM books
WHERE author = 'Alice Cyber'
ORDER BY price DESC;
```

### Example 2: Finding the Total Number of Books and Average Price Per Author
```sql
SELECT author, COUNT(*) AS total_books, AVG(price) AS avg_price
FROM books
GROUP BY author;
```

### Example 3: Selecting Books That Have "Hacking" in the Title and Cost Less Than $30
```sql
SELECT title, price
FROM books
WHERE title LIKE '%Hacking%' AND price < 30;
```

### Example 4: Deleting All Books Published Before 2010
```sql
DELETE FROM books WHERE published_date < '2010-01-01';
```

