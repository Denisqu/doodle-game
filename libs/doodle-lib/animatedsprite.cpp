//#include "animatedsprite.h"

/*

// TODO: как передавать QPixmap внутрь класса, чтобы передалось владение объектом?
AnimatedSprite::AnimatedSprite(const QPixmap& spritesheet, QObject *parent) :
    QObject(parent), QGraphicsItem(),
    mTimer(this),
    mSpriteSheet(spritesheet),
    mCurrentFrame(0)
{


    // Подключаем сигнал от таймера к слоту перелистывания кадров спрайта
    connect(mTimer, &QTimer::timeout, this, &AnimatedSprite::nextFrame);
    timer->start(25);   // Запускаем спрайт на генерацию сигнала с периодичность 25 мс
}

QRectF AnimatedSprite::boundingRect() const
{
    return QRectF(-10,-10,20,20);
}



void AnimatedSprite::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    /* В отрисовщике графического объекта отрисовываем спрайт
     * Разберём все параметры данной функции
     * Первых два аргумента - это координат X и Y куда помещается QPixmap
     * Третий аргумент - это указатель на QPixmap
     * 4 и 5 аргументы - Координаты в В изображении QPixmap, откуда будет отображаться изображение
     * Задавая координату X с помощью перемнной currentFrame мы будем как бы передвигать камеру
     * по спрайту
     * и последние два аргумента - это ширина и высота отображаем части изображение, то есть кадра
     * */

/*
    painter->drawPixmap(-10,-10, *mSpriteSheet, mCurrentFrame, 0, 20,20);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void AnimatedSprite::nextFrame()
{
    /* По сигналу от таймера передвигаем на 20 пикселей точку отрисовки
     * Если currentFrame = 300 то обнуляем его, поскольку размер sprite sheet 300 пикселей на 20
     * */

/*
    mCurrentFrame += 20;
    if (mCurrentFrame >= 300 ) mCurrentFrame = 0;
    this->update(-10,-10,20,20); // и перерисовываем графический объект с новым кадром спрайта
}


*/
