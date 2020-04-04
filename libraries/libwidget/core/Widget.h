#pragma once

#include <libgraphic/Shape.h>
#include <libsystem/utils/List.h>

struct Widget;
struct Event;
struct Painter;
struct Window;

typedef void (*WidgetDestroyCallback)(struct Widget *widget);
typedef void (*WidgetPaintCallback)(struct Widget *widget, struct Painter *painter);
typedef void (*WidgetEventCallback)(struct Widget *widget, struct Event *event);

typedef struct Widget
{
    const char *classname;

    WidgetDestroyCallback destroy;
    WidgetPaintCallback paint;
    WidgetEventCallback event;

    Rectangle bound;
    struct Widget *parent;
    struct Window *window;
    List *childs;
} Widget;

#define WIDGET(__subclass) ((Widget *)(__subclass))

void widget_initialize(
    Widget *widget,
    const char *classname,
    Widget *parent,
    Rectangle bound);

void widget_destroy(Widget *widget);

void widget_add_child(Widget *widget, Widget *child);

void widget_remove_child(Widget *widget, Widget *child);

void widget_dump(Widget *widget, int depth);

void widget_dispatch_event(Widget *widget, struct Event *event);

void widget_handle_event(Widget *widget, struct Event *event);

void widget_paint(Widget *widget, struct Painter *painter);
