### 试验环境

Windows10

MySQL 8.0.20



### 安装配置

1. 安装MySQL，添加$PATH\bin到环境路径
2. 登录MySQL：运行cmd，`mysql -h 主机名 -u 用户名 -p`
3. 退出登录：`\quit`



### 数据库控制语言(DCL)

#### **创建用户**

命令：**CREATE USER  'user_name'@'host'  IDENTIFIED BY  'password';**

host写%表示监听所有连接， IDENTIFIED BY  'password'可以不写，表示没有密码。

#### **删除用户**

命令：**DROP  USER  ‘user_name’@‘host’;** 

#### **授权用户**

命令：**GRANT ’privileges‘ ON  databasename.tablename  TO  ‘username’@‘host’;**

privileges：表示要授予什么权力，例如可以有 select ，insert ，delete，update等，如果要授予全部权力，则填 ALL PRIVILEGES，详细信息见：[Privileges Provided by MySQL](https://dev.mysql.com/doc/refman/8.0/en/privileges-provided.html)。

databasename.tablename：表示用户的权限能用在哪个库的哪个表中，如果想要用户的权限很作用于所有的数据库所有的表，则填 \*.\*，*是一个通配符，表示全部。

’username‘@‘host’：表示授权给哪个用户。

例：`grant all privileges on storage.* to 'easy'@'%';`

#### 撤销用户授权

命令：**REVOKE  privileges  ON  database.tablename  FROM  ‘username’@‘host’；**

#### 刷新权限设置

命令：**flush privileges;**

<br/>

### 数据库定义语言(DDL)

命令：**SHOW DATABASES;**，列出所有数据库。

命令：**USE 'database'**，选择数据库。

命令：**CREATE 'database';**，创建数据库。

命令：**DROP 'database';**，删除数据库。

命令：**SHOW TABLES;**，列出当前数据库的所有表。

命令：**SHOW GRANTS;**，显示授权用户的权限。

### 创建表

命令：**CREATE TABLE 'table_name' ('column1_name' 'column1_type' 'column1_limit', 'column2..., PRIMARY KEY ( 'column1_name' ))ENGINE=InnoDB DEFAULT CHARSET=utf8;**

命令：**SHOW COLUMNS FROM 'table';**，列出表定义，可以用**DESCRIBE 'table'**代替。

命令：**SHOW CREATE TABLE 'table_name'\G；**，显示创建表时的CREATE语句。

table_name、column_name都是自己命名的。

column_type：MySQL支持的[Data Types](https://dev.mysql.com/doc/refman/8.0/en/data-types.html)、[MySQL 数据类型](https://www.runoob.com/mysql/mysql-data-types.html)。

column_limit：`NOT NULL`-不能为空，`AUTO_INCREMENT`-自增，`PRIMARY KEY`-主键，`CHARACTER SET 'utf8'`-设置字符串类型，

PRIMARY KEY关键字用于定义列主键。 

ENGINE 设置存储引擎，CHARSET 设置编码。

### 数据库操纵语言(DML)

#### INSERT

```sql
INSERT INTO table_name [ (field1, field2,...fieldN) ] VALUES  (valueA1,valueA2,...valueAN),......,(valueN1,valueN2,...valueNN)
```



#### SELECT

```
SELECT 'column_name','column_name'
FROM 'table_name'
[WHERE Clause]
[LIMIT N][ OFFSET M]
```



#### UPDATE

```
update 表名 set 字段名 = 新的值  where 字段名 = 值
```



#### DELETE

```
delete from 表名 where 字段名称 = 值
```



### 参考

#### Sqlite支持的数据类型

https://www.runoob.com/sqlite/sqlite-data-types.html