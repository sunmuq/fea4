#ifndef sortabletreewidgetitem_H
#define sortabletreewidgetitem_H

#include <QTreeWidgetItem>

class SortableTreeWidgetItem : public QTreeWidgetItem{
public:
	bool operator< ( const QTreeWidgetItem & other ) const;
};

#endif
