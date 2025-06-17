#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <stdexcept>

using namespace std;

// ==================== SUPPORTING CLASSES ====================

class Order {
private:
    int id;
    string description;
    double totalAmount;
    string paymentType;
    string paymentInfo;

public:
    Order(int id, const string& desc, double amount)
        : id(id), description(desc), totalAmount(amount) {
    }

    // Getters
    int getId() const { return id; }
    string getDescription() const { return description; }
    double getTotalAmount() const { return totalAmount; }
    string getPaymentType() const { return paymentType; }
    string getPaymentInfo() const { return paymentInfo; }

    // Setters for payment
    void setPaymentInfo(const string& type, const string& info) {
        paymentType = type;
        paymentInfo = info;
    }

    string toString() const {
        return "Order{id=" + to_string(id) +
            ", description='" + description +
            "', amount=$" + to_string(totalAmount) + "}";
    }
};

// ==================== BEFORE: DIP VIOLATION ====================
// ❌ BAD EXAMPLE - Tight Coupling

class MySQLDatabase_Bad {
public:
    void save(const Order& order) {
        cout << " MySQL: Saving to MySQL database: " << order.toString() << endl;
    }

    Order findById(int id) {
        return Order(id, "MySQL Order #" + to_string(id), 25.99);
    }
};

class EmailNotification_Bad {
public:
    void send(const string& message) {
        cout << " Email: Sending email - " << message << endl;
    }
};

//  VIOLATION: RestaurantService yang melanggar DIP
class BadRestaurantService {
private:
    MySQLDatabase_Bad* database;          // Direct dependency on concrete class
    EmailNotification_Bad* notification;  // Direct dependency on concrete class

public:
    BadRestaurantService() {
        database = new MySQLDatabase_Bad();      // Tight coupling - hard coded!
        notification = new EmailNotification_Bad(); // Tight coupling - hard coded!
        cout << " BadRestaurantService: Created with tight coupling" << endl;
    }

    ~BadRestaurantService() {
        delete database;
        delete notification;
    }

    void processOrder(const Order& order) {
        database->save(order);
        notification->send("Order " + to_string(order.getId()) + " processed!");
        cout << " Order processed with TIGHT COUPLING" << endl;
        cout << "   Problem: Sulit ganti database atau notification!" << endl;
    }
};
