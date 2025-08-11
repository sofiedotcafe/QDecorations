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

#ifndef QCATPPUCCIN_DECORATIONS_PLUGIN_H
#define QCATPPUCCIN_DECORATIONS_PLUGIN_H

#include <QtWaylandClient/private/qwaylanddecorationplugin_p.h>

using namespace QtWaylandClient;

class QCatppuccinDecorationsPlugin : public QWaylandDecorationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QWaylandDecorationFactoryInterface_iid FILE "QCatppuccinDecorations.json")
public:
    QWaylandAbstractDecoration *create(const QString &system,
                                       const QStringList &paramList) override;
};

#endif // QCATPPUCCIN_DECORATIONS_PLUGIN_H
