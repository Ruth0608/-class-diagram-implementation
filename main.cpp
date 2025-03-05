#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <limits>  // Needed for numeric_limits
 
using namespace std;
 
class Product {
public:
    string id;
    string name;
    float price;
 
    Product(string id, string name, float price) : id(id), name(name), price(price) {
    }
};
 
struct CartItem {
    Product product;
    int quantity;
 
    CartItem(Product p, int q) : product(p), quantity(q) {
    }
};
 
class ShoppingCart {
    int cartId;
    vector<CartItem> items;
    float totalPrice = 0.0f;
 
public:
    ShoppingCart(int cartId) : cartId(cartId) {
    }
 
    void addProduct(Product product, int quantity) {
        for (auto &item: items) {
            if (item.product.id == product.id) {
                item.quantity += quantity;
                totalPrice += product.price * quantity;
                return;
            }
        }
        items.emplace_back(product, quantity);
        totalPrice += product.price * quantity;
    }
 
    // Modified to return true if the product was found and removed
    bool removeProduct(string productId, int quantity) {
        bool found = false;
        for (int i = 0; i < items.size(); i++) {
            if (items[i].product.id == productId) {
                found = true;
                int &currentQty = items[i].quantity;
                if (quantity >= currentQty) {
                    totalPrice -= items[i].product.price * currentQty;
                    items.erase(items.begin() + i);
                } else {
                    currentQty -= quantity;
                    totalPrice -= items[i].product.price * quantity;
                }
                break;
            }
        }
        return found;
    }
 
    void displayCart(bool simple = false) {
        if (items.empty()) {
            cout << "Your cart is empty.\n";
            return;
        }
        // Set formatting for currency values
        cout << fixed << setprecision(2);
 
        // Table header
        cout << left << setw(15) << "Product ID"
             << setw(20) << "Name"
             << setw(10) << "Price"
             << setw(10) << "Quantity"
             << setw(10) << "Total" << endl;
        // Table rows
        for (CartItem &item: items) {
            cout << left << setw(15) << item.product.id
                 << setw(20) << item.product.name
                 << setw(10) << item.product.price
                 << setw(10) << item.quantity
                 << setw(10) << (item.product.price * item.quantity) << endl;
        }
        if (!simple) {
            cout << "\nTotal Price: Php " << totalPrice << "\n";
        }
    }
 
    bool isEmpty() {
        return items.empty();
    }
 
    float getTotalPrice() {
        return totalPrice;
    }
};
 
class Order {
public:
    int orderId;
    ShoppingCart cart;
    string status;
 
    Order(int orderId, const ShoppingCart &cart) : orderId(orderId), cart(cart), status("Pending") {
    }
 
    void placeOrder() {
        status = "Order Placed";
    }
 
    void cancelOrder() {
        status = "Order Cancelled";
    }
 
    // Detailed order view
    void displayOrder() {
        cout << "Order ID: " << orderId << endl;
        cout << "Total Amount: Php " << cart.getTotalPrice() << endl;
        cout << "Order Details:" << endl;
        cart.displayCart(true);
    }
};
 
class User {
public:
    int userId;
    string name;
    string email;
    bool loggedIn = false;
 
    User() {
    }
 
    void registerUser() {
        cout << "\n--- User Registration ---\n";
        cout << "Enter your name: ";
        getline(cin >> ws, name);
        cout << "Enter your email: ";
        getline(cin, email);
        userId = 1;
        cout << "Registration successful!\n";
    }
 
    void login() {
        if (name.empty() || email.empty()) {
            cout << "No registered user found. Please register first.\n";
            return;
        }
        string inputEmail;
        cout << "\n--- User Login ---\n";
        cout << "Enter your email to login: ";
        getline(cin >> ws, inputEmail);
        if (inputEmail == email) {
            loggedIn = true;
            cout << "Login successful!\n";
        } else {
            cout << "Incorrect email. Login failed.\n";
        }
    }
 
    void logout() {
        loggedIn = false;
        cout << "Logged out successfully.\n";
    }
};
 
int getNumberInput(string message, int min, int max) {
    string input;
    cout << message;
 
    while (true) {
        getline(cin, input);
 
        try {
            size_t pos;
            int number = stoi(input, &pos);
            // If pos is not equal to the input length, there were extra characters
            if (pos != input.size()) {
                throw invalid_argument("Extra characters detected");
            }
            if (number >= min && number <= max) {
                return number;
            }
            cout << "Please enter a number between " << min << " and " << max << ": ";
        } catch (exception &e) {
            cout << "Invalid input. Please enter a number: ";
        }
    }
}
 
// Helper function to validate yes or no input
char getYesNoInput(const string &prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (input == "Y" || input == "y" || input == "N" || input == "n") {
            return input[0];
        }
        cout << "Invalid input. Please enter Y or N.\n";
    }
}
 
// Function to display the order history as a table in the main menu
void displayOrderHistory(vector<Order> &orderHistory) {
    if (orderHistory.empty()) {
        return;
    }
    cout << "\nOrder History:" << endl;
 
    for (auto &order: orderHistory) {
        cout << "Order ID: " << order.orderId << endl;
        cout << "Total Amount: Php " << order.cart.getTotalPrice() << endl;
        cout << "Order Details:" << endl;
        order.cart.displayCart(true);
        cout << endl;
    }
    cout << endl;
}
 
int main() {
    User user;
 
    // List of available products
    vector<Product> products = {
        Product("ABC", "Pencil", 20.0f),
        Product("CDE", "Paper", 10.0f),
        Product("GHI", "Eraser", 5.0f),
        Product("JKL", "Crayons", 50.0f),
        Product("MNO", "Ballpen", 15.0f),
    };
    int orderIdCounter = 1;
    vector<Order> orderHistory;
 
    bool running = true;
    while (running) {
        cout << "\nWelcome to my Shopping Cart\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. View Order History\n";
        cout << "4. Exit\n";
 
        int choice = getNumberInput("Enter your choice: ", 1, 4);
 
        if (choice == 1) {
            user.registerUser();
        } else if (choice == 2) {
            user.login();
            if (user.loggedIn) {
                ShoppingCart cart(1);
                bool shopping = true;
                while (shopping) {
                    cout << "\n--- Shopping Menu ---\n";
                    cout << "1. View Products\n";
                    cout << "2. Add Product to Cart\n";
                    cout << "3. Remove Product from Cart\n";
                    cout << "4. View Shopping Cart\n";
                    cout << "5. Place Order\n";
                    cout << "6. Logout\n";
 
                    int shopChoice = getNumberInput("Enter your choice: ", 1, 6);
                    switch (shopChoice) {
                        case 1: {
                            cout << "\nAvailable Products:\n";
                            for (Product &prod: products) {
                                cout << "ID: " << prod.id
                                     << ", Name: " << prod.name
                                     << ", Price: Php " << prod.price << endl;
                            }
                            break;
                        }
                        case 2: {
                            cout << "Enter Product ID to add: ";
                            string prodId;
                            getline(cin, prodId);
                            bool found = false;
                            for (Product &prod: products) {
                                if (prod.id == prodId) {
                                    int qty = getNumberInput("Enter quantity: ", 1, 100);
                                    cart.addProduct(prod, qty);
                                    cout << prod.name << " added to cart (x" << qty << ").\n";
                                    found = true;
                                    break;
                                }
                            }
                            if (!found) {
                                cout << "Product not found.\n";
                            }
                            break;
                        }
                        case 3: {
                            if (cart.isEmpty()) {
                                cout << "Cart is empty.\n";
                                break;
                            }
                            cart.displayCart();
                            cout << "Enter Product ID to remove: ";
                            string prodId;
                            getline(cin, prodId);
                            int qty = getNumberInput("Enter quantity to remove: ", 1, 100);
                            bool removed = cart.removeProduct(prodId, qty);
                            if (removed)
                                cout << "Product removed from the cart.\n";
                            else
                                cout << "Product not found in the cart.\n";
                            break;
                        }
                        case 4: {
                            cart.displayCart();
                            break;
                        }
                        case 5: {
                            if (cart.isEmpty()) {
                                cout << "Cart is empty. Cannot place order.\n";
                            } else {
                                char checkoutChoice = getYesNoInput("Do you want to check out all the products (Y/N)? ");
                                if (checkoutChoice == 'Y' || checkoutChoice == 'y') {
                                    Order order(orderIdCounter, cart);
                                    order.placeOrder();
                                    cout << "Order placed successfully!\n";
                                    order.displayOrder();
                                    // Save the order in order history
                                    orderHistory.push_back(order);
                                    orderIdCounter++;
                                    // Reset the cart for new orders
                                    cart = ShoppingCart(1);
                                } else {
                                    cout << "Returning to shopping menu.\n";
                                }
                            }
                            break;
                        }
                        case 6: {
                            shopping = false;
                            user.logout();
                            break;
                        }
                    }
                }
            }
        } else if (choice == 3) {
            displayOrderHistory(orderHistory);
        } else {
            running = false;
            cout << "Exiting the application. Goodbye!\n";
            break;
        }
    }
    return 0;
}
