/*
 * Copyright (C) 2023 Jan Grulich <jgrulich@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef QCATPPUCCIN_DECORATIONS_H
#define QCATPPUCCIN_DECORATIONS_H

#include <QtCore/QDateTime>
#include <QtGui/QPixmap>

#include <QtWaylandClient/private/qwaylandabstractdecoration_p.h>

#include <memory>

using namespace QtWaylandClient;

class QDBusVariant;
class QPainter;

class QCatppuccinDecorations : public QWaylandAbstractDecoration
{
    Q_OBJECT
public:
    enum ColorType {
        Background,
        BackgroundInactive,
        Foreground,
        ForegroundInactive,
        Border,
        BorderInactive,
        ButtonBackground,
        ButtonBackgroundInactive,
        HoveredButtonBackground,
        PressedButtonBackground
    };

    enum CatppuccinFlavor { Latte, Frappe, Macchiato, Mocha };
    Q_ENUM(CatppuccinFlavor)
    static const QMap<ColorType, QColor> catppuccinPalettes[4];

    enum Placement { Left = 0, Right = 1 };
    enum Button { None = 0x0, Close = 0x1, Minimize = 0x02, Maximize = 0x04 };
    Q_DECLARE_FLAGS(Buttons, Button);
    enum ButtonIcon { CloseIcon, MinimizeIcon, MaximizeIcon, RestoreIcon };

    QCatppuccinDecorations();
    virtual ~QCatppuccinDecorations() = default;

protected:
#ifdef HAS_QT6_SUPPORT
    QMargins margins(MarginsType marginsType = Full) const override;
#else
    QMargins margins() const override;
#endif
    void paint(QPaintDevice *device) override;
    void paintButton(Button button, QPainter *painter);
    bool handleMouse(QWaylandInputDevice *inputDevice, const QPointF &local, const QPointF &global,
                     Qt::MouseButtons b, Qt::KeyboardModifiers mods) override;
#if QT_VERSION >= 0x060000
    bool handleTouch(QWaylandInputDevice *inputDevice, const QPointF &local, const QPointF &global,
                     QEventPoint::State state, Qt::KeyboardModifiers mods) override;
#else
    bool handleTouch(QWaylandInputDevice *inputDevice, const QPointF &local, const QPointF &global,
                     Qt::TouchPointState state, Qt::KeyboardModifiers mods) override;
#endif

private Q_SLOTS:
    void settingChanged(const QString &group, const QString &key, const QDBusVariant &value);

private:
    void initConfiguration();
    void updateColors(CatppuccinFlavor variant);
    void updateIcons();
    void updateTitlebarLayout(const QString &layout);
    QRect windowContentGeometry() const;

    void forceRepaint();

    void processMouseTop(QWaylandInputDevice *inputDevice, const QPointF &local, Qt::MouseButtons b,
                         Qt::KeyboardModifiers mods);
    void processMouseBottom(QWaylandInputDevice *inputDevice, const QPointF &local,
                            Qt::MouseButtons b, Qt::KeyboardModifiers mods);
    void processMouseLeft(QWaylandInputDevice *inputDevice, const QPointF &local,
                          Qt::MouseButtons b, Qt::KeyboardModifiers mods);
    void processMouseRight(QWaylandInputDevice *inputDevice, const QPointF &local,
                           Qt::MouseButtons b, Qt::KeyboardModifiers mods);

    bool clickButton(Qt::MouseButtons b, Button btn);
    bool doubleClickButton(Qt::MouseButtons b, const QPointF &local, const QDateTime &currentTime);
    bool updateButtonHoverState(Button hoveredButton);

    QRectF buttonRect(Button button) const;

    // Default GNOME configuration
    Placement m_placement = Right;
    QMap<Button, uint> m_buttons;

    QStaticText m_windowTitle;
    Button m_clicking = None;

    Buttons m_hoveredButtons = None;
    QDateTime m_lastButtonClick;
    QPointF m_lastButtonClickPosition;

    QMap<ColorType, QColor> m_colors;
    std::unique_ptr<QFont> m_font;
    QPixmap m_shadowPixmap;
    QMap<ButtonIcon, QString> m_icons;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QCatppuccinDecorations::Buttons)

#endif // QCATPPUCCIN_DECORATIONS_H
