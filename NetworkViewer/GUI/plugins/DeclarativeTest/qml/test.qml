import QtQuick 1.1

Rectangle
{
    id: window
    width: 320
    height: 240
    color: "red"

    Text {
        id: variable_text
        anchors.fill: window
        color: "black"
        text: currentDateTime
    }

}
