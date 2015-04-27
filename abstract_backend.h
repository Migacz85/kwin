/********************************************************************
 KWin - the KDE window manager
 This file is part of the KDE project.

Copyright (C) 2015 Martin Gräßlin <mgraesslin@kde.org>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/
#ifndef KWIN_ABSTRACT_BACKEND_H
#define KWIN_ABSTRACT_BACKEND_H
#include <kwin_export.h>
#include <QImage>
#include <QObject>

namespace KWin
{

class OpenGLBackend;
class QPainterBackend;
class Screens;

namespace Wayland
{
class WaylandCursorTheme;
}

class KWIN_EXPORT AbstractBackend : public QObject
{
    Q_OBJECT
public:
    virtual ~AbstractBackend();

    virtual void installCursorFromServer();
    virtual void installCursorImage(Qt::CursorShape shape);
    virtual Screens *createScreens(QObject *parent = nullptr);
    virtual OpenGLBackend *createOpenGLBackend();
    virtual QPainterBackend *createQPainterBackend();

    bool usesSoftwareCursor() const {
        return m_softWareCursor;
    }
    QImage softwareCursor() const {
        return m_cursor.image;
    }
    QPoint softwareCursorHotspot() const {
        return m_cursor.hotspot;
    }
    void markCursorAsRendered();

    bool handlesOutputs() const {
        return m_handlesOutputs;
    }

Q_SIGNALS:
    void cursorChanged();

protected:
    explicit AbstractBackend(QObject *parent = nullptr);
    void setSoftWareCursor(bool set);
    void updateCursorFromServer();
    void updateCursorImage(Qt::CursorShape shape);
    void handleOutputs() {
        m_handlesOutputs = true;
    }

private Q_SLOTS:
    void installThemeCursor(quint32 id, const QPoint &hotspot);

private:
    void triggerCursorRepaint();
    bool m_softWareCursor = false;
    struct {
        QPoint hotspot;
        QImage image;
        QPoint lastRenderedPosition;
    } m_cursor;
    Wayland::WaylandCursorTheme *m_cursorTheme = nullptr;
    bool m_handlesOutputs = false;
};

}

#endif