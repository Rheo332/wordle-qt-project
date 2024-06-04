#include "customlineedit.h"

CustomLineEdit::CustomLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    setReadOnly(true);
    setAlignment(Qt::AlignCenter);
    setFont(QFont(this->font().family(), 24, QFont::Bold));
}

CustomLineEdit::~CustomLineEdit()
{
    delete keyPressAnimationGroup;
    delete wrongWordAnimationGroup;
    delete resetAnimationGroup;
}

void CustomLineEdit::startKeyPressAnimation()
{
    keyPressAnimationGroup->start();
}

void CustomLineEdit::startResetAnimation()
{
    resetAnimationGroup->start();
}

void CustomLineEdit::startWrongWordAnimation()
{
    wrongWordAnimationGroup->start();
}

void CustomLineEdit::setupAnimation()
{
    QRect rectStartValue = QRect(this->geometry());
    QRect rectEndValue = QRect(this->geometry()).adjusted(-7, -7, 7, 7);

    QPropertyAnimation *rectGrowAnimation = new QPropertyAnimation(this, "geometry");
    rectGrowAnimation->setDuration(130);
    rectGrowAnimation->setStartValue(rectStartValue);
    rectGrowAnimation->setEndValue(rectEndValue);
    rectGrowAnimation->setEasingCurve(QEasingCurve::OutQuad);

    QPropertyAnimation *rectShrinkAnimation = new QPropertyAnimation(this, "geometry");
    rectShrinkAnimation->setDuration(180);
    rectShrinkAnimation->setStartValue(rectEndValue);
    rectShrinkAnimation->setEndValue(rectStartValue);
    rectShrinkAnimation->setEasingCurve(QEasingCurve::OutBounce);

    keyPressAnimationGroup->addAnimation(rectGrowAnimation);
    keyPressAnimationGroup->addAnimation(rectShrinkAnimation);

    QString name = this->objectName();
    QChar colChar = name.at(name.size() - 1);
    QChar rowChar = name.at(name.size() - 3);
    int col = colChar.digitValue();
    int row = rowChar.digitValue();

    QPropertyAnimation *rectGrowAnimation2 = new QPropertyAnimation(this, "geometry");
    rectGrowAnimation2->setDuration(100);
    rectGrowAnimation2->setStartValue(rectStartValue);
    rectGrowAnimation2->setEndValue(rectEndValue);
    rectGrowAnimation2->setEasingCurve(QEasingCurve::OutQuad);

    QPropertyAnimation *rectShrinkAnimation2 = new QPropertyAnimation(this, "geometry");
    rectShrinkAnimation2->setDuration(100);
    rectShrinkAnimation2->setStartValue(rectEndValue);
    rectShrinkAnimation2->setEndValue(rectStartValue);
    rectShrinkAnimation2->setEasingCurve(QEasingCurve::OutQuad);

    resetAnimationGroup->addPause((row + col - 2) * 70);
    resetAnimationGroup->addAnimation(rectGrowAnimation2);
    resetAnimationGroup->addAnimation(rectShrinkAnimation2);

    QObject::connect(resetAnimationGroup,
                     &QSequentialAnimationGroup::finished,
                     this,
                     &CustomLineEdit::resetLineEdit);

    // TODO: add wrongWordAnimationGroup
}

void CustomLineEdit::keyPressEvent(QKeyEvent *event)
{
    emit keyPressSignal(event);
}

void CustomLineEdit::resetLineEdit()
{
    setText("");
    setStyleSheet("");
}
