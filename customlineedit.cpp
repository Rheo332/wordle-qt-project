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

void CustomLineEdit::setupKeyPressAnimation()
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
}

void CustomLineEdit::setupResetAnimation()
{
    QRect rectStartValue = QRect(this->geometry());
    QRect rectEndValue = QRect(this->geometry()).adjusted(-7, -7, 7, 7);

    QString name = this->objectName();
    QChar colChar = name.at(name.size() - 1);
    QChar rowChar = name.at(name.size() - 3);
    int col = colChar.digitValue();
    int row = rowChar.digitValue();

    QPropertyAnimation *rectGrowAnimation = new QPropertyAnimation(this, "geometry");
    rectGrowAnimation->setDuration(100);
    rectGrowAnimation->setStartValue(rectStartValue);
    rectGrowAnimation->setEndValue(rectEndValue);
    rectGrowAnimation->setEasingCurve(QEasingCurve::OutQuad);

    QPropertyAnimation *rectShrinkAnimation = new QPropertyAnimation(this, "geometry");
    rectShrinkAnimation->setDuration(100);
    rectShrinkAnimation->setStartValue(rectEndValue);
    rectShrinkAnimation->setEndValue(rectStartValue);
    rectShrinkAnimation->setEasingCurve(QEasingCurve::OutQuad);

    resetAnimationGroup->addPause((row + col - 2) * 70);
    resetAnimationGroup->addAnimation(rectGrowAnimation);
    resetAnimationGroup->addAnimation(rectShrinkAnimation);

    QObject::connect(resetAnimationGroup,
                     &QSequentialAnimationGroup::finished,
                     this,
                     &CustomLineEdit::resetLineEdit);
}

void CustomLineEdit::setupWrongWordAnimation()
{
    QRect rectStartValue = QRect(this->geometry());
    QRect rectPos1Value = QRect(this->geometry()).adjusted(-10, 0, -10, 0);
    QRect rectPos2Value = QRect(this->geometry()).adjusted(10, 0, 10, 0);

    QPropertyAnimation *rectMoveLeftAnimation = new QPropertyAnimation(this, "geometry");
    rectMoveLeftAnimation->setDuration(100);
    rectMoveLeftAnimation->setStartValue(rectStartValue);
    rectMoveLeftAnimation->setEndValue(rectPos1Value);
    rectMoveLeftAnimation->setEasingCurve(QEasingCurve::OutBounce);

    QPropertyAnimation *rectMoveRightAnimation = new QPropertyAnimation(this, "geometry");
    rectMoveRightAnimation->setDuration(100);
    rectMoveRightAnimation->setStartValue(rectPos1Value);
    rectMoveRightAnimation->setEndValue(rectPos2Value);
    rectMoveRightAnimation->setEasingCurve(QEasingCurve::OutBounce);

    QPropertyAnimation *rectMoveBackAnimation = new QPropertyAnimation(this, "geometry");
    rectMoveBackAnimation->setDuration(100);
    rectMoveBackAnimation->setStartValue(rectPos2Value);
    rectMoveBackAnimation->setEndValue(rectStartValue);
    rectMoveBackAnimation->setEasingCurve(QEasingCurve::OutBounce);

    wrongWordAnimationGroup->addAnimation(rectMoveLeftAnimation);
    wrongWordAnimationGroup->addAnimation(rectMoveRightAnimation);
    wrongWordAnimationGroup->addAnimation(rectMoveBackAnimation);
}

void CustomLineEdit::setupAnimation()
{
    setupKeyPressAnimation();
    setupResetAnimation();
    setupWrongWordAnimation();
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
