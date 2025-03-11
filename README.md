# C++ File Sharing Server

This project is a **lightweight and secure file-sharing server written in C++**, designed to provide a **fast and easy-to-use** solution for file sharing.  

## 🔧 Project Status
**Work in Progress** – This project is under active development.  
**Goal** – Build a simple and secure file-sharing server.  

## 🛠️ Planned Features
**File upload & download**  
**Support for HTTP/REST API**  
**Minimal web interface**  

## 🚀 Future Enhancements
**Docker & Kubernetes support** for scalability and easy deployment.  

## 🏗️ Installation & Setup
**This project is in development, setup instructions will be updated soon.**  
**Dependency** 
```bash
crow asio libpq libpqxx 
```

### **Clone the Repository**
```bash
git clone https://github.com/your-username/your-repo.git
cd your-repo
```

### **Manual startup via G++**
```bash
g++ -std=c++17 \
-I/opt/homebrew/Cellar/crow/1.2.1.2/include \
-I/opt/homebrew/Cellar/asio/1.30.2/include \
-I/opt/homebrew/Cellar/libpqxx/7.10.0/include/ \
-L/opt/homebrew/Cellar/libpqxx/7.10.0/lib/ \
-L/opt/homebrew/Cellar/postgresql@14/14.17/lib/postgresql@14/ \
-Wl,-rpath,/opt/homebrew/Cellar/postgresql@14/14.17/lib/postgresql@14/ \
-o test main.cpp -lpqxx -lpq
´´´
