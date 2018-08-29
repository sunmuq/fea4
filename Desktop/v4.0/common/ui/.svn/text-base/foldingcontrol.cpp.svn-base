#include "foldingcontrol.h"
#include <QVBoxLayout>
#include <QFrame>
#include <QPushButton>
#include <QDebug>

class FoldingControlItemPrivate {
public:
	FoldingControlItem *q;
	QString m_name;
};
FoldingControlItem::FoldingControlItem()
{
	d=new FoldingControlItemPrivate;
	d->q=this;
}
FoldingControlItem::~FoldingControlItem()
{
	delete d;
}
QString FoldingControlItem::name() const {
	return d->m_name;
}
void FoldingControlItem::setRange(int min,int max) {
}
void FoldingControlItem::setLabel(QString label) {
}
void FoldingControlItem::setName(const QString &name) {
	d->m_name=name;
}

/////////////////////////////////////////////////////////////////////////////////////
class FoldingControlSliderPrivate {
public:
	FoldingControlSlider *q;
	QSlider *m_slider;
	QLabel *m_label;
};
FoldingControlSlider::FoldingControlSlider() {
	d=new FoldingControlSliderPrivate;
	d->q=this;
	d->m_slider=new QSlider;
	d->m_slider->setOrientation(Qt::Horizontal);
	d->m_label=new QLabel;
	d->m_label->setAlignment(Qt::AlignHCenter);
}
FoldingControlSlider::~FoldingControlSlider() {
	delete d;
}
void FoldingControlSlider::setRange(int min,int max) {
	sliderObject()->setRange(min,max);
}
void FoldingControlSlider::setLabel(QString txt) {
	labelObject()->setText(txt);
}
QSlider *FoldingControlSlider::sliderObject() {
	return d->m_slider;
}
QLabel *FoldingControlSlider::labelObject() {
	return d->m_label;
}
QVariant FoldingControlSlider::value() const {
	return d->m_slider->value();
}
void FoldingControlSlider::setValue(const QVariant &val) {
	d->m_slider->setValue(val.toInt());
}
QList<QWidget *> FoldingControlSlider::widgets() const {
	QList<QWidget *> ret;
	ret << d->m_slider << d->m_label;
	return ret;
}
void FoldingControlSlider::connectOnChanged(QObject *receiver,const char *method,Qt::ConnectionType type) {
	QObject::connect(sliderObject(),SIGNAL(valueChanged(int)),receiver,method,type);
}
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
class FoldingControlSpinBoxPrivate {
public:
	FoldingControlSpinBox *q;
	QSpinBox *m_spinbox;
	QLabel *m_label;
	QWidget *m_widget;
	QHBoxLayout *m_layout;
};
FoldingControlSpinBox::FoldingControlSpinBox() {
	d=new FoldingControlSpinBoxPrivate;
	d->q=this;
	d->m_spinbox=new QSpinBox;
	d->m_label=new QLabel;
	
	d->m_widget=new QWidget;
	d->m_layout=new QHBoxLayout;
	d->m_layout->setContentsMargins(0,0,0,0);
	d->m_widget->setLayout(d->m_layout);
	d->m_layout->addWidget(d->m_label);
	d->m_layout->addWidget(d->m_spinbox);
}
FoldingControlSpinBox::~FoldingControlSpinBox() {
	delete d;
}
void FoldingControlSpinBox::setRange(int min,int max) {
	spinBoxObject()->setRange(min,max);
}
void FoldingControlSpinBox::setLabel(QString txt) {
	labelObject()->setText(txt);
}
QSpinBox *FoldingControlSpinBox::spinBoxObject() {
	return d->m_spinbox;
}
QLabel *FoldingControlSpinBox::labelObject() {
	return d->m_label;
}
QVariant FoldingControlSpinBox::value() const {
	return d->m_spinbox->value();
}
void FoldingControlSpinBox::setValue(const QVariant &val) {
	d->m_spinbox->setValue(val.toInt());
}
QList<QWidget *> FoldingControlSpinBox::widgets() const {
	QList<QWidget *> ret;
	ret << d->m_widget;
	return ret;
}
void FoldingControlSpinBox::connectOnChanged(QObject *receiver,const char *method,Qt::ConnectionType type) {
	QObject::connect(spinBoxObject(),SIGNAL(valueChanged(int)),receiver,method,type);
}
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
class FoldingControlCheckBoxPrivate {
public:
	FoldingControlCheckBox *q;
	QCheckBox *m_checkbox;
};
FoldingControlCheckBox::FoldingControlCheckBox() {
	d=new FoldingControlCheckBoxPrivate;
	d->q=this;
	d->m_checkbox=new QCheckBox;
}
FoldingControlCheckBox::~FoldingControlCheckBox() {
	delete d;
}
void FoldingControlCheckBox::setLabel(QString txt) {
	checkBoxObject()->setText(txt);
}
QCheckBox *FoldingControlCheckBox::checkBoxObject() {
	return d->m_checkbox;
}
QVariant FoldingControlCheckBox::value() const {
	return (d->m_checkbox->checkState()==Qt::Checked);
}
void FoldingControlCheckBox::setValue(const QVariant &val) {
	if (val.toBool()) 
		d->m_checkbox->setCheckState(Qt::Checked);
	else
		d->m_checkbox->setCheckState(Qt::Unchecked);
}
QList<QWidget *> FoldingControlCheckBox::widgets() const {
	QList<QWidget *> ret;
	ret << d->m_checkbox;
	return ret;
}
void FoldingControlCheckBox::connectOnChanged(QObject *receiver,const char *method,Qt::ConnectionType type) {
	QObject::connect(checkBoxObject(),SIGNAL(stateChanged(int)),receiver,method,type);
}
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
class FoldingControlRadioButtonPrivate {
public:
	FoldingControlRadioButton *q;
	QRadioButton *m_radiobutton;
};
FoldingControlRadioButton::FoldingControlRadioButton() {
	d=new FoldingControlRadioButtonPrivate;
	d->q=this;
	d->m_radiobutton=new QRadioButton;
}
FoldingControlRadioButton::~FoldingControlRadioButton() {
	delete d;
}
void FoldingControlRadioButton::setLabel(QString txt) {
	radioButtonObject()->setText(txt);
}
QRadioButton *FoldingControlRadioButton::radioButtonObject() {
	return d->m_radiobutton;
}
QVariant FoldingControlRadioButton::value() const {
	return (d->m_radiobutton->isChecked());
}
void FoldingControlRadioButton::setValue(const QVariant &val) {
	if (val.toBool()) 
		d->m_radiobutton->setChecked(true);
	else
		d->m_radiobutton->setChecked(false);
}
QList<QWidget *> FoldingControlRadioButton::widgets() const {
	QList<QWidget *> ret;
	ret << d->m_radiobutton;
	return ret;
}
void FoldingControlRadioButton::connectOnChanged(QObject *receiver,const char *method,Qt::ConnectionType type) {
	QObject::connect(radioButtonObject(),SIGNAL(toggled(bool)),receiver,method,type);
}
/////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////
class FoldingControlPushButtonPrivate {
public:
	FoldingControlPushButton *q;
	QPushButton *m_pushbutton;
	QToolButton *m_toolbutton;
};
FoldingControlPushButton::FoldingControlPushButton() {
	d=new FoldingControlPushButtonPrivate;
	d->q=this;
	d->m_pushbutton=new QPushButton;
	d->m_toolbutton=0;
}
FoldingControlPushButton::~FoldingControlPushButton() {
	delete d;
}
void FoldingControlPushButton::useToolButton() {
	delete d->m_pushbutton;
	d->m_pushbutton=0;
	d->m_toolbutton=new QToolButton;
}
void FoldingControlPushButton::setLabel(QString txt) {
	if (d->m_pushbutton) d->m_pushbutton->setText(txt);
	else d->m_toolbutton->setText(txt);
}
QPushButton *FoldingControlPushButton::pushButtonObject() {
	return d->m_pushbutton;
}
QToolButton *FoldingControlPushButton::toolButtonObject() {
	return d->m_toolbutton;
}
QVariant FoldingControlPushButton::value() const {
	return 0;
}
void FoldingControlPushButton::setValue(const QVariant &val) {
}
QList<QWidget *> FoldingControlPushButton::widgets() const {
	QList<QWidget *> ret;
	if (d->m_pushbutton) ret << d->m_pushbutton;
	else ret << d->m_toolbutton;
	return ret;
}
void FoldingControlPushButton::connectOnClicked(QObject *receiver,const char *method,Qt::ConnectionType type) {
	if (d->m_pushbutton) QObject::connect(d->m_pushbutton,SIGNAL(clicked()),receiver,method,type);
	else QObject::connect(d->m_toolbutton,SIGNAL(clicked()),receiver,method,type);
}
/////////////////////////////////////////////////////////////////////////////////






/////////////////////////////////////////////////////////////////////////////////////
class FoldingControlLineEditPrivate {
public:
	FoldingControlLineEdit *q;
	QWidget *m_widget;
	QHBoxLayout *m_layout;
	QLineEdit *m_lineedit;
	QLabel *m_label;
};
FoldingControlLineEdit::FoldingControlLineEdit() {
	d=new FoldingControlLineEditPrivate;
	d->q=this;
	d->m_lineedit=new QLineEdit;
	d->m_label=new QLabel;
	
	d->m_widget=new QWidget;
	d->m_layout=new QHBoxLayout;
	d->m_layout->setContentsMargins(0,0,0,0);
	d->m_widget->setLayout(d->m_layout);
	d->m_layout->addWidget(d->m_label);
	d->m_layout->addWidget(d->m_lineedit);
}
FoldingControlLineEdit::~FoldingControlLineEdit() {
	delete d;
}
void FoldingControlLineEdit::setLabel(QString txt) {
	labelObject()->setText(txt);
}
QLineEdit *FoldingControlLineEdit::lineEditObject() {
	return d->m_lineedit;
}
QLabel *FoldingControlLineEdit::labelObject() {
	return d->m_label;
}
QVariant FoldingControlLineEdit::value() const {
	return d->m_lineedit->text();
}
void FoldingControlLineEdit::setValue(const QVariant &val) {
	d->m_lineedit->setText(val.toString());
}
QList<QWidget *> FoldingControlLineEdit::widgets() const {
	QList<QWidget *> ret;
	ret << d->m_widget;
	return ret;
}
void FoldingControlLineEdit::connectOnChanged(QObject *receiver,const char *method,Qt::ConnectionType type) {
	QObject::connect(lineEditObject(),SIGNAL(textEdited(QString)),receiver,method,type);
}
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
class FoldingControlComboBoxPrivate {
public:
	FoldingControlComboBox *q;
	QComboBox *m_combobox;
	QLabel *m_label;
	QWidget *m_widget;
	QHBoxLayout *m_layout;
	QString m_alternate_value;
};
FoldingControlComboBox::FoldingControlComboBox() {
	d=new FoldingControlComboBoxPrivate;
	d->q=this;
	d->m_combobox=new QComboBox;
	d->m_label=new QLabel;
	d->m_label->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
	d->m_combobox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
	
	d->m_widget=new QWidget;
	d->m_layout=new QHBoxLayout;
	d->m_layout->setContentsMargins(0,0,0,0);
	d->m_widget->setLayout(d->m_layout);
	d->m_layout->addWidget(d->m_label);
	d->m_layout->addWidget(d->m_combobox);
}
FoldingControlComboBox::~FoldingControlComboBox() {
	delete d;
}
QComboBox *FoldingControlComboBox::comboBoxObject() {
	return d->m_combobox;
}
void FoldingControlComboBox::setLabel(QString txt) {
	labelObject()->setText(txt);
}
QVariant FoldingControlComboBox::value() const {
	if (!d->m_alternate_value.isEmpty())
		return d->m_alternate_value;
	else
		return d->m_combobox->itemData(d->m_combobox->currentIndex(),Qt::UserRole);
}
void FoldingControlComboBox::setValue(const QVariant &val) {
	d->m_alternate_value="";
	for (int i=0; i<d->m_combobox->count(); i++) {
		if (d->m_combobox->itemData(i,Qt::UserRole)==val.toString()) {
			d->m_combobox->setCurrentIndex(i);
			return;
		}
	}
	//d->m_alternate_value=val.toString();
}
QList<QWidget *> FoldingControlComboBox::widgets() const {
	QList<QWidget *> ret;
	ret << d->m_widget;
	return ret;
}
void FoldingControlComboBox::connectOnChanged(QObject *receiver,const char *method,Qt::ConnectionType type) {
	QObject::connect(comboBoxObject(),SIGNAL(currentIndexChanged(QString)),receiver,method,type);
}
void FoldingControlComboBox::clear() {
	d->m_combobox->clear();
}
void FoldingControlComboBox::addItem(const QString &text,const QString &value) {
	QString val=value;
	if (val.isEmpty()) val=text;
	d->m_combobox->addItem(text,val);
}
void FoldingControlComboBox::addItem(const QIcon &icon, const QString &text,const QString &value) {
	QString val=value;
	if (val.isEmpty()) val=text;	
	d->m_combobox->addItem(icon,text,val);
}
QLabel *FoldingControlComboBox::labelObject() {
	return d->m_label;
}
/////////////////////////////////////////////////////////////////////////////////

#ifdef USING_COLOR_BUTTON
/////////////////////////////////////////////////////////////////////////////////////
class FoldingControlColorButtonPrivate {
public:
	FoldingControlColorButton *q;
	QwwColorButton *m_colorbutton;
	QLabel *m_label;
	QWidget *m_widget;
	QLayout *m_layout;
};
FoldingControlColorButton::FoldingControlColorButton() {
	d=new FoldingControlColorButtonPrivate;
	d->q=this;
	d->m_colorbutton=new QwwColorButton;
	d->m_label=new QLabel;
	
	d->m_label->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
	d->m_colorbutton->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
	
	d->m_colorbutton->setShowName(false);
	
	d->m_widget=new QWidget;
	d->m_layout=new QHBoxLayout;
	d->m_layout->setContentsMargins(0,0,0,0);
	d->m_widget->setLayout(d->m_layout);	
	d->m_layout->addWidget(d->m_label);
	d->m_layout->addWidget(d->m_colorbutton);
}
FoldingControlColorButton::~FoldingControlColorButton() {
	delete d;
}
void FoldingControlColorButton::setLabel(QString txt) {
	labelObject()->setText(txt);
}
QwwColorButton *FoldingControlColorButton::colorButtonObject() {
	return d->m_colorbutton;
}
QLabel *FoldingControlColorButton::labelObject() {
	return d->m_label;
}
QVariant FoldingControlColorButton::value() const {
	return d->m_colorbutton->currentColor();
}
void FoldingControlColorButton::setValue(const QVariant &val) {
	colorButtonObject()->setCurrentColor(val.value<QColor>());
}
QList<QWidget *> FoldingControlColorButton::widgets() const {
	QList<QWidget *> ret;
	ret << d->m_widget;
	return ret;
}
void FoldingControlColorButton::connectOnChanged(QObject *receiver,const char *method,Qt::ConnectionType type) {
	QObject::connect(colorButtonObject(),SIGNAL(colorPicked(QColor)),receiver,method,type);
}
/////////////////////////////////////////////////////////////////////////////////
#endif

/////////////////////////////////////////////////////////////////////////////////////
class FoldingControlWidgetPrivate {
public:
	FoldingControlWidget *q;
	QWidget *m_widget;
};
FoldingControlWidget::FoldingControlWidget() {
	d=new FoldingControlWidgetPrivate;
	d->q=this;
	d->m_widget=0;
}
FoldingControlWidget::~FoldingControlWidget() {
	delete d;
}
QWidget *FoldingControlWidget::widgetObject() {
	return d->m_widget;
}
void FoldingControlWidget::setWidget(QWidget *W) {
	d->m_widget=W;
}
QVariant FoldingControlWidget::value() const {
	return 0;
}
void FoldingControlWidget::setValue(const QVariant &val) {
}
QList<QWidget *> FoldingControlWidget::widgets() const {
	QList<QWidget *> ret;
	ret << d->m_widget;
	return ret;
}
/////////////////////////////////////////////////////////////////////////////////



class FoldingControlGroupPrivate {
public:
	FoldingControlGroup *q;
	QString m_name;
	QList<FoldingControlItem *> m_items;
	bool m_expanded;
	bool m_collapsible;
	bool m_begin_row_on_next_item;
	bool m_add_spacing_at_end;
	QHBoxLayout *m_current_row_layout;
	
	QPushButton *m_button;
	QWidget *m_frame;
	QVBoxLayout *m_layout;
	QSpacerItem *m_extra_space;
	
	void add_item(FoldingControlItem *it);
};
FoldingControlGroup::FoldingControlGroup(QString name_in)
{
	d=new FoldingControlGroupPrivate;
	d->q=this;
	d->m_name=name_in;
	d->m_button=new QPushButton;
	d->m_frame=new QWidget;
	d->m_extra_space=new QSpacerItem(0,0);
	d->m_layout=new QVBoxLayout;
	d->m_layout->setSpacing(3);
	d->m_layout->setContentsMargins(3,3,3,3);
	d->m_frame->setLayout(d->m_layout);
	d->m_expanded=false;
	d->m_collapsible=true;
	d->m_begin_row_on_next_item=false;
	d->m_current_row_layout=0;
}
FoldingControlGroup::~FoldingControlGroup()
{
	qDeleteAll(d->m_items);
	delete d;
}
QStringList FoldingControlGroup::itemNames() const {
	QStringList ret;
	for (int i=0; i<d->m_items.count(); i++) {
		ret << d->m_items[i]->name();
	}
	return ret;
}
QString FoldingControlGroup::name() const {
	return d->m_name;
}
bool FoldingControlGroup::expanded() const {
	return d->m_expanded;
}
void FoldingControlGroup::setExpanded(bool val) {
	if (d->m_expanded==val) return;
	if (!d->m_collapsible) val=true;
	d->m_expanded=val;
	d->m_frame->setVisible(val);
	if (val) d->m_extra_space->changeSize(0,15);
	else d->m_extra_space->changeSize(0,0);
}
void FoldingControlGroup::setCollapsible(bool val) {
	d->m_collapsible=val;
	if (!val) setExpanded(true);
}
void FoldingControlGroup::setLabel(const QString &label) {
	d->m_button->setText(label);
}
FoldingControlItem *FoldingControlGroup::item(QString name) {
	foreach (FoldingControlItem *it,d->m_items) {
		if (it->name()==name) return it;
	}
	return 0;
}
FoldingControlSlider *FoldingControlGroup::addSlider(QString name,QString label,int min,int max,int val) {
	FoldingControlSlider *it=new FoldingControlSlider;
	it->setName(name);
	it->setLabel(label);
	it->setRange(min,max);
	it->setValue(val);
	d->add_item(it);
	return it;
}
FoldingControlSpinBox *FoldingControlGroup::addSpinBox(QString name,QString label,int min,int max,int val) {
	FoldingControlSpinBox *it=new FoldingControlSpinBox;
	it->setName(name);
	it->setLabel(label);
	it->setRange(min,max);
	it->setValue(val);
	d->add_item(it);
	return it;
}
FoldingControlCheckBox *FoldingControlGroup::addCheckBox(QString name,QString label,bool checked) {
	FoldingControlCheckBox *it=new FoldingControlCheckBox;
	it->setName(name);
	it->setLabel(label);
	it->setValue(checked);
	d->add_item(it);
	return it;
}
FoldingControlRadioButton *FoldingControlGroup::addRadioButton(QString name,QString label,bool checked) {
	FoldingControlRadioButton *it=new FoldingControlRadioButton;
	it->setName(name);
	it->setLabel(label);
	it->setValue(checked);
	d->add_item(it);
	return it;
}
FoldingControlPushButton *FoldingControlGroup::addPushButton(QString name,QString label) {
	FoldingControlPushButton *it=new FoldingControlPushButton;
	it->setName(name);
	it->setLabel(label);
	d->add_item(it);
	return it;
}
FoldingControlPushButton *FoldingControlGroup::addToolButton(QString name,QString label) {
	FoldingControlPushButton *it=new FoldingControlPushButton;
	it->useToolButton();
	it->setName(name);
	it->setLabel(label);
	d->add_item(it);
	return it;
}
FoldingControlLineEdit *FoldingControlGroup::addLineEdit(QString name,QString label,QString txt) {
	FoldingControlLineEdit *it=new FoldingControlLineEdit;
	it->setName(name);
	it->setLabel(label);
	it->setValue(txt);
	d->add_item(it);
	return it;
}
FoldingControlComboBox *FoldingControlGroup::addComboBox(QString name,QString label) {
	FoldingControlComboBox *it=new FoldingControlComboBox;
	it->setName(name);
	it->setLabel(label);
	d->add_item(it);
	return it;
}
#ifdef USING_COLOR_BUTTON
FoldingControlColorButton *FoldingControlGroup::addColorButton(QString name,QString label,QColor col) {
	FoldingControlColorButton *it=new FoldingControlColorButton;
	it->setName(name);
	it->setLabel(label);
	it->setValue(col);
	d->add_item(it);
	return it;
}
#endif
FoldingControlWidget *FoldingControlGroup::addWidget(QString name,QWidget *W) {
	FoldingControlWidget *it=new FoldingControlWidget;
	it->setName(name);
	it->setWidget(W);
	d->add_item(it);
	return it;
}
void FoldingControlGroupPrivate::add_item(FoldingControlItem *it) {
	if (m_begin_row_on_next_item) {
		m_begin_row_on_next_item=false;
		m_current_row_layout=new QHBoxLayout;
		m_layout->addLayout(m_current_row_layout);
	}
	QList<QWidget *> list=it->widgets();
	foreach (QWidget *W,list) {
		if (m_current_row_layout)
			m_current_row_layout->addWidget(W);
		else
			m_layout->addWidget(W);
	}
	m_items << it;
}
void FoldingControlGroup::beginRow(bool add_spacing_at_end) {
	d->m_begin_row_on_next_item=true;
	d->m_add_spacing_at_end=add_spacing_at_end;
}
void FoldingControlGroup::endRow() {
	if ((d->m_current_row_layout)&&(d->m_add_spacing_at_end)) d->m_current_row_layout->addStretch();
	d->m_current_row_layout=0;
}

class FoldingControlPrivate {
public:
	FoldingControl *q;
	QList<FoldingControlGroup *> m_groups;
	QVBoxLayout *m_layout;
};
FoldingControl::FoldingControl(QWidget *parent) : QScrollArea(parent) 
{
	d=new FoldingControlPrivate;
	d->q=this;
	
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setWidgetResizable(true);
	
	QWidget *W=new QWidget;
	d->m_layout=new QVBoxLayout;
	d->m_layout->setSpacing(3);
	d->m_layout->setContentsMargins(3,3,3,3);
	W->setLayout(d->m_layout);
	setWidget(W);
	W->show();
}
FoldingControl::~FoldingControl()
{
	qDeleteAll(d->m_groups);
	d->m_groups.clear();
	delete d;
}
QStringList FoldingControl::groupNames() const {
	QStringList ret;
	foreach (FoldingControlGroup *G,d->m_groups) {
		ret << G->name();
	}
	return ret;
}
FoldingControlGroup *FoldingControl::group(QString name) {
	foreach (FoldingControlGroup *G,d->m_groups) {
		if (G->name()==name) return G;
	}
	return 0;
}
FoldingControlGroup *FoldingControl::addGroup(QString name,QString label) {
	FoldingControlGroup *G=new FoldingControlGroup(name);
	d->m_groups << G;
	G->setLabel(label);
	connect(G->d->m_button,SIGNAL(clicked()),this,SLOT(slot_group_button_clicked()));
	return G;
}
QVariant FoldingControl::value(QString group_name,QString item_name) {
	FoldingControlGroup *G=group(group_name);
	if (!G) return QVariant();
	FoldingControlItem *it=G->item(item_name);
	if (!it) return QVariant();
	return it->value();
}
void FoldingControl::setValue(QString group_name,QString item_name,QVariant value) {
	FoldingControlGroup *G=group(group_name);
	if (!G) return;
	FoldingControlItem *it=G->item(item_name);
	if (!it) return;
	it->setValue(value);
}
FoldingControlItem *FoldingControl::item(QString group_name,QString item_name) {
	FoldingControlGroup *G=group(group_name);
	if (!G) return 0;
	FoldingControlItem *it=G->item(item_name);
	return it;
}
void FoldingControl::updateControls() {
	while (d->m_layout->count()>0)
		d->m_layout->takeAt(0);
	foreach (FoldingControlGroup *G,d->m_groups) {
		d->m_layout->addWidget(G->d->m_button);
		d->m_layout->addWidget(G->d->m_frame);
		d->m_layout->addItem(G->d->m_extra_space);
		G->d->m_frame->setVisible(G->expanded());
	}
	d->m_layout->addStretch();
}
void FoldingControl::slot_group_button_clicked() {
	foreach (FoldingControlGroup *G,d->m_groups) {
		if (G->d->m_button==sender()) {
			G->setExpanded(!G->expanded());
		}
	}
	d->m_layout->update();
}

void FoldingControl::loadSettings(QSettings &settings) {
	QStringList grnames=groupNames();
	foreach (QString grname,grnames) {
		FoldingControlGroup *G=group(grname);
		settings.beginGroup(grname);
		QStringList itnames=G->itemNames();
		foreach (QString itname,itnames) {
			FoldingControlItem *it=G->item(itname);
			if (settings.contains(itname)) {
				it->setValue(settings.value(itname));
			}
		}
		settings.endGroup();
	}
}
void FoldingControl::saveSettings(QSettings &settings) {
	QStringList grnames=groupNames();
	foreach (QString grname,grnames) {
		FoldingControlGroup *G=group(grname);
		settings.beginGroup(grname);
		QStringList itnames=G->itemNames();
		foreach (QString itname,itnames) {
			FoldingControlItem *it=G->item(itname);
			settings.setValue(itname,it->value());
		}
		settings.endGroup();
	}
}
#ifdef USING_JARCHIVE
void FoldingControl::loadSettings(JArchive &archive) {
	QStringList grnames=groupNames();
	foreach (QString grname,grnames) {
		FoldingControlGroup *G=group(grname);
		archive.beginGroup(grname);
		QStringList itnames=G->itemNames();
		foreach (QString itname,itnames) {
			FoldingControlItem *it=G->item(itname);
			if (archive.contains(itname)) {
				it->setValue(archive.value(itname));
			}
		}
		archive.endGroup();
	}
}
void FoldingControl::saveSettings(JArchive &archive) {
	QStringList grnames=groupNames();
	foreach (QString grname,grnames) {
		FoldingControlGroup *G=group(grname);
		archive.beginGroup(grname);
		QStringList itnames=G->itemNames();
		foreach (QString itname,itnames) {
			FoldingControlItem *it=G->item(itname);
			archive.setValue(itname,it->value());
		}
		archive.endGroup();
	}
}
#endif
void FoldingControlGroup::addItem(QString name,FoldingControlItem *it) {
	it->setName(name);
	d->add_item(it);
}
void FoldingControl::setRange(QString group_name,QString item_name,int minval,int maxval) {
	FoldingControlGroup *G=group(group_name);
	if (!G) return;
	FoldingControlItem *it=G->item(item_name);
	if (!it) return;
	it->setRange(minval,maxval);	
}



