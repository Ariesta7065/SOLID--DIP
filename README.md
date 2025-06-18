#  Restaurant Management System - DIP Implementation (C++)

Demonstrasi implementasi **Dependency Inversion Principle (DIP)** menggunakan C++ dengan studi kasus sistem manajemen restoran.

##  Deskripsi Tugas

Implementasi SOLID Principles khususnya **Dependency Inversion Principle** dengan mendemonstrasikan:

1. ** Problem**: Kode yang melanggar DIP (tight coupling)
2. ** Solution 1**: Dependency Injection
3. ** Solution 2**: Factory Pattern  
4. ** Solution 3**: Strategy Pattern
5. ** Testing**: Mock objects untuk unit testing

##  Video Demonstrasi

**Link Video**: [(https://youtu.be/gmqgi7nXoH0?si=cWzkJjPJbq_D3Fwl)]

Video mencakup:
-  Penjelasan konsep DIP dengan analogi nyata
-  Live coding dari masalah ke solusi
-  Demonstrasi 3 teknik implementasi
-  Comparison before vs after
-  Benefits dan real-world applications

##  Struktur Project

```
restaurant-management-cpp/
├── src/
│   └── restaurant_dip_demo.cpp    # Complete implementation
├── Makefile                       # Build configuration
├── README.md                      # Documentation
└── docs/
    ├── before_after_comparison.md
    └── benefits_analysis.md
```

##  Cara Menjalankan

### Prerequisites
- C++ compiler (g++, clang++)
- Make (optional, untuk easier build)
- C++14 atau lebih baru

### Quick Start:
```bash
# Clone repository
git clone https://github.com/[username]/restaurant-management-cpp.git
cd restaurant-management-cpp

# Build dan run dengan Makefile
make run

# Atau compile manual
g++ -std=c++14 -Wall -o restaurant_dip_demo src/restaurant_dip_demo.cpp
./restaurant_dip_demo
```

### Build Options:
```bash
make           # Build project
make run       # Build and run demo
make clean     # Clean build files
make debug     # Debug build
make release   # Optimized build
make help      # Show help
```

##  Before vs After Comparison

###  BEFORE (Violation)
```cpp
class BadRestaurantService {
private:
    MySQLDatabase_Bad* database;          //  Direct dependency
    EmailNotification_Bad* notification;  //  Direct dependency

public:
    BadRestaurantService() {
        database = new MySQLDatabase_Bad();      //  Hard coded
        notification = new EmailNotification_Bad(); //  Hard coded
    }
}
```

**Problems:**
- Tight coupling ke concrete classes
- Sulit testing (tidak bisa mock)
- Sulit ganti implementation
- Maintenance nightmare

###  AFTER (DIP Compliant)
```cpp
class GoodRestaurantService {
private:
    shared_ptr<DatabaseService> database;      //  Depend on abstraction
    shared_ptr<NotificationService> notification; //  Depend on abstraction

public:
    // Constructor Injection
    GoodRestaurantService(shared_ptr<DatabaseService> db, 
                         shared_ptr<NotificationService> notif) 
        : database(db), notification(notif) {}
}
```

**Benefits:**
- Loose coupling via interfaces
- Easy testing dengan mock objects
- Flexible implementation switching
- Maintainable dan extensible

##  Teknik yang Diimplementasikan

### 1. Dependency Injection
```cpp
// Interface definition
class DatabaseService {
public:
    virtual ~DatabaseService() = default;
    virtual void save(const Order& order) = 0;
    virtual Order findById(int id) = 0;
};

// Constructor injection
GoodRestaurantService(shared_ptr<DatabaseService> db, 
                     shared_ptr<NotificationService> notif);
```

### 2. Factory Pattern
```cpp
class DatabaseFactory {
public:
    static shared_ptr<DatabaseService> createDatabase(const string& type) {
        if (type == "mysql") return make_shared<MySQLDatabase>();
        if (type == "postgresql") return make_shared<PostgreSQLDatabase>();
        // ... more implementations
    }
};
```

### 3. Strategy Pattern
```cpp
class PaymentStrategy {
public:
    virtual void processPayment(double amount) = 0;
    virtual bool validatePayment(const string& info) = 0;
};

// Runtime strategy switching
processor.setStrategy(make_shared<CreditCardStrategy>());
processor.setStrategy(make_shared<DigitalWalletStrategy>());
```

##  Contoh Output Program

```
 RESTAURANT MANAGEMENT SYSTEM - DIP DEMO
============================================================

 PROBLEM: DIP Violation Example
----------------------------------------
 BadRestaurantService: Created with tight coupling
 MySQL: Saving to MySQL database: Order{id=1, description='Nasi Gudeg Special', amount=$35.000000}
 Email: Sending email - Order 1 processed!
 Order processed with TIGHT COUPLING
   Problem: Sulit ganti database atau notification!

 SOLUTION 1: Dependency Injection
----------------------------------------
 GoodRestaurantService: Created with MySQL + Email
 MySQL: Saving to MySQL database: Order{id=2, description='Sate Ayam Madura', amount=$28.500000}
 Email: Order 2 processed successfully!
 Order processed with LOOSE COUPLING (DIP compliant)

 Easy to switch implementations:
 GoodRestaurantService: Created with PostgreSQL +
