/*
   Copyright 2016 Nidium Inc. All rights reserved.
   Use of this source code is governed by a MIT license
   that can be found in the LICENSE file.
*/
#ifndef frontend_inputhandler_h__
#define frontend_inputhandler_h__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "Graphics/Geometry.h"


namespace Nidium {
namespace Graphics {
    class CanvasHandler;
}
}

namespace Nidium {
namespace Frontend {

static const char *InputEvent_Names[]
    = { "mousemove", "mousedown", "mouseup", "dblclick", "dragstart",
        "dragend",   "dragover",  "drop",    "drag",     "mousewheel" };


class InputEvent
{
public:
    enum Type
    {
        kMouseMove_Type = 0,
        kMouseClick_Type,
        kMouseClickRelease_Type,
        kMouseDoubleClick_Type,
        kMouseDragStart_Type,
        kMouseDragEnd_Type,
        kMouseDragOver_Type,
        kMouseDrop_Type,
        kMouseDrag_Type,
        kMouseWheel_Type
    };

    InputEvent(Type type,
               int ix,
               int iy,
               uint32_t *idata   = NULL,
               uint8_t idata_len = 0)
        : m_x(ix), m_y(iy), m_Next(NULL), m_PassThroughCanvas(NULL),
          m_Handler(NULL), m_Origin(NULL), m_depthAffectedCanvas(0),
          m_Type(type)
    {

        if (idata && idata_len <= 8) {
            memcpy(m_data, idata, sizeof(uint32_t) * idata_len);
        }
    }

    InputEvent *dupWithHandler(Graphics::CanvasHandler *handler)
    {
        InputEvent *dup = new InputEvent(*this);
        dup->m_Handler  = handler;
        dup->m_Origin   = this;

        m_PassThroughCanvas = handler;

        return dup;
    }

    bool isInRect(Graphics::Rect rect)
    {
        return rect.contains(m_x, m_y);
    }

    Type getType() const
    {
        return m_Type;
    }

    void inc()
    {
        m_depthAffectedCanvas++;
    }

    unsigned getDepth() const
    {
        return m_depthAffectedCanvas;
    }

    static const char *GetName(int type)
    {
        return InputEvent_Names[type];
    }

    Graphics::CanvasHandler *getUnderneathCanvas() const
    {
        return m_PassThroughCanvas;
    }

    void setData(int index, uint32_t data)
    {
        m_data[index] = data;
    }

    int m_x, m_y;
    uint32_t m_data[8];
    InputEvent *m_Next;
    Graphics::CanvasHandler *m_PassThroughCanvas;
    Graphics::CanvasHandler *m_Handler;
    InputEvent *m_Origin;
    unsigned m_depthAffectedCanvas;

private:
    Type m_Type;
};

class InputHandler
{
public:

    void pushEvent(InputEvent *ev);

    void clear();

    InputEvent *getEvents() const
    {
        return m_InputEvents.head;
    }

private:
    struct
    {
        InputEvent *head = NULL;
        InputEvent *queue = NULL;
    } m_InputEvents;

};

}
}

#endif
