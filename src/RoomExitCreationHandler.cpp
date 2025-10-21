/***************************************************************************
 *   Copyright (C) 2025 by OpenAI                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "RoomExitCreationHandler.h"

#include "TArea.h"
#include "TRoomDB.h"

#include "pre_guard.h"
#include <QMouseEvent>
#include <QtGlobal>
#include "post_guard.h"

RoomExitCreationHandler::RoomExitCreationHandler(T2DMap& mapWidget)
: mMapWidget(mapWidget)
{
}

bool RoomExitCreationHandler::matches(const T2DMap::MapInteractionContext& context) const
{
    if (!context.event || !mMapWidget.mpMap || !mMapWidget.mpMap->mpRoomDB) {
        return false;
    }

    switch (context.event->type()) {
    case QEvent::MouseMove:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
        return true;
    default:
        return false;
    }
}

bool RoomExitCreationHandler::handle(T2DMap::MapInteractionContext& context)
{
    if (!context.event || !mMapWidget.mpMap || !mMapWidget.mpMap->mpRoomDB) {
        return false;
    }

    switch (context.event->type()) {
    case QEvent::MouseMove:
        if (mMapWidget.mExitLinkDragActive) {
            mMapWidget.updateExitLinkDrag(context.widgetPositionF, context.mapPoint);
            return true;
        }

        if (context.buttons == Qt::NoButton) {
            if (mMapWidget.mMapViewOnly) {
                mMapWidget.resetExitHandleHover();
            } else {
                mMapWidget.updateExitHandleHover(context.widgetPositionF, context.area);
            }
        }
        return false;
    case QEvent::MouseButtonPress:
        if (context.button == Qt::LeftButton && !mMapWidget.mMapViewOnly) {
            if (mMapWidget.beginExitLinkDrag(context.widgetPositionF, context.mapPoint)) {
                return true;
            }
        }
        return false;
    case QEvent::MouseButtonRelease:
        if (context.button == Qt::LeftButton && mMapWidget.mExitLinkDragActive) {
            mMapWidget.endExitLinkDrag();
            if (!mMapWidget.mMapViewOnly) {
                mMapWidget.updateExitHandleHover(context.widgetPositionF, context.area);
            } else {
                mMapWidget.resetExitHandleHover();
            }
            return true;
        }
        return false;
    default:
        return false;
    }
}
