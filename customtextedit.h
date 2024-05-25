#ifndef CUSTOMTEXTEDIT_H
#define CUSTOMTEXTEDIT_H

#include <QTextEdit>

class CustomTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    CustomTextEdit(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // CUSTOMTEXTEDIT_H
