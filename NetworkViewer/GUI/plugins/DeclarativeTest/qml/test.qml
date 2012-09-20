import QtQuick 1.1
import NETVLibrary 1.0

Rectangle
{
    id: window
    width: 320
    height: 240
    color: "red"

    QMLVariable
    {
        id: myQMLVariable
        value: 0
    }

    Text {
        id: variable_text
        anchors.fill: window
        color: "black"
        text: myVariable.getValue()

        function update_value()
        {
            text = myVariable.getValue();
        }

        Connections
        {
            target: myVariable
            onValueChanged: variable_text.text = myVariable.getValue()
        }
    }

}
