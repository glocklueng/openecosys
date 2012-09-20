import QtQuick 1.1





Rectangle {
    id: item_test
    x: 500
    y: 500
    width: 200
    height: 200
    radius: 10
    color: "orange"

    property variant variable: null


    Rectangle {
        id: window_rect
        color: "pink"
        width: item_test.width
        height: 30
        anchors.top: item_test.top
        radius: item_test.radius

        Text
        {
            id: text_label
            color: "red"
            width: parent.width
            height: parent.height
            anchors.top: parent.top
            anchors.left: parent.left
            text: variable.getName()
        }

        MouseArea
        {
            id:window_label_mouse_area
            anchors.fill: window_rect
            drag.target: item_test
            drag.axis: Drag.XAxis | Drag.YAxis
            drag.minimumX: 0
            drag.maximumX: item_test.parent.width - item_test.width
            drag.minimumY: 0
            drag.maximumY: item_test.parent.height - item_test.height
        }
    }

    TextInput {
        id: textInput
        anchors.top: window_rect.bottom
        width: parent.width
        height: 50
        text: variable.getValue()
        Connections
        {
            target: variable
            onValueChanged: textInput.text = item_test.variable.getValue()
        }
    }

    Rectangle
    {
        id: close_rect
        anchors.left: item_test.left
        anchors.bottom: item_test.bottom
        width: 30
        height: 30
        color: "yellow"
        radius: item_test.radius

        Text {
            id: close_label
            color: "black"
            anchors.fill: parent
            text: "X"
        }

        MouseArea
        {
            id: close_mouse_area
            anchors.fill: close_label

            onPressed:
            {
                print("clicked")
                close_label.color = "blue"

            }
            onReleased:
            {
                item_test.destroy();
            }
        }


    }


    Rectangle
    {
        id: bottom_corner
        width: 30
        height: 30
        color: "yellow"
        anchors.right: item_test.right
        anchors.bottom: item_test.bottom
        radius: item_test.radius

        MouseArea
        {
            id: bottom_corner_mouse_area
            anchors.fill: bottom_corner
            hoverEnabled: true
            acceptedButtons: Qt.RightButton | Qt.LeftButton

            property real startX: 0
            property real startY: 0

            onMousePositionChanged:
            {

                print("Mouse Position changed",mouseX,mouseY)

                if (mouse.buttons == Qt.LeftButton)
                {

                    if (mouseX > -item_test.width && mouseY > -item_test.height)
                    {
                        print("mouse and buttons, ", mouse.buttons)
                        var previous_width = item_test.width
                        var previous_height = item_test.height
                        print("Changing item test width ", previous_width)
                        print("Changing item test height ", previous_height)
                        previous_width -= (startX - mouseX)
                        previous_height -= (startY - mouseY)
                        item_test.width = previous_width
                        item_test.height = previous_height
                    }
                }

            }

            onPressed:
            {

                startX = mouseX
                startY = mouseY

                text_label.text = "on_pressed"
                print("onPressed:",mouseX,mouseY, mouse.button)
            }

            onReleased:
            {


            }

        }
    }
}
