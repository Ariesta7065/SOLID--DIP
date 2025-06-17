#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <stdexcept>

using namespace std;

// ==================== AFTER: DIP COMPLIANT ====================
//  GOOD EXAMPLE - Loose Coupling with Abstractions

//  STEP 1: Create Abstractions (Interfaces)
class DatabaseService {
public:
    virtual ~DatabaseService() = default;
    virtual void save(const Order& order) = 0;
    virtual Order findById(int id) = 0;
    virtual string getType() const = 0;
};

class NotificationService {
public:
    virtual ~NotificationService() = default;
    virtual void send(const string& message) = 0;
    virtual string getType() const = 0;
};

//  STEP 2: Concrete Implementations
class MySQLDatabase : public DatabaseService {
public:
    void save(const Order& order) override {
        cout << " MySQL: Saving to MySQL database: " << order.toString() << endl;
    }

    Order findById(int id) override {
        return Order(id, "MySQL Order #" + to_string(id), 25.99);
    }

    string getType() const override { return "MySQL"; }
};

class PostgreSQLDatabase : public DatabaseService {
public:
    void save(const Order& order) override {
        cout << " PostgreSQL: Saving to PostgreSQL database: " << order.toString() << endl;
    }

    Order findById(int id) override {
        return Order(id, "PostgreSQL Order #" + to_string(id), 29.99);
    }

    string getType() const override { return "PostgreSQL"; }
};

class MongoDatabase : public DatabaseService {
public:
    void save(const Order& order) override {
        cout << " MongoDB: Saving to MongoDB database: " << order.toString() << endl;
    }

    Order findById(int id) override {
        return Order(id, "MongoDB Order #" + to_string(id), 27.50);
    }

    string getType() const override { return "MongoDB"; }
};

class EmailNotification : public NotificationService {
public:
    void send(const string& message) override {
        cout << " Email: " << message << endl;
    }

    string getType() const override { return "Email"; }
};

class SMSNotification : public NotificationService {
public:
    void send(const string& message) override {
        cout << " SMS: " << message << endl;
    }

    string getType() const override { return "SMS"; }
};

class SlackNotification : public NotificationService {
public:
    void send(const string& message) override {
        cout << " Slack: " << message << endl;
    }

    string getType() const override { return "Slack"; }
};

//  SOLUTION 1: DEPENDENCY INJECTION
class GoodRestaurantService {
private:
    shared_ptr<DatabaseService> database;
    shared_ptr<NotificationService> notification;

public:
    // Constructor Injection - depend on abstractions!
    GoodRestaurantService(shared_ptr<DatabaseService> db,
        shared_ptr<NotificationService> notif)
        : database(db), notification(notif) {
        cout << " GoodRestaurantService: Created with "
            << db->getType() << " + " << notif->getType() << endl;
    }

    void processOrder(const Order& order) {
        database->save(order);
        notification->send("Order " + to_string(order.getId()) + " processed successfully!");
        cout << " Order processed with LOOSE COUPLING (DIP compliant)" << endl;
    }

    Order getOrder(int id) {
        return database->findById(id);
    }

    string getConfiguration() const {
        return database->getType() + " + " + notification->getType();
    }
};

//  SOLUTION 2: FACTORY PATTERN
class DatabaseFactory {
public:
    static shared_ptr<DatabaseService> createDatabase(const string& type) {
        if (type == "mysql") {
            return make_shared<MySQLDatabase>();
        }
        else if (type == "postgresql") {
            return make_shared<PostgreSQLDatabase>();
        }
        else if (type == "mongodb") {
            return make_shared<MongoDatabase>();
        }
        else {
            throw invalid_argument("Unknown database type: " + type);
        }
    }
};

class NotificationFactory {
public:
    static shared_ptr<NotificationService> createNotification(const string& type) {
        if (type == "email") {
            return make_shared<EmailNotification>();
        }
        else if (type == "sms") {
            return make_shared<SMSNotification>();
        }
        else if (type == "slack") {
            return make_shared<SlackNotification>();
        }
        else {
            throw invalid_argument("Unknown notification type: " + type);
        }
    }
};

class RestaurantManager {
private:
    shared_ptr<GoodRestaurantService> restaurantService;

public:
    void initialize(const string& dbType, const string& notificationType) {
        cout << "  Initializing restaurant with " << dbType
            << " and " << notificationType << endl;

        auto database = DatabaseFactory::createDatabase(dbType);
        auto notification = NotificationFactory::createNotification(notificationType);

        restaurantService = make_shared<GoodRestaurantService>(database, notification);
    }

    void processOrder(const Order& order) {
        if (restaurantService) {
            restaurantService->processOrder(order);
        }
        else {
            cout << " Restaurant not initialized!" << endl;
        }
    }

    string getConfiguration() const {
        return restaurantService ? restaurantService->getConfiguration() : "Not initialized";
    }
};

//  SOLUTION 3: STRATEGY PATTERN
class PaymentStrategy {
public:
    virtual ~PaymentStrategy() = default;
    virtual void processPayment(double amount) = 0;
    virtual bool validatePayment(const string& paymentInfo) = 0;
    virtual string getPaymentType() const = 0;
};

class CreditCardStrategy : public PaymentStrategy {
public:
    void processPayment(double amount) override {
        cout << " Processing credit card payment: $" << amount << endl;
    }

    bool validatePayment(const string& paymentInfo) override {
        return paymentInfo.length() == 16; // Simple validation
    }

    string getPaymentType() const override { return "Credit Card"; }
};

class DigitalWalletStrategy : public PaymentStrategy {
public:
    void processPayment(double amount) override {
        cout << " Processing digital wallet payment: $" << amount << endl;
    }

    bool validatePayment(const string& paymentInfo) override {
        return !paymentInfo.empty(); // Simple validation
    }

    string getPaymentType() const override { return "Digital Wallet"; }
};

class CashStrategy : public PaymentStrategy {
public:
    void processPayment(double amount) override {
        cout << " Processing cash payment: $" << amount << endl;
    }

    bool validatePayment(const string& paymentInfo) override {
        return true; // Cash always valid
    }

    string getPaymentType() const override { return "Cash"; }
};

class PaymentProcessor {
private:
    shared_ptr<PaymentStrategy> strategy;

public:
    PaymentProcessor(shared_ptr<PaymentStrategy> strat) : strategy(strat) {
        cout << " PaymentProcessor initialized with: " << strat->getPaymentType() << endl;
    }

    void setStrategy(shared_ptr<PaymentStrategy> newStrategy) {
        strategy = newStrategy;
        cout << " Payment strategy changed to: " << strategy->getPaymentType() << endl;
    }

    bool processOrderPayment(const Order& order) {
        cout << " Processing payment for order: " << order.getId() << endl;

        if (strategy->validatePayment(order.getPaymentInfo())) {
            strategy->processPayment(order.getTotalAmount());
            cout << " Payment successful!" << endl;
            return true;
        }
        else {
            cout << " Payment validation failed!" << endl;
            return false;
        }
    }

    string getCurrentStrategy() const {
        return strategy->getPaymentType();
    }
};

// ==================== MOCK CLASSES FOR TESTING ====================
class MockDatabase : public DatabaseService {
public:
    void save(const Order& order) override {
        cout << " MOCK DATABASE: Save called for order " << order.getId() << endl;
    }

    Order findById(int id) override {
        return Order(id, "Mock Order", 0.0);
    }

    string getType() const override { return "Mock Database"; }
};

class MockNotification : public NotificationService {
public:
    void send(const string& message) override {
        cout << " MOCK NOTIFICATION: " << message << endl;
    }

    string getType() const override { return "Mock Notification"; }
};

// ==================== DEMO FUNCTIONS ====================

void printSeparator(const string& title) {
    cout << "\n" << string(60, '=') << endl;
    cout << "  " << title << endl;
    cout << string(60, '=') << endl;
}

void printSubSeparator(const string& title) {
    cout << "\n" << string(40, '-') << endl;
    cout << "  " << title << endl;
    cout << string(40, '-') << endl;
}

void demonstrateProblem() {
    printSubSeparator(" PROBLEM: DIP Violation Example");

    cout << "Mari kita lihat kode yang melanggar DIP:" << endl;
    cout << "- RestaurantService depend langsung ke MySQLDatabase" << endl;
    cout << "- RestaurantService depend langsung ke EmailNotification" << endl;
    cout << "- Hard coded dependencies di constructor" << endl << endl;

    BadRestaurantService badService;
    Order order1(1, "Nasi Gudeg Special", 35.00);
    badService.processOrder(order1);

    cout << "\n Masalah:" << endl;
    cout << "   1. Sulit ganti database (MySQL ke PostgreSQL)" << endl;
    cout << "   2. Sulit ganti notification (Email ke SMS)" << endl;
    cout << "   3. Sulit testing (tidak bisa mock)" << endl;
    cout << "   4. Tight coupling = maintenance nightmare!" << endl;
}

void demonstrateDependencyInjection() {
    printSubSeparator(" SOLUTION 1: Dependency Injection");

    cout << "Dependency Injection: Inject dependencies dari luar!" << endl;
    cout << "- Depend on abstractions (interfaces)" << endl;
    cout << "- Constructor injection" << endl;
    cout << "- Loose coupling achieved!" << endl << endl;

    // MySQL + Email
    auto mysqlDb = make_shared<MySQLDatabase>();
    auto emailNotif = make_shared<EmailNotification>();
    GoodRestaurantService service1(mysqlDb, emailNotif);

    Order order2(2, "Sate Ayam Madura", 28.50);
    service1.processOrder(order2);

    cout << "\n Easy to switch implementations:" << endl;

    // PostgreSQL + SMS  
    auto postgresDb = make_shared<PostgreSQLDatabase>();
    auto smsNotif = make_shared<SMSNotification>();
    GoodRestaurantService service2(postgresDb, smsNotif);

    Order order3(3, "Rendang Padang", 42.00);
    service2.processOrder(order3);

    cout << "\n Benefits:" << endl;
    cout << "    Flexible - mudah ganti implementation" << endl;
    cout << "    Testable - bisa inject mock objects" << endl;
    cout << "    Maintainable - perubahan terisolasi" << endl;
}

void demonstrateFactoryPattern() {
    printSubSeparator(" SOLUTION 2: Factory Pattern");

    cout << "Factory Pattern: Create objects tanpa specify exact class" << endl;
    cout << "- Configuration-based object creation" << endl;
    cout << "- Easy to add new implementations" << endl;
    cout << "- Centralized object creation logic" << endl << endl;

    RestaurantManager manager;

    // MongoDB + Slack
    manager.initialize("mongodb", "slack");
    Order order4(4, "Gado-gado Jakarta", 22.00);
    manager.processOrder(order4);

    cout << "\n Easy configuration switch:" << endl;

    // PostgreSQL + Email
    manager.initialize("postgresql", "email");
    Order order5(5, "Bakso Malang", 18.50);
    manager.processOrder(order5);

    cout << "\n Real-world usage:" << endl;
    cout << "   - Load from config file: database.type=mysql" << endl;
    cout << "   - Environment variables: NOTIF_TYPE=slack" << endl;
    cout << "   - Runtime configuration changes" << endl;
}

void demonstrateStrategyPattern() {
    printSubSeparator(" SOLUTION 3: Strategy Pattern");

    cout << "Strategy Pattern: Family of algorithms yang interchangeable" << endl;
    cout << "- Runtime algorithm switching" << endl;
    cout << "- Encapsulate algorithm families" << endl;
    cout << "- Open/Closed principle compliance" << endl << endl;

    Order order6(6, "Ayam Bakar Taliwang", 45.00);

    // Credit Card Payment
    auto creditCardStrategy = make_shared<CreditCardStrategy>();
    PaymentProcessor processor(creditCardStrategy);

    order6.setPaymentInfo("credit_card", "1234567890123456");
    processor.processOrderPayment(order6);

    cout << "\n Runtime strategy switching:" << endl;

    // Digital Wallet Payment
    auto walletStrategy = make_shared<DigitalWalletStrategy>();
    processor.setStrategy(walletStrategy);
    order6.setPaymentInfo("wallet", "wallet123");
    processor.processOrderPayment(order6);

    // Cash Payment
    auto cashStrategy = make_shared<CashStrategy>();
    processor.setStrategy(cashStrategy);
    order6.setPaymentInfo("cash", "");
    processor.processOrderPayment(order6);

    cout << "\n Use cases:" << endl;
    cout << "   - Payment methods (Credit Card, Wallet, Cash)" << endl;
    cout << "   - Sorting algorithms (QuickSort, MergeSort)" << endl;
    cout << "   - Compression algorithms (ZIP, RAR, 7Z)" << endl;
}

void demonstrateTesting() {
    printSubSeparator(" TESTING: Easy Mocking with DIP");

    cout << "DIP membuat testing menjadi sangat mudah!" << endl;
    cout << "- Mock objects untuk isolate testing" << endl;
    cout << "- No external dependencies needed" << endl;
    cout << "- Fast and reliable unit tests" << endl << endl;

    auto mockDb = make_shared<MockDatabase>();
    auto mockNotif = make_shared<MockNotification>();
    GoodRestaurantService testService(mockDb, mockNotif);

    Order testOrder(999, "Test Order", 99.99);
    testService.processOrder(testOrder);

    cout << "\n Testing benefits:" << endl;
    cout << "    No database connection needed" << endl;
    cout << "    No email service needed" << endl;
    cout << "    Fast test execution" << endl;
    cout << "    Predictable test results" << endl;
}

void demonstrateBenefits() {
    printSubSeparator("🌟 BENEFITS SUMMARY");

    cout << "Dependency Inversion Principle memberikan:" << endl << endl;

    cout << " FLEXIBILITY:" << endl;
    cout << "   - Mudah ganti implementation tanpa ubah client code" << endl;
    cout << "   - Support multiple implementations simultaneously" << endl << endl;

    cout << " TESTABILITY:" << endl;
    cout << "   - Easy mocking untuk unit testing" << endl;
    cout << "   - Isolated testing tanpa external dependencies" << endl << endl;

    cout << " EXTENSIBILITY:" << endl;
    cout << "   - Tambah fitur baru tanpa ubah existing code" << endl;
    cout << "   - Open/Closed principle compliance" << endl << endl;

    cout << " MAINTAINABILITY:" << endl;
    cout << "   - Perubahan terisolasi per module" << endl;
    cout << "   - Reduced coupling = easier maintenance" << endl << endl;

    cout << " TEAM PRODUCTIVITY:" << endl;
    cout << "   - Parallel development dengan interfaces" << endl;
    cout << "   - Clear separation of concerns" << endl << endl;

    cout << " DIP Quote:" << endl;
    cout << "\"True strength comes from flexibility." << endl;
    cout << " Like bamboo that bends but never breaks.\" 🎋" << endl;
}

// ==================== MAIN FUNCTION ====================

int main() {
    printSeparator(" RESTAURANT MANAGEMENT SYSTEM - DIP DEMO");

    cout << "Welcome to Dependency Inversion Principle demonstration!" << endl;
    cout << "Hari ini kita akan belajar bagaimana DIP menyelesaikan masalah tight coupling." << endl;
    cout << "\nTopik yang akan dibahas:" << endl;
    cout << "1.  Problem: DIP Violation (Tight Coupling)" << endl;
    cout << "2.  Solution 1: Dependency Injection" << endl;
    cout << "3.  Solution 2: Factory Pattern" << endl;
    cout << "4.  Solution 3: Strategy Pattern" << endl;
    cout << "5.  Testing Benefits" << endl;
    cout << "6.  Summary of Benefits" << endl;

    // 1. Demonstrate the problem
    demonstrateProblem();

    // 2. Solution 1: Dependency Injection
    demonstrateDependencyInjection();

    // 3. Solution 2: Factory Pattern
    demonstrateFactoryPattern();

    // 4. Solution 3: Strategy Pattern  
    demonstrateStrategyPattern();

    // 5. Testing demonstration
    demonstrateTesting();

    // 6. Benefits summary
    demonstrateBenefits();

    printSeparator(" DEMO COMPLETED!");
    cout << "Key Takeaway: DIP = Depend on abstractions, not concretions!" << endl;
    cout << "High-level modules should not depend on low-level modules." << endl;
    cout << "Both should depend on abstractions." << endl << endl;

    cout << "Next steps:" << endl;
    cout << "1.  Study the code structure" << endl;
    cout << "2.  Try modifying implementations" << endl;
    cout << "3.  Add more test cases" << endl;
    cout << "4.  Apply DIP in your own projects" << endl;
    cout << "\nHappy coding! " << endl;

    return 0;
}
