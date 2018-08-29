#ifndef foldingcontrol_H
#define foldingcontrol_H

#include <QSettings>
#include <QWidget>
#include <QVariant>
#include <QSlider>
#include <QLabel>
#include <QCheckBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QScrollArea>
#include <QPushButton>
#include <QToolButton>
#include <QSpinBox>
#include <QComboBox>
#ifdef USING_COLOR_BUTTON
#include "qwwcolorbutton.h"
#endif
#ifdef USING_JARCHIVE
#include "jarchive.h"
#endif

class FoldingControlItemPrivate;
class FoldingControlGroupPrivate;
class FoldingControlPrivate;
class FoldingControlItem;
class FoldingControlGroup;
class FoldingControl;

class FoldingControlItem {
public:
	friend class FoldingControlItemPrivate;
	friend class FoldingControlGroupPrivate;
	friend class FoldingControlPrivate;
	friend class FoldingControlGroup;
	friend class FoldingControl;
public:
	FoldingControlItem();
	virtual ~FoldingControlItem();
	void setName(const QString &name);
	QString name() const;
	virtual QVariant value() const=0;
	virtual void setValue(const QVariant &val)=0;
	virtual QList<QWidget *> widgets() const=0;
	virtual void setRange(int min,int max);
	virtual void setLabel(QString label);
private:
	FoldingControlItemPrivate *d;
};

class FoldingControlSliderPrivate;
class FoldingControlSlider : public FoldingControlItem {
public:
	FoldingControlSlider();
	virtual ~FoldingControlSlider();
	void setRange(int min,int max);
	void setLabel(QString txt);
	QSlider *sliderObject();
	QLabel *labelObject();
	void connectOnChanged(QObject *receiver,const char *method,Qt::ConnectionType type = Qt::AutoConnection );
	virtual QVariant value() const;
	virtual void setValue(const QVariant &val);
	virtual QList<QWidget *> widgets() const;
private:
	FoldingControlSliderPrivate *d;
};

class FoldingControlSpinBoxPrivate;
class FoldingControlSpinBox : public FoldingControlItem {
public:
	FoldingControlSpinBox();
	virtual ~FoldingControlSpinBox();
	void setRange(int min,int max);
	void setLabel(QString txt);
	QSpinBox *spinBoxObject();
	QLabel *labelObject();
	void connectOnChanged(QObject *receiver,const char *method,Qt::ConnectionType type = Qt::AutoConnection );
	virtual QVariant value() const;
	virtual void setValue(const QVariant &val);
	virtual QList<QWidget *> widgets() const;
private:
	FoldingControlSpinBoxPrivate *d;
};

class FoldingControlCheckBoxPrivate;
class FoldingControlCheckBox : public FoldingControlItem {
public:
	FoldingControlCheckBox();
	virtual ~FoldingControlCheckBox();
	void setLabel(QString txt);
	QCheckBox *checkBoxObject();
	void connectOnChanged(QObject *receiver,const char *method,Qt::ConnectionType type = Qt::AutoConnection );
	virtual QVariant value() const;
	virtual void setValue(const QVariant &val);
	virtual QList<QWidget *> widgets() const;
private:
	FoldingControlCheckBoxPrivate *d;
};

class FoldingControlRadioButtonPrivate;
class FoldingControlRadioButton : public FoldingControlItem {
public:
	FoldingControlRadioButton();
	virtual ~FoldingControlRadioButton();
	void setLabel(QString txt);
	QRadioButton *radioButtonObject();
	void connectOnChanged(QObject *receiver,const char *method,Qt::ConnectionType type = Qt::AutoConnection );
	virtual QVariant value() const;
	virtual void setValue(const QVariant &val);
	virtual QList<QWidget *> widgets() const;
private:
	FoldingControlRadioButtonPrivate *d;
};

class FoldingControlPushButtonPrivate;
class FoldingControlPushButton : public FoldingControlItem {
public:
	FoldingControlPushButton();
	virtual ~FoldingControlPushButton();
	void setLabel(QString txt);
	QPushButton *pushButtonObject();
	QToolButton *toolButtonObject();
	void useToolButton();
	void connectOnClicked(QObject *receiver,const char *method,Qt::ConnectionType type = Qt::AutoConnection );
	virtual QVariant value() const;
	virtual void setValue(const QVariant &val);
	virtual QList<QWidget *> widgets() const;
	
private:
	FoldingControlPushButtonPrivate *d;
};

class FoldingControlLineEditPrivate;
class FoldingControlLineEdit : public FoldingControlItem {
public:
	FoldingControlLineEdit();
	virtual ~FoldingControlLineEdit();
	void setLabel(QString txt);
	QLabel *labelObject();
	QLineEdit *lineEditObject();
	void connectOnChanged(QObject *receiver,const char *method,Qt::ConnectionType type = Qt::AutoConnection );
	virtual QVariant value() const;
	virtual void setValue(const QVariant &val);
	virtual QList<QWidget *> widgets() const;
private:
	FoldingControlLineEditPrivate *d;
};

class FoldingControlComboBoxPrivate;
class FoldingControlComboBox : public FoldingControlItem {
public:
	FoldingControlComboBox();
	virtual ~FoldingControlComboBox();
	QComboBox *comboBoxObject();
	QLabel *labelObject();
	void connectOnChanged(QObject *receiver,const char *method,Qt::ConnectionType type = Qt::AutoConnection );
	void setLabel(QString txt);
	virtual QVariant value() const;
	virtual void setValue(const QVariant &val);
	virtual QList<QWidget *> widgets() const;
	void clear();
	void addItem(const QString &text,const QString &value="");
	void addItem(const QIcon &icon, const QString &text,const QString &value="");
private:
	FoldingControlComboBoxPrivate *d;
};

#ifdef USING_COLOR_BUTTON
class FoldingControlColorButtonPrivate;
class FoldingControlColorButton : public FoldingControlItem {
public:
	FoldingControlColorButton();
	virtual ~FoldingControlColorButton();
	void setLabel(QString txt);
	QwwColorButton *colorButtonObject();
	QLabel *labelObject();
	void connectOnChanged(QObject *receiver,const char *method,Qt::ConnectionType type = Qt::AutoConnection );
	virtual QVariant value() const;
	virtual void setValue(const QVariant &val);
	virtual QList<QWidget *> widgets() const;
private:
	FoldingControlColorButtonPrivate *d;
};
#endif

class FoldingControlWidgetPrivate;
class FoldingControlWidget : public FoldingControlItem {
public:
	FoldingControlWidget();
	virtual ~FoldingControlWidget();
	void setWidget(QWidget *W);
	QWidget *widgetObject();
	virtual QVariant value() const;
	virtual void setValue(const QVariant &val);
	virtual QList<QWidget *> widgets() const;
private:
	FoldingControlWidgetPrivate *d;
};

class FoldingControlGroup {
public:
	friend class FoldingControlGroupPrivate;
	friend class FoldingControlPrivate;
	friend class FoldingControl;
private:
	FoldingControlGroup(QString name_in);
public:
	virtual ~FoldingControlGroup();
	QStringList itemNames() const;
	FoldingControlItem *item(QString name);
	QString name() const;
	bool expanded() const;
	void setExpanded(bool val);
	void setCollapsible(bool val);
	void setLabel(const QString &label);
	FoldingControlSlider *addSlider(QString name,QString label,int min=0,int max=100,int val=50);
	FoldingControlSpinBox *addSpinBox(QString name,QString label,int min=0,int max=100,int val=0);
	FoldingControlCheckBox *addCheckBox(QString name,QString label,bool checked);
	FoldingControlRadioButton *addRadioButton(QString name,QString label,bool checked);
	FoldingControlPushButton *addPushButton(QString name,QString label);
	FoldingControlPushButton *addToolButton(QString name,QString label);
	FoldingControlLineEdit *addLineEdit(QString name,QString label,QString txt);
	FoldingControlComboBox *addComboBox(QString name,QString label);
	#ifdef USING_COLOR_BUTTON
	FoldingControlColorButton *addColorButton(QString name,QString label,QColor col);
	#endif
	FoldingControlWidget *addWidget(QString name,QWidget *W);
	void addItem(QString name,FoldingControlItem *it);
	void beginRow(bool add_spacing_at_end=true);
	void endRow();
public:
private:
	FoldingControlGroupPrivate *d;
};

class FoldingControl : public QScrollArea {
	Q_OBJECT
public:
	friend class FoldingControlPrivate;
	FoldingControl(QWidget *parent=0);
	virtual ~FoldingControl();
	QStringList groupNames() const;
	FoldingControlGroup *group(QString name);
	FoldingControlGroup *addGroup(QString name,QString label);
	void updateControls();
	QVariant value(QString group_name,QString item_name);
	void setValue(QString group_name,QString item_name,QVariant value);
	void setRange(QString group_name,QString item_name,int minval,int maxval);
	FoldingControlItem *item(QString group_name,QString item_name);
	void saveSettings(QSettings &settings);
	void loadSettings(QSettings &settings);
	#ifdef USING_JARCHIVE
	void loadSettings(JArchive &archive);
	void saveSettings(JArchive &archive);
	#endif
private slots:
	void slot_group_button_clicked();
private:
	FoldingControlPrivate *d;
};

#endif
