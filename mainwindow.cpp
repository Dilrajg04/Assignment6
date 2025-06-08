#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <memory>
#include <unordered_map>
#include <map>
#include <stdexcept>
#include <QMessageBox>

//Custom Exceptions for error handling
class DuplicateItemException : public std::runtime_error {
public:
    DuplicateItemException() : std::runtime_error("Item already exists.") {}
};
class ItemNotFoundException : public std::runtime_error {
public:
    ItemNotFoundException() : std::runtime_error("Item not found.") {}
};
//StoredItem class holds single inventory items
class StoredItem {
public:
    StoredItem(QString id, QString desc, QString loc)
        : id(id), description(desc), location(loc) {}

    QString getId() const { return id; }
    QString getDescription() const { return description; }
    QString getLocation() const { return location; }
private:
    QString id;
    QString description;
    QString location;
};

//StorageManager handles all backend inventory operations like add,find and remove
class StorageManager {
public:
    // Add a new item to the inventory
    void addItem(std::shared_ptr<StoredItem> item) {
        if (items.find(item->getId()) != items.end())
            throw DuplicateItemException();  // Prevent duplicate entries
        items[item->getId()] = item;
    }
    // Find an item using ID
    std::shared_ptr<StoredItem> findById(QString id) {
        if (items.find(id) == items.end())
            throw ItemNotFoundException();
        return items[id];
    }
    // Remove an item by ID
    void removeItem(QString id) {
        if (items.erase(id) == 0)
            throw ItemNotFoundException();
    }
    // Return all items, sorted by description
    std::map<QString, std::shared_ptr<StoredItem>> listItemsByDescription() {
        std::map<QString, std::shared_ptr<StoredItem>> sorted;
        for (auto& pair : items)
            sorted[pair.second->getDescription()] = pair.second;
        return sorted;
    }
    // Clear all items in the inventory(the red button)
    void clearAll() {
        items.clear();
    }

private:
    std::unordered_map<QString, std::shared_ptr<StoredItem>> items;
};

//GUI-----------------------------------------------

//initializes the user interface
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}
// Destructor(cleans up UI memory)
MainWindow::~MainWindow()
{
    delete ui;
}
// Global instance of the backend manager
StorageManager manager;
//Add Ite: makes sure of the format and add its to the inventory
void MainWindow::on_addBtn_clicked()
{
    QString input = ui->lineEdit->text();
    QStringList parts = input.split(',');

    if (parts.size() != 3) {
        QMessageBox::critical(this, "Input Error", "Format Example: (011,Flashlight,Bedside Table)");
        return;
    }

    QString id   = parts[0].trimmed();
    QString desc = parts[1].trimmed();
    QString loc  = parts[2].trimmed();

    try {
        auto item = std::make_shared<StoredItem>(id, desc, loc);
        manager.addItem(item);
        ui->errortxt->setText("Item added.");
        // Refresh list view
        ui->listWidget->clear();
        auto sorted = manager.listItemsByDescription();
        for (auto& pair : sorted) {
            auto i = pair.second;
            ui->listWidget->addItem(i->getId() + " | " + i->getDescription() + " | " + i->getLocation());
        }

    } catch (const DuplicateItemException& e) {
        QMessageBox::critical(this, "Duplicate Error", e.what());
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }

    ui->lineEdit->clear();//clears the text after pressing a button
}

//Find Item: Looks up item by ID and displays it
void MainWindow::on_findBtn_clicked()
{
    QString id = ui->lineEdit->text().trimmed();

    try {
        auto item = manager.findById(id);
        ui->errortxt->setText("Found: " + item->getDescription() + " @ " + item->getLocation());
    } catch (const ItemNotFoundException& e) {
        QMessageBox::critical(this, "Not Found", e.what());
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());

        //No clear lineEdit since its easier to find an Item and delete it without typing in the ID again.
    }
}

//Remove Item: Deletes item from inventory by ID
void MainWindow::on_removeBtn_clicked()
{
    QString id = ui->lineEdit->text().trimmed();

    try {
        manager.removeItem(id);
        ui->errortxt->setText("Item removed.");

        // Refresh list view
        ui->listWidget->clear();
        auto sorted = manager.listItemsByDescription();
        for (auto& pair : sorted) {
            auto i = pair.second;
            ui->listWidget->addItem(i->getId() + " | " + i->getDescription() + " | " + i->getLocation());
        }

    } catch (const ItemNotFoundException& e) {
        QMessageBox::critical(this, "Not Found", e.what());
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }

    ui->lineEdit->clear();
}
// Clear All: Clears the entire inventory and UI
void MainWindow::on_clearBtn_clicked()
{
    manager.clearAll();
    ui->listWidget->clear();
    ui->errortxt->setText("Inventory cleared.");
    ui->lineEdit->clear();
}
// File > Exit: Closes the application(may appear different on MacOs)
void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}
//Help > About: Shows app info and creator name(may appear different on MacOS)
void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "About", "Created by Dilraj Grewal\nAssignment 6");
}
