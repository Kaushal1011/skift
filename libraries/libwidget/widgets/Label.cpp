#include <libgraphic/Painter.h>
#include <libsystem/core/CString.h>
#include <libwidget/Window.h>
#include <libwidget/widgets/Label.h>

Label::Label(Widget *parent, String text)
    : Label(parent, text, Anchor::LEFT)
{
}

Label::Label(Widget *parent, String text, Anchor anchor)
    : Widget(parent)
{
    _text = text;
    _anchor = anchor;
}

void Label::paint(Painter &painter, Recti rectangle)
{
    __unused(rectangle);

    painter.draw_string_within(
        *font(),
        _text.cstring(),
        bound(),
        _anchor,
        color(THEME_FOREGROUND));
}

Vec2i Label::size()
{
    return font()->mesure_string(_text.cstring()).size();
}
