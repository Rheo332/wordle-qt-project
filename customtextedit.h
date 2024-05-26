#ifndef CUSTOMTEXTEDIT_H
#define CUSTOMTEXTEDIT_H

#include <QKeyEvent>
#include <QTextEdit>

class CustomTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    CustomTextEdit(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

Q_SIGNALS:
    void keyPress(QKeyEvent *event);
};

#endif // CUSTOMTEXTEDIT_H
