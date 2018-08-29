#include "sortabletreewidgetitem.h"

bool SortableTreeWidgetItem::operator< ( const QTreeWidgetItem & other ) const  
{
	int sortCol = treeWidget()->sortColumn();
	bool ok1,ok2;
	QString str1=text(sortCol);
	QString str2=other.text(sortCol);
	double myNumber = str1.toDouble(&ok1);
	double otherNumber = str2.toDouble(&ok2);
	if ((ok1)&&(ok2)) return myNumber < otherNumber;
	else if (ok1) return true;
	else if (ok2) return false;
	else return QString::compare(str1,str2)<0;
}
