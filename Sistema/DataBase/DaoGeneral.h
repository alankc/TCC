/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DaoGeneral.h
 * Author: x
 *
 * Created on 11 de Abril de 2018, 14:05
 */

#ifndef DAOGENERAL_H
#define DAOGENERAL_H

#include <string>

#include <driver.h>
#include <exception.h>
#include <resultset.h>
#include <prepared_statement.h>

class DaoGeneral
{
public:
    DaoGeneral(std::string host, std::string schema, std::string user, std::string password);
    virtual ~DaoGeneral();
    
    sql::ResultSet* ExecuteQuery(std::string statement);
    bool ExecuteUpdate(std::string statement);
    bool ExecuteUpdate(std::vector<std::string> statements);
private:
    std::string host;
    std::string schema;
    std::string user;
    std::string password;  
};

#endif /* DAOGENERAL_H */

