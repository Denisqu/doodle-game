#ifndef SPRITEANIMATOR_H
#define SPRITEANIMATOR_H

#include <QObject>

class AnimatedSprite;

class SpriteAnimator : public QObject
{
    Q_OBJECT
public:
    explicit SpriteAnimator(QObject *parent = nullptr);

private:
    QList<AnimatedSprite> mAnimatedSprites;

signals:

};

#endif // SPRITEANIMATOR_H
