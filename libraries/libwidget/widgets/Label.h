#pragma once

#include <libutils/String.h>
#include <libwidget/Widget.h>

class Label : public Widget
{
private:
    String _text = "Label";
    Anchor _anchor = Anchor::LEFT;

public:
    void text(String text)
    {
        _text = text;
        should_repaint();
    }

    Label(Widget *parent, String text);

    Label(Widget *parent, String text, Anchor anchor);

    void paint(Painter &painter, Recti rectangle);

    Vec2i size();
};
