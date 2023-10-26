#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

#include <QObject>
#include <QGraphicsItem>
#include <QTimer>
#include <QPixmap>
#include <QPainter>

class AnimatedSprite : public QObject, public QGraphicsItem
{
/*
    Q_OBJECT
public:
    explicit AnimatedSprite(const QPixmap& spritesheet, QObject *parent = 0);

signals:

public slots:

private slots:
    void nextFrame();   // Слот для пролистывания изображения в QPixmap

private:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual QRectF boundingRect() const override;

private:
    const QTimer mTimer;      // Таймер для пролистывания изображения в QPixmap
    const QPixmap mSpriteSheet;   // В данный объект QPixamp будет помещён спрайт
    int mCurrentFrame;   // Координата X, с которой начинает очередной кадр спрайта
*/
};

#endif // ANIMATEDSPRITE_H
