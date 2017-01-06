#ifndef SKINUTILS_H
#define SKINUTILS_H

#include "pch.h"

#define ZIP_SKIN_NAME        "skin.xml"

#define TAG_SKIN             "skin"

// Player window
#define TAG_PLAYER_WINDOW    "player_window"
#define TAG_PLAYER_PLAY      "play"
#define TAG_PLAYER_PAUSE     "pause"
#define TAG_PLAYER_PREV      "prev"
#define TAG_PLAYER_NEXT      "next"
#define TAG_PLAYER_STOP      "stop"
#define TAG_PLAYER_OPEN      "open"
#define TAG_PLAYER_MUTE      "mute"
#define TAG_PLAYER_LYRIC     "lyric"
#define TAG_PLAYER_EQUALIZER "equalizer"
#define TAG_PLAYER_PLAYLIST  "playlist"
#define TAG_PLAYER_BROWSER   "browser"
#define TAG_PLAYER_MINIMIZE  "minimize"
#define TAG_PLAYER_MINIMODE  "minimode"
#define TAG_PLAYER_EXIT      "exit"
#define TAG_PLAYER_PROGRESS  "progress"
#define TAG_PLAYER_VOLUME    "volume"
#define TAG_PLAYER_ICON      "icon"
#define TAG_PLAYER_INFO      "info"
#define TAG_PLAYER_LED       "led"
#define TAG_PLAYER_STEREO    "stereo"
#define TAG_PLAYER_STATUS    "status"
#define TAG_PLAYER_VISUAL    "visual"

// Lyric window
#define TAG_LYRIC_WINDOW         "lyric_window"
#define TAG_LYRIC_LYRIC          "lyric"
#define TAG_LYRIC_CLOSE          "close"
#define TAG_LYRIC_ONTOP          "ontop"
#define TAG_LYRIC_DESKLRC        "desklrc"

// Equalizer window
#define TAG_EQUALIZER_WINDOW     "equalizer_window"
#define TAG_EQUALIZER_CLOSE      "close"
#define TAG_EQUALIZER_ENABLED    "enabled"
#define TAG_EQUALIZER_PROFILE    "profile"
#define TAG_EQUALIZER_RESET      "reset"
#define TAG_EQUALIZER_BALANCE    "balance"
#define TAG_EQUALIZER_SURROUND   "surround"
#define TAG_EQUALIZER_PREAMP     "preamp"
#define TAG_EQUALIZER_EQFACTOR   "eqfactor"

// Playlist window
#define TAG_PLAYLIST_WINDOW      "playlist_window"
#define TAG_PLAYLIST_CLOSE     "close"
#define TAG_PLAYLIST_TOOLBAR   "toolbar"
#define TAG_PLAYLIST_SCROLLBAR "scrollbar"
#define TAG_PLAYLIST_PLAYLIST  "playlist"

// Mini window
#define TAG_MINI_WINDOW          "mini_window"
#define TAG_MINI_PLAY     "play"
#define TAG_MINI_PAUSE    "pause"
#define TAG_MINI_PREV     "prev"
#define TAG_MINI_NEXT     "next"
#define TAG_MINI_STOP     "stop"
#define TAG_MINI_OPEN     "open"
#define TAG_MINI_MUTE     "mute"
#define TAG_MINI_LYRIC    "lyric"
#define TAG_MINI_MINIMODE "minimode"
#define TAG_MINI_MINIMIZE "minimize"
#define TAG_MINI_EXIT     "exit"
#define TAG_MINI_ICON     "icon"
#define TAG_MINI_LED      "led"
#define TAG_MINI_INFO     "info"

// Desktio lyric
#define TAG_DESKLRC_BAR          "desklrc_bar"
#define TAG_DESKTOP_LYRIC_ICON     "icon"
#define TAG_DESKTOP_LYRIC_PLAY     "play"
#define TAG_DESKTOP_LYRIC_PAUSE    "pause"
#define TAG_DESKTOP_LYRIC_PREV     "prev"
#define TAG_DESKTOP_LYRIC_NEXT     "next"
#define TAG_DESKTOP_LYRIC_LIST     "list"
#define TAG_DESKTOP_LYRIC_SETTINGS "settings"
#define TAG_DESKTOP_LYRIC_KALAOK   "kalaok"
#define TAG_DESKTOP_LYRIC_LINES    "lines"
#define TAG_DESKTOP_LYRIC_LOCK     "lock"
#define TAG_DESKTOP_LYRIC_ONTOP    "ontop"
#define TAG_DESKTOP_LYRIC_RETURN   "return"
#define TAG_DESKTOP_LYRIC_CLOSE    "close"

// Browsder window
#define TAG_BROWSER_WINDOW          "browser_window"
#define TAG_BROWSER_CLOSE           "close"
#define TAG_BROWSER_BACKWARD        "backward"
#define TAG_BROWSER_FORWARD         "forward"
#define TAG_BROWSER_REFRESH         "refresh"
#define TAG_BROWSER_STARTUP         "startup"
#define TAG_BROWSER_LINKTXT         "linktxt"
#define TAG_BROWSER_BROWSER         "browser"
#define TAG_BROWSER_MINIBTN         "minibtn"
#define TAG_BROWSER_DOWNLOAD_BUTTON "downloadbtn"

// Attributes
#define ATTR_ALIGN               "align"
#define ATTR_AUTHOR              "author"
#define ATTR_BACKGROUND_COLOR    "background_olor"
#define ATTR_BAR_IMAGE           "bar_image"
#define ATTR_BLOCK_COLOR         "block_color"
#define ATTR_BOTTOM_COLOR        "bottom_color"
#define ATTR_BUTTONS_IMAGE       "buttons_image"
#define ATTR_CKBOX_IMAGE         "ckbox_image"
#define ATTR_COLOR               "color"
#define ATTR_COLOR_BKGND         "Color_Bkgnd"
#define ATTR_COLOR_BKGND2        "Color_Bkgnd2"
#define ATTR_COLOR_DURATION      "Color_Duration"
#define ATTR_COLOR_HILIGHT       "Color_Hilight"
#define ATTR_COLOR_NUMBER        "Color_Number"
#define ATTR_COLOR_SELECT        "Color_Select"
#define ATTR_COLOR_TEXT          "Color_Text"
#define ATTR_EMAIL               "email"
#define ATTR_EQ_INTERVAL         "eq_interval"
#define ATTR_FILL_IMAGE          "fill_image"
#define ATTR_FONT                "font"
#define ATTR_FONT_SIZE           "font_size"
#define ATTR_HIGHLIGHT_COLOR     "hilight_color"
#define ATTR_HOT_IMAGE           "hot_image"
#define ATTR_ICON                "icon"
#define ATTR_IMAGE               "image"
#define ATTR_INTERVAL            "interval"
#define ATTR_MIDDLE_COLOR        "middle_color"
#define ATTR_NAME                "name"
#define ATTR_POSITION            "position"
#define ATTR_RESIZE_RECT         "resize_rect"
#define ATTR_SELECTED_IMAGE      "selected_image"
#define ATTR_TEXT_COLOR          "text_color"
#define ATTR_THUMB_IMAGE         "thumb_image"
#define ATTR_THUMB_RESIZE_CENTER "thumb_resize_center"
#define ATTR_TOP_COLOR           "top_color"
#define ATTR_TRANSPARENT_COLOR   "transparent_color"
#define ATTR_URL                 "url"
#define ATTR_VERSION             "version"
#define ATTR_VERTICAL            "vertical"

namespace SkinUtils {
    QRect extractGeometry(QDomElement e, QString attr=ATTR_POSITION);
    QRect extractResizeRect(QDomElement e, QString attr=ATTR_RESIZE_RECT);
    Qt::Alignment extractAlignment(QDomElement e, QString attr=ATTR_ALIGN);
    QFont extractFont(QDomElement e);
    QRect strToGeometry(QString s);
    QRect strToRect(QString s);
    bool strToBool(QString s);
    Qt::Alignment strToAlign(QString s);
}

#endif // SKINUTILS_H
