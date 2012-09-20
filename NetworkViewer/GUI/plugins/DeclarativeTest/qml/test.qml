import QtQuick 1.1
import NETVLibrary 1.0




Rectangle
{
    id: appWindow
    width: 1024
    height: 768
    color: "white"

    ListModel {

        id: testModel

        ListElement {
            name: "Box1"
            file: "box.qml"
        }
        ListElement {
            name: "Box2"
            file: "box.qml"
        }
        ListElement {
            name: "Box3"
            file: "box.qml"
        }
    }

    BorderImage
    {
        anchors.fill: appWindow
        source: "qrc:/DeclarativeTest/images/brushed_aluminum.png"
        horizontalTileMode: BorderImage.Repeat
        verticalTileMode: BorderImage.Repeat
        opacity: 0.3
    }

    function create_box_component(variable)
    {
        print("should create component")

        var dynamic_component = Qt.createComponent("qrc:/DeclarativeTest/qml/box.qml");

         if (dynamic_component.status == Component.Ready)
         {
             print("component ready")

             var dynamic_window = dynamic_component.createObject(appWindow, {"x": 100, "y": 100, "variable": variable});

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
    } // create_box_component

/*
    Image {
        source: "qrc:/DeclarativeTest/images/brushed_aluminum.png"
    }
*/

    GridView {
        id: myListView

        width: 180; height: parent.height
        anchors.right: parent.right
        anchors.top: parent.top

        model: moduleList[0].variables()
        delegate: Rectangle {
            id: wrapper
            color: "cyan"
            height: 50
            width: myListView.width

            BorderImage
            {
                anchors.fill: wrapper
                source: "qrc:/DeclarativeTest/images/clear_aluminum.png"
                horizontalTileMode: BorderImage.Stretch
                verticalTileMode: BorderImage.Stretch
            }

            Column {

                id: column
                Text {
                    id: name_label
                    text: model.modelData.getName()
                }
                Text {
                    id: value_label
                    text: model.modelData.getValue()

                    Connections
                    {
                        target: model.modelData
                        onValueChanged: value_label.text = model.modelData.getValue()
                    }
                }
            }



            MouseArea {
                anchors.fill: wrapper

                onPressed:
                {
                    print(name_label.text + " pressed ...")
                }

                onReleased:
                {
                    print(name_label.text + " released ...")
                    create_box_component(model.modelData)
                }

            }
        }
    }//GridView
}//AppWindow
