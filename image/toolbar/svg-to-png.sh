#!/bin/bash
# svg-to-png
# Convert SVG files to 1x and 2x PNGs. Dump a list of Qt resource
# file entries upon successful completion.
#
# Copyright 2014 Gerald Combs <gerald [AT] wireshark.org>
#
# Wireshark - Network traffic analyzer
# By Gerald Combs <gerald@wireshark.org>
# Copyright 1998 Gerald Combs
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

COMMON_ARGS="--export-area-page"
ONE_X_ARGS="--export-width=24 --export-height=24"
TWO_X_ARGS="--export-width=48 --export-height=48"

set_source_svg() {
    local out_icon=$1
    case $out_icon in
    x-capture-options)
        SOURCE_SVG=x-capture-options-gear.svg
        ;;
    x-capture-restart)
        SOURCE_SVG=x-capture-restart-fin.svg
        ;;
    x-capture-stop)
        SOURCE_SVG=x-capture-stop-red.svg
        ;;
    *)
        SOURCE_SVG=$out_icon.svg
        ;;
    esac
}

ICONS="
    edit-find
    go-first
    go-jump
    go-last
    go-next
    go-previous
    x-capture-file-reload
    x-capture-options
    x-capture-restart
    x-capture-start.on
    x-capture-start
    x-capture-stop
    x-colorize-packets
    x-resize-columns
    x-stay-last
    zoom-in
    zoom-original
    zoom-out
    "

# XXX Add support for 16 pixel icons.
for SIZE in 24 ; do
    SIZE_DIR=${SIZE}x${SIZE}
    cd $SIZE_DIR

    QRC_FILES=""
    for ICON in $ICONS ; do
        set_source_svg $ICON
        ONE_X_PNG=${ICON}.png
        TWO_X_PNG=${ICON}@2x.png

        inkscape $COMMON_ARGS $ONE_X_ARGS \
            --file=$SOURCE_SVG --export-png=$ONE_X_PNG || exit 1

        inkscape $COMMON_ARGS $TWO_X_ARGS \
            --file=$SOURCE_SVG --export-png=$TWO_X_PNG || exit 1

        QRC_FILES="${QRC_FILES} ${ONE_X_PNG} ${TWO_X_PNG}"
    done

    # Save & close have to be done individually.

    for ICON in x-capture-file-close x-capture-file-save ; do
        ONE_X_PNG=${ICON}.png
        TWO_X_PNG=${ICON}@2x.png
        ONE_X_SVG=${ICON}.svg
        TWO_X_SVG=${ICON}@2x.svg

        inkscape $COMMON_ARGS $ONE_X_ARGS \
            --file=$ONE_X_SVG --export-png=$ONE_X_PNG || exit 1

        inkscape $COMMON_ARGS $TWO_X_ARGS \
            --file=$TWO_X_SVG --export-png=$TWO_X_PNG || exit 1

        QRC_FILES="${QRC_FILES} ${ONE_X_PNG} ${TWO_X_PNG}"
    done

    for QRC_FILE in $QRC_FILES ; do
        echo "        <file>toolbar/${SIZE_DIR}/${QRC_FILE}</file>"
    done
done