#include "StorageManager.h"
#include <stdexcept>

void StorageManager::addItem(std::shared_ptr<StoredItem> item) {
    if (items.find(item->getId()) != items.end())
        throw std::runtime_error("Item already exists.");
    items[item->getId()] = item;
}

std::shared_ptr<StoredItem> StorageManager::findById(QString id) {
    if (items.find(id) == items.end())
        throw std::runtime_error("Item not found.");
    return items[id];
}

void StorageManager::removeItem(QString id) {
    if (items.erase(id) == 0)
        throw std::runtime_error("Item not found.");
}

std::map<QString, std::shared_ptr<StoredItem>> StorageManager::listItemsByDescription() {
    std::map<QString, std::shared_ptr<StoredItem>> sorted;
    for (auto& pair : items)
        sorted[pair.second->getDescription()] = pair.second;
    return sorted;
}
