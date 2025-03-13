#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <pqxx/pqxx>
#include <filesystem>
#include "env.h"

using namespace std;
namespace fs = filesystem;

class Database {
private:
    unique_ptr<pqxx::connection> conn;
    string DB_HOST, DB_PORT, DB_NAME, DB_USER, DB_PASSWORD, DB_TABLE;

public:
    Database() {
        try {
            string env_path = fs::absolute(".env").string();
            load_env_file(env_path);

            // Store environment variables as class members
            DB_HOST = get_env("DB_HOST");
            DB_PORT = get_env("DB_PORT");
            DB_NAME = get_env("DB_NAME");
            DB_USER = get_env("DB_USER");
            DB_PASSWORD = get_env("DB_PASSWORD");
            DB_TABLE = get_env("DB_TABLE");

            // PostgreSQL connection string
            string connStr = "host=" + DB_HOST +
                             " port=" + DB_PORT +
                             " dbname=" + DB_NAME +
                             " user=" + DB_USER +
                             " password=" + DB_PASSWORD;

            // Connect to the database
            conn = make_unique<pqxx::connection>(connStr);

            if (conn->is_open()) {
                cout << "Connected to PostgreSQL at: " << DB_HOST << endl;
            } else {
                throw runtime_error("Connection to PostgreSQL failed!");
            }

        } catch (const exception& e) {
            cerr << "Database connection error: " << e.what() << "\n";
        }
    }

    ~Database() {
        if (conn && conn->is_open()) {
            cout << "Closing database connection.\n";
            conn->close();
        }
    }
	
    // CREATE 
    bool createUser(const string& username, const string& password) {
        return executeQuery("INSERT INTO " + DB_TABLE +
                            " (username, password) VALUES (" +
                            conn->quote(username) + ", " + conn->quote(password) + ")",
                            "User created: " + username);
    }

    // READ
    bool readUser(const string& username) {
        try {
            pqxx::nontransaction txn(*conn);
            string query = "SELECT username FROM " + DB_TABLE +
                           " WHERE username = " + txn.quote(username);
            pqxx::result result = txn.exec(query);
            return !result.empty();  // Returns true if user exists
        } catch (const exception& e) {
            cerr << "Error reading from DB: " << e.what() << "\n";
            return false;
        }
    }

    // UPDATE
    bool updateUser(const string& username, const string& newPassword) {
        return executeQuery("UPDATE " + DB_TABLE +
                            " SET password = " + conn->quote(newPassword) +
                            " WHERE username = " + conn->quote(username),
                            "Password updated for: " + username);
    }

    // DELETE
    bool deleteUser(const string& username) {
        return executeQuery("DELETE FROM " + DB_TABLE +
                            " WHERE username = " + conn->quote(username),
                            "User removed: " + username);
    }

private:
    // Execute query helper function
    bool executeQuery(const string& query, const string& successMessage) {
        try {
            pqxx::work txn(*conn);
            txn.exec(query);
            txn.commit();
            cout << successMessage << "\n";
            return true;
        } catch (const std::exception& e) {
            cerr << "Query execution error: " << e.what() << "\n";
            return false;
        }
    }
};

#endif // DATABASE_H
