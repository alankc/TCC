/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DaoGeneral.cpp
 * Author: x
 * 
 * Created on 11 de Abril de 2018, 14:05
 */

#include "DaoGeneral.h"

DaoGeneral::DaoGeneral(std::string host, std::string schema, std::string user, std::string password) : host(host), schema(schema), user(user), password(password)
{

}

DaoGeneral::~DaoGeneral()
{
}

sql::ResultSet* DaoGeneral::ExecuteQuery(std::string statement)
{
    try
    {
        sql::Driver *driver;
        sql::Connection *con;
        sql::PreparedStatement *pstmt;
        sql::ResultSet* res;

        //Create a connection
        driver = get_driver_instance();
        con = driver->connect(host, user, password);
        //Connect to the MySQL test database 
        con->setSchema(schema);

        pstmt = con->prepareStatement(statement);

        res = pstmt->executeQuery();

        con->close();
        
        delete con;
        delete pstmt;

        return res;
    }
    catch (sql::SQLException &e)
    {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        return NULL;
    }
}

bool DaoGeneral::ExecuteUpdate(std::string statement)
{
    try
    {
        sql::Driver *driver;
        sql::Connection *con;
        sql::PreparedStatement *pstmt;

        //Create a connection
        driver = get_driver_instance();
        con = driver->connect(host, user, password);
        //Connect to the MySQL test database 
        con->setSchema(schema);

        pstmt = con->prepareStatement(statement);
        auto vlr = pstmt->executeUpdate();

        con->close();
        
        delete con;
        delete pstmt;

        if (vlr == 0)
            return false;

        return true;
    }
    catch (sql::SQLException &e)
    {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        return false;
    }
}

bool DaoGeneral::ExecuteUpdate(std::vector<std::string> statements)
{
    sql::Driver *driver;
    sql::Connection *con = NULL;
    sql::Statement *stmt = NULL;
    sql::PreparedStatement *pstmt = NULL;

    try
    {
        //Create a connection
        driver = get_driver_instance();
        con = driver->connect(host, user, password);
        //Connect to the MySQL test database 
        con->setSchema(schema);

        stmt = con->createStatement();
        stmt->execute("START TRANSACTION;");

        for (auto statement : statements)
        {
            pstmt = con->prepareStatement(statement);
            pstmt->executeUpdate();
        }

        con->commit();
        con->close();
        
        delete con;
        delete stmt;
        delete pstmt;

        return true;
    }
    catch (sql::SQLException &e)
    {
        con->rollback();
        con->close();
        
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        return false;
    }
}

