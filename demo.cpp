#include <iostream>
#include <string>
#include <memory>

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/statement.h>

#define MYSQL_HOST "localhost:3306"
#define MYSQL_USER "root"
#define MYSQL_PASSWD "rootme"
#define MYSQL_DATABASE "recommend_sys"

int main() {
    sql::mysql::MySQL_Driver* driver;

    driver = sql::mysql::get_mysql_driver_instance();
    std::unique_ptr<sql::Connection> con(driver->connect(MYSQL_HOST, MYSQL_USER, MYSQL_PASSWD));
    con->setSchema(MYSQL_DATABASE);
    std::unique_ptr<sql::Statement> stmt(con->createStatement());
    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("select * from movies"));

    while (res->next())
        std::cout << res->getInt("movie_id") << ", " << res->getString("movie_name") << std::endl;

    return 0;
}
