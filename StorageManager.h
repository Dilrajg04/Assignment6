#ifndef STORAGEMANAGER_H
#define STORAGEMANAGER_H

#include "StoredItem.h"
#include <memory>
#include <unordered_map>
#include <map>

class StorageManager {
public:
    void addItem(std::shared_ptr<StoredItem> item);
    std::shared_ptr<StoredItem> findById(QString id);
    void removeItem(QString id);
    std::map<QString, std::shared_ptr<StoredItem>> listItemsByDescription();

private:
    std::unordered_map<QString, std::shared_ptr<StoredItem>> items;
};

#endif // STORAGEMANAGER_H
