# C++ File Sharing Server

This project is a **lightweight, secure, and high-performance file-sharing server written in C++**, designed for efficient and reliable file transfer. It provides a RESTful API for file management and is optimized for scalability and ease of deployment.

## Project Status
- **Development Phase**: The project is under active development.
- **Objective**: To build a minimal, secure, and efficient file-sharing server with RESTful API support.

## Planned Features
- **File Upload & Download**: Secure and efficient file transfers.
- **HTTP/REST API Support**: RESTful endpoints for file operations.
- **Minimal Web Interface**: Simple UI for managing file uploads and downloads.

## Future Enhancements
- **Docker & Kubernetes Support**: Containerized deployment for scalability.
- **User Authentication & Access Control**: Role-based access management.
- **Database Integration**: PostgreSQL for file metadata storage.

## Installation & Setup
### Dependencies
Ensure the following libraries are installed before compiling:
- `crow`
- `asio`
- `libpq`
- `libpqxx`

### Clone the Repository
```bash
git clone https://github.com/l3ox64/lightshare
cd lightshare
```

### Manual Compilation with g++ on macos (arm)
Use the following command to compile the project:
```bash
g++ -std=c++17 \
-I/opt/homebrew/Cellar/crow/1.2.1.2/include \
-I/opt/homebrew/Cellar/asio/1.30.2/include \
-I/opt/homebrew/Cellar/libpqxx/7.10.0/include/ \
-L/opt/homebrew/Cellar/libpqxx/7.10.0/lib/ \
-L/opt/homebrew/Cellar/postgresql@14/14.17/lib/postgresql@14/ \
-Wl,-rpath,/opt/homebrew/Cellar/postgresql@14/14.17/lib/postgresql@14/ \
-o server main.cpp -lpqxx -lpq
```

## Contributions & Roadmap
- Contributions are welcome; please open an issue or submit a pull request.
- A formal roadmap will be provided as the project progresses.

### License
This project is licensed under [MIT License](LICENSE).
