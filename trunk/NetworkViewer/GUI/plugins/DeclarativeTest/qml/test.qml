import QtQuick 1.1
import NETVLibrary 1.0

Rectangle
{
    id: window
    width: 320
    height: 240
    color: "cyan"
    radius: 10

    QMLVariable
    {
        id: myQMLVariable
        value: 0
    }
    Column {
        anchors.fill: window

        Text {
            id: variable_value
            width: parent.width
            color: "black"
            text: myVariable.getValue()

            Connections
            {
                target: myVariable
                onValueChanged: variable_value.text = myVariable.getValue()
            }
        }
        Text {
            width: parent.width
            id: variable_name
            text: myVariable.getName()
        }
        Text {
            width: parent.width
            id: variable_type
            text: myVariable.getType()
        }
        Text {
            width: parent.width
            id: variable_description
            wrapMode: "WordWrap"
            text: myVariable.getDescription()
        }
    }

}
