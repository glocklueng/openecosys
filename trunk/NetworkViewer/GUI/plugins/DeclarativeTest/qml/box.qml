import QtQuick 1.1


Rectangle {
    id: item_test
    x: 500
    y: 500
    width: 200
    height: 200
    radius: 10
    color: "light gray"
    opacity: 0.8

    property variant variable: null

    BorderImage{
        anchors.fill: item_test
        source: "qrc:/DeclarativeTest/images/brushed_aluminum.png"
        horizontalTileMode: BorderImage.Repeat
        verticalTileMode: BorderImage.Repeat
    }

    Item {
        id: window_rect
        width: item_test.width
        height: 30
        anchors.top: item_test.top

        BorderImage{
            anchors.fill: window_rect
            source: "qrc:/DeclarativeTest/images/clear_aluminum.png"
            horizontalTileMode: BorderImage.Stretch
            verticalTileMode: BorderImage.Stretch
        }

        Text
        {
            id: text_label
            color: "black"
            width: parent.width
            height: parent.height
            anchors.horizontalCenter: window_rect.horizontalCenter
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
        anchors.centerIn: item_test
        width: parent.width
        height: 50
        font.pixelSize: 18
        text: variable.getValue()
        Connections
        {
            target: variable
            onValueChanged: textInput.text = item_test.variable.getValue()
        }
    }

    Item
    {
        id: close_rect
        anchors.right: window_rect.right
        width: window_rect.height
        height: window_rect.height

        BorderImage
        {
            anchors.fill: close_rect
            source: "qrc:/DeclarativeTest/images/close.png"
        }

        MouseArea
        {
            id: close_mouse_area
            anchors.fill: close_rect

            onPressed:
            {

            }
            onReleased:
            {
                item_test.destroy();
            }
        }


    }


    Item
    {
        id: bottom_corner
        width: 30
        height: 30
        anchors.right: item_test.right
        anchors.bottom: item_test.bottom


        BorderImage
        {
            opacity: 1
            id:resize_image
            anchors.fill: bottom_corner
            source: "qrc:/DeclarativeTest/images/resize.png"
            visible: false
        }

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
                resize_image.visible = true
                //print("Mouse Position changed",mouseX,mouseY)

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
                print("onPressed:",mouseX,mouseY, mouse.button)
            }

            onReleased:
            {


            }

        }
    }
}
