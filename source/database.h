#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <pqxx/pqxx>

// 📌 Costanti per la connessione a PostgreSQL
constexpr auto DB_HOST = "localhost";       // Indirizzo del server PostgreSQL
constexpr auto DB_PORT = "5432";            // Porta predefinita di PostgreSQL
constexpr auto DB_NAME = "l3ox";            // Nome del database
constexpr auto DB_USER = "l3ox";            // Nome utente del database
constexpr auto DB_PASSWORD = "l3ox";        // Password del database
constexpr auto DB_TABLE = "crowapp";        // Nome della tabella

class Database {
public:
    Database() {
        try {
            // Costruisce la stringa di connessione a PostgreSQL
            std::string connStr = "host=" + std::string(DB_HOST) +
                                  " port=" + std::string(DB_PORT) +
                                  " dbname=" + std::string(DB_NAME) +
                                  " user=" + std::string(DB_USER) +
                                  " password=" + std::string(DB_PASSWORD);

            // Crea la connessione
            conn = std::make_unique<pqxx::connection>(connStr);

            if (conn->is_open()) {
                std::cout << "✅ Connessione riuscita a PostgreSQL\n";
            } else {
                throw std::runtime_error("❌ Errore di connessione a PostgreSQL");
            }
        } catch (const std::exception& e) {
            std::cerr << "❌ Errore di connessione: " << e.what() << "\n";
        }
    }

    ~Database() {
        if (conn && conn->is_open()) {
            std::cout << "🔌 Connessione chiusa.\n";
        }
    }

    // 📌 Crea un nuovo utente nella tabella
    bool createUser(const std::string& username, const std::string& password) {
        return executeQuery("INSERT INTO " + std::string(DB_TABLE) +
                            " (username, password) VALUES (" +
                            conn->quote(username) + ", " + conn->quote(password) + ")",
                            "✅ Utente creato: " + username);
    }

    // 📌 Legge un utente dalla tabella
    bool readUser(const std::string& username) {
        try {
            pqxx::nontransaction txn(*conn);
            std::string query = "SELECT username FROM " + std::string(DB_TABLE) +
                                " WHERE username = " + txn.quote(username);
            pqxx::result result = txn.exec(query);

            if (!result.empty()) {
                std::cout << "✅ Utente trovato: " << result[0][0].c_str() << "\n";
                return true;
            } else {
                std::cout << "❌ Utente non trovato: " << username << "\n";
                return false;
            }
        } catch (const std::exception& e) {
            std::cerr << "❌ Errore lettura utente: " << e.what() << "\n";
            return false;
        }
    }

    // 📌 Aggiorna la password di un utente
    bool updateUser(const std::string& username, const std::string& newPassword) {
        return executeQuery("UPDATE " + std::string(DB_TABLE) +
                            " SET password = " + conn->quote(newPassword) +
                            " WHERE username = " + conn->quote(username),
                            "✅ Password aggiornata per: " + username);
    }

    // 📌 Cancella un utente dalla tabella
    bool deleteUser(const std::string& username) {
        return executeQuery("DELETE FROM " + std::string(DB_TABLE) +
                            " WHERE username = " + conn->quote(username),
                            "✅ Utente eliminato: " + username);
    }

private:
    std::unique_ptr<pqxx::connection> conn;

    // 📌 Funzione generica per eseguire query con messaggi di output
    bool executeQuery(const std::string& query, const std::string& successMessage) {
        try {
            pqxx::work txn(*conn);
            txn.exec(query);
            txn.commit();
            std::cout << successMessage << "\n";
            return true;
        } catch (const std::exception& e) {
            std::cerr << "❌ Errore esecuzione query: " << e.what() << "\n";
            return false;
        }
    }
};

#endif // DATABASE_H

