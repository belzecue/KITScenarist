#include "ActItem.h"

#include "TextUtils.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>
#include <QScrollBar>
#include <QStyleOptionGraphicsItem>

namespace {
    /**
     * @brief Минимальная ширина акта
     * @brief Используется, если ещё не задана сцена
     */
    const int ACT_MIN_WIDTH = 100;

    /**
     * @brief Высота акта
     */
    const int ACT_HEIGHT = 30;
}


ActItem::ActItem(QGraphicsItem* _parent) :
    QObject(),
    QGraphicsItem(_parent),
    m_shadowEffect(new QGraphicsDropShadowEffect)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    setCacheMode(QGraphicsItem::NoCache);

    m_shadowEffect->setBlurRadius(7);
    m_shadowEffect->setXOffset(0);
    m_shadowEffect->setYOffset(1);
    setGraphicsEffect(m_shadowEffect.data());
}

void ActItem::setUuid(const QString& _uuid)
{
    if (m_uuid != _uuid) {
        m_uuid = _uuid;
    }
}

QString ActItem::uuid() const
{
    return m_uuid;
}

void ActItem::setTitle(const QString& _title)
{
    if (m_title != _title) {
        m_title = _title;
        update();
    }
}

QString ActItem::title() const
{
    return m_title;
}

void ActItem::setDescription(const QString& _description)
{
    if (m_description != _description) {
        m_description = _description;
        update();
    }
}

QString ActItem::description() const
{
    return m_description;
}

void ActItem::setColors(const QString& _colors)
{
    if (m_colors != _colors) {
        m_colors = _colors;
        update();
    }
}

QString ActItem::colors() const
{
    return m_colors;
}

int ActItem::type() const
{
    return Type;
}

QRectF ActItem::boundingRect() const
{
    QRectF result(0, 0, ACT_MIN_WIDTH, ACT_HEIGHT);
    if (scene() != nullptr
        && !scene()->views().isEmpty()) {
        const QGraphicsView* view = scene()->views().first();
        const QPointF viewTopLeftPoint = view->mapToScene(QPoint(0, 0));
        const int scrollDelta = view->verticalScrollBar()->isVisible() ? view->verticalScrollBar()->width() : 0;
        const QPointF viewTopRightPoint = view->mapToScene(QPoint(view->width() - scrollDelta, 0));
        result.setLeft(viewTopLeftPoint.x());
        result.setWidth(viewTopRightPoint.x() - viewTopLeftPoint.x());
    }

    return result;
}

void ActItem::paint(QPainter* _painter, const QStyleOptionGraphicsItem* _option, QWidget* _widget)
{
    Q_UNUSED(_widget);

    _painter->save();

    {
        const QPalette palette = _option->palette;
        const QRectF actRect = boundingRect();
        const QStringList colors = m_colors.split(";", QString::SkipEmptyParts);

        //
        // Рисуем фон
        //
        // ... заданным цветом, если он задан
        //
        if (!colors.isEmpty()) {
            _painter->setBrush(QColor(colors.first()));
            _painter->setPen(QColor(colors.first()));
        }
        //
        // ... или стандартным цветом
        //
        else {
            _painter->setBrush(palette.base());
            _painter->setPen(palette.base().color());
        }
        _painter->drawRect(actRect);

        //
        // Рисуем дополнительные цвета
        //
        if (!m_colors.isEmpty()) {
            QStringList colorsNamesList = m_colors.split(";", QString::SkipEmptyParts);
            colorsNamesList.removeFirst();
            //
            // ... если они есть
            //
            if (!colorsNamesList.isEmpty()) {
                const qreal additionalColorWidth = 20;
                QRectF colorRect(actRect.left(), actRect.top(), additionalColorWidth, actRect.height());
                for (int colorIndex = colorsNamesList.size(); colorIndex > 0; --colorIndex) {
                    colorRect.moveLeft(actRect.right() - colorIndex*additionalColorWidth);
                    _painter->fillRect(colorRect, QColor(colorsNamesList.value(colorIndex - 1)));
                }
            }
        }

        //
        // Рисуем заголовок
        //
        QTextOption textoption;
        textoption.setAlignment(Qt::AlignTop | Qt::AlignLeft);
        textoption.setWrapMode(QTextOption::NoWrap);
        QFont font = _painter->font();
        font.setBold(true);
        _painter->setFont(font);
        _painter->setPen(palette.text().color());
        const int titleHeight = _painter->fontMetrics().height();
        const QRectF titleRect(actRect.left() + 7, 9, _painter->fontMetrics().width(m_title), titleHeight);
        _painter->drawText(titleRect, m_title, textoption);

        //
        // Рисуем описание
        //
        font.setBold(false);
        _painter->setFont(font);
        const int spacing = titleRect.height() / 2;
        const QRectF descriptionRect(titleRect.right() + spacing, 9, actRect.size().width() - titleRect.width() - spacing - 7, titleHeight);
        _painter->drawText(descriptionRect, m_description, textoption);
    }

    _painter->restore();
}

QVariant ActItem::itemChange(QGraphicsItem::GraphicsItemChange _change, const QVariant& _value)
{
    switch (_change) {
        case ItemPositionChange: {
            QPointF newPositionCorrected = _value.toPointF();
            newPositionCorrected.setX(0);
            return QGraphicsItem::itemChange(_change, newPositionCorrected);
        }

        default: {
            break;
        }
    }

    return QGraphicsItem::itemChange(_change, _value);
}
