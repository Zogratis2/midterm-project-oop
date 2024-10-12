#include <iostream>
#include <string>
#include <iomanip> 
#include <limits>  
using namespace std;

string to_lower(const string& str) {
    string lowerStr = str;
    for (char& c : lowerStr) {
        c = tolower(c);
    }
    return lowerStr;
}


int getValidInt() {
    int value;
    while (!(cin >> value) || value < 0) {
        cout << "Invalid input! Please enter a valid positive integer: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return value;
}


double getValidDouble() {
    double value;
    while (!(cin >> value) || value < 0) {
        cout << "Invalid input! Please enter a valid positive number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return value;
}


string getValidCategory() {
    string category;
    while (true) {
        cin >> category;
        category = to_lower(category); 

        if (category == "clothing" || category == "electronics" || category == "entertainment") {
            
            category[0] = toupper(category[0]);
            return category;
        } else {
            cout << "Invalid category! Please enter one of the following: Clothing, Electronics, Entertainment: ";
        }
    }
}


class IItem {
public:
    virtual void displayItem() const = 0; 
    virtual string getId() const = 0; 
    virtual string getName() const = 0; 
    virtual int getQuantity() const = 0; 
    virtual double getPrice() const = 0; 
    virtual ~IItem() {} 
};


class Item : public IItem {
private:
    string id;
    string name;
    int quantity;
    double price;

public:
   
    Item(string itemId, string itemName, int itemQuantity, double itemPrice)
        : id(itemId), name(itemName), quantity(itemQuantity), price(itemPrice) {}

    
    void displayItem() const override {
        cout << left << setw(10) << id << setw(20) << name
             << setw(10) << quantity << setw(10) << fixed << setprecision(2) << price << endl;
    }

    string getId() const override {
        return id;
    }

    string getName() const override {
        return name;
    }

    int getQuantity() const override {
        return quantity;
    }

    double getPrice() const override {
        return price;
    }

    
    void setQuantity(int newQuantity) { quantity = newQuantity; }
    void setPrice(double newPrice) { price = newPrice; }
};


void swapItems(Item*& a, Item*& b) {
    Item* temp = a;
    a = b;
    b = temp;
}


class Inventory {
private:
    Item* clothing[100];        
    Item* electronics[100];    
    Item* entertainment[100];   
    int clothingCount = 0;      
    int electronicsCount = 0;   
    int entertainmentCount = 0;  

    
    pair<Item**, int*> getCategory(const string& category) {
        if (category == "Clothing") {
            return { clothing, &clothingCount };
        } else if (category == "Electronics") {
            return { electronics, &electronicsCount };
        } else if (category == "Entertainment") {
            return { entertainment, &entertainmentCount };
        }
        return { nullptr, nullptr };
    }

    
    Item* findItemById(Item* items[], int& count, const string& id) {
        for (int i = 0; i < count; ++i) {
            if (items[i]->getId() == id)
                return items[i];
        }
        return nullptr;
    }

public:
   
    void addItem(const string& category, const string& id, const string& name, int quantity, double price) {
        auto [items, count] = getCategory(category);
        if (items) {
            items[*count] = new Item(id, name, quantity, price);
            (*count)++;
            cout << "Item added successfully!" << endl;
        } else {
            cout << "Category " << category << " does not exist!" << endl;
        }
    }

    
    void updateItem(const string& id) {
        Item* item = findItemByIdInAllCategories(id);
        if (item) {
            int choice;
            cout << "Do you want to update: 1 - Quantity, 2 - Price? ";
            choice = getValidInt();

            if (choice == 1) {
                cout << "Enter new quantity: ";
                int newQuantity = getValidInt();
                cout << "Quantity of Item " << item->getId() << " updated from " << item->getQuantity() << " to " << newQuantity << endl;
                item->setQuantity(newQuantity);
            } else if (choice == 2) {
                cout << "Enter new price: ";
                double newPrice = getValidDouble();
                cout << "Price of Item " << item->getId() << " updated from " << item->getPrice() << " to " << newPrice << endl;
                item->setPrice(newPrice);
            } else {
                cout << "Invalid choice!" << endl;
            }
        } else {
            cout << "Item not found!" << endl;
        }
    }

   
    void removeItem(const string& id) {
        auto [items, count] = findItemCategory(id);
        if (items) {
            for (int i = 0; i < *count; ++i) {
                if (items[i]->getId() == id) {
                    cout << "Item " << items[i]->getId() << " has been removed from the inventory." << endl;
                    delete items[i];
                    items[i] = items[--(*count)]; 
                    return;
                }
            }
        }
        cout << "Item not found!" << endl;
    }

    
    void searchItem(const string& id) {
        Item* item = findItemByIdInAllCategories(id);
        if (item) {
            cout << "Item found: ";
            item->displayItem();
        } else {
            cout << "Item not found!" << endl;
        }
    }

   
    void sortItems(const string& category) {
        auto [items, count] = getCategory(category);
        if (items) {
            // Simple bubble sort for demonstration
            for (int i = 0; i < *count - 1; ++i) {
                for (int j = 0; j < *count - i - 1; ++j) {
                    if (items[j]->getName() > items[j + 1]->getName()) {
                        swapItems(items[j], items[j + 1]);
                    }
                }
            }
            cout << "Items sorted successfully!" << endl;
        } else {
            cout << "Category " << category << " does not exist!" << endl;
        }
    }

    
   void displayLowStockItems() {
    cout << left << setw(10) << "ID" 
         << setw(20) << "Name" 
         << setw(10) << "Quantity" 
         << setw(10) << "Price" 
         << setw(15) << "Category" << endl;

    cout << string(65, '-') << endl;

    for (int i = 0; i < clothingCount; ++i) {
        if (clothing[i]->getQuantity() <= 5) {
            clothing[i]->displayItem();
            cout << setw(15) << "Clothing" << endl;
        }
    }

    for (int i = 0; i < electronicsCount; ++i) {
        if (electronics[i]->getQuantity() <= 5) {
            electronics[i]->displayItem();
            cout << setw(15) << "Electronics" << endl;
        }
    }

    for (int i = 0; i < entertainmentCount; ++i) {
        if (entertainment[i]->getQuantity() <= 5) {
            entertainment[i]->displayItem();
            cout << setw(15) << "Entertainment" << endl;
        }
    }
}


   
    void displayItemsByCategory(const string& category) {
        auto [items, count] = getCategory(category);
        if (items) {
            cout << left << setw(10) << "ID" << setw(20) << "Name"
                 << setw(10) << "Quantity" << setw(10) << "Price" << endl;

            for (int i = 0; i < *count; ++i) {
                items[i]->displayItem();
            }
        } else {
            cout << "Category " << category << " does not exist!" << endl;
        }
    }

    
    void displayAllItems() {
        cout << "Displaying all items:" << endl;

        cout << left << setw(10) << "ID" << setw(20) << "Name"
             << setw(10) << "Quantity" << setw(10) << "Price" << endl;

        for (int i = 0; i < clothingCount; ++i) {
            clothing[i]->displayItem();
        }
        for (int i = 0; i < electronicsCount; ++i) {
            electronics[i]->displayItem();
        }
        for (int i = 0; i < entertainmentCount; ++i) {
            entertainment[i]->displayItem();
        }
    }

    
    ~Inventory() {
        for (int i = 0; i < clothingCount; ++i) {
            delete clothing[i];
        }
        for (int i = 0; i < electronicsCount; ++i) {
            delete electronics[i];
        }
        for (int i = 0; i < entertainmentCount; ++i) {
            delete entertainment[i];
        }
    }

private:
    
    pair<Item**, int*> findItemCategory(const string& id) {
        if (findItemById(clothing, clothingCount, id)) return { clothing, &clothingCount };
        if (findItemById(electronics, electronicsCount, id)) return { electronics, &electronicsCount };
        if (findItemById(entertainment, entertainmentCount, id)) return { entertainment, &entertainmentCount };
        return { nullptr, nullptr };
    }

    
    Item* findItemByIdInAllCategories(const string& id) {
        for (int i = 0; i < clothingCount; ++i) {
            if (clothing[i]->getId() == id) return clothing[i];
        }
        for (int i = 0; i < electronicsCount; ++i) {
            if (electronics[i]->getId() == id) return electronics[i];
        }
        for (int i = 0; i < entertainmentCount; ++i) {
            if (entertainment[i]->getId() == id) return entertainment[i];
        }
        return nullptr;
    }
};

int main() {
    Inventory inventory;
    int choice;

    do {
        cout << "\n===============Inventory Management System===============\n";
        cout << "1. Add Item\n";
        cout << "2. Update Item\n";
        cout << "3. Remove Item\n";
        cout << "4. Display Items by Category\n";
        cout << "5. Display All Items\n";
        cout << "6. Search Item\n";
        cout << "7. Sort Items\n";
        cout << "8. Display Low Stock Items\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";
        choice = getValidInt();

        switch (choice) {
            case 1: {
                cout << "Enter category (Clothing, Electronics, Entertainment): ";
                string category = getValidCategory();
                cout << "Enter item ID: ";
                string id;
                cin >> id;
                cout << "Enter item name: ";
                string name;
                cin.ignore(); 
                getline(cin, name);
                cout << "Enter quantity: ";
                int quantity = getValidInt();
                cout << "Enter price: ";
                double price = getValidDouble();
                inventory.addItem(category, id, name, quantity, price);
                break;
            }
            case 2: {
                cout << "Enter item ID to update: ";
                string id;
                cin >> id;
                inventory.updateItem(id);
                break;
            }
            case 3: {
                cout << "Enter item ID to remove: ";
                string id;
                cin >> id;
                inventory.removeItem(id);
                break;
            }
            case 4: {
                cout << "Enter category to display (Clothing, Electronics, Entertainment): ";
                string category = getValidCategory();
                inventory.displayItemsByCategory(category);
                break;
            }
            case 5: {
                inventory.displayAllItems();
                break;
            }
            case 6: {
                cout << "Enter item ID to search: ";
                string id;
                cin >> id;
                inventory.searchItem(id);
                break;
            }
            case 7: {
                cout << "Enter category to sort (Clothing, Electronics, Entertainment): ";
                string category = getValidCategory();
                inventory.sortItems(category);
                break;
            }
            case 8: {
    			cout << "Displaying all items with quantity 5 and below:" << endl;
    			inventory.displayLowStockItems();
   				break;
				   }

            
            case 9: {
                cout << "Exiting the program. Byebye!" << endl;
                break;
            }
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 9);

    return 0;
}
