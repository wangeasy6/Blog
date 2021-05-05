---
title: QT代码小结 - UI进阶
date: 2021-05-01 13:11:06
toc: true
categories:
  - Technology
tags:
  - QT
  - c++
  - 树莓派
---

之前用QT Widgets写界面，在UI设计的效果上总要打个8折，主要就是卡在透明和阴影这两个效果上，而这两个效果对于界面的高级感又有很大的影响。

一度想要换Qt Quick，然而经过不断的探索，现在终于解决了这个难题。顺便记录一下其它几个样式的效果。

<!--more-->

<br/>

## 运行环境

硬件环境：树莓派CM4

软件环境：Qt 5.11.3

<br/>


## 圆角无锯齿+透明+阴影子界面

![MessageBox](/resources/qt_widgets_ui_advanced/message_box.png)

以一个提示弹窗为例，主要[代码实现](/resources/qt_widgets_ui_advanced/TransparentShadowEffacts.tar.gz)：

message_box.h

```c++
...
protected:
    void paintEvent(QPaintEvent *event);
...
```

message_box.cpp

```c++
#include "message_box.h"
#include "ui_message_box.h"

#include <QGraphicsDropShadowEffect>
#include <QPainter>

MessageBox::MessageBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageBox)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);          // 设置无窗体
    this->setAttribute(Qt::WA_TranslucentBackground);       // 设置背景透明

    // 相对父窗体居中
    if(this->parent() && this->parent()->isWidgetType())
    {
        this->move((this->parentWidget()->width() - this->width()) / 2,
               (this->parentWidget()->height() - this->height()) / 2);
    }

    // 阴影效果
    QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect(this);
    shadow_effect->setOffset(4,8);                          // 阴影偏移
    shadow_effect->setColor(QColor(0, 0x1E, 0x9A, 102));    // 阴影颜色
    shadow_effect->setBlurRadius(40);                       // 模糊半径
    this->setGraphicsEffect(shadow_effect);
}

void MessageBox::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    int radius = 10;
    QPainter painter(this);                                                     // 圆角大小
    painter.setRenderHint(QPainter::Antialiasing);                              // 抗锯齿
    painter.setBrush(QBrush( QColor(255, 255, 255, 255), Qt::SolidPattern));    // 背景颜色，透明度
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(0, 0, this->width(), this->height(), radius, radius);
}

MessageBox::~MessageBox()
{
    delete ui;
}
```

先要设置无窗体和背景透明，此时设置`background-image`、`border-image`就不会生效了，使用`paintEvent`绘制背景，最后给整体添加阴影效果。

如果想要使用UI给的非纯色图片作为背景，则可以使用`painter.drawPixmap(0, 0, this->width(), this->height(), QPixmap(":/resources/images/message_box_bg.png"));`来绘制背景图片，在图片素材中画出圆角边，甚至是带上阴影效果都是可以的。

阴影效果也可以添加给QPushButton、QLabel等控件。

<br/>

## 图片圆形显示

一般用于头像图片的圆形显示，使用`background-radius`或者`setMask`等方法会有锯齿问题。

实现代码：

```c++
// 此方法返回一个width*height的圆形图片
QPixmap PixmapToRound(const QPixmap &src, int width, int height)
{
    if (src.isNull()) {
        return QPixmap();
    }

    // 缩放到指定大小
    QPixmap src_scaled= src.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    // 新建一个背景透明的空白图片
    QPixmap pixmap(width, height);
    pixmap.fill(Qt::transparent);

    // 使用图片作为QPainter的绘制平面
    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    // 构造圆形的剪切板
    QPainterPath path;
    path.addEllipse(0, 0, width, height);
    painter.setClipPath(path);

    // 绘制
    painter.drawPixmap(0, 0, width, height, src_scaled);

    return pixmap;
}
```

<br/>

## 按钮多选一

QPushButton也可以像QCheckBox一样做互斥选项和多选一的效果。这样做的方便之处在于不用管QCheckBox的indicator。

实现代码：

```c++
QPushButton *btn1 = new QWidget(this);
btn1->setCheckable(true);
btn1->setAutoExclusive(true);
btn1->setStyleSheet("QPushButton{background-color:#2C7AD1;outline: none}"
                    "QPushButton:pressed{background-color:#3A3A3F}"
                    "QPushButton:checked{background-color:#3A3A3F}"
                   );

QPushButton *btn2 = new QWidget(this);
btn2->setCheckable(true);
btn2->setAutoExclusive(true);
btn2->setChecked(true);					//只能设置一个为选择状态
btn2->setStyleSheet("QPushButton{background-color:#2C7AD1;outline: none}"
                    "QPushButton:pressed{background-color:#3A3A3F}"
                    "QPushButton:checked{background-color:#3A3A3F}"
                   );
```

以上代码也可以直接在设计界面设置，勾选相应的勾选项，添加相应的样式即可。

<br/>

## Button渐变背景

如果UI的按钮只是简单的做了一个渐变效果，而你又不想因为这个添加一个素材，就可以直接用代码生成一个渐变背景的效果。

对于控件的background样式，除了常见的Qt::SolidPattern（实心模式），还有Qt::RadialGradientPattern（径向渐变）、Qt::LinearGradientPattern（线性渐变）、Qt::ConicalGradientPattern（锥形渐变）。

样式代码：

```css
/* linear gradient from white to green */
QTextEdit {
    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                stop:0 white, stop: 0.4 rgba(10, 20, 30, 40),
                                stop:1 rgb(0, 200, 230, 200));
}

/* conical gradient from white to green */
QTextEdit {
    background: qconicalgradient(cx:0.5, cy:0.5, angle:30,
                                 stop:0 white, stop:1 #00FF00);
}

/* radial gradient from white to green */
QTextEdit {
    background: qradialgradient(cx:0, cy:0, radius: 1,
                                fx:0.5, fy:0.5, stop:0 white, stop:1 green);
}
```

<br/>

## HMAC-SHA1

为什么要记一下这个算法呢，因为网上最多链接的那个算法是错的，google了好几页才找到这个算法。

实现代码：

```c++
QString hmac_sha1(const QString &key, const QString &secret)
{
   int text_length;
   QByteArray K;
   int K_length;

   K_length = secret.size();
   text_length = key.size();
   K = secret.toLatin1();

   if (K_length > 64) {
        QByteArray tempSecret;
        tempSecret.append(secret);
        K = QCryptographicHash::hash(tempSecret, QCryptographicHash::Sha1);
        K_length = 20;
   }

   QByteArray ipad;
   QByteArray opad;
   ipad.fill(0, 64);
   opad.fill(0, 64);
   ipad.replace(0, K_length, K);
   opad.replace(0, K_length, K);

   for (int i = 0; i < 64; i++) {
        ipad[i] = ipad[i] ^ 0x36;
        opad[i] = opad[i] ^ 0x5c;
   }

   QByteArray context;
   context.append(ipad, 64);
   context.append(key);

   QByteArray Sha1 = QCryptographicHash::hash(context, QCryptographicHash::Sha1);

   context.clear();
   context.append(opad, 64);
   context.append(Sha1);

   Sha1.clear();
   Sha1 = QCryptographicHash::hash(context, QCryptographicHash::Sha1);

   return Sha1.toHex();
}
```