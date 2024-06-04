#ifndef CUSTOMLINEEDIT_H
#define CUSTOMLINEEDIT_H

#include <QKeyEvent>
#include <QLineEdit>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

class CustomLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    CustomLineEdit(QWidget *parent = 0);
    ~CustomLineEdit();
    void setupAnimation();
    void startKeyPressAnimation();
    void startResetAnimation();
    void startWrongWordAnimation();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    QSequentialAnimationGroup *keyPressAnimationGroup = new QSequentialAnimationGroup();
    QSequentialAnimationGroup *resetAnimationGroup = new QSequentialAnimationGroup();
    QSequentialAnimationGroup *wrongWordAnimationGroup = new QSequentialAnimationGroup();

signals:
    void keyPressSignal(QKeyEvent *event);

private slots:
    void resetLineEdit();
};

#endif // CUSTOMLINEEDIT_H
