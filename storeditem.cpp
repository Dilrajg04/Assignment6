#include "StoredItem.h"

StoredItem::StoredItem(QString id, QString desc, QString loc)
    : id(id), description(desc), location(loc) {}

QString StoredItem::getId() const { return id; }
QString StoredItem::getDescription() const { return description; }
QString StoredItem::getLocation() const { return location; }
