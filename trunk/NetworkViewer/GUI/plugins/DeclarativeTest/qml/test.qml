import QtQuick 1.1
import NETVLibrary 1.0

Rectangle
{
    id: appWindow
    width: 1024
    height: 768

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

            Rectangle {
                id : button
                width: parent.width
                height: 20
                color: "gray"

                Text {
                    id: button_text
                    width: parent.width
                    height: parent.height
                    text: "Create patate"
                }
                MouseArea {
                    id: button_mouse_area
                    anchors.fill: button

                    function create_component()
                    {
                        print("should create component")

                        var dynamic_component = Qt.createComponent("box.qml");

                         if (dynamic_component.status == Component.Ready)
                         {
                             print("component ready")

                             var dynamic_window = dynamic_component.createObject(appWindow, {"x": 100, "y": 100});

                             if (dynamic_window == null)
                             {
                                 print("unable to create window")
                             }
                             else
                             {

                                 print("window ready")
                             }
                         }
                         else
                         {
                             print("ERROR - Component not found!")
                         }
                    }

                    onPressed:
                    {
                        print("button pressed")

                        button.color = "red"
                        create_component();
                    }

                    onReleased:
                    {
                        print("button released")

                        button.color = "gray"
                    }
                }
            }
        }

    }
}//AppWindow
